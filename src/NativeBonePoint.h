#pragma once
#include "NativeObjectName.h"
#include <cmath>

namespace dg::native_bone {
struct Point {float x=0,y=0,z=0;};
struct Name {int index=0,number=0;};
struct Array {uintptr_t data=0;int count=0,capacity=0;};
using LocationFn=Point*(*)(void*,Point*,Name);
inline bool Finite(Point p){return std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z)&&
    std::abs(p.x)<1e7f&&std::abs(p.y)<1e7f&&std::abs(p.z)<1e7f;}
inline bool Boundary(uintptr_t base){
    constexpr unsigned char sig[]={0x40,0x53,0x48,0x83,0xEC,0x60,0x48,0x8B,0x05,0x6B,0x03,0x05,0x02,0x48,0x33,0xC4};
    return base&&!std::memcmp(reinterpret_cast<void*>(base+0x17A2380),sig,sizeof(sig));
}
// Only for a mesh belonging to a live native callback's local pawn. The
// returned point is a copy; no component transform or firing socket is written.
template<class Reader,size_t N>
bool Location(Reader& reader,uintptr_t base,LocationFn location,uintptr_t mesh,
              uintptr_t actor,unsigned bone,const char (&expected)[N],Point& point){
    uintptr_t owner=0,asset=0,vt=0,entry=0;Array info{};
    struct Bone {Name name;int parent;} value{};
    if(!location||!reader.Read(mesh,0xE8,owner)||owner!=actor||!reader.Read(mesh,0,vt)||
       !reader.Read(vt,0x3F8,entry)||entry!=base+0x17A2380||!reader.Read(mesh,0x5A0,asset)||
       !reader.Read(asset,0x190,info)||!info.data||info.count<=int(bone)||info.count>info.capacity||info.capacity>512||
       !reader.Read(info.data,size_t(bone)*12,value)||
       !native_objects::NameValueIs(base,value.name.index,value.name.number,expected))return false;
    Point result{};
    if(location(reinterpret_cast<void*>(mesh),&result,value.name)!=&result||!Finite(result))return false;
    point=result;return true;
}
}
