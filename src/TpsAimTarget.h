#pragma once
#include <algorithm>
#include <cmath>

namespace dg::tps_aim {
template<class Vector> bool FinitePoint(const Vector& p){
    return std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z);
}
template<class Vector> float Along(const Vector& from,const Vector& to,const Vector& direction){
    return (to.x-from.x)*direction.x+(to.y-from.y)*direction.y+(to.z-from.z)*direction.z;
}
// Keep ordinary native collisions unchanged. A camera can touch geometry
// behind the muzzle: retry from the muzzle's plane along the SAME ray, so
// the intervening camera segment cannot turn a shot back at its owner.
template<class Ray,class Vector,class Query>
bool ForwardTarget(const Ray& ray,const Vector* source,Vector& target,bool& hit,Query query){
    if(!FinitePoint(ray.origin)||!FinitePoint(ray.direction)||
       (source&&!FinitePoint(*source))||!query(ray,target,hit)||!FinitePoint(target))return false;
    if(!source||Along(*source,target,ray.direction)>0.01f)return true;
    Ray retry=ray;
    const float advance=std::max(0.0f,Along(ray.origin,*source,ray.direction))+1.0f;
    if(!std::isfinite(advance)||advance>100000.0f)return false;
    retry.origin={ray.origin.x+advance*ray.direction.x,
                  ray.origin.y+advance*ray.direction.y,
                  ray.origin.z+advance*ray.direction.z};
    if(!query(retry,target,hit)||!FinitePoint(target))return false;
    if(Along(*source,target,ray.direction)>0.01f)return true;
    // Initial penetration may still return a zero-distance/backward hit.
    // The native projectile/beam collision still stops at forward cover.
    target={source->x+100000.0f*ray.direction.x,source->y+100000.0f*ray.direction.y,
            source->z+100000.0f*ray.direction.z};hit=false;
    return FinitePoint(target);
}
}
