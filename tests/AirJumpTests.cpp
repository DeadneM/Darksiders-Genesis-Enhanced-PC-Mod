#include "../src/AirJumpFeature.cpp"
#include <iostream>
#include <stdexcept>
namespace {
unsigned char actors[2][0x1100]{},movement[2][0xA10]{},meshes[2][0x730]{},anims[2][0x320]{};
unsigned calls=0,impulses=0,inputCalls=0,stopCalls=0,setCalls=0;
bool inputSucceeded=false,local=true,nativeAllowed=true,paused=false,cinematic=false,rejectAirSetter=false;
dg::local_players::Context contexts[2]{};dg::local_players::Slot slots[2]{};
template<class T> void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
template<class T> T Get(void* p,size_t at){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+at,sizeof(value));return value;}
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
int NativeCount(void* p){return Get<int>(Get<void*>(p,0x740),0x974);}
void NativeSetCount(void* p,int count){++setCalls;Put(Get<void*>(p,0x740),0x974,count);}
void NativeSetAirCount(void* p,int count){if(!rejectAirSetter)Put<unsigned char>(p,0xA00,static_cast<unsigned char>(count));}
bool NativeJump(void* p,bool replay){
    ++calls;Check(replay,"native boolean argument stays intact");
    if(!nativeAllowed)return false;
    const int count=dg::air_jump::Count(p,0x69FCB5);if(count>=2)return false;
    if(count==0||Get<float>(p,0x860)>=Get<float>(p,0x9E8))++impulses;
    NativeSetCount(p,count+1);NativeSetAirCount(p,2);return true;
}
void Observe(unsigned i){dg::air_jump::OnLivePlayer(contexts[i],slots[i].generation);}
int AnimationUpdate(unsigned i){
    const int count=dg::air_jump::Count(movement[i],0x68D65B);
    Put(anims[i],0x2E8,count);return count;
}
void Released(unsigned i){Put<unsigned char>(actors[i],0xE10+30,0);Put<unsigned char>(actors[i],0x430,0);}
void Held(unsigned i){Put<unsigned char>(actors[i],0xE10+30,1);Put<unsigned char>(actors[i],0x430,4);}
bool Jump(unsigned i){return dg::air_jump::HookJump(movement[i],true);}
void BeginGlide(unsigned i){
    Put<unsigned char>(movement[i],0x1B0,6);Put<unsigned char>(movement[i],0x1B1,6);
    Put<unsigned char>(actors[i],0xD14,1);Observe(i);
}
void NativeStopGlide(void* actor){
    ++stopCalls;
    for(unsigned i=0;i<2;++i)if(actor==actors[i]){
        Put<unsigned char>(movement[i],0x1B0,3);Put<unsigned char>(movement[i],0x1B1,0);Put<unsigned char>(actors[i],0xD14,0);
    }
}
void NativeInput(void* actor){
    ++inputCalls;inputSucceeded=false;
    for(unsigned i=0;i<2;++i)if(actor==actors[i]&&!(Get<unsigned char>(actors[i],0xD14)&1)){
        Held(i);inputSucceeded=Jump(i);
    }
}
void Exhaust(unsigned i){
    Put(actors[i],0x974,2);Put<unsigned char>(movement[i],0xA00,2);Put(anims[i],0x2E8,2);
    Put<unsigned char>(movement[i],0x1B0,3);Put<unsigned char>(actors[i],0xD14,0);Released(i);
}
void Run(){
    using namespace dg::air_jump;
    g_base=0x140000000;g_ready=true;g_count=NativeCount;g_jump=NativeJump;g_input=NativeInput;
    g_stopGlide=NativeStopGlide;g_setCount=NativeSetCount;g_setAirCount=NativeSetAirCount;
    for(unsigned i=0;i<2;++i){
        auto& c=contexts[i];c.index=int(i);c.character=reinterpret_cast<uintptr_t>(actors[i]);c.kind=dg::local_players::PoseKind::Foot;slots[i].generation=7+i;
        Put(actors[i],0x388,static_cast<void*>(movement[i]));Put<unsigned char>(actors[i],0x118,3);
        Put(movement[i],0x850,static_cast<void*>(actors[i]));Put(movement[i],0x740,static_cast<void*>(actors[i]));
        Put(actors[i],0x380,static_cast<void*>(meshes[i]));Put(meshes[i],0x718,static_cast<void*>(anims[i]));Exhaust(i);
    }
    auto& rt=dg::runtime::Get();rt.infiniteDoubleJumpMode=0;Observe(0);
    Check(AnimationUpdate(0)==2&&!Jump(0),"Off retains native count and two-jump limit");
    rt.infiniteDoubleJumpMode=2;Observe(0);Observe(1);
    Held(0);Check(AnimationUpdate(0)==2,"held native jump action cannot rearm or erase its animation");Released(0);
    Put(anims[0],0x2E8,1);Check(AnimationUpdate(0)==2,"animation must first observe the successful double jump");
    const unsigned before=impulses;
    for(unsigned i=0;i<32;++i){
        const unsigned player=i%2;Released(player);
        Check(AnimationUpdate(player)==1,"animation receives the actual available count between consecutive jumps");
        Check(NativeCount(movement[player])==1&&Get<unsigned char>(movement[player],0xA00)==1,"animation and automatic glide use the same rearmed cycle");
        Check(Count(movement[player],0x69B767)==1&&Count(movement[player],0x69FCB5)==1,"all native consumers see the same real count, no spoofing");
        Put(movement[player],0x860,.2f);Put(movement[player],0x9E8,float(i+1));Held(player);
        Check(Jump(player)&&AnimationUpdate(player)==2,"native successful extra jump republishes two to animation");
        Check(Get<float>(movement[player],0x860)==.2f,"scoped deadline restored");
    }
    Check(impulses-before==32,"all admitted late extra jumps keep the native impulse");
    // These are native state/count contracts, not a replacement for checking
    // the actual rendered animation and long sequences in the shipping game.
    Released(0);Check(AnimationUpdate(0)==1,"prepare pending jump");
    rt.infiniteDoubleJumpMode=0;Observe(0);
    Check(NativeCount(movement[0])==2&&Get<unsigned char>(movement[0],0xA00)==2&&!Jump(0),"OFF restores an unused owned rearm");
    rt.infiniteDoubleJumpMode=2;Observe(0);AnimationUpdate(0);Put(actors[0],0x974,0);Put<unsigned char>(movement[0],0xA00,0);
    rt.infiniteDoubleJumpMode=0;Observe(0);Check(NativeCount(movement[0])==0,"landing values win over restoration");
    rt.infiniteDoubleJumpMode=2;Observe(0);Exhaust(0);rejectAirSetter=true;
    Check(AnimationUpdate(0)==2&&!g_players[0].armed,"partial native pair rejection restores the original count");rejectAirSetter=false;
    nativeAllowed=false;AnimationUpdate(0);Check(!Jump(0)&&g_players[0].armed,"native ability locks remain authoritative and preserve pending credit");nativeAllowed=true;
    Held(0);Check(Jump(0),"a later allowed request consumes the pending jump");Released(0);Put(anims[0],0x2E8,2);
    local=false;Check(AnimationUpdate(0)==2&&!Jump(0),"foreign actors stay native");local=true;
    paused=true;Check(AnimationUpdate(0)==2,"pause cannot rearm");paused=false;
    cinematic=true;Check(AnimationUpdate(0)==2,"cinematics cannot rearm");cinematic=false;
    Put<unsigned char>(actors[0],0xD14,8);Check(AnimationUpdate(0)==2,"ledge special modes stay native");Put<unsigned char>(actors[0],0xD14,0);
    Put<unsigned char>(actors[0],0x118,2);Check(AnimationUpdate(0)==2,"non-authority owner stays native");Put<unsigned char>(actors[0],0x118,3);
    contexts[0].kind=dg::local_players::PoseKind::Horse;Check(AnimationUpdate(0)==2,"mount excluded");contexts[0].kind=dg::local_players::PoseKind::Foot;
    Put(actors[0],0x974,3);Check(AnimationUpdate(0)==3&&!Jump(0),"unexpected custom counts fail closed");Exhaust(0);
    rt.infiniteDoubleJumpMode=1;Observe(0);Observe(1);Check(AnimationUpdate(0)==2&&!Jump(0),"After gliding starts without a credit");
    BeginGlide(0);const unsigned ins=inputCalls,stops=stopCalls;HookInput(actors[0]);
    Check(inputSucceeded&&inputCalls==ins+1&&stopCalls==stops+1,"native request exits owned glide and requests exactly one native jump");
    Check(!g_players[0].credit&&!g_players[0].armed,"only successful native jump consumes the glide credit");
    Released(0);Put(anims[0],0x2E8,2);Check(AnimationUpdate(0)==2&&!Jump(0),"cannot repeat After gliding without a new glide");
    Check(AnimationUpdate(1)==2,"P1 glide cannot grant P2 a jump");
    Observe(0);BeginGlide(0);nativeAllowed=false;HookInput(actors[0]);
    Check(!inputSucceeded&&g_players[0].credit&&g_players[0].armed,"rejected native request retains the glide credit");
    nativeAllowed=true;HookInput(actors[0]);Check(inputSucceeded&&!g_players[0].credit,"later native success consumes retained credit");
    Released(0);BeginGlide(0);NativeStopGlide(actors[0]);Observe(0);++slots[0].generation;
    Check(AnimationUpdate(0)==2,"new generation discards the previous glide credit");
    rt.infiniteDoubleJumpMode=0;BeginGlide(0);const unsigned stopped=stopCalls;HookInput(actors[0]);
    Check(stopCalls==stopped&&!inputSucceeded,"Off preserves native glide and input");
    rt.infiniteDoubleJumpMode=2;paused=true;HookInput(actors[0]);Check(stopCalls==stopped,"pause cannot stop a glide");paused=false;
    local=false;HookInput(actors[0]);Check(stopCalls==stopped,"foreign actors cannot stop a glide");local=true;
    for(unsigned i=0;i<2;++i){Exhaust(i);Observe(i);}
    const unsigned readonly=setCalls;Count(movement[0],0x123456);Check(setCalls==readonly,"unrelated getter calls never rearm");
}
}
namespace dg::native_tps {bool OnGameThread(){return true;}bool CinematicActive(unsigned){return cinematic;}bool IsWorldPaused(void*){return paused;}}
namespace dg::local_players {
bool ResolveActor(void* p,Context& c){if(!local)return false;for(unsigned i=0;i<2;++i)if(p==actors[i]){c=contexts[i];return true;}return false;}
Slot PublishedSlot(unsigned i){return i<2?slots[i]:Slot{};}
}
int main(){try{Run();std::cout<<"Native air-jump cycle, animation count publication, paired glide state, rollback and local isolation passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
