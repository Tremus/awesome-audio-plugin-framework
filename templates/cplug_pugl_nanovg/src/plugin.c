#include <cplug.h>
#include <nanovg_compat.h>
#include <pugl/pugl.h>
#include <pugl/stub.h>

#include <math.h>
#include <string.h>

#ifdef _WIN32
#define my_assert(cond) (cond) ? (void)0 : __debugbreak()
#else
#define my_assert(cond) (cond) ? (void)0 : __builtin_debugtrap()
#endif

// Apparently denormals aren't a problem on ARM & M1?
// https://en.wikipedia.org/wiki/Subnormal_number
// https://www.kvraudio.com/forum/viewtopic.php?t=575799
#if __arm64__
#define DISABLE_DENORMALS
#define RESTORE_DENORMALS
#elif defined(_WIN32)
// https://softwareengineering.stackexchange.com/a/337251
#include <immintrin.h>
#define DISABLE_DENORMALS                                                                                              \
    unsigned int oldMXCSR = _mm_getcsr();       /*read the old MXCSR setting  */                                       \
    unsigned int newMXCSR = oldMXCSR |= 0x8040; /* set DAZ and FZ bits        */                                       \
    _mm_setcsr(newMXCSR);                       /* write the new MXCSR setting to the MXCSR */
#define RESTORE_DENORMALS _mm_setcsr(oldMXCSR);
#else
#include <fenv.h>
#define DISABLE_DENORMALS                                                                                              \
    fenv_t _fenv;                                                                                                      \
    fegetenv(&_fenv);                                                                                                  \
    fesetenv(FE_DFL_DISABLE_SSE_DENORMS_ENV);
#define RESTORE_DENORMALS fesetenv(&_fenv);
#endif

#define ARRLEN(a)              (sizeof(a) / sizeof((a)[0]))
#define CPLUG_EVENT_QUEUE_MASK (CPLUG_EVENT_QUEUE_SIZE - 1)

typedef struct ParamInfo
{
    float min;
    float max;
    float defaultValue;
    int   flags;
} ParamInfo;

typedef struct MyPlugin
{
    CplugHostContext* cplug_ctx;

    ParamInfo paramInfo[kParameterCount];

    float    sampleRate;
    uint32_t maxBufferSize;

    float paramValuesAudio[kParameterCount];

    float oscPhase; // 0-1
    int   midiNote; // -1 == not playing, 0-127+ playing
    float velocity; // 0-1

    // GUI zone
    void* gui;
    float paramValuesMain[kParameterCount];

    // Single reader writer queue. Pretty sure atomics aren't required, but here anyway
    cplug_atomic_i32 mainToAudioHead;
    cplug_atomic_i32 mainToAudioTail;
    CplugEvent       mainToAudioQueue[CPLUG_EVENT_QUEUE_SIZE];

    cplug_atomic_i32 audioToMainHead;
    cplug_atomic_i32 audioToMainTail;
    CplugEvent       audioToMainQueue[CPLUG_EVENT_QUEUE_SIZE];
} MyPlugin;

void sendParamEventFromMain(MyPlugin* plugin, uint32_t type, uint32_t paramIdx, double value);

void cplug_libraryLoad() {};
void cplug_libraryUnload() {};

void* cplug_createPlugin(CplugHostContext* ctx)
{
    MyPlugin* p  = (MyPlugin*)calloc(1, sizeof(MyPlugin));
    p->cplug_ctx = ctx;

    // Init params
    p->paramInfo[kParameterFloat].flags        = CPLUG_FLAG_PARAMETER_IS_AUTOMATABLE;
    p->paramInfo[kParameterFloat].max          = 100.0f;
    p->paramInfo[kParameterFloat].defaultValue = 50.0f;

    p->paramValuesAudio[kParameterInt]       = 2.0f;
    p->paramInfo[kParameterInt].flags        = CPLUG_FLAG_PARAMETER_IS_AUTOMATABLE | CPLUG_FLAG_PARAMETER_IS_INTEGER;
    p->paramInfo[kParameterInt].min          = 2.0f;
    p->paramInfo[kParameterInt].max          = 5.0f;
    p->paramInfo[kParameterInt].defaultValue = 2.0f;

    p->paramInfo[kParameterBool].flags = CPLUG_FLAG_PARAMETER_IS_BOOL;
    p->paramInfo[kParameterBool].max   = 1.0f;

    p->midiNote = -1;

    return p;
}
void cplug_destroyPlugin(void* ptr)
{
    // Free any allocated resources in your plugin here
    free(ptr);
}

