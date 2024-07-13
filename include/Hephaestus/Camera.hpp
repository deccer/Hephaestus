#pragma once

#include <Hephaestus/VectorMath.hpp>

struct SCamera {

    glm::vec3 Position = {0.0f, 0.0f, 5.0f};
    float Pitch = {};
    float Yaw = {glm::radians(-90.0f)};

    auto GetForwardDirection() -> const glm::vec3
    {
        return glm::vec3{glm::cos(Pitch) * glm::cos(Yaw),
                         glm::sin(Pitch),
                         glm::cos(Pitch) * glm::sin(Yaw)};
    }

    auto GetViewMatrix() -> const glm::mat4
    {
        return glm::lookAt(Position, Position + GetForwardDirection(), glm::vec3(0, 1, 0));
    }
};

inline SCamera g_mainCamera = {};