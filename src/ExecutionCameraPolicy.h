#pragma once
#include <cmath>
#include <cstdint>
#include <algorithm>

namespace dg::execution_camera {
struct Follow {
    uintptr_t local=0,actor=0,world=0,mesh=0;
    uint32_t generation=0;
    float baseline=0,offset=0;uint64_t tick=0;bool sampled=false,active=false;
    float Update(uintptr_t nextLocal,uintptr_t nextActor,uintptr_t nextWorld,uintptr_t nextMesh,
                 float rootZ,float pelvisZ,bool executing,uint64_t now,uint32_t nextGeneration=0){
        if(!nextLocal||!nextActor||!nextWorld||!nextMesh||!std::isfinite(rootZ)||!std::isfinite(pelvisZ)){
            *this={};return 0;
        }
        if(local!=nextLocal||actor!=nextActor||world!=nextWorld||mesh!=nextMesh||generation!=nextGeneration||now<tick||now-tick>300){
            *this={};local=nextLocal;actor=nextActor;world=nextWorld;mesh=nextMesh;generation=nextGeneration;tick=now;
        }
        const float difference=pelvisZ-rootZ;
        if(std::abs(difference)>2500){*this={};return 0;}
        const float dt=std::min(float(now-tick)/1000.0f,0.1f);tick=now;
        float target=0;
        if(!executing){
            // Freeze this ordinary body-to-capsule baseline for the entire
            // execution. Floor executions stay inside its 40-unit dead zone.
            if(std::abs(difference)<=300){baseline=difference;sampled=true;}
        }else if(sampled){
            const float rise=difference-baseline;
            if(rise>40&&rise<=2000)target=rise-40;
        }
        active=executing;
        const float blend=1-std::exp(-dt*18);
        offset+=(target-offset)*blend;
        if(offset<0.05f)offset=0;
        return offset;
    }
};
}