/* --------------------------------------------------------------------------------------------------------
 * Busses */
uint32_t cplug_getNumInputBusses(void* ptr) { return 1; }
uint32_t cplug_getNumOutputBusses(void* ptr) { return 1; }
uint32_t cplug_getInputBusChannelCount(void* ptr, uint32_t idx)
{
    if (idx == 0)
        return 2; // 1 bus, stereo
    return 0;
}

uint32_t cplug_getOutputBusChannelCount(void* ptr, uint32_t idx)
{
    if (idx == 0)
        return 2; // 1 bus, stereo
    return 0;
}

void cplug_getInputBusName(void* ptr, uint32_t idx, char* buf, size_t buflen)
{
    snprintf(buf, buflen, "%s", "Stereo Input");
}

void cplug_getOutputBusName(void* ptr, uint32_t idx, char* buf, size_t buflen)
{
    snprintf(buf, buflen, "%s", "Stereo Output");
}

/* --------------------------------------------------------------------------------------------------------
 * Parameters */

uint32_t cplug_getNumParameters(void* ptr)
{
    MyPlugin* p = ptr;
    return ARRLEN(p->paramInfo);
}

uint32_t cplug_getParameterID(void* ptr, uint32_t paramIndex) { return paramIndex; }

void cplug_getParameterName(void* ptr, uint32_t paramId, char* buf, size_t buflen)
{
    static const char* param_names[CPLUG_NUM_PARAMS] = {
        "Example Float Parameter",
        "Example Int Parameter",
        "Example Bool Parameter"};
    _Static_assert(ARRLEN(param_names) == CPLUG_NUM_PARAMS, "Did you add new parameters?");
    snprintf(buf, buflen, "%s", param_names[paramId]);
}

double cplug_getParameterValue(void* ptr, uint32_t index)
{
    const MyPlugin* plugin = (MyPlugin*)ptr;
    double          val    = plugin->paramValuesAudio[index];
    if (plugin->paramInfo[index].flags & CPLUG_FLAG_PARAMETER_IS_INTEGER)
        val = round(val);
    return val;
}

double cplug_getDefaultParameterValue(void* ptr, uint32_t index)
{
    MyPlugin* plugin = (MyPlugin*)ptr;
    return plugin->paramInfo[index].defaultValue;
}

void cplug_setParameterValue(void* ptr, uint32_t index, double value)
{
    MyPlugin* plugin = (MyPlugin*)ptr;

    ParamInfo* info = &plugin->paramInfo[index];
    if (value < info->min)
        value = info->min;
    if (value > info->max)
        value = info->max;
    plugin->paramValuesAudio[index] = (float)value;

    // Send incoming param update to GUI
    if (plugin->gui)
    {
        int queueWritePos = cplug_atomic_load_i32(&plugin->audioToMainHead) & CPLUG_EVENT_QUEUE_MASK;

        plugin->audioToMainQueue[queueWritePos].parameter.type  = CPLUG_EVENT_PARAM_CHANGE_UPDATE;
        plugin->audioToMainQueue[queueWritePos].parameter.id    = index;
        plugin->audioToMainQueue[queueWritePos].parameter.value = value;

        cplug_atomic_fetch_add_i32(&plugin->audioToMainHead, 1);
        cplug_atomic_fetch_and_i32(&plugin->audioToMainHead, CPLUG_EVENT_QUEUE_MASK);
    }
}

