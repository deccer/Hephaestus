#pragma once

#include <cstdint>
#include <string>

enum class EWindowStyle {
    Windowed,
    Fullscreen,
    FullscreenExclusive
};

enum class EResolutionStartup {
    NinetyPercentOfScreenSize,
    UseApplicationSettings
};

struct SApplicationSettings {
    int32_t ResolutionWidth;
    int32_t ResolutionHeight;
    float ResolutionScale;
    EResolutionStartup ResolutionStartup;
    EWindowStyle WindowStyle;
    bool IsDebug;
    bool IsVSyncEnabled;
    std::string Title;
};