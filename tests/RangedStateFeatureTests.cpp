#include "../src/RangedStateFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
using namespace dg::ranged_state;
struct Fixture {
    unsigned char actor[0x1100]{},controller[0x400]{},resource[0x400]{},movement[0x200]{},component[0x120]{};
    unsigned char cls[0x100]{},package[0x80]{},function[0x100]{},property[0x80]{},parameter[0x80]{},kind[0x80]{},functionKind[0x80]{},table[0x20]{};
    unsigned char code[30]{},items[4*24]{},entries[8][128]{};
    uintptr_t names[8]{},chunks[1]{},objects[1]{};
} f;
dg::local_players::Context context{};
unsigned calls=0;bool held=false,thread=true,pause=false,cinema=false,radial=false,reenter=false;
template<class T>void Put(void* p,size_t at,T v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
void Check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
void Label(void* object,int index,const char* name){Put(object,0x18,index);strcpy_s(reinterpret_cast<char*>(f.entries[index]+12),116,name);}
void Live(void* object,int index){Put(object,0xC,index);Put(f.items,size_t(index)*24,reinterpret_cast<uintptr_t>(object));Put(f.items,size_t(index)*24+16,index+1);}
unsigned char QueryAction(void*,unsigned char){return held?1:0;}
void NativeRelease(void* actor,void* function,void* args,void* result){
    Check(actor==f.actor&&function==f.function&&!result&&*static_cast<unsigned char*>(args)==0,"only verified setter receives zero on current actor");
    ++calls;Check(g_servicing,"native dispatch has a reentrancy fence");
    if(reenter)OnLivePlayer(context,1,true);
    Put<unsigned char>(actor,0x1098,0);
}
void Fresh(std::vector<unsigned char>& image){
    f={};calls=0;held=pause=cinema=radial=reenter=false;thread=true;
    g_class={};g_setter={};g_players={};g_mode=-1;g_servicing=false;g_requests=0;
    g_base=reinterpret_cast<uintptr_t>(image.data());g_ready=true;g_query=QueryAction;g_event=NativeRelease;
    for(int i=0;i<8;++i)f.names[i]=reinterpret_cast<uintptr_t>(f.entries[i]);
    f.chunks[0]=reinterpret_cast<uintptr_t>(f.names);f.objects[0]=reinterpret_cast<uintptr_t>(f.items);
    Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(f.chunks));
    Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(f.objects));Put(image.data(),0x3959694,4);
    Label(f.cls,0,"Strife_Blueprint_C");Label(f.package,1,"/Game/Blueprints/Strife_Blueprint");Label(f.kind,2,"ByteProperty");
    Label(f.function,3,"SetAimMode");Label(f.property,4,"AimMode");Label(f.parameter,5,"mode");
    Label(f.functionKind,6,"Function");Put(f.function,0x10,reinterpret_cast<uintptr_t>(f.functionKind));
    Put(f.cls,0x20,reinterpret_cast<uintptr_t>(f.package));Put(f.cls,0x48,reinterpret_cast<uintptr_t>(f.function));
    Put(f.function,0x20,reinterpret_cast<uintptr_t>(f.cls));Put(f.function,0x28,reinterpret_cast<uintptr_t>(f.property));
    Put(f.function,0x48,reinterpret_cast<uintptr_t>(f.parameter));
    for(auto* p:{f.property,f.parameter}){Put(p,0x10,reinterpret_cast<uintptr_t>(f.kind));Put(p,0x34,1);}
    Put(f.property,0x20,reinterpret_cast<uintptr_t>(f.cls));Put(f.property,0x44,0x1098);
    Put(f.parameter,0x20,reinterpret_cast<uintptr_t>(f.function));
    const unsigned char code[]={15,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,11,83};
    std::memcpy(f.code,code,sizeof(code));Put(f.code,1,reinterpret_cast<uintptr_t>(f.property));Put(f.code,10,reinterpret_cast<uintptr_t>(f.property));Put(f.code,19,reinterpret_cast<uintptr_t>(f.parameter));
    Put(f.function,0x58,reinterpret_cast<uintptr_t>(f.code));Put(f.function,0x60,30);Put(f.function,0x64,30);
    Live(f.actor,0);Live(f.cls,1);Live(f.function,2);
    Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.cls));Put<unsigned char>(f.actor,0x118,3);
    Put(f.actor,0x348,reinterpret_cast<uintptr_t>(f.controller));Put(f.controller,0x350,reinterpret_cast<uintptr_t>(f.actor));
    Put(f.actor,0xEA0,reinterpret_cast<uintptr_t>(f.resource));Put(f.resource,0x37C,100.f);
    Put(f.actor,0xC70,reinterpret_cast<uintptr_t>(f.component));Put(f.component,0xE8,reinterpret_cast<uintptr_t>(f.actor));
    Put(f.actor,0x388,reinterpret_cast<uintptr_t>(f.movement));Put<unsigned char>(f.movement,0x1B0,1);
    Put(f.actor,0xE90,-1);Put<unsigned char>(f.actor,0x1098,1);
    Put(f.actor,0x8D0,reinterpret_cast<uintptr_t>(f.table));Put(f.table,0x18,reinterpret_cast<uintptr_t>(&QueryAction));
    context={};context.index=0;context.kind=dg::local_players::PoseKind::Foot;
    context.localPlayer=1;context.world=2;context.character=context.pawn=reinterpret_cast<uintptr_t>(f.actor);context.controller=reinterpret_cast<uintptr_t>(f.controller);
}
void Run(){
    std::vector<unsigned char> image(0x3960000);
    Fresh(image);reenter=true;OnLivePlayer(context,1,true);OnLivePlayer(context,1,true);
    Check(calls==1&&g_mode==0x1098,"mixed-case native Mode resolves and orphan mode requests one native release");
    for(int guard=0;guard<15;++guard){
        Fresh(image);bool allowed=true;
        switch(guard){
        case 0:held=true;break;
        case 1:Put(f.component,0x100,1);Put(f.component,0x104,1);break;
        case 2:Put(f.actor,0xE90,0);break;
        case 3:Put<unsigned char>(f.movement,0x1B0,3);break;
        case 4:pause=true;break;
        case 5:cinema=true;break;
        case 6:radial=true;break;
        case 7:allowed=false;break;
        case 8:context.kind=dg::local_players::PoseKind::Horse;break;
        case 9:context.world=0;break;
        case 10:context.controller=0;break;
        case 11:Put(f.resource,0x37C,0.f);break;
        case 12:thread=false;break;
        case 13:Put<unsigned char>(f.actor,0x1098,0);break;
        case 14:Put<unsigned char>(f.actor,0x1098,2);break;
        }
        OnLivePlayer(context,1,allowed);Check(calls==0,"valid aim/fire/ability/lock/menu/air/ownership states stay native");
    }
    for(int invalid=0;invalid<7;++invalid){
        Fresh(image);
        if(invalid==0)f.code[27]^=1;
        if(invalid==1)Put(f.code,10,reinterpret_cast<uintptr_t>(f.parameter));
        if(invalid==2)Put(f.parameter,0x44,1);
        if(invalid==3)Put(f.parameter,0x20,reinterpret_cast<uintptr_t>(f.cls));
        if(invalid==4)Put(f.function,0x64,29);
        if(invalid==5)Label(f.cls,0,"War_Blueprint_C");
        if(invalid==6)Put(f.function,0x10,reinterpret_cast<uintptr_t>(f.kind));
        OnLivePlayer(context,1,true);Check(!calls,"unknown script/parameter/owner/schema never reaches engine dispatch");
    }
    Fresh(image);context.index=1;OnLivePlayer(context,1,true);Check(calls==1&&g_players[1].actor&&!g_players[0].actor,"per-player state does not share a latch");
    Fresh(image);OnLivePlayer(context,1,true);Put<unsigned char>(f.actor,0x1098,1);OnLivePlayer(context,1,true);Check(calls==1,"failed native effect cannot be retried every frame");
    Shutdown();Check(!g_ready&&!g_event&&!g_query,"shutdown clears all call boundaries");
}
}
namespace dg::native_tps {
bool OnGameThread(){return thread;}bool IsWorldPaused(void*){return pause;}
bool CinematicActive(unsigned){return cinema;}bool RadialActive(unsigned){return radial;}
}
int main(){try{Run();std::cout<<"Ranged state native setter boundary and lifecycle passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