double cplug_denormaliseParameterValue(void* ptr, uint32_t index, double normalised)
{
    const MyPlugin*  plugin = (MyPlugin*)ptr;
    const ParamInfo* info   = &plugin->paramInfo[index];

    double denormalised = normalised * (info->max - info->min) + info->min;

    if (denormalised < info->min)
        denormalised = info->min;
    if (denormalised > info->max)
        denormalised = info->max;
    return denormalised;
}

double cplug_normaliseParameterValue(void* ptr, uint32_t index, double denormalised)
{
    const MyPlugin*  plugin = (MyPlugin*)ptr;
    const ParamInfo* info   = &plugin->paramInfo[index];

    // If this fails, your param range is likely not initialised, causing a division by zero and producing infinity
    double normalised = (denormalised - info->min) / (info->max - info->min);
    my_assert(normalised == normalised);

    if (normalised < 0.0f)
        normalised = 0.0f;
    if (normalised > 1.0f)
        normalised = 1.0f;
    return normalised;
}

double cplug_parameterStringToValue(void* ptr, uint32_t index, const char* str)
{
    double          value;
    const MyPlugin* plugin = (MyPlugin*)ptr;
    const unsigned  flags  = plugin->paramInfo[index].flags;

    if (flags & CPLUG_FLAG_PARAMETER_IS_INTEGER)
        value = (double)atoi(str);
    else
        value = atof(str);

    return value;
}

void cplug_parameterValueToString(void* ptr, uint32_t index, char* buf, size_t bufsize, double value)
{
    const MyPlugin* plugin = (MyPlugin*)ptr;
    const uint32_t  flags  = plugin->paramInfo[index].flags;

    if (flags & CPLUG_FLAG_PARAMETER_IS_BOOL)
        value = value >= 0.5 ? 1 : 0;

    if (flags & (CPLUG_FLAG_PARAMETER_IS_INTEGER | CPLUG_FLAG_PARAMETER_IS_BOOL))
        snprintf(buf, bufsize, "%d", (int)value);
    else
        snprintf(buf, bufsize, "%.2f", value);
}

void cplug_getParameterRange(void* ptr, uint32_t index, double* min, double* max)
{
    const MyPlugin* plugin = (MyPlugin*)ptr;
    *min                   = plugin->paramInfo[index].min;
    *max                   = plugin->paramInfo[index].max;
}

uint32_t cplug_getParameterFlags(void* ptr, uint32_t index)
{
    const MyPlugin* plugin = (MyPlugin*)ptr;
    return plugin->paramInfo[index].flags;
}

/* --------------------------------------------------------------------------------------------------------
 * Audio/MIDI Processing */

uint32_t cplug_getLatencyInSamples(void* ptr) { return 0; }
uint32_t cplug_getTailInSamples(void* ptr) { return 0; }

void cplug_setSampleRateAndBlockSize(void* ptr, double sampleRate, uint32_t maxBlockSize)
{
    MyPlugin* plugin      = (MyPlugin*)ptr;
    plugin->sampleRate    = (float)sampleRate;
    plugin->maxBufferSize = maxBlockSize;
}

