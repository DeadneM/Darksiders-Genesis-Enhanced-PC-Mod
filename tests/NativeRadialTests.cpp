#include "../src/NativeTpsFeature.cpp"
#include "MouseFireRouting.h"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
alignas(16) unsigned char widgets[2][0x2A0]{},actors[2][0x1000]{},controllers[2][0x400]{},states[2][0x600]{},cls[0x80]{};
void* tree[1]{cls+0x30};
unsigned char items[48]{};void* chunks[1]{items};
dg::local_players::Context owners[2]{};
bool visible[2]{true,true};
unsigned generation[2]{1,1};
int Index(void* object){return object==widgets[1]?1:0;}
template<class T> void Put(void* p,size_t o,T value){std::memcpy(static_cast<unsigned char*>(p)+o,&value,sizeof(value));}
void Check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
bool Visible(void* object){return visible[Index(object)];}
void* Controller(void* object){return controllers[Index(object)];}
void State(void* object,unsigned char state,bool){Put(object,0x254,state);}
void Run(){
    std::vector<unsigned char> image(0x3960000);
    dg::native_tps::g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x380E0B0,static_cast<void*>(cls));Put(cls,0x30,tree+0);Put(cls,0x38,0);
    Put(image.data(),0x3959694,2);Put(image.data(),0x3959680,chunks+0);
    for(int i=0;i<2;++i){
        Put(widgets[i],0x10,static_cast<void*>(cls));Put(widgets[i],0xC,i);
        Put(items,size_t(i)*24,reinterpret_cast<uintptr_t>(widgets[i]));Put(items,size_t(i)*24+16,7);
        Put(controllers[i],0x318,reinterpret_cast<uintptr_t>(states[i]));Put(states[i],0x5C0,reinterpret_cast<uintptr_t>(actors[i]));
        owners[i].index=i;owners[i].localPlayer=0x1000+uintptr_t(i);owners[i].character=reinterpret_cast<uintptr_t>(actors[i]);
        owners[i].controller=reinterpret_cast<uintptr_t>(controllers[i]);
    }
    using namespace dg::native_tps;
    g_ready=true;g_gameThread=GetCurrentThreadId();g_visible=Visible;g_widgetController=Controller;g_widgetState=State;
    ObserveUiEvent(CaptureUiEvent(widgets[0]));
    Check(!RadialActive(0),"constructed visible SHOWN widget does not arm a selector");
    HookWidgetState(widgets[0],1,false);
    Check(RadialActive(0)&&!RadialActive(1),"actual native opening protects only its owning player without any button");
    HookWidgetState(widgets[0],0,false);
    Check(RadialActive(0),"native SHOWN remains selecting after opening");
    HookWidgetState(widgets[1],1,false);
    Check(RadialActive(0)&&RadialActive(1),"two local equipment selectors remain independently owned");
    HookWidgetState(widgets[0],2,false);
    Check(!RadialActive(0)&&RadialActive(1),"native close releases only its owning player's axes");
    generation[1]++;
    Check(!RadialActive(1),"respawn generation expires an old selector");
    HookWidgetState(widgets[1],1,false);Put(items,40,8);
    Check(!RadialActive(1),"recycled widget serial cannot retain selection authority");
    dg::tps_input::MouseFireRouting mouse;
    Check(mouse.Begin(5,1)==1&&mouse.Begin(5,2)==1,"held mouse route survives a binding change");
    Check(mouse.Release(5)==1&&mouse.Release(5)==0,"route releases its original native button exactly once");
    Check(mouse.Begin(1,-1)==-1&&mouse.Release(1)==-1,"suppressed DOWN keeps its UP suppressed");
    Check(mouse.Begin(99,1)==0&&mouse.Release(-1)==0,"invalid physical buttons cannot index routing memory");
}
}
namespace dg::local_players {
bool Ready(){return true;}
bool CameraCinematic(unsigned){return false;}
Slot PublishedSlot(unsigned index){if(index>1)return {};return {owners[index].localPlayer,owners[index].character,int(index),GetTickCount64(),generation[index]};}
bool ResolveActor(void* object,Context& out){for(int i=0;i<2;++i)if(object==actors[i]){out=owners[i];return true;}return false;}
}
int main(){try{Run();std::cout<<"Native radial lifecycle/owners and held mouse release passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
