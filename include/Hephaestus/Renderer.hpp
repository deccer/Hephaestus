#pragma once

#include <Hephaestus/VectorMath.hpp>
#include <Hephaestus/Application.hpp>
#include <Hephaestus/Types.hpp>
#include <Hephaestus/Camera.hpp>

struct SGpuVertexPosition {
    glm::vec3 Position;
};

struct SGpuVertexNormalUvTangent {
    uint32_t Normal;
    uint32_t Tangent;
    glm::vec3 UvAndTangentSign;
};

struct SGpuVertexPositionColor {
    glm::vec3 Position;
    glm::vec4 Color;
};

struct SGpuGlobalUniforms {

    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::vec4 CameraPosition;
};

struct SGpuGlobalLight {
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::vec4 Direction;
    glm::vec4 Strength;
};

struct SDebugLine {
    glm::vec3 StartPosition;
    glm::vec4 StartColor;
    glm::vec3 EndPosition;
    glm::vec4 EndColor;
};

struct SGpuMesh {
    std::string_view Name;
    uint32_t VertexPositionBuffer;
    uint32_t VertexNormalUvTangentBuffer;
    uint32_t IndexBuffer;

    size_t VertexCount;
    size_t IndexCount;

    glm::mat4 InitialTransform;
};

struct SGpuMaterial {
    glm::vec4 BaseColor;
    glm::vec4 Factors; // use .x = basecolor factor .y = normal strength, .z = metalness, .w = roughness

    uint64_t BaseColorTexture;
    uint64_t NormalTexture;

    uint64_t ArmTexture;
    uint64_t EmissiveTexture;
};

struct SGlobalLight {

    float Azimuth;
    float Elevation;
    glm::vec3 Color;
    float Strength;

    int32_t ShadowMapSize;
    float ShadowMapNear;
    float ShadowMapFar;

    bool ShowFrustum;
};

struct SGpuObject {
    glm::mat4x4 WorldMatrix;
    glm::ivec4 InstanceParameter;
};

struct SMeshInstance {
    glm::mat4 WorldMatrix;
};

auto LoadRenderer() -> bool;
auto UnloadRenderer() -> void;
auto ResizeFramebuffersIfNecessary(const SApplicationSettings& applicationSettings) -> void;
auto Render(entt::registry& registry, float deltaTime, SCamera& camera) -> void;
auto RenderUi(entt::registry& registry, float deltaTime) -> void;