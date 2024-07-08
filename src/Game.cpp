#include <Hephaestus/Game.hpp>
#include <entt/entt.hpp>

auto GameLoad(entt::registry& registry) -> bool {
    return true;
}

auto GameUpdate(entt::registry& registry, float deltaTime) -> void {

}

auto GameRender(entt::registry& registry, float deltaTime) -> void {

}

auto GameUnload() -> void {

}

auto RunGame(const SApplicationSettings& applicationSettings) -> void {

    RunApplication(applicationSettings,
                   &GameLoad,
                   &GameUpdate,
                   &GameRender,
                   &GameUnload);
}