#pragma once

#include <Hephaestus/VectorMath.hpp>

#include <cstdint>

struct SGpuMaterial {
    glm::vec4 BaseColor;
    /*
     * .x = base color factor
     * .y = normal strength,
     * .z = metalness,
     * .w = roughness
     */
    glm::vec4 Factors;

    uint64_t BaseColorTexture;
    uint64_t NormalTexture;

    uint64_t ArmTexture;
    uint64_t EmissiveTexture;
};