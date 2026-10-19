#include "../src/AirJumpFeature.cpp"
#include <iostream>
#include <stdexcept>
namespace {
unsigned char actors[2][0x1100]{},movement[2][0xA10]{};unsigned calls=0,impulses=0,inputCalls=0,stopCalls=0;bool inputSucceeded=false;bool local=true,nativeAllowed=true,paused=false,cinematic=false;
dg::local_players::Context contexts[2]{};dg::local_players::Slot slots[2]{};
template<class T> void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
template<class T> T Get(void* p,size_t at){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+at,sizeof(value));return value;}
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
int NativeCount(void* p){auto* actor=Get<void*>(p,0x850);return Get<int>(actor,0x974);}
bool NativeJump(void* p,bool replay){
    ++calls;Check(replay,"native boolean argument stays intact");
    if(!nativeAllowed)return false; // native non-count restrictions still decide
    const int count=dg::air_jump::Count(p,0x69FCB5);if(count>=2)return false;
    // Native 6A017A can return success without an impulse after its deadline.
    if(count==0||Get<float>(p,0x860)>=Get<float>(p,0x9E8))++impulses;
    Put(Get<void*>(p,0x850),0x974,count+1);return true;
}
void Observe(unsigned i){dg::air_jump::OnLivePlayer(contexts[i],slots[i].generation);}
void BeginGlide(unsigned i){
    Put<unsigned char>(movement[i],0x1B0,6);Put<unsigned char>(movement[i],0x1B1,6);
    Put<unsigned char>(actors[i],0xD14,1);Observe(i);
}
void EndGlide(unsigned i){
    Put<unsigned char>(movement[i],0x1B0,3);Put<unsigned char>(movement[i],0x1B1,0);
    Put<unsigned char>(actors[i],0xD14,0);Observe(i);
}
bool Jump(unsigned i){return dg::air_jump::HookJump(movement[i],true);}
bool Request(unsigned i){
    // Separate native admission stage: it must pass before DoJump is called.
    // This reproduces the missing upstream gate; it is not an end-to-end
    // simulation of the engine's animation, input or movement system.
    if(Get<unsigned char>(actors[i],0xD14)&1)return false;
    if(dg::air_jump::Count(movement[i],0x69B78D)>=3)return false;
    return Jump(i);
}
void NativeStopGlide(void* actor){
    ++stopCalls;
    for(unsigned i=0;i<2;++i)if(actor==actors[i])EndGlide(i);
}
void NativeInput(void* actor){
    ++inputCalls;inputSucceeded=false;
    for(unsigned i=0;i<2;++i)if(actor==actors[i])inputSucceeded=Request(i);
}
void Run(){
    using namespace dg::air_jump;g_base=0x140000000;g_ready=true;g_count=NativeCount;g_jump=NativeJump;g_input=NativeInput;g_stopGlide=NativeStopGlide;
    for(unsigned i=0;i<2;++i){
        auto& c=contexts[i];c.index=int(i);c.character=reinterpret_cast<uintptr_t>(actors[i]);c.kind=dg::local_players::PoseKind::Foot;slots[i].generation=7+i;
        Put(actors[i],0x388,static_cast<void*>(movement[i]));Put<unsigned char>(actors[i],0x118,3);
        Put(movement[i],0x850,static_cast<void*>(actors[i]));Put<unsigned char>(movement[i],0x1B0,3);Put(actors[i],0x974,2);
    }
    auto& rt=dg::runtime::Get();rt.infiniteDoubleJumpMode=0;Observe(0);Check(!Jump(0),"Off retains native two-jump limit");
    rt.infiniteDoubleJumpMode=2;Observe(0);Observe(1);
    Put(actors[0],0x974,2);Put(actors[1],0x974,2);
    Check(Count(movement[0],0x69B767)==2&&Count(movement[0],0x69B78D)==2,"all admission and restricted-action reads stay native");
    const unsigned before=impulses;
    for(unsigned i=0;i<32;++i){
        const unsigned player=i%2;const float delay=.2f,elapsed=float(i+1);
        Put(movement[player],0x860,delay);Put(movement[player],0x9E8,elapsed);
        Check(Request(player),"Always passes admission and repeats the native action for both players");
        Check(Get<float>(movement[player],0x860)==delay,"extra-jump deadline restored after every call");
    }
    Check(impulses-before==32,"every admitted extra jump receives the native impulse beyond its old deadline");
    Put(actors[0],0x974,3);Check(!Request(0),"unexpected custom counter values fail closed");Put(actors[0],0x974,2);
    Check(NativeCount(movement[0])==2&&Count(movement[0],0x69FCB5)==2,"real replicated count and outside reads remain native");
    nativeAllowed=false;Check(!Jump(0),"does not bypass native action locks");nativeAllowed=true;
    local=false;Check(!Jump(0),"foreign actors stay native");local=true;
    paused=true;Check(!Jump(0),"pause stays native");paused=false;
    rt.infiniteDoubleJumpMode=1;Observe(0);Observe(1);Check(!Jump(0),"After gliding starts with no extra jump");
    BeginGlide(0);Check(!Jump(0),"gliding itself retains native jump restrictions");
    Observe(0);EndGlide(0);Check(Jump(0),"native custom glide grants one jump after returning to falling");
    Observe(0);Check(!Jump(0),"the used credit cannot rearm without another glide");Check(!Jump(1),"P1 glide cannot grant P2 a jump");
    BeginGlide(0);EndGlide(0);
    nativeAllowed=false;Check(!Jump(0),"a rejected native action preserves its glide credit");nativeAllowed=true;Check(Jump(0),"credit is consumed only on native success");
    Put<unsigned char>(movement[0],0x1B0,1);Observe(0);Put<unsigned char>(movement[0],0x1B0,3);Check(!Jump(0),"landing clears unused credit");
    rt.infiniteDoubleJumpMode=2;Observe(0);++slots[0].generation;Check(Jump(0),"Always resolves the current owner without waiting for a camera observation");
    cinematic=true;Check(!Jump(0),"cinematics retain native limits");cinematic=false;
    Put<unsigned char>(actors[0],0x118,2);Check(!Jump(0),"non-authority owner excluded");Put<unsigned char>(actors[0],0x118,3);
    rt.infiniteDoubleJumpMode=1;Observe(0);BeginGlide(0);EndGlide(0);
    ++slots[0].generation;Check(!Jump(0),"respawn invalidates a glide credit before the next observation");
    rt.infiniteDoubleJumpMode=2;
    Observe(0);contexts[0].kind=dg::local_players::PoseKind::Horse;Check(!Jump(0),"horse excluded");
    contexts[0].kind=dg::local_players::PoseKind::Foot;
    const unsigned startInputs=inputCalls,startStops=stopCalls;
    for(unsigned i=0;i<16;++i){
        const unsigned player=i%2;BeginGlide(player);HookInput(actors[player]);
        Check(inputSucceeded,"Always ends an owned native glide and lets the native jump request succeed");
    }
    Check(inputCalls-startInputs==16&&stopCalls-startStops==16,"one native input and glide exit per request, both players");
    rt.infiniteDoubleJumpMode=1;Observe(0);BeginGlide(0);HookInput(actors[0]);
    Check(inputSucceeded,"After gliding consumes one observed native glide credit");
    HookInput(actors[0]);Check(!inputSucceeded,"After gliding cannot repeat without a new glide");
    BeginGlide(0);nativeAllowed=false;HookInput(actors[0]);Check(!inputSucceeded,"native ability locks still decide after glide exit");
    nativeAllowed=true;HookInput(actors[0]);Check(inputSucceeded,"rejected request preserves credit");
    rt.infiniteDoubleJumpMode=0;BeginGlide(0);const unsigned stopped=stopCalls;HookInput(actors[0]);
    Check(stopCalls==stopped&&!inputSucceeded,"Off preserves gliding and calls only native input");
    rt.infiniteDoubleJumpMode=2;paused=true;HookInput(actors[0]);Check(stopCalls==stopped,"pause cannot end a glide");paused=false;
    local=false;HookInput(actors[0]);Check(stopCalls==stopped,"foreign players cannot end a glide");local=true;
    Check(g_scope.component==nullptr&&!g_scope.allow,"scope is restored after every native call");
}
}
namespace dg::native_tps {bool OnGameThread(){return true;}bool CinematicActive(unsigned){return cinematic;}bool IsWorldPaused(void*){return paused;}}
namespace dg::local_players {
bool ResolveActor(void* p,Context& c){if(!local)return false;for(unsigned i=0;i<2;++i)if(p==actors[i]){c=contexts[i];return true;}return false;}
Slot PublishedSlot(unsigned i){return i<2?slots[i]:Slot{};}
}
int main(){try{Run();std::cout<<"Native air-jump wrapper, glide credits and local isolation passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