void cplug_process(void* ptr, CplugProcessContext* ctx)
{
    DISABLE_DENORMALS

    MyPlugin* plugin = (MyPlugin*)ptr;

    // Audio thread has chance to respond to incoming GUI events before being sent to the host
    int head = cplug_atomic_load_i32(&plugin->mainToAudioHead) & CPLUG_EVENT_QUEUE_MASK;
    int tail = cplug_atomic_load_i32(&plugin->mainToAudioTail);

    while (tail != head)
    {
        CplugEvent* event = &plugin->mainToAudioQueue[tail];

        if (event->type == CPLUG_EVENT_PARAM_CHANGE_UPDATE)
            plugin->paramValuesAudio[event->parameter.id] = event->parameter.value;

        ctx->enqueueEvent(ctx, event, 0);

        tail++;
        tail &= CPLUG_EVENT_QUEUE_MASK;
    }
    cplug_atomic_exchange_i32(&plugin->mainToAudioTail, tail);

    // "Sample accurate" process loop
    CplugEvent event;
    int        frame = 0;
    while (ctx->dequeueEvent(ctx, &event, frame))
    {
        switch (event.type)
        {
        case CPLUG_EVENT_PARAM_CHANGE_UPDATE:
            cplug_setParameterValue(plugin, event.parameter.id, event.parameter.value);
            break;
        case CPLUG_EVENT_MIDI:
        {
            static const uint8_t MIDI_NOTE_OFF         = 0x80;
            static const uint8_t MIDI_NOTE_ON          = 0x90;
            static const uint8_t MIDI_NOTE_PITCH_WHEEL = 0xe0;

            if ((event.midi.status & 0xf0) == MIDI_NOTE_ON)
            {
                plugin->midiNote = event.midi.data1;
                plugin->velocity = (float)event.midi.data2 / 127.0f;
            }
            if ((event.midi.status & 0xf0) == MIDI_NOTE_OFF)
            {
                int note = event.midi.data1;
                if (note == plugin->midiNote)
                    plugin->midiNote = -1;
                plugin->velocity = (float)event.midi.data2 / 127.0f;
            }
            if ((event.midi.status & 0xf0) == MIDI_NOTE_PITCH_WHEEL)
            {
                int pb = (int)event.midi.data1 | ((int)event.midi.data2 << 7);
            }
            break;
        }
        case CPLUG_EVENT_PROCESS_AUDIO:
        {
            // If your plugin does not require sample accurate processing, use this line below to break the loop
            // frame = event.processAudio.endFrame;

            float** output = ctx->getAudioOutput(ctx, 0);
            CPLUG_LOG_ASSERT(output != NULL)
            CPLUG_LOG_ASSERT(output[0] != NULL);
            CPLUG_LOG_ASSERT(output[1] != NULL);

            if (plugin->midiNote == -1)
            {
                // Silence
                memset(&output[0][frame], 0, sizeof(float) * (event.processAudio.endFrame - frame));
                memset(&output[1][frame], 0, sizeof(float) * (event.processAudio.endFrame - frame));
                frame = event.processAudio.endFrame;
            }
            else
            {
                float phase = plugin->oscPhase;

                float Hz  = 440.0f * exp2f(((float)plugin->midiNote - 69.0f) * 0.0833333f);
                float inc = Hz / plugin->sampleRate;
                float dB  = -60.0f + plugin->velocity * 54; // -6dB max
                float vol = powf(10.0f, dB / 20.0f);

                for (; frame < event.processAudio.endFrame; frame++)
                {
                    static const float pi = 3.141592653589793f;

                    float sample = vol * sinf(2 * pi * phase);

                    for (int ch = 0; ch < 2; ch++)
                        output[ch][frame] = sample;

                    phase += inc;
                    phase -= (int)phase;
                }

                plugin->oscPhase = phase;
            }
            break;
        }
        default:
            break;
        }
    }
    RESTORE_DENORMALS
}

/* --------------------------------------------------------------------------------------------------------
 * State */

// In these methods we will use a very basic binary preset format: a flat array of param values

void cplug_saveState(void* userPlugin, const void* stateCtx, cplug_writeProc writeProc)
{
    MyPlugin* plugin = (MyPlugin*)userPlugin;
    writeProc(stateCtx, plugin->paramValuesAudio, sizeof(plugin->paramValuesAudio));
}

void cplug_loadState(void* userPlugin, const void* stateCtx, cplug_readProc readProc)
{
    MyPlugin* plugin = (MyPlugin*)userPlugin;

    float vals[kParameterCount + 1];

    int64_t bytesRead = readProc(stateCtx, vals, sizeof(vals));

    if (bytesRead == sizeof(plugin->paramValuesAudio))
    {
        // Send update to queue so we notify host
        for (int i = 0; i < kParameterCount; i++)
        {
            plugin->paramValuesAudio[i] = vals[i];
            plugin->paramValuesMain[i]  = vals[i];
            sendParamEventFromMain(plugin, CPLUG_EVENT_PARAM_CHANGE_UPDATE, i, vals[i]);
        }
    }
}

