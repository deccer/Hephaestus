#pragma once

#include <Hephaestus/VectorMath.hpp>

#include <optional>
#include <string>

#include <entt/entt.hpp>

class TScene {
public:
    TScene() = default;
    virtual ~TScene() = default;

    virtual auto Load() -> bool = 0;

    auto GetRegistry() -> entt::registry&;

protected:
    auto AddEntity(std::optional<entt::entity> parent,
                   glm::mat4x4 initialTransform,
                   const std::string &assetMeshName,
                   const std::string &assetMaterialName) -> entt::entity;

private:
    entt::registry _registry;
};

