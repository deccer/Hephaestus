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

struct TApplicationCreateInfo {
    TApplicationSettings Settings = {};
    TScene* Scene = nullptr;
    TRenderer* Renderer = nullptr;
};

class TApplication {
public:
    explicit TApplication(const TApplicationCreateInfo& applicationCreateInfo);
    ~TApplication() = default;
    auto Run() -> void;

protected:
    friend class TApplicationAccess;

    virtual auto Initialize() -> bool;
    virtual auto Load() -> bool;
    virtual auto Unload() -> void;

    auto OnWindowFocusGained() -> void;
    auto OnWindowFocusLost() -> void;
    auto OnWindowFramebufferSizeChanged(int32_t framebufferWidth,
                                        int32_t framebufferHeight) -> void;

private:
    TApplicationSettings _applicationSettings;
    TApplicationContext _applicationContext;
    std::unique_ptr<TRenderer> _renderer;
    std::unique_ptr<TScene> _scene;
    GLFWwindow* _window;
    ImGuiContext* _guiContext;
};