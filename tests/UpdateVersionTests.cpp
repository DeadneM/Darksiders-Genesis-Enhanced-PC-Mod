#include "UpdateVersionPolicy.h"
#include <string_view>
using namespace dg;
static_assert(!update::NewerThanCurrent(build::Major,build::Minor));
static_assert(!update::NewerThanCurrent(build::Major,build::Minor-1));
static_assert(!update::NewerThanCurrent(0,99));
static_assert(update::NewerThanCurrent(build::Major,build::Minor+1));
static_assert(update::NewerThanCurrent(build::Major+1,0));
static_assert(update::MatchesCurrent(build::Major,build::Minor,build::Archive));
static_assert(!update::MatchesCurrent(build::Major,build::Minor,"another-archive.zip"));
static_assert(!update::MatchesCurrent(1,22,build::Archive));
static_assert(std::string_view(build::Archive).find(build::Version)!=std::string_view::npos);
int main(){return 0;}
