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
    unsigned char entries[96][128]{};uintptr_t pointers[96]{};uintptr_t chunks[1]{};int count=0;
    Names(){chunks[0]=reinterpret_cast<uintptr_t>(pointers);}
    int Add(const char* value,bool wide=false){
        const int id=count++;Check(count<=96,"name fixture capacity");
        pointers[id]=reinterpret_cast<uintptr_t>(entries[id]);Put<uint32_t>(entries[id],8,wide?1:0);
        const size_t length=std::strlen(value)+1;Check(length*(wide?2:1)<116,"name fixture length");
        if(wide){for(size_t i=0;i<length;++i)Put<wchar_t>(entries[id],12+i*2,wchar_t(value[i]));}
        else std::memcpy(entries[id]+12,value,length);
        return id;
    }
    void Object(void* p,const char* name){Put(p,0x18,Add(name));Put<int>(p,0x1C,0);}
};
struct SkinArray {uintptr_t data;int count,capacity;};
unsigned filterCalls=0,setSkinCalls=0;int filterMode=0;
dg::cosmetics::RowHandle foreign{};
void NativeFilter(void* save,void*){
    ++filterCalls;auto& a=*reinterpret_cast<SkinArray*>(static_cast<unsigned char*>(save)+0x240);
    auto* records=reinterpret_cast<dg::cosmetics::SkinEntry*>(a.data);int kept=0;
    if(a.count<=32)for(int i=0;i<a.count;++i)
        if(records[i].type==2||filterMode==1)records[kept++]=records[i];
    a.count=kept;
    if(filterMode==2)records[a.count++]={foreign,8,{}};
    if(filterMode==3)a.capacity=33;
    if(filterMode==4)dg::runtime::Get().bonusMountSkinsEnabled=false;
}
void NativeSetSkin(void* data,const dg::cosmetics::RowHandle* row){
    ++setSkinCalls;auto& a=*static_cast<SkinArray*>(data);
    Check(a.count<a.capacity,"native setter fixture has room");
    reinterpret_cast<dg::cosmetics::SkinEntry*>(a.data)[a.count++]={*row,dg::cosmetics::BonusType(*row),{}};
}
void Run(){
    using namespace dg::cosmetics;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    // ABI/whitelist tests alone cannot distinguish neighbouring native functions.
    Put(image.data(),0x27D7870,g_base+0x27D61D0);Put(image.data(),0x27D7878,g_base+0x7C4440);
    std::memcpy(image.data()+0x27D61D0,"IsSkinUnlocked",15);
    const unsigned char code[]={0x48,0x8B,0xC4,0x56,0x57,0x41,0x56,0x48,0x83,0xEC,0x70,0x48,0xC7,0x40,0xB8,0xFE};
    std::memcpy(image.data()+UnlockRva,code,sizeof(code));image[0x7C44C7]=0xE8;
    Put<int32_t>(image.data(),0x7C44C8,int32_t(UnlockRva)-0x7C44CC);
    Check(UnlockBoundaryMatches(g_base),"registered unlock wrapper and direct native call agree");
    Put(image.data(),0x27D7878,g_base+0x7C4390);
    Check(!UnlockBoundaryMatches(g_base),"adjacent IsSkinEquipped wrapper rejected");
    Put(image.data(),0x27D7878,g_base+0x7C4440);
    Put<int32_t>(image.data(),0x7C44C8,int32_t(0x574860)-0x7C44CC);
    Check(!UnlockBoundaryMatches(g_base),"wrong equipped target rejected despite correct wrapper label");
    Put<int32_t>(image.data(),0x7C44C8,int32_t(UnlockRva)-0x7C44CC);
    image[UnlockRva]^=1;Check(!UnlockBoundaryMatches(g_base),"changed native prologue rejected");image[UnlockRva]^=1;
    const unsigned char filterCode[]={0x48,0x8B,0xC4,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x83,0xEC,0x60};
    const unsigned char setterCode[]={0x48,0x89,0x5C,0x24,0x10,0x56,0x48,0x83,0xEC,0x20,0x48,0x83,0x3A,0x00,0x48,0x8B};
    const unsigned char dlcCode[]={0x80,0x78,0x60,0x00,0x74,0x5F};
    std::memcpy(image.data()+SkinFilterRva,filterCode,16);std::memcpy(image.data()+SetSkinRva,setterCode,16);
    std::memcpy(image.data()+0x6A2583,dlcCode,6);image[SkinFilterCaller-5]=0xE8;
    Put<int32_t>(image.data(),SkinFilterCaller-4,int32_t(SkinFilterRva)-int32_t(SkinFilterCaller));
    Check(SkinFilterBoundaryMatches(g_base),"retail load call, filter, DLC predicate and native setter agree");
    image[0x6A2585]^=1;Check(!SkinFilterBoundaryMatches(g_base),"changed DLC field cannot install the retention hook");image[0x6A2585]^=1;
    image[SkinFilterCaller-4]^=1;Check(!SkinFilterBoundaryMatches(g_base),"changed caller rejects the filter hook");image[SkinFilterCaller-4]^=1;
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
    SkinEntry records[4]{};SkinArray nativeList{reinterpret_cast<uintptr_t>(records),2,4};
    dg::native_objects::MemoryReader reader;
    Check(ReadSkinList(reader,reinterpret_cast<uintptr_t>(&nativeList)).valid,"native record list accepted");
    nativeList.capacity=33;Check(!ReadSkinList(reader,reinterpret_cast<uintptr_t>(&nativeList)).valid,"unbounded native arrays rejected");
    nativeList={0,2,2};Check(!ReadSkinList(reader,reinterpret_cast<uintptr_t>(&nativeList)).valid,"missing arrays fail open");
    nativeList={0,0,0};Check(ReadSkinList(reader,reinterpret_cast<uintptr_t>(&nativeList)).valid,"empty list remains valid");
    unsigned char save[0x300]{},saveClass[0x60]{},instance[0x60]{},instanceClass[0x60]{};
    unsigned char otherTable[0x40]{},otherPackage[0x40]{},despairTable[0x40]{},despairPackage[0x40]{};
    names.Object(saveClass,"MayhemSaveGame");names.Object(instanceClass,"MayhemGameInstance");
    Put(save,0x10,reinterpret_cast<uintptr_t>(saveClass));Put(instance,0x10,reinterpret_cast<uintptr_t>(instanceClass));
    names.Object(table,"RuinSkinTable");names.Object(package,"/Game/Data/ArtData/Skins/RuinSkinTable");
    names.Object(despairTable,"MayhemSkinTable");names.Object(despairPackage,"/Game/Data/ArtData/Skins/MayhemSkinTable");
    Put(despairTable,0x10,reinterpret_cast<uintptr_t>(cls));Put(despairTable,0x20,reinterpret_cast<uintptr_t>(despairPackage));
    names.Object(otherTable,"OtherSkinTable");names.Object(otherPackage,"/Game/OtherSkins");
    Put(otherTable,0x10,reinterpret_cast<uintptr_t>(cls));Put(otherTable,0x20,reinterpret_cast<uintptr_t>(otherPackage));
    const RowHandle despair{reinterpret_cast<uintptr_t>(despairTable),names.Add("MayhemSkin_Despair"),0};
    const RowHandle rampage{reinterpret_cast<uintptr_t>(table),names.Add("RuinSkin_Rampage"),0};
    foreign={reinterpret_cast<uintptr_t>(otherTable),names.Add("ForeignSkin"),0};
    Check(BonusType(despair)==8&&BonusType(rampage)==6,"mount types agree with native row records");
    unsigned char instanceOriginal[sizeof(instance)]{},tableOriginal[sizeof(table)]{};
    std::memcpy(instanceOriginal,instance,sizeof(instance));std::memcpy(tableOriginal,table,sizeof(table));
    g_selectionReady=true;g_filter=NativeFilter;g_setSkin=NativeSetSkin;
    auto reset=[&]{filterCalls=0;setSkinCalls=0;filterMode=0;rt.bonusMountSkinsEnabled=true;
        records[0]={foreign,2,{}};records[1]={despair,8,{}};records[2]={foreign,5,{}};records[3]={rampage,6,{}};
        Put(save,0x240,SkinArray{reinterpret_cast<uintptr_t>(records),4,4});};
    auto selected=[&]{return ReadSkinList(reader,reinterpret_cast<uintptr_t>(save)+0x240);};
    reset();FilterSelection(save,instance,SkinFilterCaller);
    Check(filterCalls==1&&setSkinCalls==2&&selected().count==3&&ContainsType(selected(),2)&&ContainsType(selected(),8)&&ContainsType(selected(),6)&&!ContainsType(selected(),5),
        "both previous bonus choices survive; armor remains, foreign DLC filtering still runs once");
    reset();rt.bonusMountSkinsEnabled=false;FilterSelection(save,instance,SkinFilterCaller);
    Check(filterCalls==1&&!setSkinCalls&&selected().count==1,"OFF uses exact native filtering");
    reset();FilterSelection(save,instance,0);
    Check(filterCalls==1&&!setSkinCalls,"unknown call site cannot retain skins");
    reset();records[1]={foreign,3,{}};records[3]={foreign,4,{}};FilterSelection(save,instance,SkinFilterCaller);
    Check(!setSkinCalls&&selected().count==1,"a save without a selected bonus never gains one");
    reset();Put<uintptr_t>(save,0x10,0);FilterSelection(save,instance,SkinFilterCaller);Put(save,0x10,reinterpret_cast<uintptr_t>(saveClass));
    Check(filterCalls==1&&!setSkinCalls,"unknown save class cannot retain skins");
    reset();filterMode=1;FilterSelection(save,instance,SkinFilterCaller);
    Check(filterCalls==1&&!setSkinCalls&&selected().count==4,"native-owned/PAK rows cause no extra setter or duplication");
    reset();filterMode=2;FilterSelection(save,instance,SkinFilterCaller);
    Check(setSkinCalls==1&&selected().entries[1].row==foreign,"a later native choice of the same type wins");
    reset();records[3]=records[1];FilterSelection(save,instance,SkinFilterCaller);
    Check(!setSkinCalls,"duplicate types reject ambiguous retention");
    reset();records[1].type=2;records[3].type=2;FilterSelection(save,instance,SkinFilterCaller);
    Check(!setSkinCalls,"row/type mismatch cannot overwrite armor");
    reset();filterMode=3;FilterSelection(save,instance,SkinFilterCaller);
    Check(!setSkinCalls,"invalid post-filter container fails open");
    reset();filterMode=4;FilterSelection(save,instance,SkinFilterCaller);
    Check(!setSkinCalls,"option disabled during native filtering stops retention");
    reset();g_filtering=true;FilterSelection(save,instance,SkinFilterCaller);g_filtering=false;
    Check(filterCalls==1&&!setSkinCalls,"reentrant filtering stays native");
    Check(ReadSkinList(reader,reinterpret_cast<uintptr_t>(save)+0x240).count==1,"last mock filter result remains native");
    Check(!std::memcmp(instance,instanceOriginal,sizeof(instance))&&!std::memcmp(table,tableOriginal,sizeof(table)),
        "load retention never changes DLC ownership or shared table flags");
    g_selectionReady=false;

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
