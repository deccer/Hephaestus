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

struct SApplicationCreateInfo {
    SApplicationSettings Settings = {};
    TScene* Scene = nullptr;
    CRenderer* Renderer = nullptr;
};

class Application {
public:
    explicit Application(const SApplicationCreateInfo& applicationCreateInfo);
    ~Application() = default;
    auto Run() -> void;

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
    std::unique_ptr<CRenderer> _renderer;
    std::unique_ptr<TScene> _scene;
    GLFWwindow* _window;
    ImGuiContext* _guiContext;
};