#pragma once

#include <Xacor/VectorMath.hpp>
#include <Xacor/Renderer.hpp>
#include <Xacor/Scene.hpp>

#include <memory>
#include <string>

struct GLFWwindow;
struct ImGuiContext;

struct IApplication {
public:
    virtual ~IApplication() = default;
    virtual auto Run() -> void = 0;
};

enum class EWindowStyle {
    Windowed,
    Fullscreen,
    FullscreenExclusive
};

enum class EResolutionStartup {
    NinetyPercentOfScreenSize,
    UseApplicationSettings
};

struct SApplicationSettings {
    int32_t ResolutionWidth;
    int32_t ResolutionHeight;
    float ResolutionScale;
    EResolutionStartup ResolutionStartup;
    EWindowStyle WindowStyle;
    bool IsDebug;
    bool IsVSyncEnabled;
    std::string Title;
};

struct SApplicationCreateInfo {
    SApplicationSettings Settings = {};
    IScene* Scene = nullptr;
    IRenderer* Renderer = nullptr;
};

struct SApplicationContext {
    bool IsEditor = false;
    bool SleepWhenWindowHasNoFocus = false;
    bool WindowHasFocus = false;
    bool WindowFramebufferResized = false;
    bool SceneViewerResized = false;
    bool CursorJustEntered = false;
    bool CursorIsActive = true;
    float CursorSensitivity = 0.0025f;

    glm::dvec2 CursorPosition = {};
    glm::dvec2 CursorFrameOffset = {};
    glm::ivec2 WindowFramebufferSize = {};
    glm::ivec2 WindowFramebufferScaledSize = {};
    glm::ivec2 SceneViewerSize = {};
    glm::ivec2 SceneViewerScaledSize = {};

    uint64_t FrameCounter = 0;
};

class Application : public IApplication {
public:
    explicit Application(const SApplicationCreateInfo& applicationCreateInfo);
    ~Application() override = default;
    auto Run() -> void override;

protected:
    friend class ApplicationAccess;

    virtual auto Initialize() -> bool;
    virtual auto Load() -> bool;
    virtual auto Unload() -> void;

    auto OnWindowFocusGained() -> void;
    auto OnWindowFocusLost() -> void;
    auto OnWindowFramebufferSizeChanged(int32_t framebufferWidth,
                                        int32_t framebufferHeight) -> void;

private:
    SApplicationSettings _applicationSettings;
    SApplicationContext _applicationContext;
    std::unique_ptr<IRenderer> _renderer;
    std::unique_ptr<IScene> _scene;
    GLFWwindow* _window;
    ImGuiContext* _guiContext;
};