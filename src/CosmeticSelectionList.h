#pragma once
#include <array>
#include <cstdint>
#include <cstddef>

namespace dg::cosmetics {
struct RowHandle {
    uintptr_t table=0;int index=-1,number=0;
    bool operator==(const RowHandle&) const = default;
};
struct SkinEntry {RowHandle row{};unsigned char type=0,padding[7]{};};
static_assert(sizeof(RowHandle)==16&&sizeof(SkinEntry)==24);
struct SkinList {std::array<SkinEntry,32> entries{};int count=0;bool valid=false;};
template<class Reader> SkinList ReadSkinList(Reader& reader,uintptr_t address){
    struct Array {uintptr_t data;int count,capacity;} array{};
    SkinList out;
    if(!reader.Read(address,0,array)||array.count<0||array.count>array.capacity||array.capacity>32||
       (array.count&&!array.data))return out;
    for(int i=0;i<array.count;++i)
        if(!reader.Read(array.data,size_t(i)*sizeof(SkinEntry),out.entries[size_t(i)]))return {};
    out.count=array.count;out.valid=true;return out;
}
inline bool ContainsType(const SkinList& list,unsigned char type){
    for(int i=0;i<list.count;++i)if(list.entries[size_t(i)].type==type)return true;
    return false;
}
}
