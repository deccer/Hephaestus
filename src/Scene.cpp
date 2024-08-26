#include <Hephaestus/Scene.hpp>

#include <Hephaestus/Components/MeshComponent.hpp>
#include <Hephaestus/Components/TransformComponent.hpp>
#include <Hephaestus/Components/MaterialComponent.hpp>
#include <Hephaestus/Components/ParentComponent.hpp>
#include <Hephaestus/Components/TagCreateGpuResourcesComponent.hpp>

auto TScene::GetRegistry() -> entt::registry& {
    return _registry;
}

auto TScene::AddEntity(std::optional<entt::entity> parent,
                       glm::mat4x4 initialTransform,
                       const std::string &assetMeshName,
                       const std::string &assetMaterialName) -> entt::entity {

    auto entity = _registry.create();
    if (parent.has_value()) {
        auto parentComponent = _registry.get_or_emplace<TParentComponent>(parent.value());
        parentComponent.Children.push_back(entity);
        _registry.emplace<TChildOfComponent>(entity, parent.value());
    }
    if (!assetMeshName.empty()) {
        _registry.emplace<TMeshComponent>(entity, assetMeshName);
    }
    if (!assetMaterialName.empty()) {
        _registry.emplace<TMaterialComponent>(entity, assetMaterialName);
    }
    _registry.emplace<TTransformComponent>(entity, initialTransform);
    _registry.emplace<TTagCreateGpuResourcesComponent>(entity);

    return entity;
}
