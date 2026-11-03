#include "GraphicsAdapterStartup.h"
#include <cstdio>
#include <string>
namespace {
unsigned failures=0;
void Check(bool value,const char* label){if(!value){++failures;std::printf("FAIL %s\n",label);}}
}
int main(){
    wchar_t dir[MAX_PATH]{},mod[MAX_PATH]{},engine[MAX_PATH]{};
    if(!GetTempPathW(MAX_PATH,dir)||!GetTempFileNameW(dir,L"DGM",0,mod)||!GetTempFileNameW(dir,L"DGE",0,engine))return 1;
    WritePrivateProfileStringW(L"Core.System",L"Paths",L"preserve me",engine);
    WritePrivateProfileStringW(L"SystemSettings",L"Unrelated",L"42",engine);
    for(int n=0;n<=4;++n){
        wchar_t value[8]{};swprintf_s(value,L"%d",n);
        WritePrivateProfileStringW(L"System",L"GraphicsAdapter",value,mod);
        const auto result=dg::graphics_adapter::PrepareFiles(mod,engine);
        Check(result.requested==n&&result.verified==n&&!result.error,"chosen adapter is written before config-cache load");
        Check(static_cast<int>(GetPrivateProfileIntW(L"SystemSettings",L"r.GraphicsAdapter",-1,engine))==n,"actual file contains selected adapter");
        wchar_t retained[64]{};GetPrivateProfileStringW(L"Core.System",L"Paths",L"",retained,64,engine);
        Check(std::wstring(retained)==L"preserve me"&&GetPrivateProfileIntW(L"SystemSettings",L"Unrelated",0,engine)==42,"unrelated engine settings remain exact");
        // Model a later save using the values read by the engine at startup.
        const int cached=static_cast<int>(GetPrivateProfileIntW(L"SystemSettings",L"r.GraphicsAdapter",-1,engine));
        swprintf_s(value,L"%d",cached);WritePrivateProfileStringW(L"SystemSettings",L"r.GraphicsAdapter",value,engine);
        Check(static_cast<int>(GetPrivateProfileIntW(L"SystemSettings",L"r.GraphicsAdapter",-1,engine))==n,"later cache save retains the early preference");
    }
    WritePrivateProfileStringW(L"System",L"GraphicsAdapter",L"99",mod);
    Check(dg::graphics_adapter::PrepareFiles(mod,engine).verified==4,"old 0..4 clamp preserved");
    WritePrivateProfileStringW(L"System",L"GraphicsAdapter",L"-1",mod);
    Check(dg::graphics_adapter::PrepareFiles(mod,engine).verified==0,"negative preference keeps old fallback");
    Check(dg::graphics_adapter::PrepareFiles(nullptr,engine).verified==-1,"invalid paths refuse preparation");
    DeleteFileW(mod);
    Check(dg::graphics_adapter::PrepareFiles(mod,engine).verified==-1&&GetFileAttributesW(mod)==INVALID_FILE_ATTRIBUTES,"missing profile is not fabricated during loader attach");
    DeleteFileW(engine);
    Check(dg::graphics_adapter::PrepareFiles(mod,engine).verified==-1&&GetFileAttributesW(engine)==INVALID_FILE_ATTRIBUTES,"missing game config is not fabricated during loader attach");
    std::printf("GPU startup preparation: %u failures\n",failures);return failures?1:0;
}
