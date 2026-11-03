#include "SelectedParticleLoader.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
namespace {
using namespace dg::selected_particle;
struct Object {unsigned char b[0x200]{};};
Object cls{},functionClass{},strClass{},structClass{},softClass{},objectClass{},cdo{},makeFn{},loadFn{},asset{},props[4]{};
uintptr_t chunks[1]{},names[32]{},nameChunks[1]{};unsigned char items[32*24]{},entries[32][256]{};unsigned next=0,nextName=0,calls=0;uintptr_t base=0;
template<class T>void Put(void* p,size_t at,T v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void Live(Object& o,int serial=1){const auto i=next++;Put(o.b,0xC,int(i));Put(items,size_t(i)*24,reinterpret_cast<uintptr_t>(o.b));Put(items,size_t(i)*24+16,serial);}
void Name(Object& o,const char* name){const auto i=nextName++;names[i]=reinterpret_cast<uintptr_t>(entries[i]);std::memcpy(entries[i]+12,name,std::strlen(name)+1);Put(o.b,0x18,int(i));}
void Property(unsigned i,const char* name,Object& type,int size,int at,Object& fn){auto& p=props[i];Name(p,name);Put(p.b,0x10,reinterpret_cast<uintptr_t>(type.b));Put(p.b,0x34,size);Put(p.b,0x44,at);uintptr_t prev=0;dg::owned_particle::Read(reinterpret_cast<uintptr_t>(fn.b),0x48,prev);Put(p.b,0x28,prev);Put(fn.b,0x48,reinterpret_cast<uintptr_t>(p.b));}
void Event(void* context,void* fn,void* args){
 Check(context==cdo.b,"only revalidated native library CDO receives static calls");++calls;
 auto* b=static_cast<unsigned char*>(args);
 if(fn==makeFn.b){uintptr_t text=0;std::memcpy(&text,b,8);Check(std::wstring(reinterpret_cast<wchar_t*>(text))==L"/Game/VFX/Test.Test","canonical package.object path");Put(b,16,uint64_t(777));}
 else {Check(fn==loadFn.b,"only verified load function");int weak=0;std::memcpy(&weak,b,4);Check(weak==-1,"empty native weak reference starts invalid");uint64_t name=0;std::memcpy(&name,b+16,8);Check(name==777,"native MakeSoftObjectPath output, no invented name index");Put(b,40,reinterpret_cast<void*>(asset.b));}
}
void Run(){
 std::vector<unsigned char> image(0x3960000);base=reinterpret_cast<uintptr_t>(image.data());chunks[0]=reinterpret_cast<uintptr_t>(items);nameChunks[0]=reinterpret_cast<uintptr_t>(names);
 Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(chunks));Put(image.data(),0x3959694,32);Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));
 Object* objects[]={&cls,&functionClass,&strClass,&structClass,&softClass,&objectClass,&cdo,&makeFn,&loadFn,&asset};
 const char* labels[]={"KismetSystemLibrary","Function","StrProperty","StructProperty","SoftObjectProperty","ObjectProperty","Default__KismetSystemLibrary","MakeSoftObjectPath","LoadAsset_Blocking","Test"};
 for(unsigned i=0;i<10;++i){Live(*objects[i],i==6?0:int(i+1));Name(*objects[i],labels[i]);}
 Put(cdo.b,8,uint32_t(0x30));Put(cdo.b,0x10,reinterpret_cast<uintptr_t>(cls.b));Put(cls.b,0x48,reinterpret_cast<uintptr_t>(makeFn.b));Put(makeFn.b,0x28,reinterpret_cast<uintptr_t>(loadFn.b));
 Put(makeFn.b,0x10,reinterpret_cast<uintptr_t>(functionClass.b));Put(loadFn.b,0x10,reinterpret_cast<uintptr_t>(functionClass.b));Put(makeFn.b,0xC0,base+0x1D79110);Put(loadFn.b,0xC0,base+0x1D78A90);
 Property(0,"PathString",strClass,16,0,makeFn);Property(1,"ReturnValue",structClass,24,16,makeFn);Property(2,"Asset",softClass,40,0,loadFn);Property(3,"ReturnValue",objectClass,8,40,loadFn);
 State s{};Observe(base,reinterpret_cast<uintptr_t>(cdo.b),s);Check(Context(base,s),"rooted native CDO with lazy serial0 is accepted");
 Check(Load(base,"/Game/VFX/Test","Test",s,Event)==asset.b&&calls==2,"selected asset load uses verified engine conversion and reference ABI");
 s.busy=true;Check(!Load(base,"/Game/VFX/Test","Test",s,Event)&&calls==2,"reentrant request suppressed");s.busy=false;
 Check(!Load(base,"/Other/Test","Test",s,Event)&&calls==2,"noncatalog roots refused");
 State bad=s;bad.load={};Put(props[2].b,0x34,8);Check(!Load(base,"/Game/VFX/Test","Test",bad,Event)&&calls==2,"wrong soft-reference layout makes no native call");Put(props[2].b,0x34,40);
 Put(items,size_t(s.index)*24,uintptr_t(0));Check(!Load(base,"/Game/VFX/Test","Test",s,Event)&&calls==2,"lost native context never called");
 std::cout<<"Selected asset loader: exact ABI, canonical path, lazy CDO serial, reentry and expiry passed\n";
}
}
int main(){try{Run();return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
