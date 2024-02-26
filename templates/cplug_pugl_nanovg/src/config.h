#ifndef PLUGIN_CONFIG_H
#define PLUGIN_CONFIG_H

#define CPLUG_COMPANY_NAME "AAPF"
#define CPLUG_COMPANY_EMAIL ""
#define CPLUG_PLUGIN_NAME "CPLUG + Pugl + NanoVG Template"
#define CPLUG_PLUGIN_URI "https://github.com/Tremus/awesome-audio-plugin-framework"
#define CPLUG_PLUGIN_VERSION "1.0.0"

#define CPLUG_IS_INSTRUMENT 1

#define CPLUG_NUM_INPUT_BUSSES 0
#define CPLUG_NUM_OUTPUT_BUSSES 1
#define CPLUG_WANT_MIDI_INPUT 1
#define CPLUG_WANT_MIDI_OUTPUT 1

#define CPLUG_WANT_GUI 1
#define CPLUG_GUI_RESIZABLE 1

#define CPLUG_VST3_CATEGORIES "Instrument|Stereo"

#define CPLUG_VST3_TUID_COMPONENT 'aapf', 'comp', 'cpgl', '+nvg'
#define CPLUG_VST3_TUID_CONTROLLER 'aapf', 'edit', 'cpgl', '+nvg'

#define CPLUG_AUV2_VIEW_CLASS CPLUGPuglView
#define CPLUG_AUV2_VIEW_CLASS_STR "CPLUGPuglView"
static const unsigned kAudioUnitProperty_UserPlugin = 'plug';

#define CPLUG_CLAP_ID "com.aapf.CPLUGPugl"
#define CPLUG_CLAP_DESCRIPTION "Example plugin"
#define CPLUG_CLAP_FEATURES CLAP_PLUGIN_FEATURE_INSTRUMENT, CLAP_PLUGIN_FEATURE_STEREO

// Examples of using common parameter types
enum Parameters
{
    kParameterFloat,
    kParameterInt,
    kParameterBool,
    kParameterCount
};

#define CPLUG_NUM_PARAMS kParameterCount

#endif // PLUGIN_CONFIG_H
