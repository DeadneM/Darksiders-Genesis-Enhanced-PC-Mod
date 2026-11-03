#pragma once
#include <array>
#include <cstdint>
#include <cstddef>
namespace dg::selected_recovery {
// Shared by the new material/library recovery paths. Shield's validated
// lifecycle stays independent. Game thread only, no timers or worker threads.
inline uint64_t nextLoadAt=0;
inline bool loadingAllowed=false; // published by the native gameplay service
struct Scope {
    uintptr_t world=0,owner=0;uint32_t serial=0;uint64_t selection=0;
    bool operator==(const Scope&)const=default;
};
template<size_t N> struct Requests {
    struct Attempt {unsigned count=0;uint64_t next=0;};
    Scope scope{};bool scoped=false;uint64_t settled=0;
    std::array<Attempt,N> entries{};
    void Enter(Scope next,uint64_t now){if(!scoped||!(scope==next)){scope=next;scoped=true;settled=now;entries={};}}
    void Forget(size_t index){if(index<N)entries[index]={};}
    void Reset(){*this={};}
    bool Exhausted(size_t index)const{return index<N&&entries[index].count>=3;}
    bool Begin(size_t index,uint64_t now,bool safe,bool contextReady){
        if(index>=N||!scoped||!safe||!contextReady||now<settled||now-settled<750)return false;
        auto& attempt=entries[index];
        if(attempt.count>=3||now<attempt.next||now<nextLoadAt)return false;
        ++attempt.count;attempt.next=now+5000;nextLoadAt=now+250;return true;
    }
};
}
