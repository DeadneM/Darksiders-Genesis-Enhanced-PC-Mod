#pragma once
#include <windows.h>
#include <cwchar>

namespace dg::graphics_adapter {
struct StartupResult {int requested=-1,verified=-1;DWORD error=0;};
inline int Normalize(int value){return value<0?0:value>4?4:value;}
// Called by the statically imported proxy, before UE reads/caches Engine.ini.
// Existing files only; no directory creation or CRT filesystem work in DllMain.
inline StartupResult PrepareFiles(const wchar_t* modIni,const wchar_t* engineIni){
    StartupResult result;
    if(!modIni||!engineIni||GetFileAttributesW(modIni)==INVALID_FILE_ATTRIBUTES||
       GetFileAttributesW(engineIni)==INVALID_FILE_ATTRIBUTES){result.error=ERROR_FILE_NOT_FOUND;return result;}
    result.requested=Normalize(static_cast<int>(GetPrivateProfileIntW(L"System",L"GraphicsAdapter",0,modIni)));
    wchar_t value[8]{};swprintf_s(value,L"%d",result.requested);
    if(!WritePrivateProfileStringW(L"SystemSettings",L"r.GraphicsAdapter",value,engineIni)){
        result.error=GetLastError();return result;
    }
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,engineIni);
    result.verified=static_cast<int>(GetPrivateProfileIntW(L"SystemSettings",L"r.GraphicsAdapter",-1,engineIni));
    if(result.verified!=result.requested){result.verified=-1;result.error=ERROR_INVALID_DATA;}
    return result;
}
inline StartupResult Prepare(HMODULE proxy){
    wchar_t modIni[MAX_PATH]{},engineIni[MAX_PATH]{};
    const DWORD written=GetModuleFileNameW(proxy,modIni,MAX_PATH);
    if(!written||written>=MAX_PATH)return {-1,-1,ERROR_INSUFFICIENT_BUFFER};
    auto* slash=wcsrchr(modIni,L'\\');
    if(!slash)return {-1,-1,ERROR_PATH_NOT_FOUND};
    const auto remaining=MAX_PATH-static_cast<size_t>(slash+1-modIni);
    if(wcscpy_s(slash+1,remaining,L"DarksidersGenesisMod.ini"))return {-1,-1,ERROR_INSUFFICIENT_BUFFER};
    const DWORD n=GetEnvironmentVariableW(L"LOCALAPPDATA",engineIni,MAX_PATH);
    if(!n||n>=MAX_PATH||wcscat_s(engineIni,L"\\THQ Nordic\\Darksiders Genesis\\Saved\\Config\\WindowsNoEditor\\Engine.ini"))
        return {-1,-1,ERROR_INSUFFICIENT_BUFFER};
    return PrepareFiles(modIni,engineIni);
}
}
