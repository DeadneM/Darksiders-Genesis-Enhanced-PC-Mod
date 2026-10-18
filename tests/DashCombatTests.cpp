#include "../src/DashCombatPolicy.h"
#include <iostream>
#include <initializer_list>
#include <limits>
#include <stdexcept>
using dg::dash_combat::AimResume;
using dg::dash_combat::Ammo;
using dg::dash_combat::SuppressForcedStandard;
namespace {
void Check(bool ok,const char* reason){if(!ok)throw std::runtime_error(reason);}
AimResume Dash(){AimResume r;r.Observe(1,2,3,.2f,100);return r;}
void Run(){
    AimResume r{};Check(!r.Take(1,2,100,true,true,true),"no repair without an observed dash");
    r=Dash();Check(!r.Take(1,2,120,true,false,true),"native active or paused abilities always win");
    Check(!r.Take(1,2,140,true,true,false),"released native AIM cannot be recreated");
    Check(r.Take(1,2,160,true,true,true),"held AIM can resume after an idle dash");
    Check(!r.Take(1,2,180,true,true,true),"native rejection cannot cause repeated start requests");
    r.Observe(1,2,3,.25f,190);Check(!r.Take(1,2,210,true,true,true),"same activation stays consumed");
    r.Observe(1,2,3,.01f,220);Check(r.Take(1,2,230,true,true,true),"reused dash object with a new clock is a new activation");
    r=Dash();Check(!r.Take(1,9,120,true,true,true)&&!r.seen,"character swap expires old intent");
    r=Dash();Check(!r.Take(9,2,120,true,true,true)&&!r.seen,"local-player ownership cannot cross slots");
    r=Dash();Check(!r.Take(1,2,3101,true,true,true)&&!r.seen,"old observations expire");
    r=Dash();Check(!r.Take(1,2,99,true,true,true)&&!r.seen,"clock rollback expires observations");
    r=Dash();Check(!r.Take(1,2,120,false,true,true)&&!r.seen,"pause/cinema/death discard pending repair");
    AimResume players[2]{Dash(),Dash()};Check(players[0].Take(1,2,120,true,true,true),"P1 request");
    Check(players[1].Take(1,2,120,true,true,true),"P1 request cannot consume P2 history");
    r=Dash();r.Observe(1,2,4,std::numeric_limits<float>::quiet_NaN(),150);Check(!r.seen,"invalid native elapsed time fails closed");
    r=Dash();r.Observe(1,2,0,.2f,150);Check(!r.seen,"null dash identity fails closed");

    Check(SuppressForcedStandard(true,true,true,Ammo::Special,false,Ammo::Unknown),"primary special blocks the animation's forced standard");
    Check(SuppressForcedStandard(true,true,false,Ammo::Unknown,true,Ammo::Special),"secondary special has the same policy");
    Check(SuppressForcedStandard(true,true,true,Ammo::Special,true,Ammo::Special),"two special requests block forced standard");
    for(bool primary:{false,true})for(bool secondary:{false,true}){
        Check(!SuppressForcedStandard(false,true,primary,Ammo::Special,secondary,Ammo::Special),"other animations stay native");
        Check(!SuppressForcedStandard(true,false,primary,Ammo::Special,secondary,Ammo::Special),"nonstandard projectile overrides stay native");
    }
    Check(!SuppressForcedStandard(true,true,false,Ammo::Special,false,Ammo::Special),"no held fire request stays native");
    Check(!SuppressForcedStandard(true,true,true,Ammo::Standard,false,Ammo::Unknown),"standard ammo stays native, including its Hotstreak");
    Check(!SuppressForcedStandard(true,true,true,Ammo::Special,true,Ammo::Standard),"mixed fire is not silently mapped to pistol sockets");
    Check(!SuppressForcedStandard(true,true,true,Ammo::Unknown,false,Ammo::Unknown),"unknown weapon stays native");
}
}
int main(){try{Run();std::cout<<"Dash combat lifecycle and forced-standard boundaries passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
