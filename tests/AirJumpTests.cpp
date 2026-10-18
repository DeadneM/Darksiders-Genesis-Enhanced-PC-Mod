#include "../src/AirJumpFeature.cpp"
#include <iostream>
#include <stdexcept>
namespace {
unsigned char actors[2][0x1100]{},movement[2][0xA10]{};unsigned calls=0;bool local=true,nativeAllowed=true,paused=false;
dg::local_players::Context contexts[2]{};dg::local_players::Slot slots[2]{};
template<class T> void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
template<class T> T Get(void* p,size_t at){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+at,sizeof(value));return value;}
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
int NativeCount(void* p){auto* actor=Get<void*>(p,0x850);return Get<int>(actor,0x974);}
bool NativeJump(void* p,bool replay){
    ++calls;Check(replay,"native boolean argument stays intact");
    if(!nativeAllowed)return false; // native non-count restrictions still decide
    const int count=dg::air_jump::Count(p,0x69FCB5);if(count>=2)return false;
    Put(Get<void*>(p,0x850),0x974,count+1);return true;
}
void Observe(unsigned i){dg::air_jump::OnLivePlayer(contexts[i],slots[i].generation);}
bool Jump(unsigned i){return dg::air_jump::HookJump(movement[i],true);}
void Run(){
    using namespace dg::air_jump;g_base=0x140000000;g_ready=true;g_count=NativeCount;g_jump=NativeJump;
    for(unsigned i=0;i<2;++i){
        auto& c=contexts[i];c.index=int(i);c.character=reinterpret_cast<uintptr_t>(actors[i]);c.kind=dg::local_players::PoseKind::Foot;slots[i].generation=7+i;
        Put(actors[i],0x388,static_cast<void*>(movement[i]));Put<unsigned char>(actors[i],0x118,3);
        Put(movement[i],0x850,static_cast<void*>(actors[i]));Put<unsigned char>(movement[i],0x1B0,3);Put(actors[i],0x974,2);
    }
    auto& rt=dg::runtime::Get();rt.infiniteDoubleJumpMode=0;Observe(0);Check(!Jump(0),"Off retains native two-jump limit");
    rt.infiniteDoubleJumpMode=2;Observe(0);Observe(1);
    for(unsigned i=0;i<8;++i)Check(Jump(i%2),"Always allows repeated native double jumps for each local player");
    Check(NativeCount(movement[0])==2&&Count(movement[0],0x69FCB5)==2,"real replicated count and outside reads remain native");
    nativeAllowed=false;Check(!Jump(0),"does not bypass native action locks");nativeAllowed=true;
    local=false;Check(!Jump(0),"foreign actors stay native");local=true;
    paused=true;Check(!Jump(0),"pause stays native");paused=false;
    rt.infiniteDoubleJumpMode=1;Observe(0);Observe(1);Check(!Jump(0),"After gliding starts with no extra jump");
    Put<unsigned char>(actors[0],0xD14,1);Observe(0);Check(Jump(0),"glide edge rearms one successful double jump");
    Observe(0);Check(!Jump(0),"continuous glide flag cannot rearm repeatedly");Check(!Jump(1),"P1 glide cannot grant P2 a jump");
    Put<unsigned char>(actors[0],0xD14,0);Observe(0);Put<unsigned char>(actors[0],0xD14,1);Observe(0);
    nativeAllowed=false;Check(!Jump(0),"a rejected native action preserves its glide credit");nativeAllowed=true;Check(Jump(0),"credit is consumed only on native success");
    Put<unsigned char>(movement[0],0x1B0,1);Observe(0);Put<unsigned char>(movement[0],0x1B0,3);Check(!Jump(0),"landing clears unused credit");
    rt.infiniteDoubleJumpMode=2;Observe(0);++slots[0].generation;Check(!Jump(0),"respawn invalidates old mode context before next live observation");
    Observe(0);contexts[0].kind=dg::local_players::PoseKind::Horse;Check(!Jump(0),"horse excluded");
    Check(g_scope.component==nullptr&&!g_scope.allow,"scope is restored after every native call");
}
}
namespace dg::native_tps {bool OnGameThread(){return true;}bool CinematicActive(unsigned){return false;}bool IsWorldPaused(void*){return paused;}}
namespace dg::local_players {
bool ResolveActor(void* p,Context& c){if(!local)return false;for(unsigned i=0;i<2;++i)if(p==actors[i]){c=contexts[i];return true;}return false;}
Slot PublishedSlot(unsigned i){return i<2?slots[i]:Slot{};}
}
int main(){try{Run();std::cout<<"Native air-jump wrapper, glide credits and local isolation passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
