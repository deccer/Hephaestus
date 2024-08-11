#pragma once

#include <Hephaestus/ApplicationSettings.hpp>
#include <Hephaestus/ApplicationContext.hpp>

struct IScene;

struct SRenderContext {
    float DeltaTime;
    bool IsSrgbDisabled;
    uint64_t FrameCounter;
};

struct IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual auto Load() -> bool = 0;
    virtual auto Unload() -> void = 0;
    virtual auto Render(SRenderContext& renderContext,
                        IScene& scene) -> void = 0;
    virtual auto RenderUserInterface(SRenderContext& renderContext,
                                     IScene& scene) -> void = 0;
};

class Renderer : public IRenderer {
public:
    explicit Renderer(const SApplicationSettings& applicationSettings,
                      const SApplicationContext& applicationContext);

    auto RenderUserInterface(SRenderContext& renderContext,
                             IScene& scene) -> void override;
protected:
    SApplicationSettings ApplicationSettings = {};
    SApplicationContext ApplicationContext = {};
};
