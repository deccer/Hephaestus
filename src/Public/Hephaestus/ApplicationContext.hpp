#pragma once

#include <cstdint>
#include <Hephaestus/VectorMath.hpp>

struct SApplicationContext {
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
};