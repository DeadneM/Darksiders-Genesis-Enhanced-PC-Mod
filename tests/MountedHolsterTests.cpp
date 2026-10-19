#include "../src/MountedHolsterFeature.cpp"
#include "ConfigStore.h"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
unsigned char characters[2][0x1100]{},horses[2][0xC10]{},components[2][2][0x600]{},assets[4][0x40]{},items[16*24]{};
void* chunks[1]{items};bool gameThread=true,reject=false;unsigned calls=0;
template<class T> void Put(void* p,size_t offset,T value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
template<class T> T Get(void* p,size_t offset){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+offset,sizeof(value));return value;}
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
dg::native_objects::Identity Register(void* p,int index){
    Put(p,0xC,index);Put(items,size_t(index)*24,reinterpret_cast<uintptr_t>(p));Put(items,size_t(index)*24+16,index+17);
    return {reinterpret_cast<uintptr_t>(p),index,index+17};
}
bool Setter(void* p,void* model){++calls;if(reject)return false;Put(p,0x5B0,model);return true;}
void Run(){
    using namespace dg::mounted_holsters;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x3959680,chunks+0);Put(image.data(),0x3959694,16);
    g_ready=true;g_setMesh=Setter;
    for(unsigned i=0;i<4;++i)g_models[i]=Register(assets[i],int(i)+1);
    for(unsigned i=0;i<2;++i){
        Put<unsigned char>(horses[i],0x118,3);Put(horses[i],0x380,uintptr_t(0x1000+i));
        for(unsigned j=0;j<2;++j){
            Register(components[i][j],5+int(i*2+j));
            Put(components[i][j],0,g_base+0x2A575C8);Put(components[i][j],0xE8,reinterpret_cast<uintptr_t>(horses[i]));
            Put(components[i][j],0x108,uintptr_t(0x1000+i));Put(components[i][j],0x110,uint64_t(0x1122334455667788));
            Put<unsigned char>(components[i][j],0x1CC,0x20);Put(components[i][j],0x5B0,static_cast<void*>(assets[j]));
            Put(horses[i],j?0xC00:0xBF8,reinterpret_cast<uintptr_t>(components[i][j]));
        }
    }
    dg::local_players::Context source{};source.index=0;source.kind=dg::local_players::PoseKind::Horse;
    source.pawn=reinterpret_cast<uintptr_t>(horses[0]);uintptr_t selected=0;
    Check(SourceComponent(source,selected)&&selected==reinterpret_cast<uintptr_t>(components[0][0]),"mounted source remains the native right holster");
    source.kind=dg::local_players::PoseKind::Foot;source.pawn=source.character=reinterpret_cast<uintptr_t>(characters[0]);
    Put<unsigned char>(characters[0],0xC18,1);Put(characters[0],0x380,uintptr_t(0x2000));
    Put(characters[0],0x1040,reinterpret_cast<uintptr_t>(components[0][0]));
    Put(components[0][0],0xE8,source.character);Put(components[0][0],0x108,uintptr_t(0x2000));
    const unsigned beforeWarm=calls;
    Check(SourceComponent(source,selected)&&calls==beforeWarm,"foot prewarm selects Strife's reciprocal pistol component without changing models");
    Put<unsigned char>(characters[0],0xC18,2);Check(!SourceComponent(source,selected),"War cannot seed the pistol model lookup");
    Put<unsigned char>(characters[0],0xC18,1);source.kind=dg::local_players::PoseKind::Ultimate;
    Check(!SourceComponent(source,selected),"ultimate forms cannot seed the normal pistol lookup");
    source.kind=dg::local_players::PoseKind::Foot;Put(components[0][0],0x108,uintptr_t(0x3000));
    Check(!SourceComponent(source,selected),"foot prewarm rejects mismatched parent ownership");
    Put(components[0][0],0xE8,reinterpret_cast<uintptr_t>(horses[0]));Put(components[0][0],0x108,uintptr_t(0x1000));
    Apply(0,reinterpret_cast<uintptr_t>(horses[0]));Apply(1,reinterpret_cast<uintptr_t>(horses[1]));
    Check(calls==4,"exactly two native model setters per local mount");
    for(unsigned i=0;i<2;++i)for(unsigned j=0;j<2;++j){
        Check(Get<void*>(components[i][j],0x5B0)==assets[j+2],"correct left/right native stowed assets");
        Check(Get<uint64_t>(components[i][j],0x110)==0x1122334455667788&&Get<unsigned char>(components[i][j],0x1CC)==0x20,"sockets and visibility untouched");
    }
    Apply(0,reinterpret_cast<uintptr_t>(horses[0]));Check(calls==4,"no repeated setter when already applied");
    dg::local_players::Context owner{};owner.index=0;owner.kind=dg::local_players::PoseKind::Horse;
    dg::runtime::Get().mountedHolstersEnabled=false;OnLivePlayer(owner);
    Check(Get<void*>(components[0][0],0x5B0)==assets[0]&&Get<void*>(components[1][0],0x5B0)==assets[2],"OFF restores this player's saved models without touching the other player");
    owner.index=1;owner.kind=dg::local_players::PoseKind::Foot;OnLivePlayer(owner);
    Check(Get<void*>(components[1][1],0x5B0)==assets[1],"dismount restores native models");
    Put(components[0][0],0xE8,uintptr_t(1));const unsigned before=calls;
    Apply(0,reinterpret_cast<uintptr_t>(horses[0]));Check(calls==before,"broken reciprocal owner prevents both replacements");
    Put(components[0][0],0xE8,reinterpret_cast<uintptr_t>(horses[0]));
    reject=true;Apply(0,reinterpret_cast<uintptr_t>(horses[0]));Check(!g_saved[0][0].component&&Get<void*>(components[0][0],0x5B0)==assets[0],"rejected native setter is not reported applied");reject=false;
    Apply(0,reinterpret_cast<uintptr_t>(horses[0]));Put(components[0][0],0x5B0,uintptr_t(0x1234));Restore(g_saved[0][0]);
    Check(Get<uintptr_t>(components[0][0],0x5B0)==0x1234&&!g_saved[0][0].component,"later native model choice wins over rollback");
    Put(items,6*24+16,999);const unsigned beforeReuse=calls;Restore(g_saved[0][1]);
    Check(calls==beforeReuse&&!g_saved[0][1].component,"stale object serial never restores into a reused component");
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    Check(GetTempPathW(MAX_PATH,directory)&&GetTempFileNameW(directory,L"DGH",0,path),"temporary INI");
    dg::config::Store a;a.SetPath(path);Check(a.Load()&&!a.mountedHolstersEnabled,"missing key defaults OFF");
    a.mountedHolstersEnabled=true;a.hotkeys[5]=dg::config::Action::RefillWrath;Check(a.SaveNow(),"setting saves");
    dg::config::Store b;b.SetPath(path);Check(b.Load()&&b.mountedHolstersEnabled&&dg::runtime::Get().mountedHolstersEnabled&&b.hotkeys==a.hotkeys,"INI and runtime round trip retain bindings");
    b.ResetDefaults(false);Check(!b.mountedHolstersEnabled&&!dg::runtime::Get().mountedHolstersEnabled,"reset returns native models");DeleteFileW(path);
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}bool IsWorldPaused(void*){return false;}bool CinematicActive(unsigned){return false;}}
int main(){try{Run();std::cout<<"Mounted holster model scope, rollback, identity and config passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
