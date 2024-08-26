#pragma once

#include <Hephaestus/VectorMath.hpp>

#include <cstdint>

struct SGpuMesh {
    uint32_t VertexPositionBuffer;
    uint32_t VertexNormalUvTangentBuffer;
    uint32_t IndexBuffer;

    std::size_t VertexCount;
    std::size_t IndexCount;

    glm::mat4 InitialTransform;
};

