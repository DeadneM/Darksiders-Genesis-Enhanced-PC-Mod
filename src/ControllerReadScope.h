#pragma once

namespace dg::controller_input {
// Some XInput providers forward to another export/provider. Apply the mod
// once to an outer game read, and never to a camera/shortcut hardware read.
// Thread-local scopes also prevent Present polling from changing game reads.
inline thread_local unsigned gameReadDepth=0,physicalReadDepth=0;
class GameRead {
    bool filter_;
public:
    GameRead():filter_(gameReadDepth++==0&&physicalReadDepth==0){}
    ~GameRead(){--gameReadDepth;}
    GameRead(const GameRead&)=delete;
    GameRead& operator=(const GameRead&)=delete;
    bool CanFilter()const{return filter_;}
};
class PhysicalRead {
public:
    PhysicalRead(){++physicalReadDepth;}
    ~PhysicalRead(){--physicalReadDepth;}
    PhysicalRead(const PhysicalRead&)=delete;
    PhysicalRead& operator=(const PhysicalRead&)=delete;
};
}
