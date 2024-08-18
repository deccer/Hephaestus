#include "Hephaestus/Scene.hpp"

Scene::Scene() {

}

Scene::~Scene() {

}

auto Scene::AddEntity(std::optional<entt::entity> parent,
                      const std::string &assetMeshName,
                      const std::string &assetMaterialName,
                      glm::mat4x4 initialTransform) -> entt::entity {

    auto entity = _registry.create();

    return entity;
}
