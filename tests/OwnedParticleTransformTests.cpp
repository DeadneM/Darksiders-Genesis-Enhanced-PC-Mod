#include "OwnedParticleTransform.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>
namespace {
using namespace dg::owned_particle;
struct Object {unsigned char b[0x300]{};};
Object component{},cls{},functionClass{},structClass{},boolClass{},scaleFn{},locationFn{},properties[8]{};
uintptr_t chunks[1]{},names[48]{},nameChunks[1]{};unsigned char items[32*24]{},entries[48][256]{};
unsigned objects=0,nameCount=0,calls=0;uintptr_t base=0;
template<class T>void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
Id Live(Object& o){unsigned i=objects++;Put(o.b,0xC,int(i));Put(items,size_t(i)*24,reinterpret_cast<uintptr_t>(o.b));Put(items,size_t(i)*24+16,int(i+1));return {reinterpret_cast<uintptr_t>(o.b),int(i),int(i+1)};}
void Name(Object& o,const char* name){unsigned i=nameCount++;Check(i<48,"name fixture capacity");names[i]=reinterpret_cast<uintptr_t>(entries[i]);std::memcpy(entries[i]+12,name,std::strlen(name)+1);Put(o.b,0x18,int(i));}
void Property(unsigned i,const char* name,Object& kind,int bytes,int offset,Object& parent){
    auto& p=properties[i];Name(p,name);Put(p.b,0x10,reinterpret_cast<uintptr_t>(kind.b));Put(p.b,0x34,bytes);Put(p.b,0x44,offset);
    uintptr_t previous=0;Read(reinterpret_cast<uintptr_t>(parent.b),0x48,previous);Put(p.b,0x28,previous);Put(parent.b,0x48,reinterpret_cast<uintptr_t>(p.b));
}
void EventCall(void* object,void* function,void* args){
    Check(object==component.b,"only the supplied private component changes");++calls;
    if(function==scaleFn.b)Put(component.b,0x150,*static_cast<Vector*>(args));
    else {
        Check(function==locationFn.b,"only verified transform setters called");auto* b=static_cast<unsigned char*>(args);
        Check(b[12]==0&&b[152]==1,"location setter disables sweep and teleports only the visual component");
        for(unsigned i=16;i<152;++i)Check(b[i]==0,"bounded zero initialized hit result");Put(component.b,0x140,*static_cast<Vector*>(args));
    }
}
void Run(){
    std::vector<unsigned char> image(0x3960000);base=reinterpret_cast<uintptr_t>(image.data());chunks[0]=reinterpret_cast<uintptr_t>(items);nameChunks[0]=reinterpret_cast<uintptr_t>(names);
    Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(chunks));Put(image.data(),0x3959694,32);Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));
    Name(cls,"ParticleSystemComponent");Name(functionClass,"Function");Name(structClass,"StructProperty");Name(boolClass,"BoolProperty");Name(scaleFn,"SetRelativeScale3D");Name(locationFn,"K2_SetRelativeLocation");
    Live(cls);Live(functionClass);Live(structClass);Live(boolClass);Live(scaleFn);Live(locationFn);auto id=Live(component);
    Put(component.b,0x10,reinterpret_cast<uintptr_t>(cls.b));Put(scaleFn.b,0x10,reinterpret_cast<uintptr_t>(functionClass.b));Put(locationFn.b,0x10,reinterpret_cast<uintptr_t>(functionClass.b));
    Put(scaleFn.b,0xC0,base+0x1DD9E30);Put(locationFn.b,0xC0,base+0x1DD8A90);Put(cls.b,0x48,reinterpret_cast<uintptr_t>(scaleFn.b));Put(scaleFn.b,0x28,reinterpret_cast<uintptr_t>(locationFn.b));
    Property(0,"RelativeScale3D",structClass,12,0x150,cls);Property(1,"RelativeLocation",structClass,12,0x140,cls);
    Property(2,"NewScale3D",structClass,12,0,scaleFn);Property(3,"NewLocation",structClass,12,0,locationFn);
    Property(4,"bSweep",boolClass,1,12,locationFn);Property(5,"SweepHitResult",structClass,136,16,locationFn);Property(6,"bTeleport",boolClass,1,152,locationFn);
    Put(component.b,0x150,Vector{2,3,4});Put(component.b,0x140,Vector{1,2,3});State s{};
    Check(Apply(base,id,{1,0,0,0},1,s,EventCall)&&calls==0,"identity defaults preserve the exact native transform");
    Check(Apply(base,id,{2,10,-2,5},.6f,s,EventCall)&&calls==2,"active effect receives size and attachment offsets");
    Vector scale{},location{};Read(id.address,0x150,scale);Read(id.address,0x140,location);
    Check(Same(scale,{2.4f,3.6f,4.8f})&&Same(location,{11,0,8}),"preset applied once to native base");
    Apply(base,id,{2,10,-2,5},.6f,s,EventCall);Check(calls==2,"unchanged settings neither drift nor call setters repeatedly");
    Apply(base,id,{1,0,0,0},1,s,EventCall);Read(id.address,0x150,scale);Read(id.address,0x140,location);
    Check(calls==4&&Same(scale,{2,3,4})&&Same(location,{1,2,3}),"reset restores baseline without accumulating offsets");
    const auto preZero=calls;Apply(base,id,{0,0,0,0},1,s,EventCall);Read(id.address,0x150,scale);
    Check(calls==preZero+1&&Same(scale,{0,0,0}),"zero size is accepted without clamping to 0.05");Apply(base,id,{1,0,0,0},1,s,EventCall);
    Put(component.b,0x150,Vector{5,5,5});Apply(base,id,{2,0,0,0},1,s,EventCall);Read(id.address,0x150,scale);Check(Same(scale,{10,10,10}),"native attachment changes rebase size");
    State rejected{};Put(properties[6].b,0x44,153);const auto before=calls;Check(!Apply(base,id,{2,0,0,0},1,rejected,EventCall)&&calls==before,"unexpected ABI layout refuses every call");Put(properties[6].b,0x44,152);
    Put(items,size_t(id.index)*24+16,99);Check(!Apply(base,id,{2,0,0,0},1,s,EventCall)&&calls==before,"expired identity never modifies a reused object");
    auto v=dg::effect_transform::Normalize({std::numeric_limits<float>::quiet_NaN(),-900,900,0});Check(v.size==1&&v.x==-500&&v.y==500,"nonfinite settings and manual input are bounded");
    std::cout<<"Owned PSC: verified ABI, live updates, defaults, rebase, expiry and no drift passed\n";
}
}
int main(){try{Run();return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
