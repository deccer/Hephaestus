#pragma once

#include <cstdint>
#include <functional>

#include <entt/entt.hpp>
#include <Hephaestus/VectorMath.hpp>

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
};

inline struct SApplicationContext {
    bool IsEditor = false;
    bool SleepWhenWindowHasNoFocus = false;
    bool WindowHasFocus = false;
    bool WindowFramebufferResized = false;
    bool SceneViewerResized = false;
    bool CursorJustEntered = false;
    bool CursorIsActive = true;
    float CursorSensitivity = 0.0025f;

    glm::dvec2 CursorPosition = {};
    glm::dvec2 CursorFrameOffset = {};
    glm::ivec2 WindowFramebufferSize = {};
    glm::ivec2 WindowFramebufferScaledSize = {};
    glm::ivec2 SceneViewerSize = {};
    glm::ivec2 SceneViewerScaledSize = {};

    uint64_t FrameCounter = 0;
} g_applicationContext;

auto RunApplication(const SApplicationSettings& applicationSettings,
                    std::function<bool(entt::registry&)>&& applicationLoad,
                    std::function<void(entt::registry&, float)>&& applicationUpdate,
                    std::function<void(entt::registry&, float)>&& applicationRender,
                    std::function<void(entt::registry&, float)>&& applicationRenderUi,
                    std::function<void()>&& applicationUnload) -> void;