void sendParamEventFromMain(MyPlugin* plugin, uint32_t type, uint32_t paramId, double value)
{
    int         mainToAudioHead = cplug_atomic_load_i32(&plugin->mainToAudioHead) & CPLUG_EVENT_QUEUE_MASK;
    CplugEvent* paramEvent      = &plugin->mainToAudioQueue[mainToAudioHead];
    paramEvent->parameter.type  = type;
    paramEvent->parameter.id    = paramId;
    paramEvent->parameter.value = value;

    cplug_atomic_fetch_add_i32(&plugin->mainToAudioHead, 1);
    cplug_atomic_fetch_and_i32(&plugin->mainToAudioHead, CPLUG_EVENT_QUEUE_MASK);

    // request_flush from CLAP host? Doesn't seem to be required
}

typedef struct MyGUI
{
    MyPlugin*   plugin;
    PuglWorld*  world;
    PuglView*   view;
    NVGcontext* nvg;
    uint32_t    width, height;

    int   mainFramebuffer;
    float pixelRatio;

    int  dragParamId;
    bool mouseDragging;
    int  dragStartX;
    int  dragStartY;

    double dragStartParamNormalised;
    double dragCurrentParamNormalised;
} MyGUI;

bool tickGUI(MyGUI* gui)
{
    bool needRedraw = false;

    MyPlugin* plugin = gui->plugin;
    int       head   = cplug_atomic_load_i32(&plugin->audioToMainHead) & CPLUG_EVENT_QUEUE_MASK;
    int       tail   = cplug_atomic_load_i32(&plugin->audioToMainTail);

    needRedraw = tail != head;

    while (tail != head)
    {
        CplugEvent* event = &plugin->audioToMainQueue[tail];

        switch (event->type)
        {
        case CPLUG_EVENT_PARAM_CHANGE_UPDATE:
        {
            uint32_t idx                 = event->parameter.id;
            plugin->paramValuesMain[idx] = (float)event->parameter.value;
            break;
        }
        default:
            break;
        }

        tail++;
        tail &= CPLUG_EVENT_QUEUE_MASK;
    }
    cplug_atomic_exchange_i32(&plugin->audioToMainTail, tail);

    return needRedraw;
}

static void handleMouseDown(MyGUI* gui, int x, int y)
{
    if (x >= 10 && x < 40 && y >= 10 && y < 40)
    {
        gui->mouseDragging = true;
        gui->dragParamId   = 0;
        gui->dragStartX    = x;
        gui->dragStartY    = y;

        double v                        = cplug_getParameterValue(gui->plugin, gui->dragParamId);
        gui->dragStartParamNormalised   = cplug_normaliseParameterValue(gui->plugin, gui->dragParamId, v);
        gui->dragCurrentParamNormalised = gui->dragStartParamNormalised;

        sendParamEventFromMain(gui->plugin, CPLUG_EVENT_PARAM_CHANGE_BEGIN, gui->dragParamId, 0);
    }
}

static void handleMouseUp(MyGUI* gui)
{
    if (gui->mouseDragging)
    {
        gui->mouseDragging = false;
        sendParamEventFromMain(gui->plugin, CPLUG_EVENT_PARAM_CHANGE_END, gui->dragParamId, 0);
    }
}

static void handleMouseMove(MyGUI* gui, int x, int y)
{
    if (gui->mouseDragging)
    {
        double nextValNormalised = gui->dragStartParamNormalised + (gui->dragStartY - y) * 0.01;
        if (nextValNormalised < 0)
            nextValNormalised = 0;
        if (nextValNormalised > 1)
            nextValNormalised = 1;
        gui->dragCurrentParamNormalised = nextValNormalised;

        double nextValDenormalised = cplug_denormaliseParameterValue(gui->plugin, gui->dragParamId, nextValNormalised);
        gui->plugin->paramValuesMain[gui->dragParamId] = (float)nextValDenormalised;
        sendParamEventFromMain(gui->plugin, CPLUG_EVENT_PARAM_CHANGE_UPDATE, gui->dragParamId, nextValDenormalised);
    }
}

