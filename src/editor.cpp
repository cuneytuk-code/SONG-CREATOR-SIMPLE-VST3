#include "editor.h"
#include <shlwapi.h>
#include <filesystem>
using namespace Steinberg;
namespace SongCreator {

std::wstring GetIndexHtmlPath()
{
    HMODULE hMod=NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCACHED_INFO,(LPCWSTR)GetIndexHtmlPath,&hMod);
    wchar_t dllBuff[MAX_PATH]; GetModuleFileNameW(hMod,dllBuff,MAX_PATH);
    std::filesystem::path dllFile(dllBuff);
    std::vector<std::filesystem::path> cands={
        dllFile.parent_path().parent_path().parent_path() / "Resources" / "index.html",
        dllFile.parent_path() / "Resources" / "index.html",
        dllFile.parent_path() / "index.html",
        std::filesystem::path(L"C:\\Program Files\\Common Files\\VST3\\SongCreator.vst3\\Contents\\Resources\\index.html"),
        std::filesystem::path(L"C:\\Program Files\\Common Files\\VST3\\SongCreator.vst3\\Resources\\index.html")
    };
    for(auto& p:cands) if(std::filesystem::exists(p)) return p.wstring();
    return L"";
}

Editor::Editor(void* ctrl,const char*) : VSTGUIEditor(ctrl){ ViewRect vr(0,0,width,height); rect=vr; }
Editor::~Editor(){ if(ctrl) ctrl->Close(); }
tresult PLUGIN_API Editor::getSize(ViewRect* s){ if(!s) return kInvalidArgument; s->left=0;s->top=0;s->right=width;s->bottom=height; return kResultTrue; }
tresult PLUGIN_API Editor::onSize(ViewRect* ns){ if(!ns) return kInvalidArgument; width=ns->getWidth(); height=ns->getHeight(); rect=*ns; if(webviewHwnd&&ctrl){ ctrl->put_Bounds({0,0,width,height}); MoveWindow(webviewHwnd,0,0,width,height,TRUE);} return kResultTrue; }
tresult PLUGIN_API Editor::attached(void* parent,FIDString type){ parentHwnd=(HWND)parent; webviewHwnd=CreateWindowExW(0,L"STATIC",L"",WS_CHILD|WS_VISIBLE,0,0,width,height,parentHwnd,NULL,GetModuleHandle(NULL),NULL); initWebView(webviewHwnd); return VSTGUIEditor::attached(parent,type); }
tresult PLUGIN_API Editor::removed(){ if(ctrl){ctrl->Close();ctrl=nullptr;} if(webviewHwnd){DestroyWindow(webviewHwnd);webviewHwnd=nullptr;} return VSTGUIEditor::removed(); }

void Editor::initWebView(HWND hwnd){
    HMODULE loader=LoadLibraryW(L"WebView2Loader.dll");
    if(!loader){ MessageBoxW(hwnd,L"WebView2Loader.dll not found. Install WebView2 Runtime from Microsoft.",L"Song Creator",MB_OK); return; }
    using CreateEnvFunc=HRESULT(WINAPI*)(PCWSTR,PCWSTR,ICoreWebView2EnvironmentOptions*,ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler*);
    auto CreateEnv=(CreateEnvFunc)GetProcAddress(loader,"CreateCoreWebView2EnvironmentWithOptions");
    if(!CreateEnv) return;
    CreateEnv(nullptr,nullptr,nullptr, Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([this,hwnd](HRESULT r,ICoreWebView2Environment* envPtr)->HRESULT{
        if(FAILED(r)||!envPtr) return r; env=envPtr;
        env->CreateCoreWebView2Controller(hwnd, Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([this,hwnd](HRESULT res,ICoreWebView2Controller* c)->HRESULT{
            if(FAILED(res)||!c) return res; ctrl=c; ctrl->get_CoreWebView2(&webview); RECT b; GetClientRect(hwnd,&b); ctrl->put_Bounds(b); navigateToIndex(); return S_OK;
        }).Get()); return S_OK;
    }).Get());
}
void Editor::navigateToIndex(){
    if(!webview) return;
    auto indexPath=GetIndexHtmlPath();
    if(indexPath.empty()||!std::filesystem::exists(indexPath)){
        std::wstring err=L"<html><body style='background:#0d061f;color:#f3f4f6;font-family:sans-serif;padding:40px;'><h1 style='color:#6366f1;'>SONG CREATOR ENGINE - Setup Needed</h1><p>index.html not found. Copy your 161KB file to:</p><code style='background:#181028;padding:10px;display:block;margin:20px 0;'>C:\\Program Files\\Common Files\\VST3\\SongCreator.vst3\\Contents\\Resources\\index.html</code><p>Or same folder as VST3 DLL. Then restart Reaper.</p></body></html>";
        webview->NavigateToString(err.c_str()); return;
    }
    std::wstring url=L"file:///"+indexPath; for(auto& c:url) if(c==L'\\') c=L'/'; webview->Navigate(url.c_str());
}
tresult PLUGIN_API Controller::initialize(FUnknown* ctx){ tresult r=EditController::initialize(ctx); return r; }
IPlugView* PLUGIN_API Controller::createView(FIDString name){ if(strcmp(name,Vst::ViewType::kEditor)==0) return new Editor(this,nullptr); return nullptr; }
}
