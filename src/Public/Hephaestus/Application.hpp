#pragma once

#include <Hephaestus/VectorMath.hpp>
#include <Hephaestus/ApplicationContext.hpp>
#include <Hephaestus/ApplicationSettings.hpp>
#include <Hephaestus/Renderer.hpp>
#include <Hephaestus/Scene.hpp>

#include <memory>
#include <string>

struct GLFWwindow;
struct ImGuiContext;

struct IApplication {
public:
    virtual ~IApplication() = default;
    virtual auto Run() -> void = 0;
};

struct SApplicationCreateInfo {
    SApplicationSettings Settings = {};
    IScene* Scene = nullptr;
    IRenderer* Renderer = nullptr;
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