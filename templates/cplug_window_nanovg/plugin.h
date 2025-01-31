#ifndef PLUGIN_CONFIG_H
#define PLUGIN_CONFIG_H

#define CPLUG_COMPANY_NAME   "AAPF"
#define CPLUG_COMPANY_EMAIL  ""
#define CPLUG_PLUGIN_NAME    "CPLUG + Cplug Window + NanoVG"
#define CPLUG_PLUGIN_URI     "https://github.com/Tremus/awesome-audio-plugin-framework"
#define CPLUG_PLUGIN_VERSION "1.0.0"

#define CPLUG_IS_INSTRUMENT 0

#define CPLUG_NUM_INPUT_BUSSES  1
#define CPLUG_NUM_OUTPUT_BUSSES 1
#define CPLUG_WANT_MIDI_INPUT   0
#define CPLUG_WANT_MIDI_OUTPUT  0

#define CPLUG_WANT_GUI      1
#define CPLUG_GUI_RESIZABLE 1

// See list of categories here: https://steinbergmedia.github.io/vst3_doc/vstinterfaces/group__plugType.html
#define CPLUG_VST3_CATEGORIES "Stereo"

#define CPLUG_VST3_TUID_COMPONENT  'aapf', 'comp', 'cpwe', '+nvg'
#define CPLUG_VST3_TUID_CONTROLLER 'aapf', 'edit', 'cpwe', '+nvg'

#define CPLUG_AUV2_VIEW_CLASS     CPLUGWindowNanoVGView
#define CPLUG_AUV2_VIEW_CLASS_STR "CPLUGWindowNanoVGView"
static const unsigned kAudioUnitProperty_UserPlugin = 'plug';

#define CPLUG_CLAP_ID          "com.aapf.CPLUGWindowNanoVG"
#define CPLUG_CLAP_DESCRIPTION "CPLUG Window NanoVG"
#define CPLUG_CLAP_FEATURES    CLAP_PLUGIN_FEATURE_STEREO

#define CPLUG_NUM_PARAMS 0

#endif // PLUGIN_CONFIG_H
