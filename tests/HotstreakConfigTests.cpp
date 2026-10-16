#include "ConfigStore.h"
#include "RuntimeSettings.h"
#include "HotstreakPolicy.h"
#include "RefillRequest.h"
#include <windows.h>
#include <cstdio>
#include <limits>
int main(){
    using namespace dg::config;
    static_assert(int(Action::UnlimitedHorseSprint)==43&&int(Action::ReduceHotstreak)==44&&
        int(Action::UnlimitedHotstreak)==45&&int(Action::RefillHotstreak)==46&&int(Action::RefillAmmo)==47&&int(Action::Count)==48);
    unsigned failed=0;auto check=[&](bool value,const char* message){if(!value){++failed;std::printf("FAIL %s\n",message);}};
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    if(!GetTempPathW(MAX_PATH,directory)||!GetTempFileNameW(directory,L"DGH",0,path))return 1;
    Store a;a.SetPath(path);
    check(!a.reduceHotstreakEnabled&&!a.unlimitedHotstreakEnabled&&a.hotstreakDecreaseDivisor==2,"new modifiers default OFF");
    check(a.hotkeys==kDefaultHotkeys,"no established hotkey is reassigned");
    a.reduceHotstreakEnabled=true;a.unlimitedHotstreakEnabled=true;a.hotstreakDecreaseDivisor=2.5f;
    a.hotkeys[8]=Action::ReduceHotstreak;a.hotkeys[9]=Action::UnlimitedHotstreak;
    a.hotkeys[10]=Action::RefillHotstreak;a.hotkeys[11]=Action::RefillAmmo;
    check(a.SaveNow(),"new settings and four appended actions save");
    Store b;b.SetPath(path);check(b.Load(),"saved configuration loads");
    check(b.reduceHotstreakEnabled&&b.unlimitedHotstreakEnabled&&b.hotstreakDecreaseDivisor==2.5f&&b.hotkeys==a.hotkeys,"real INI round-trip retains four actions and exact divisor");
    const auto& rt=dg::runtime::Get();check(rt.reduceHotstreakEnabled.load()&&rt.unlimitedHotstreakEnabled.load()&&rt.hotstreakDecreaseDivisor.load()==2.5f,"native runtime receives all settings");
    WritePrivateProfileStringW(L"Values",L"HotstreakDecreaseDivisor",L"999",path);b.Load();check(b.hotstreakDecreaseDivisor==10,"upper bound");
    WritePrivateProfileStringW(L"Values",L"HotstreakDecreaseDivisor",L"-5",path);b.Load();check(b.hotstreakDecreaseDivisor==0,"lower bound");
    WritePrivateProfileStringW(L"Values",L"HotstreakDecreaseDivisor",L"nan",path);b.Load();check(b.hotstreakDecreaseDivisor==2,"nonfinite uses safe default");
    WritePrivateProfileStringW(L"Features",L"ReduceHotstreakDecrease",nullptr,path);
    WritePrivateProfileStringW(L"Features",L"UnlimitedHotstreak",nullptr,path);
    WritePrivateProfileStringW(L"Values",L"HotstreakDecreaseDivisor",nullptr,path);
    b.Load();check(!b.reduceHotstreakEnabled&&!b.unlimitedHotstreakEnabled&&b.hotstreakDecreaseDivisor==2&&b.hotkeys==a.hotkeys,"legacy profile opts out without replacing hotkeys");
    b.ResetDefaults(false);check(!b.reduceHotstreakEnabled&&!b.unlimitedHotstreakEnabled&&b.hotstreakDecreaseDivisor==2,"reset preserves opt-in policy");
    using dg::hotstreak::DepletionAmount;using dg::hotstreak::DepletionCaller;
    check(DepletionAmount(10,true,0,false)==10&&DepletionAmount(10,true,1,false)==10,"0 and 1 remain native");
    check(DepletionAmount(10,true,2.5f,false)==4&&DepletionAmount(10,true,10,false)==1,"fractional and tenth consumption");
    check(DepletionAmount(10,false,10,false)==10&&DepletionAmount(10,true,10,true)==0,"OFF and Unlimited priority");
    check(DepletionAmount(-5,true,10,true)==-5&&DepletionAmount(10,true,std::numeric_limits<float>::infinity(),false)==10,"negative amounts and invalid factor remain native");
    check(DepletionCaller(0x68B007)&&DepletionCaller(0x68BA89)&&DepletionCaller(0x67E81E)&&DepletionCaller(0x793580),"audited decay and costs are eligible");
    check(!DepletionCaller(0x67892E)&&!DepletionCaller(0x678A93)&&!DepletionCaller(0),"explicit native resets and unknown callers are excluded");
    dg::refill::Request request{};request.Observe(100,2,1000,true);
    check(request.Queue(1100)&&!request.Queue(1101),"one request per live identity");
    check(!request.Take(100,2,1200,false)&&request.requestedActor==100,"pause waits without executing native code");
    check(request.Take(100,2,1300,true)&&!request.Take(100,2,1301,true),"request consumed exactly once");
    request.Complete(true);check(request.result==dg::refill::Result::Applied,"readback completion reported");
    request.Observe(100,2,2000,true);request.Queue(2001);request.Observe(101,3,2100,true);
    check(!request.requestedActor&&request.result==dg::refill::Result::Cancelled,"respawn cannot transfer a queued refill");
    request.Queue(2200);request.Expire(7201);check(!request.requestedActor,"request expires after five seconds");
    request.Observe(101,3,8000,true);check(!request.Queue(9500),"stale metadata cannot queue native work");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
    std::printf("Hotstreak settings, depletion and refill lifecycle: %u failures\n",failed);return failed?1:0;
}
