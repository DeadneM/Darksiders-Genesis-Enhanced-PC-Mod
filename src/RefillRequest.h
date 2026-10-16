#pragma once
#include <cstdint>
namespace dg::refill {
enum class Result { Idle,Queued,Applied,Rejected,Cancelled };
// Metadata only, guarded by the service's lock. Never dereference these identities.
struct Request {
    uintptr_t actor=0,requestedActor=0;
    uint32_t generation=0,requestedGeneration=0;
    uint64_t observed=0,requestedAt=0;
    bool available=false;
    Result result=Result::Idle;
    bool Fresh(uint64_t now)const{return available&&actor&&observed&&now>=observed&&now-observed<1500;}
    void Cancel(){requestedActor=0;result=Result::Cancelled;}
    void Expire(uint64_t now){
        if(requestedActor&&(now<requestedAt||now-requestedAt>5000))Cancel();
    }
    void Observe(uintptr_t identity,uint32_t serial,uint64_t now,bool ready){
        if(requestedActor&&(identity!=requestedActor||serial!=requestedGeneration||!ready))Cancel();
        actor=identity;generation=serial;observed=now;available=ready;Expire(now);
    }
    bool Queue(uint64_t now){
        Expire(now);
        if(requestedActor||!Fresh(now))return false;
        requestedActor=actor;requestedGeneration=generation;requestedAt=now;result=Result::Queued;return true;
    }
    bool Take(uintptr_t identity,uint32_t serial,uint64_t now,bool gameplay){
        Expire(now);
        if(!requestedActor)return false;
        if(identity!=requestedActor||serial!=requestedGeneration){Cancel();return false;}
        if(!gameplay)return false;
        requestedActor=0;return true;
    }
    void Complete(bool ok){result=ok?Result::Applied:Result::Rejected;}
};
}
