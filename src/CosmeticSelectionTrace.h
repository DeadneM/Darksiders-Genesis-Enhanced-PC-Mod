#pragma once
#include <cstdint>
#include <cstddef>
#include <initializer_list>

namespace dg::cosmetics {
struct SelectionSummary {
    uint64_t hash=1469598103934665603ull;
    int count=0;
    unsigned bonusMask=0;
    bool valid=false;
    bool operator==(const SelectionSummary&) const = default;
};
// Native MayhemPlayerSkinData contains a TArray of 24-byte records. The
// caller supplies the known bonus-row predicate. This observer never repairs,
// allocates or writes a row or an array, including on malformed/stale input.
template<class Reader,class Classify> SelectionSummary SummarizeSelection(Reader& reader,uintptr_t at,Classify classify){
    struct Array {uintptr_t data;int count,capacity;} array{};
    struct Entry {uintptr_t table;int index,number;unsigned char type,padding[7];};
    static_assert(sizeof(Entry)==24);
    SelectionSummary out;
    if(!reader.Read(at,0,array)||array.count<0||array.count>array.capacity||array.capacity>32||
       (array.count&&!array.data))return out;
    for(int i=0;i<array.count;++i){
        Entry entry{};if(!reader.Read(array.data,size_t(i)*24,entry))return {};
        for(uint64_t word:{uint64_t(entry.table),uint64_t(uint32_t(entry.index)),uint64_t(uint32_t(entry.number)),uint64_t(entry.type)})
            out.hash=(out.hash^word)*1099511628211ull;
        out.bonusMask|=classify(entry.table,entry.index,entry.number);
    }
    out.count=array.count;out.valid=true;return out;
}
}
