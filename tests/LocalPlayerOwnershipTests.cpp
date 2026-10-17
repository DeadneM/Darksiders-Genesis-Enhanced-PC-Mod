#include "LocalPlayerLayout.h"
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using namespace dg::local_players;
namespace {
void Check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct Graph {
    std::unordered_map<uintptr_t,std::vector<unsigned char>> fields;
    std::unordered_map<uintptr_t,uintptr_t> types;
    template<class T> void Put(uintptr_t object,size_t offset,T value){
        std::vector<unsigned char> bytes(sizeof(value));std::memcpy(bytes.data(),&value,sizeof(value));
        fields[object+offset]=bytes;
    }
    template<class T> bool Read(uintptr_t object,size_t offset,T& value){
        const auto it=fields.find(object+offset);
        if(it==fields.end()||it->second.size()!=sizeof(T))return false;
        std::memcpy(&value,it->second.data(),sizeof(T));return true;
    }
    bool Instance(uintptr_t object,uintptr_t type){const auto it=types.find(object);return it!=types.end()&&
        (it->second==type||(type==layout::CameraClass&&it->second==layout::PlayerCameraClass));}
};
constexpr uintptr_t World=0x100000,Level=0x110000,Instance=0x120000,Array=0x130000;
constexpr uintptr_t P0=0x200000,P1=0x300000;
uintptr_t State(uintptr_t p){return p+0x10000;}
uintptr_t PC(uintptr_t p){return p+0x20000;}
uintptr_t LP(uintptr_t p){return p+0x30000;}
uintptr_t Camera(uintptr_t p){return p+0x40000;}
void Player(Graph& g,uintptr_t p,int index,int controllerId){
    g.types[p]=layout::PlayerClass;g.types[State(p)]=layout::StateClass;
    g.types[PC(p)]=layout::ControllerClass;g.types[LP(p)]=layout::LocalPlayerClass;
    g.types[Camera(p)]=layout::PlayerCameraClass;
    g.Put(p,0x330,State(p));g.Put(p,0x348,PC(p));g.Put(p,0x20,Level);
    g.Put(State(p),0x5C0,p);g.Put(State(p),0x108,PC(p));
    g.Put(PC(p),0x318,State(p));g.Put(PC(p),0x398,LP(p));g.Put(PC(p),0x350,p);
    g.Put(LP(p),0x30,PC(p));g.Put(LP(p),0xE0,controllerId);
    g.Put(Array,size_t(index)*8,LP(p));
    g.Put(p,0xBA0,Camera(p));g.Put(p,0xBA8,uintptr_t(0));
    g.Put(Camera(p),0x20,p);g.Put(Camera(p),0xF0,World);
}
Graph Coop(){
    Graph g;g.types[World]=layout::WorldClass;g.types[Level]=layout::LevelClass;
    g.types[Instance]=layout::GameInstanceClass;
    g.Put(Level,0x20,World);g.Put(World,0x160,Instance);
    g.Put(Instance,0x38,Array);g.Put(Instance,0x40,2);g.Put(Instance,0x44,4);
    Player(g,P0,0,0);Player(g,P1,1,1);return g;
}
void Run(){
    Context a{},b{};auto g=Coop();
    Check(layout::Camera(g,Camera(P0),a)&&layout::Camera(g,Camera(P1),b),"two reciprocal player views resolve");
    Check(a.index==0&&b.index==1&&a.character!=b.character&&a.controllerId==0&&b.controllerId==1,"distinct owners and native pad ids");
    // Native array order, not pawn names, controller id or callback order.
    g.Put(LP(P0),0xE0,3);g.Put(LP(P1),0xE0,0);
    Check(layout::Resolve(g,P1,b)&&layout::Resolve(g,P0,a)&&a.index==0&&b.index==1&&a.controllerId==3&&b.controllerId==0,"input ids are not player indices");
    g.Put(Array,0,LP(P1));g.Put(Array,8,LP(P0));
    Check(layout::Resolve(g,P1,b)&&b.index==0,"native reorder is respected");
    g=Coop();constexpr uintptr_t respawn=0x500000;
    // Same controller/local player, replacement pawn after respawn.
    g.types[respawn]=layout::PlayerClass;g.Put(respawn,0x330,State(P1));
    g.Put(respawn,0x348,PC(P1));g.Put(respawn,0x20,Level);
    g.Put(State(P1),0x5C0,respawn);g.Put(PC(P1),0x350,respawn);
    Check(layout::Resolve(g,respawn,b)&&b.index==1&&b.character==respawn,"P2 respawn cannot steal P1");
    Check(!layout::Resolve(g,P1,b),"stale old pawn fails reciprocal ownership");
    Check(layout::Resolve(g,P0,a)&&a.index==0&&a.character==P0,"P1 unchanged after P2 respawn");
    g=Coop();g.types[LP(P1)]=0;
    Check(!layout::Resolve(g,P1,b),"remote controller without native LocalPlayer rejected");
    g=Coop();g.Put(LP(P1),0x30,PC(P0));
    Check(!layout::Resolve(g,P1,b),"mismatched reciprocal controller rejected");
    g=Coop();g.Put(Array,0,LP(P1));
    Check(!layout::Resolve(g,P1,b),"duplicate local player rejected");
    g=Coop();g.Put(Instance,0x40,5000000);
    Check(!layout::Resolve(g,P1,b),"unbounded array rejected");
    g=Coop();g.Put(Camera(P1),0xF0,uintptr_t(0xDEAD));
    Check(!layout::Camera(g,Camera(P1),b),"camera in another world rejected");
    g=Coop();g.Put(P1,0xBA0,uintptr_t(0));
    Check(!layout::Camera(g,Camera(P1),b),"unregistered player camera rejected");
    g=Coop();g.types[Camera(P1)]=0;
    Check(!layout::Camera(g,Camera(P1),b),"cinematic/NPC camera stays native");
    g=Coop();constexpr uintptr_t fallback=0x800000,cameraActor=0x900000,manager=0xA00000;
    g.types[fallback]=layout::CameraClass;g.types[cameraActor]=layout::CameraActorClass;
    g.types[manager]=layout::ManagerClass;g.Put(fallback,0x20,cameraActor);g.Put(fallback,0xF0,World);
    g.Put(cameraActor,0x108,manager);g.Put(manager,0x310,PC(P1));g.Put(PC(P1),0x3B8,manager);
    Check(layout::Camera(g,fallback,b)&&b.index==1,"UE fallback camera belongs to its reciprocal player camera manager");
    g.Put(PC(P1),0x3B8,uintptr_t(0));
    Check(!layout::Camera(g,fallback,b),"unowned fallback camera cannot inherit a player's view");
    g=Coop();constexpr uintptr_t horse=0x600000;
    g.types[horse]=layout::HorseClass;g.Put(horse,0x348,PC(P1));g.Put(horse,0x330,State(P1));g.Put(horse,0x20,Level);g.Put(PC(P1),0x350,horse);
    Check(layout::Camera(g,Camera(P1),b)&&b.kind==PoseKind::Horse&&b.pawn==horse&&b.index==1,"own possessed horse supplies pose");
    Check(layout::Resolve(g,horse,b)&&b.index==1,"live horse ownership is reciprocal");
    constexpr uintptr_t horseCamera=0x680000;
    g.types[horseCamera]=layout::PlayerCameraClass;g.Put(horseCamera,0x20,horse);g.Put(horseCamera,0xF0,World);
    Check(layout::Camera(g,horseCamera,b)&&b.pawn==horse&&b.index==1,"native camera owned by the current horse remains gameplay");
    g.Put(P1,0x330,uintptr_t(0));g.Put(P1,0x348,uintptr_t(0));
    Check(layout::Camera(g,Camera(P1),b)&&b.pawn==horse&&b.index==1,"normal pawn unpossessed during mounting remains owned");
    g=Coop();constexpr uintptr_t form=0x700000;
    g.types[form]=layout::FormClass;g.Put(form,0x330,State(P1));g.Put(form,0x20,Level);g.Put(P1,0xE78,form);
    Check(layout::Camera(g,Camera(P1),b)&&b.kind==PoseKind::Ultimate&&b.pawn==form,"own ultimate form while normal pawn retained");
    Check(layout::Resolve(g,form,b)&&b.index==1,"ultimate look actor maps to its local player");
    constexpr uintptr_t formCamera=0x780000;
    g.types[formCamera]=layout::PlayerCameraClass;g.Put(formCamera,0x20,form);g.Put(formCamera,0xF0,World);
    Check(layout::Camera(g,formCamera,b)&&b.kind==PoseKind::Ultimate&&b.index==1,"native camera owned by reciprocal active form remains gameplay");
    g.Put(form,0x330,State(P0));
    Check(layout::Camera(g,Camera(P1),b)&&b.kind==PoseKind::Foot&&b.pawn==P1,"another player's form cannot supply the pose");
    g=Coop();constexpr uintptr_t manager0=0xB00000,manager1=0xC00000,scene=0xD00000;
    for(const auto p:{P0,P1}){
        const auto m=p==P0?manager0:manager1;
        g.types[m]=layout::ManagerClass;g.Put(PC(p),0x3B8,m);g.Put(m,0x310,PC(p));
        g.Put(m,0xEA0,p);g.Put(m,0x1440,uintptr_t(0));
    }
    Check(layout::View(g,Camera(P0),0)==NativeView::Gameplay&&layout::View(g,Camera(P0),1)==NativeView::Gameplay,"pause/wheels on native player view are gameplay");
    g.types[scene]=layout::CameraActorClass;g.Put(scene,0x108,uintptr_t(0));
    g.Put(manager1,0x1440,scene);g.Put(manager1,0x19E0,1.0f);
    Check(layout::View(g,Camera(P0),0)==NativeView::Gameplay&&layout::View(g,Camera(P0),1)==NativeView::ExternalCamera,"P2 scene blend does not suspend P1");
    g.Put(manager1,0x19E0,0.0f);
    Check(layout::View(g,Camera(P0),1)==NativeView::Gameplay,"stale external pending pointer cannot invent a cutscene after its blend expired");
    g.Put(manager1,0x19E0,1.0f);
    g.Put(manager1,0xEA0,scene);g.Put(manager1,0x1440,P1);
    Check(layout::View(g,Camera(P0),1)==NativeView::ExternalCamera,"return blend retains external camera until native handoff");
    g.Put(manager1,0x19E0,0.0f);
    Check(layout::View(g,Camera(P0),1)==NativeView::Gameplay,"completed native return blend releases the cinematic immediately");
    g.Put(manager1,0xEA0,P1);g.Put(manager1,0x1440,uintptr_t(0));
    Check(layout::View(g,Camera(P0),1)==NativeView::Gameplay,"native handoff resumes gameplay immediately without angle/distance heuristic");
    g.Put(scene,0x108,manager1);g.Put(manager1,0xEA0,scene);
    Check(layout::View(g,Camera(P0),1)==NativeView::Gameplay,"manager-owned fallback is not a cutscene");
    g.Put(manager1,0x310,PC(P0));
    Check(layout::View(g,Camera(P0),1)==NativeView::Unknown,"mismatched camera manager cannot prove a scene");
}
}
int main(){try{Run();std::cout<<"Native local-player ownership/respawn/isolation checks passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
