#pragma once
#include <windows.h>
#include <cstdint>
#include <cstring>
#include "NativeObjectIdentity.h"

namespace dg::native_objects {
struct MemoryReader {
    template<class T> bool Read(uintptr_t object,size_t offset,T& value){
        if(!object||object>UINTPTR_MAX-offset)return false;
        const auto* pointer=reinterpret_cast<const void*>(object+offset);
        MEMORY_BASIC_INFORMATION memory{};
        if(!VirtualQuery(pointer,&memory,sizeof(memory))||memory.State!=MEM_COMMIT||
           (memory.Protect&(PAGE_GUARD|PAGE_NOACCESS)))return false;
        const auto access=memory.Protect&0xFF;
        if(access!=PAGE_READONLY&&access!=PAGE_READWRITE&&access!=PAGE_WRITECOPY&&
           access!=PAGE_EXECUTE_READ&&access!=PAGE_EXECUTE_READWRITE&&
           access!=PAGE_EXECUTE_WRITECOPY)return false;
        const auto at=reinterpret_cast<uintptr_t>(pointer)-reinterpret_cast<uintptr_t>(memory.BaseAddress);
        if(at>memory.RegionSize||sizeof(value)>memory.RegionSize-at)return false;
        std::memcpy(&value,pointer,sizeof(value));return true;
    }
};

inline bool CaptureLive(uintptr_t base,const void* liveObject,Identity& identity){
    MemoryReader reader;
    const auto address=reinterpret_cast<uintptr_t>(liveObject);
    if(Capture(reader,base,address,identity))return true;
    // UE initializes a serial lazily when the first weak reference is made.
    // Use the game's own ordinary FWeakObjectPtr assignment on this LIVE
    // callback object; never invent a serial or write the object-table bytes.
    int32_t index=-1,serial=0;uint32_t flags=0,objectFlags=0;uintptr_t item=0;
    if(!address||!reader.Read(address,8,objectFlags)||(objectFlags&0x30u)||
       !reader.Read(address,0xC,index)||!Item(reader,base,index,item,serial,flags)||
       item!=address||serial!=0)return false;
    constexpr unsigned char assignBytes[]={0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x85,0xD2,0x74,0x1A,0x8B,0x52};
    constexpr unsigned char allocatorBytes[]={0x40,0x57,0x48,0x83,0xEC,0x30,0x4C,0x8B,0xC9,0x3B,0x51,0x24,0x7D,0x2B,0x8B,0xC2};
    if(std::memcmp(reinterpret_cast<const void*>(base+0xAD1D20),assignBytes,sizeof(assignBytes))||
       std::memcmp(reinterpret_cast<const void*>(base+0xAB1030),allocatorBytes,sizeof(allocatorBytes)))return false;
    struct Weak {int32_t index=-1,serial=0;} weak;
    using Assign=void(*)(Weak*,const void*);
    reinterpret_cast<Assign>(base+0xAD1D20)(&weak,liveObject);
    return weak.index==index&&weak.serial>0&&Capture(reader,base,address,identity)&&identity.serial==weak.serial;
}
}
