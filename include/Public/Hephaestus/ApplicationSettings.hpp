#pragma once

#include <cstdint>
#include <string>

enum class TWindowStyle {
    Windowed,
    Fullscreen,
    FullscreenExclusive
};

enum class TResolutionStartup {
    NinetyPercentOfScreenSize,
    UseApplicationSettings
};

struct TApplicationSettings {
    int32_t ResolutionWidth;
    int32_t ResolutionHeight;
    float ResolutionScale;
    TResolutionStartup ResolutionStartup;
    TWindowStyle WindowStyle;
    bool IsDebug;
    bool IsVSyncEnabled;
    std::string Title;
};