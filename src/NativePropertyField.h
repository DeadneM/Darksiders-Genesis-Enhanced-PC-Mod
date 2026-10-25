#pragma once
#include "NativeObjectName.h"

namespace dg::native_objects {
// UE4 UField/UProperty reflection for exact named contracts. Never scans memory
// for a value or accepts a guessed member offset.
inline bool PropertyField(uintptr_t base,uintptr_t cls,const char* name,const char* kind,int size,int& offset){
    MemoryReader r;
    for(int level=0;cls&&level<16;++level){
        uintptr_t field=0;if(!r.Read(cls,0x48,field))return false;
        for(int i=0;field&&i<512;++i){
            uintptr_t type=0,next=0;int index=0,number=0,bytes=0,at=-1;
            if(!r.Read(field,0x10,type)||!r.Read(field,0x28,next)||next==field||!r.Read(field,0x18,index)||!r.Read(field,0x1C,number))return false;
            if(NameValueEquals(base,index,number,name)){
                if(!r.Read(type,0x18,index)||!r.Read(type,0x1C,number)||!NameValueEquals(base,index,number,kind)||
                   !r.Read(field,0x34,bytes)||bytes!=size||!r.Read(field,0x44,at)||at<0||at>65536-size)return false;
                offset=at;return true;
            }
            field=next;
        }
        if(field||!r.Read(cls,0x40,cls))return false;
    }
    return false;
}
}
