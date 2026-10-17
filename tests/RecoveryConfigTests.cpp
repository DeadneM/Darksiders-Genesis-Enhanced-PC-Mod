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
        Check(store.Load()&&!store.actionRecoveryEnabled&&store.actionRecoveryDelayMs==75,"existing recovery toggle and delay load");
        Check(store.hotkeys[7]==dg::config::Action::ActionRecovery&&int(dg::config::Action::ActionRecovery)==3,"old hotkey token and ID retained");
        store.actionRecoveryEnabled=true;Check(store.SaveNow(),"save reactivated recovery");
        Check(dg::runtime::Get().actionRecoveryEnabled.load()&&dg::runtime::Get().actionRecoveryDelayMs.load()==75,"functional settings published to native callback");
        wchar_t value[64]{};
        GetPrivateProfileStringW(L"Values",L"ActionRecoveryMultiplier",L"",value,64,path);
        Check(wcscmp(value,L"9.5")==0,"retired personal value is preserved without an active compiled setting");
        Check(GetPrivateProfileIntW(L"Values",L"DodgeEarlyUnlockMs",0,path)==77&&GetPrivateProfileIntW(L"Controls",L"TPSChargedFirePadMask",0,path)==123,"unrelated legacy personal keys retained");
        WritePrivateProfileStringW(L"Values",L"ActionRecoveryDelayMs",L"nan",path);Check(store.Load()&&store.actionRecoveryDelayMs==0,"invalid delay becomes safe default");
        WritePrivateProfileStringW(L"Values",L"ActionRecoveryDelayMs",L"900",path);Check(store.Load()&&store.actionRecoveryDelayMs==300,"delay upper bound");
        WritePrivateProfileStringW(L"Values",L"ActionRecoveryDelayMs",L"-9",path);Check(store.Load()&&store.actionRecoveryDelayMs==0,"delay lower bound");
        WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
        std::cout<<"Recovery settings, legacy file preservation and hotkey compatibility passed\n";return 0;
    }catch(const std::exception& e){WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);std::cerr<<e.what()<<'\n';return 1;}
}
