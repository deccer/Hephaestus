#pragma once

#include <Hephaestus/VectorMath.hpp>

struct TGpuVertexPosition {
    glm::vec3 Position;
};

struct TGpuVertexNormalUvTangent {
    glm::vec3 Normal;
    glm::vec2 Uv;
    glm::vec4 Tangent;
};

struct TGpuVertexPositionNormalUvTangent {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 Uv;
    glm::vec4 Tangent;
};