void drawFrame(struct MyGUI* gui)
{
    NVGcontext* nvg = gui->nvg;

    nvgBindFramebuffer(nvg, gui->mainFramebuffer);
    nvgBeginFrame(nvg, gui->width, gui->height, gui->pixelRatio);

    const NVGcolor colourBG    = {0.8f, 0.2f, 0.8f, 1.0f};
    const NVGcolor colourParam = {0.2f, 0.8f, 0.2f, 1.0f};

    nvgClearWithColor(nvg, colourBG);

    nvgBeginPath(nvg);
    nvgStrokeColor(nvg, colourParam);

    // Param border
    nvgRect(nvg, 10, 10, 40, 40);
    nvgStroke(nvg);

    // Param amount
    nvgBeginPath(nvg);
    float denorm = gui->plugin->paramValuesMain[0];
    float norm   = cplug_normaliseParameterValue(gui->plugin, 0, denorm);
    nvgFillColor(nvg, colourParam);
    nvgRect(nvg, 10, 10 + (1 - norm) * 40, 40, norm * 40);
    nvgFill(nvg);

    nvgEndFrame(nvg);
}

void presentFrame(struct MyGUI* gui)
{
    NVGcontext* nvg = gui->nvg;

    nvgBindFramebuffer(nvg, 0);
    nvgBeginFrame(nvg, gui->width, gui->height, gui->pixelRatio);

    NVGpaint img;
    memset(&img, 0, sizeof(img));

    float transform_scale = 1.0f / gui->pixelRatio;

    nvgTransformScale(img.xform, transform_scale, transform_scale);
    img.extent[0]  = gui->pixelRatio * gui->width;
    img.extent[1]  = gui->pixelRatio * gui->height;
    img.innerColor = (NVGcolor){1.0f, 1.0f, 1.0f, 1.0f};
    img.image      = gui->mainFramebuffer;

    nvgBeginPath(nvg);
    nvgRect(nvg, 0.0f, 0.0f, gui->width, gui->height);
    nvgFillPaint(nvg, img);
    nvgFill(nvg);
    nvgResetTransform(nvg);

    nvgEndFrame(nvg);
#ifdef _WIN32
    d3dnvgPresent(nvg);
#endif
}

PuglStatus myPuglEventFunc(PuglView* view, const PuglEvent* event)
{
    struct MyGUI* gui = (struct MyGUI*)puglGetHandle(view);

    switch (event->type)
    {
    case PUGL_REALIZE:
    {
        gui->pixelRatio = NVG_DEFAULT_PIXEL_RATIO;
        gui->nvg        = nvgCreateContext(
            (void*)puglGetNativeView(gui->view),
            NVG_DEFAULT_CONTEXT_FLAGS,
            gui->pixelRatio * gui->width,
            gui->pixelRatio * gui->height);
        my_assert(gui->nvg != NULL);

        gui->mainFramebuffer =
            nvgCreateFramebuffer(gui->nvg, gui->pixelRatio * gui->width, gui->pixelRatio * gui->height, 0);
        my_assert(gui->mainFramebuffer != 0);
        break;
    }
    case PUGL_UNREALIZE:
    {
        nvgDeleteFramebuffer(gui->nvg, gui->mainFramebuffer);
        gui->mainFramebuffer = 0;
        nvgDeleteContext(gui->nvg);
        gui->nvg = NULL;
        // change
        break;
    }
    case PUGL_CONFIGURE:
    {
        if (gui->nvg != NULL)
        {
            if (gui->mainFramebuffer != 0)
                nvgDeleteFramebuffer(gui->nvg, gui->mainFramebuffer);
            nvgSetViewBounds(
                gui->nvg,
                (void*)puglGetNativeView(gui->view),
                gui->pixelRatio * gui->width,
                gui->pixelRatio * gui->height);
            gui->mainFramebuffer =
                nvgCreateFramebuffer(gui->nvg, gui->pixelRatio * gui->width, gui->pixelRatio * gui->height, 0);
        }
        break;
    }
    case PUGL_MOTION:
    {
        double scaleFactor = puglGetScaleFactor(gui->view);
        double x           = event->motion.x / scaleFactor;
        double y           = event->motion.y / scaleFactor;
        handleMouseMove(gui, x, y);
        break;
    }
    case PUGL_BUTTON_PRESS:
    {
        double scaleFactor = puglGetScaleFactor(gui->view);
        double x           = event->motion.x / scaleFactor;
        double y           = event->motion.y / scaleFactor;
        handleMouseDown(gui, x, y);
        break;
    }
    case PUGL_BUTTON_RELEASE:
    {
        handleMouseUp(gui);
        break;
    }
    case PUGL_UPDATE:
        break;
    case PUGL_EXPOSE:
        break;
    case PUGL_TIMER:
        tickGUI(gui);
        drawFrame(gui);
        presentFrame(gui);
        break;
    default:
        break;
    }

    return PUGL_SUCCESS;
}

