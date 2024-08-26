#pragma once

#include <Hephaestus/ApplicationSettings.hpp>
#include <Hephaestus/ApplicationContext.hpp>

struct TScene;

struct SRenderContext {
    float DeltaTime;
    bool IsSrgbDisabled;
    uint64_t FrameCounter;
};

class CRenderer {
public:
    explicit CRenderer(const SApplicationSettings& applicationSettings,
                       const SApplicationContext& applicationContext) : ApplicationSettings(applicationSettings), ApplicationContext(applicationContext) {}

    virtual ~CRenderer() = default;
    virtual auto Load() -> bool = 0;
    virtual auto Unload() -> void = 0;
    virtual auto Render(SRenderContext& renderContext,
                        TScene& scene) -> void = 0;
    virtual auto RenderUserInterface(SRenderContext& renderContext,
                                     TScene& scene) -> void = 0;
protected:
    SApplicationSettings ApplicationSettings = {};
    SApplicationContext ApplicationContext = {};
};

