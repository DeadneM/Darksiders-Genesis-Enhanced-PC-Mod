#pragma once
#include <string>
#include <cstdint>

namespace dg::update {
struct Status {
    bool busy = false;
    bool hasLatest = false;
    bool checkedBeta = false;
    bool newer = false;
    bool sameBuild = false;
    int major = 0;
    int minor = 0;
    std::string latest;
    std::string message = "Choose a channel, then Check for Updates.";
};
Status GetStatus();
uint64_t NoticeId(bool beta);
void Check(bool beta);
void InstallAfterExit(bool beta);
void RestoreAfterExit();
void Invalidate();
}
