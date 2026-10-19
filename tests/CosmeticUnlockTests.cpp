#include "../src/CosmeticUnlockFeature.cpp"
#include "ConfigStore.h"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
unsigned calls=0;bool owned=false;
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
template<class T> void Put(void* p,size_t offset,T value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
bool Native(void*,const dg::cosmetics::RowHandle*){++calls;return owned;}
struct Names {
    unsigned char entries[32][128]{};uintptr_t pointers[32]{};uintptr_t chunks[1]{};int count=0;
    Names(){chunks[0]=reinterpret_cast<uintptr_t>(pointers);}
    int Add(const char* value,bool wide=false){
        const int id=count++;Check(count<=32,"name fixture capacity");
        pointers[id]=reinterpret_cast<uintptr_t>(entries[id]);Put<uint32_t>(entries[id],8,wide?1:0);
        const size_t length=std::strlen(value)+1;Check(length*(wide?2:1)<116,"name fixture length");
        if(wide){for(size_t i=0;i<length;++i)Put<wchar_t>(entries[id],12+i*2,wchar_t(value[i]));}
        else std::memcpy(entries[id]+12,value,length);
        return id;
    }
    void Object(void* p,const char* name){Put(p,0x18,Add(name));Put<int>(p,0x1C,0);}
};
void Run(){
    using namespace dg::cosmetics;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    Names names;Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(names.chunks));
    unsigned char cls[0x40]{},table[0x40]{},package[0x40]{};
    names.Object(cls,"DataTable");Put(table,0x10,reinterpret_cast<uintptr_t>(cls));Put(table,0x20,reinterpret_cast<uintptr_t>(package));
    g_ready=true;g_original=Native;auto& rt=dg::runtime::Get();
    rt.bonusMountSkinsEnabled=false;rt.progressionSkinsEnabled=false;
    RowHandle h{reinterpret_cast<uintptr_t>(table),names.Add("RuinSkin_Rampage"),0};
    names.Object(table,"RuinSkinTable");names.Object(package,"/Game/Data/ArtData/Skins/RuinSkinTable");
    Check(!HookCheck(nullptr,&h)&&calls==1,"OFF calls native once and preserves locked skin");
    rt.progressionSkinsEnabled=true;
    Check(!HookCheck(nullptr,&h),"progression switch cannot unlock bonus mount");
    rt.bonusMountSkinsEnabled=true;
    Check(HookCheck(nullptr,&h),"exact Rampage row enabled");
    h.number=1;Check(!HookCheck(nullptr,&h),"numeric row alias rejected");h.number=0;
    names.Object(package,"/Game/Other/RuinSkinTable");Check(!HookCheck(nullptr,&h),"same object label in foreign package rejected");
    names.Object(table,"MayhemSkinTable");names.Object(package,"/Game/Data/ArtData/Skins/MayhemSkinTable");h.index=names.Add("MayhemSkin_Despair");
    Check(HookCheck(nullptr,&h),"exact Despair row enabled");
    names.Object(table,"WarSkinTable");names.Object(package,"/Game/Data/ArtData/Skins/WarSkinTable");
    const char* war[]={"WarSkin_Abyssal","WarSkin_Silver","WarSkin_Gold"};
    for(const auto* row:war){h.index=names.Add(row);Check(HookCheck(nullptr,&h),"War cosmetic row enabled");}
    rt.progressionSkinsEnabled=false;Check(!HookCheck(nullptr,&h),"bonus mount switch cannot unlock progression skin");
    names.Object(table,"StrifeSkinTable");names.Object(package,"/Game/Data/ArtData/Skins/StrifeSkinTable");
    rt.progressionSkinsEnabled=true;
    const char* strife[]={"StrifeSkin_Abyssal","StrifeSkin_Silver","StrifeSkin_Gold"};
    for(const auto* row:strife){h.index=names.Add(row,true);Check(HookCheck(nullptr,&h),"Strife cosmetic wide row enabled");}
    h.index=names.Add("StrifeSkin_Default");Check(!HookCheck(nullptr,&h),"default and arbitrary rows not whitelisted");
    h.index=names.Add("WarSkin_Gold");Check(!HookCheck(nullptr,&h),"row from the other character's table rejected");
    owned=true;rt.bonusMountSkinsEnabled=false;rt.progressionSkinsEnabled=false;
    Check(HookCheck(nullptr,nullptr),"native owned or PAK unlocked availability never revoked");
    Check(names.count<=32,"fixture remained bounded");

    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    Check(GetTempPathW(MAX_PATH,directory)&&GetTempFileNameW(directory,L"DGC",0,path),"temporary INI");
    dg::config::Store a;a.SetPath(path);Check(a.Load()&&!a.bonusMountSkinsEnabled&&!a.progressionSkinsEnabled&&a.pistolLeftModel==0&&a.pistolRightModel==0,"old INI defaults preserve native appearances");
    a.pistolRightModel=2;a.pistolLeftModel=1;a.bonusMountSkinsEnabled=true;a.progressionSkinsEnabled=true;
    a.menuKey=VK_F10;a.hotkeys[4]=dg::config::Action::RefillWrath;Check(a.SaveNow(),"cosmetic settings save");
    dg::config::Store b;b.SetPath(path);Check(b.Load()&&b.pistolRightModel==2&&b.pistolLeftModel==1&&b.bonusMountSkinsEnabled&&b.progressionSkinsEnabled&&b.hotkeys==a.hotkeys&&b.menuKey==VK_F10,"independent settings and personal bindings round trip");
    Check(rt.pistolRightModel.load()==2&&rt.pistolLeftModel.load()==1&&rt.progressionSkinsEnabled.load(),"runtime publishes settings");
    WritePrivateProfileStringW(L"Values",L"PistolRightModel",L"999",path);
    WritePrivateProfileStringW(L"Values",L"PistolLeftModel",L"-1",path);
    Check(b.Load()&&b.pistolRightModel==0&&b.pistolLeftModel==0,"invalid style values safely return to Native");
    b.ResetDefaults(false);Check(!b.bonusMountSkinsEnabled&&!b.progressionSkinsEnabled&&b.pistolRightModel==0&&b.pistolLeftModel==0&&!rt.progressionSkinsEnabled.load(),"reset clears cosmetic opt-ins");
    DeleteFileW(path);
}
}
int main(){try{Run();std::cout<<"Cosmetic row whitelist, native fallback and configuration passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
