#include "plug.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"
using namespace Steinberg;
namespace SongCreator {
Plug::Plug() {}
tresult PLUGIN_API Plug::initialize(FUnknown* ctx){ tresult r=AudioEffect::initialize(ctx); if(r!=kResultOk) return r; addAudioOutput(STR16("Stereo Out"), Vst::SpeakerArr::kStereo); addEventInput(STR16("Event In"),1); return kResultOk; }
tresult PLUGIN_API Plug::terminate(){return AudioEffect::terminate();}
tresult PLUGIN_API Plug::setActive(TBool s){return AudioEffect::setActive(s);}
tresult PLUGIN_API Plug::setupProcessing(Vst::ProcessSetup& s){return AudioEffect::setupProcessing(s);}
tresult PLUGIN_API Plug::canProcessSampleSize(int32 ss){return ss==Vst::kSample32?kResultTrue:kResultFalse;}
tresult PLUGIN_API Plug::setState(IBStream*){return kResultOk;}
tresult PLUGIN_API Plug::getState(IBStream*){return kResultOk;}
tresult PLUGIN_API Plug::setComponentState(IBStream*){return kResultOk;}
tresult PLUGIN_API Plug::process(Vst::ProcessData&){return kResultOk;}
}
