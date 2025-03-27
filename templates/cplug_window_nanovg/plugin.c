#include <cplug.h>

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define my_assert(cond) (cond) ? (void)0 : __debugbreak()
#else
#define my_assert(cond) (cond) ? (void)0 : __builtin_debugtrap()
#endif

struct GUI;
typedef struct GUI GUI;

typedef struct Plugin
{
    CplugHostContext* host_ctx;

    GUI* gui;
    int  width;
    int  height;
} Plugin;

// Global lifecycle
void cplug_libraryLoad() {}
void cplug_libraryUnload() {}

// Plugin lifecycle
void* cplug_createPlugin(CplugHostContext* ctx)
{
    Plugin* p = calloc(1, sizeof(Plugin));

    p->width  = 800;
    p->height = 400;
    return p;
}
void cplug_destroyPlugin(void* _p) { free(_p); }

// Channels
uint32_t cplug_getInputBusChannelCount(void* _p, uint32_t bus_idx) { return 2; }
uint32_t cplug_getOutputBusChannelCount(void* _p, uint32_t bus_idx) { return 2; }
void     cplug_getInputBusName(void* ptr, uint32_t idx, char* buf, size_t buflen) { snprintf(buf, buflen, "Input"); }
void     cplug_getOutputBusName(void* ptr, uint32_t idx, char* buf, size_t buflen) { snprintf(buf, buflen, "Output"); }
// Parameters
uint32_t cplug_getParameterID(void* ptr, uint32_t paramIndex) { return 0; }
uint32_t cplug_getParameterFlags(void* ptr, uint32_t paramId) { return 0; }
void     cplug_getParameterName(void* ptr, uint32_t paramId, char* buf, size_t buflen) { snprintf(buf, buflen, ""); }
void     cplug_getParameterRange(void* ptr, uint32_t paramId, double* min, double* max) {}
double   cplug_denormaliseParameterValue(void* ptr, uint32_t paramId, double normalised) { return 0; }
double   cplug_normaliseParameterValue(void* ptr, uint32_t paramId, double value) { return 0; }
double   cplug_getDefaultParameterValue(void* ptr, uint32_t paramId) { return 0; }
double   cplug_getParameterValue(void* ptr, uint32_t paramId) { return 0; }
double   cplug_parameterStringToValue(void* ptr, uint32_t paramId, const char* str) { return 0; }
void     cplug_parameterValueToString(void* ptr, uint32_t paramId, char* buf, size_t bufsize, double value) {}
void     cplug_setParameterValue(void* ptr, uint32_t paramId, double value) {}
// Audio
uint32_t cplug_getLatencyInSamples(void* ptr) { return 0; }
uint32_t cplug_getTailInSamples(void* ptr) { return 0; }
void     cplug_setSampleRateAndBlockSize(void* userPlugin, double sampleRate, uint32_t maxBlockSize) {}

void cplug_process(void* userPlugin, CplugProcessContext* ctx)
{
    // Plugin* p = userPlugin;

    float** output = ctx->getAudioOutput(ctx, 0);
    if (!output)
        return;

    for (int ch = 0; ch < 2; ch++)
        memset(output[ch], 0, sizeof(float) * ctx->numFrames);
}

// Program state
void cplug_saveState(void* userPlugin, const void* stateCtx, cplug_writeProc writeProc) {}
void cplug_loadState(void* userPlugin, const void* stateCtx, cplug_readProc readProc) {}

//=============================================================================

#include <cplug_extensions/window.h>
#include <nanovg_compat.h>

struct GUI
{
    Plugin*     plugin;
    void*       pw;
    NVGcontext* nvg;
};

void pw_get_info(PWGetInfo* info)
{
    if (info->type == PW_INFO_INIT_SIZE)
    {
        Plugin* p              = (Plugin*)info->init_size.plugin;
        info->init_size.width  = p->width;
        info->init_size.height = p->height;
    }
    else if (info->type == PW_INFO_CONSTRAIN_SIZE)
    {
        if (info->constrain_size.width > 1000)
            info->constrain_size.width = 1000;
        if (info->constrain_size.height > 1000)
            info->constrain_size.height = 1000;
    }
}

void* pw_create_gui(void* _plugin, void* pw)
{
    Plugin* plugin = (Plugin*)_plugin;
    GUI*    gui    = (GUI*)calloc(1, sizeof(*gui));

    plugin->gui = gui;
    gui->plugin = plugin;
    gui->pw     = pw;

    // init
    void* window_ptr = pw_get_native_window(pw);
    gui->nvg         = nvgCreateContext(window_ptr, NVG_DEFAULT_CONTEXT_FLAGS, plugin->width, plugin->height);
    my_assert(gui->nvg != NULL);

    return gui;
}

void pw_destroy_gui(void* _gui)
{
    GUI* gui = (GUI*)_gui;

    gui->plugin->gui = NULL;

    // deinit
    nvgDeleteContext(gui->nvg);

    free(gui);
}

void pw_tick(void* _gui)
{
    GUI* gui = (GUI*)_gui;

    NVGcontext* nvg           = gui->nvg;
    int         window_width  = gui->plugin->width;
    int         window_height = gui->plugin->height;

    nvgBindFramebuffer(nvg, 0);
    nvgBeginFrame(nvg, window_width, window_height, 1);
    nvgClearWithColor(nvg, nvgRGBAf(0, 0, 0, 1));

    nvgBeginPath(nvg);
    float cx = window_width / 2.0f;
    float cy = window_height / 2.0f;
    float r  = (window_width > window_height ? window_height : window_width) / 2.0f;
    nvgCircle(nvg, cx, cy, r);
    nvgFillColor(nvg, nvgRGBAf(1, 1, 1, 1));
    nvgFill(nvg);

    nvgEndFrame(nvg);
#ifdef _WIN32
    d3dnvgPresent(nvg);
#endif
}

bool pw_event(const PWEvent* event)
{
    if (event->type == PW_EVENT_RESIZE)
    {
        GUI* gui = (GUI*)event->gui;

        gui->plugin->width  = event->resize.width;
        gui->plugin->height = event->resize.height;

        void* window_ptr = pw_get_native_window(gui->pw);
        nvgSetViewBounds(gui->nvg, window_ptr, gui->plugin->width, gui->plugin->height);
    }
    else if (event->type == PW_EVENT_MOUSE_LEFT_DOWN)
    {
        pw_beep();
    }
    return false;
}
