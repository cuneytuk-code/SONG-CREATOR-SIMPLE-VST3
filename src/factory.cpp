#include "public.sdk/source/main/pluginfactory.h"
#include "plug.h"
#include "editor.h"
#define stringPluginName "Song Creator Engine"
#define stringCompanyName "OALI TARI"
using namespace Steinberg; using namespace SongCreator;
BEGIN_FACTORY_DEF(stringCompanyName, "", "")
DEF_CLASS2(INLINE_UID_FROM_FUID(Vst::AudioEffectUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Instrument|Synth","1.0.0",kVstVersionString,Plug::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(Vst::EditControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"","1.0.0",kVstVersionString,Controller::createInstance)
END_FACTORY
