#pragma once
#include <array>
#include <cstddef>
#include <cstdint>

namespace dg::native_objects {
// Exact supported UE4 build: FUObjectItem is 24 bytes; serial at +16.
// Stored addresses are identities. Expiry checks read the object table only,
// never an old UObject, so address reuse cannot reuse modified baselines.
struct Identity {
    uintptr_t address=0;
    int32_t index=-1,serial=0;
    bool operator==(const Identity&) const = default;
    explicit operator bool() const { return address&&index>=0&&serial>0; }
};

template<class Reader>
bool Item(Reader& reader,uintptr_t base,int32_t index,uintptr_t& address,
          int32_t& serial,uint32_t& flags){
    int32_t count=0;
    uintptr_t chunks=0,chunk=0;
    if(index<0||!reader.Read(base+0x3959694,0,count)||count<=0||count>2000000||index>=count||
       !reader.Read(base+0x3959680,0,chunks)||!chunks||
       !reader.Read(chunks,size_t(index/65536)*8,chunk)||!chunk)return false;
    const auto offset=size_t(index%65536)*24;
    return reader.Read(chunk,offset,address)&&address&&reader.Read(chunk,offset+8,flags)&&
        reader.Read(chunk,offset+16,serial)&&serial>=0&&!(flags&0x30000000u);
}

template<class Reader>
bool Alive(Reader& reader,uintptr_t base,const Identity& identity){
    uintptr_t address=0;int32_t serial=0;uint32_t flags=0;
    return identity&&Item(reader,base,identity.index,address,serial,flags)&&
        address==identity.address&&serial==identity.serial;
}

template<class Reader>
bool Capture(Reader& reader,uintptr_t base,uintptr_t liveObject,Identity& identity){
    int32_t index=-1,serial=0;uint32_t objectFlags=0,flags=0;uintptr_t address=0;
    if(!liveObject||!reader.Read(liveObject,8,objectFlags)||(objectFlags&0x30u)||
       !reader.Read(liveObject,0xC,index)||!Item(reader,base,index,address,serial,flags)||
       address!=liveObject||serial<=0)return false;
    identity={address,index,serial};return true;
}

template<class Value,size_t Capacity>
struct Baselines {
    struct Entry {Identity identity{};Value value{};};
    std::array<Entry,Capacity> entries{};
    Value* Find(const Identity& identity){
        for(auto& entry:entries)if(entry.identity==identity)return &entry.value;
        return nullptr;
    }
    template<class Reader>
    Value* Acquire(Reader& reader,uintptr_t base,const Identity& identity){
        if(!identity)return nullptr;
        if(auto* found=Find(identity))return found;
        for(auto& entry:entries){
            if(!entry.identity||!Alive(reader,base,entry.identity)){
                entry={identity,{}};return &entry.value;
            }
        }
        // Never evict a still-live modified object. A full cache fails open.
        return nullptr;
    }
};
}
