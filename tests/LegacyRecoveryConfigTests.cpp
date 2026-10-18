#include "ConfigStore.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <iostream>
#include <stdexcept>
namespace {
void Check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
}
int main(){
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    if(!GetTempPathW(MAX_PATH,directory)||!GetTempFileNameW(directory,L"DGR",0,path))return 1;
    try{
        dg::config::Store store;store.SetPath(path);Check(store.Load(),"load config");
        WritePrivateProfileStringW(L"Features",L"ActionRecovery",L"0",path);
        WritePrivateProfileStringW(L"Values",L"ActionRecoveryDelayMs",L"75",path);
        WritePrivateProfileStringW(L"Values",L"ActionRecoveryMultiplier",L"9.5",path);
        WritePrivateProfileStringW(L"Values",L"DodgeEarlyUnlockMs",L"77",path);
        WritePrivateProfileStringW(L"Controls",L"TPSChargedFirePadMask",L"123",path);
        WritePrivateProfileStringW(L"Hotkeys",L"F8",L"ActionRecovery",path);
        Check(store.Load(),"legacy recovery keys do not prevent loading");
        Check(store.hotkeys[7]==dg::config::Action::ActionRecovery&&int(dg::config::Action::ActionRecovery)==3,"old token/ID retained without renumbering active actions");
        Check(store.SaveNow(),"save ignores retired settings");
        Check(GetPrivateProfileIntW(L"Features",L"ActionRecovery",99,path)==0&&
            GetPrivateProfileIntW(L"Values",L"ActionRecoveryDelayMs",99,path)==75,
            "retired personal keys remain byte-value compatible and inactive");
        wchar_t value[64]{};
        GetPrivateProfileStringW(L"Values",L"ActionRecoveryMultiplier",L"",value,64,path);
        Check(wcscmp(value,L"9.5")==0,"retired personal value is preserved without an active compiled setting");
        Check(GetPrivateProfileIntW(L"Values",L"DodgeEarlyUnlockMs",0,path)==77&&GetPrivateProfileIntW(L"Controls",L"TPSChargedFirePadMask",0,path)==123,"unrelated legacy personal keys retained");
        WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
        std::cout<<"Retired recovery key preservation and hotkey compatibility passed\n";return 0;
    }catch(const std::exception& e){WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);std::cerr<<e.what()<<'\n';return 1;}
}
