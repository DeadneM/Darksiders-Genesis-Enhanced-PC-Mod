#pragma once
#include <cstdint>
#include <cstddef>

namespace dg::native_objects {
// One game-thread discovery slice only. Never retain chunk pointers between
// callbacks or use this as a persistent UObject identity/baseline.
class DiscoveryObjectBatch {
    uintptr_t chunks_=0,chunk_=0;int count_=0,chunkIndex_=-1;
public:
    DiscoveryObjectBatch(uintptr_t chunks,int count):chunks_(chunks),count_(count){}
    template<class Reader> bool Read(Reader& reader,int index,uintptr_t& object){
        object=0;
        if(!chunks_||index<0||index>=count_||count_>2000000)return false;
        const int chunkIndex=index/65536;
        if(chunkIndex!=chunkIndex_){
            uintptr_t chunk=0;
            if(!reader.Read(chunks_,size_t(chunkIndex)*8,chunk)||!chunk)return false;
            chunk_=chunk;chunkIndex_=chunkIndex;
        }
        struct Head {uintptr_t object=0;uint32_t flags=0,cluster=0;};
        static_assert(sizeof(Head)==16);
        Head item{};
        if(!reader.Read(chunk_,size_t(index%65536)*24,item)||!item.object||(item.flags&0x30000000u))return false;
        object=item.object;return true;
    }
};
}