void* cplug_createGUI(void* userPlugin)
{
    struct MyGUI* gui = calloc(1, sizeof(struct MyGUI));

    gui->plugin = userPlugin;
    gui->width  = 640;
    gui->height = 360;

    gui->world = puglNewWorld(PUGL_MODULE, PUGL_WORLD_THREADS);
    puglSetWorldString(gui->world, PUGL_CLASS_NAME, CPLUG_PLUGIN_NAME);

    gui->view = puglNewView(gui->world);

    puglSetSizeHint(gui->view, PUGL_DEFAULT_SIZE, gui->width, gui->height);
    puglSetViewHint(gui->view, PUGL_RESIZABLE, PUGL_TRUE);

    puglSetHandle(gui->view, gui);
    puglSetEventFunc(gui->view, myPuglEventFunc);

    return gui;
}

void cplug_destroyGUI(void* userGUI)
{
    struct MyGUI* gui = userGUI;
    puglFreeView(gui->view);
    puglFreeWorld(gui->world);
    free(gui);
}

void cplug_setParent(void* userGUI, void* hwnd_or_nsview)
{
    struct MyGUI* gui = userGUI;
    if (puglGetParentWindow(gui->view))
    {
        puglStopTimer(gui->view, 0);
        puglSetParentWindow(gui->view, (PuglNativeView)NULL);
        puglUnrealize(gui->view);
    }
    if (hwnd_or_nsview)
    {
        puglSetParentWindow(gui->view, (PuglNativeView)hwnd_or_nsview);
        puglSetBackend(gui->view, puglStubBackend());
        PuglStatus status = puglRealize(gui->view);
        my_assert(status == PUGL_SUCCESS);
        puglStartTimer(gui->view, 0, 0.016);
    }
}

void cplug_setVisible(void* userGUI, bool visible)
{
    struct MyGUI* gui = userGUI;
    if (visible)
        puglShow(gui->view, PUGL_SHOW_RAISE);
    else
        puglHide(gui->view);
}

void cplug_setScaleFactor(void* userGUI, float scale) {}

void cplug_getSize(void* userGUI, uint32_t* width, uint32_t* height)
{
    struct MyGUI* gui = userGUI;

    *width  = gui->width;
    *height = gui->height;
}

void cplug_checkSize(void* userGUI, uint32_t* width, uint32_t* height)
{
    if (*width < 100)
        *width = 100;
    if (*height < 100)
        *height = 100;

    if (*width > 1280)
        *width = 1280;
    if (*height > 720)
        *height = 720;
}

bool cplug_setSize(void* userGUI, uint32_t width, uint32_t height)
{
    struct MyGUI* gui = userGUI;
    gui->width        = width;
    gui->height       = height;

    double   scaleFactor = puglGetScaleFactor(gui->view);
    PuglRect rect        = {0, 0, width * scaleFactor, height * scaleFactor};

    return PUGL_SUCCESS == puglSetFrame(gui->view, rect);
}
