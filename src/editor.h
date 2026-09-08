#pragma once
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "public.sdk/source/vst/vsteditor.h"
#include <windows.h>
#include <wrl.h>
#include <string>
#include <WebView2.h>
namespace SongCreator {
class Editor : public Steinberg::Vst::VSTGUIEditor
{
public:
    Editor(void* c, const char* p);
    ~Editor() SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API attached(void* parent, Steinberg::FIDString type) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API removed() SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API getSize(Steinberg::ViewRect* size) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API onSize(Steinberg::ViewRect* newSize) SMTG_OVERRIDE;
    static void* createInstance(void* controller){return new Editor(controller,nullptr);}
private:
    void initWebView(HWND hwnd);
    void navigateToIndex();
    HWND parentHwnd=nullptr; HWND webviewHwnd=nullptr;
    Microsoft::WRL::ComPtr<ICoreWebView2Environment> env;
    Microsoft::WRL::ComPtr<ICoreWebView2Controller> ctrl;
    Microsoft::WRL::ComPtr<ICoreWebView2> webview;
    int width=1400; int height=900;
};
class Controller : public Steinberg::Vst::EditController
{
public:
    static Steinberg::FUnknown* createInstance(void* ctx){return (Steinberg::Vst::IEditController*)new Controller();}
    Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* ctx) SMTG_OVERRIDE;
    Steinberg::IPlugView* PLUGIN_API createView(Steinberg::FIDString name) SMTG_OVERRIDE;
};
}
