#include "public.sdk/source/main/pluginfactory.h"
#include "plug.h"
#include "editor.h"
#define PName "Song Creator Engine"
#define Company "OALI TARI"
using namespace Steinberg; using namespace SongCreator;
BEGIN_FACTORY_DEF(Company,"","")
DEF_CLASS2(INLINE_UID_FROM_FUID(Vst::AudioEffectUID),PClassInfo::kManyInstances,kVstAudioEffectClass,PName,Vst::kDistributable,"Instrument|Synth","1.0.0",kVstVersionString,Plug::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(Vst::EditControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,PName " Controller",0,"","1.0.0",kVstVersionString,Controller::createInstance)
END_FACTORY
