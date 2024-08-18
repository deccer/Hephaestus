#pragma once

#include <Hephaestus/VectorMath.hpp>

#include <optional>
#include <string>

#include <entt/entt.hpp>

struct IScene {
public:
    virtual ~IScene() = default;

    virtual auto AddEntity(std::optional<entt::entity> parent,
                           const std::string &assetMeshName,
                           const std::string &assetMaterialName,
                           glm::mat4x4 initialTransform) -> entt::entity = 0;
};

class Scene : public IScene {
public:
    Scene();

    ~Scene() override;

    auto AddEntity(std::optional<entt::entity> parent,
                   const std::string &assetMeshName,
                   const std::string &assetMaterialName,
                   glm::mat4x4 initialTransform) -> entt::entity override;

private:
    entt::registry _registry;
};

