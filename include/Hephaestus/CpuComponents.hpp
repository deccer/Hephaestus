#pragma once

#include <Hephaestus/VectorMath.hpp>
#include <string>
#include <entt/entt.hpp>

struct SComponentMesh {
    std::string Mesh;
};

struct SComponentMaterial {
    std::string Material;
};

struct SComponentTransform : public glm::mat4x4
{
    using glm::mat4x4::mat4x4;
    using glm::mat4x4::operator=;
    SComponentTransform() = default;
    SComponentTransform(glm::mat4x4 const& g) : glm::mat4x4(g) {}
    SComponentTransform(glm::mat4x4&& g) : glm::mat4x4(std::move(g)) {}
};

struct SComponentParent {
    std::vector<entt::entity> Children;
};

struct SComponentChildOf {
    entt::entity Parent;
};

struct SComponentCreateGpuResources {
};