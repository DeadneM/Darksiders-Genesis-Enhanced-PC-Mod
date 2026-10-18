#include "TpsAimTarget.h"
#include <iostream>
#include <stdexcept>
#include <limits>
struct Point {float x,y,z;};
struct Ray {Point origin,direction;};
void Check(bool b,const char* message){if(!b)throw std::runtime_error(message);}
int main(){try{
    using dg::tps_aim::ForwardTarget;
    const Ray ray{{-300,20,100},{1,0,0}};const Point muzzle{0,0,100};Point target{};bool hit=false;int calls=0;
    auto front=[&](const Ray& r,Point& p,bool& h){++calls;Check(r.origin.x==-300,"normal camera origin unchanged");p={50,20,100};h=true;return true;};
    Check(ForwardTarget(ray,&muzzle,target,hit,front)&&hit&&target.x==50&&calls==1,"forward cover preserved, including near cover");
    calls=0;
    auto behind=[&](const Ray& r,Point& p,bool& h){++calls;h=true;p=calls==1?Point{-200,20,100}:Point{80,20,100};if(calls==2)Check(r.origin.x==1&&r.origin.y==20,"retry follows same camera ray beyond muzzle plane, no lateral offset");return true;};
    Check(ForwardTarget(ray,&muzzle,target,hit,behind)&&hit&&target.x==80&&calls==2,"camera obstruction behind the shot cannot reverse aiming");
    calls=0;
    auto penetration=[&](const Ray&,Point& p,bool& h){++calls;p={-20,20,100};h=true;return true;};
    Check(ForwardTarget(ray,&muzzle,target,hit,penetration)&&!hit&&target.x>0&&calls==2,"persistent start penetration falls forward without a false collision hit");
    auto miss=[](const Ray&,Point& p,bool& h){p={99700,20,100};h=false;return true;};
    Check(ForwardTarget(ray,&muzzle,target,hit,miss)&&!hit&&target.y==20,"unobstructed sky target geometry remains unchanged");
    const Ray bad{{0,0,0},{std::numeric_limits<float>::quiet_NaN(),0,0}};
    Check(!ForwardTarget(bad,&muzzle,target,hit,miss),"invalid ray fails open");
    std::cout<<"Forward collision targets preserve cover and reject camera-side reversal\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
