#pragma once

#include <cstdint>
#include <functional>

#include <entt/entt.hpp>

enum class EWindowStyle {
    Windowed,
    Fullscreen,
    FullscreenExclusive
};

struct SApplicationSettings {
    int32_t ResolutionWidth;
    int32_t ResolutionHeight;
    float ResolutionScale;
    EWindowStyle WindowStyle;
    bool IsDebug;
    bool IsVSyncEnabled;
};

auto RunApplication(const SApplicationSettings &applicationSettings,
                    std::function<bool(entt::registry&)>&& applicationLoad,
                    std::function<void(entt::registry&, float)>&& applicationUpdate,
                    std::function<void(entt::registry&, float)>&& applicationRender,
                    std::function<void()>&& applicationUnload) -> void;
