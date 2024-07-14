#include <Hephaestus/Game.hpp>
#include <Hephaestus/Assets.hpp>
#include <Hephaestus/CpuComponents.hpp>
#include <Hephaestus/Input.hpp>

#include <entt/entt.hpp>
#include <glad/gl.h>

#include <random>

SApplicationSettings g_applicationSettings = {};
const glm::vec3 g_unitX = glm::vec3{1.0f, 0.0f, 0.0f};
const glm::vec3 g_unitY = glm::vec3{0.0f, 1.0f, 0.0f};
const glm::vec3 g_unitZ = glm::vec3{0.0f, 0.0f, 1.0f};

float g_cameraSpeed = 4.0f;

auto AddEntity(
    entt::registry& registry,
    std::optional<entt::entity> parent,
    const std::string& assetMeshName,
    const std::string& assetMaterialName,
    glm::mat4x4 initialTransform) -> entt::entity {

    auto entityId = registry.create();
    if (parent.has_value()) {
        auto parentComponent = registry.get_or_emplace<SComponentParent>(parent.value());
        parentComponent.Children.push_back(entityId);
        registry.emplace<SComponentChildOf>(entityId, parent.value());
    }
    if (!assetMeshName.empty()) {
        registry.emplace<SComponentMesh>(entityId, assetMeshName);
    }
    if (!assetMaterialName.empty()) {
        registry.emplace<SComponentMaterial>(entityId, assetMaterialName);
    }
    registry.emplace<SComponentTransform>(entityId, initialTransform);
    registry.emplace<SComponentCreateGpuResources>(entityId);

    return entityId;
}

auto AddEntity(
    entt::registry& registry,
    std::optional<entt::entity> parent,
    const std::string& modelName,
    glm::mat4 initialTransform) -> entt::entity {

    auto entityId = registry.create();
    if (parent.has_value()) {
        auto& parentComponent = registry.get_or_emplace<SComponentParent>(parent.value());
        parentComponent.Children.push_back(entityId);

        registry.emplace<SComponentChildOf>(entityId, parent.value());
    }

    auto& modelMeshesNames = GetAssetModelMeshNames(modelName);
    for(auto& modelMeshName : modelMeshesNames) {

        auto& assetMesh = GetAssetMesh(modelMeshName);

        AddEntity(registry, entityId, modelMeshName, assetMesh.MaterialName, initialTransform);
    }

    return entityId;
}

auto GameLoad(entt::registry& registry) -> bool {

    if (!LoadRenderer()) {
        return false;
    }

    LoadModelFromFile("SM_Deccer_Cubes_Textured", "data/default/SM_Deccer_Cubes_Textured.glb");

    AddEntity(registry, std::nullopt, "SM_Deccer_Cubes_Textured", glm::mat4(1.0f));

    auto scale = glm::scale(glm::mat4(1.0f), glm::vec3{1.0f, 1.0f, 1.0f});
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_real_distribution distribution100(0.0f, 100.0f);

    for (auto i = 0; i < 50; i++) {
        AddEntity(registry, std::nullopt, "SM_Deccer_Cubes_Textured",
                  glm::translate(glm::mat4(1.0f), glm::vec3{ -50.0f + distribution100(generator),
                                                             -50.0f + distribution100(generator),
                                                             -50.0f + distribution100(generator) }) * scale);
    }

    return true;
}

auto HandleCamera(float deltaTime) -> void {

    //g_cursorIsActive = glfwGetMouseButton(g_window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE;
    //glfwSetInputMode(g_window, GLFW_CURSOR, g_cursorIsActive ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

    auto& inputState = GetInputState();

    g_applicationContext.CursorIsActive = !inputState.Mouse.IsButtonPressed(EMouseButton::Right);

    const glm::vec3 forward = g_mainCamera.GetForwardDirection();
    const glm::vec3 right = glm::normalize(glm::cross(forward, g_unitY));

    float tempCameraSpeed = g_cameraSpeed;
    if (inputState.Keyboard.IsKeyPressed(EKey::KeyLeftShift)) {
        tempCameraSpeed *= 4.0f;
    }
    if (inputState.Keyboard.IsKeyPressed(EKey::KeyLeftAlt)) {
        tempCameraSpeed *= 40.0f;
    }
    if (inputState.Keyboard.IsKeyPressed(EKey::KeyLeftControl)) {
        tempCameraSpeed *= 0.25f;
    }
    if (inputState.Keyboard.IsKeyPressed(EKey::KeyW)) {
        g_mainCamera.Position += forward * deltaTime * tempCameraSpeed;
    }
    if (inputState.Keyboard.IsKeyPressed(EKey::KeyS)) {
        g_mainCamera.Position -= forward * deltaTime * tempCameraSpeed;
    }
    if (inputState.Keyboard.IsKeyPressed(EKey::KeyA)) {
        g_mainCamera.Position -= right * deltaTime * tempCameraSpeed;
    }
    if (inputState.Keyboard.IsKeyPressed(EKey::KeyD)) {
        g_mainCamera.Position += right * deltaTime * tempCameraSpeed;
    }
    if (inputState.Keyboard.IsKeyPressed(EKey::KeyQ)) {
        g_mainCamera.Position.y -= deltaTime * tempCameraSpeed;
    }
    if (inputState.Keyboard.IsKeyPressed(EKey::KeyE)) {
        g_mainCamera.Position.y += deltaTime * tempCameraSpeed;
    }

    if (!g_applicationContext.CursorIsActive) {
        auto& deltaPosition = inputState.Mouse.GetDeltaCursorPosition();
        g_mainCamera.Yaw += static_cast<float>(deltaPosition.x * g_applicationContext.CursorSensitivity);
        g_mainCamera.Pitch += static_cast<float>(deltaPosition.y * g_applicationContext.CursorSensitivity);
        g_mainCamera.Pitch = glm::clamp(g_mainCamera.Pitch, -glm::half_pi<float>() + 1e-4f, glm::half_pi<float>() - 1e-4f);
    }

    g_applicationContext.CursorFrameOffset = {0.0, 0.0};
    inputState.Mouse.SetDeltaCursorPosition(0.0f, 0.0f);
}

auto GameUpdate(entt::registry& registry, float deltaTime) -> void {

    HandleCamera(deltaTime);
}

auto GameRender(entt::registry& registry, float deltaTime) -> void {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Resize if necessary
    ResizeFramebuffersIfNecessary(g_applicationSettings);
    Render(registry, deltaTime, g_mainCamera);
}

auto GameRenderUi(entt::registry& registry, float deltaTime) -> void {

    RenderUi(registry, deltaTime);
}

auto GameUnload() -> void {

    UnloadRenderer();
}

auto RunGame(const SApplicationSettings& applicationSettings) -> void {

    g_applicationSettings = applicationSettings;
    RunApplication(applicationSettings,
                   &GameLoad,
                   &GameUpdate,
                   &GameRender,
                   &GameRenderUi,
                   &GameUnload);
}