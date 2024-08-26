#pragma once

#include <Hephaestus/ApplicationSettings.hpp>
#include <Hephaestus/ApplicationContext.hpp>

struct TScene;

struct TRenderContext {
    float DeltaTime;
    bool IsSrgbDisabled;
    uint64_t FrameCounter;
};

class TRenderer {
public:
    explicit TRenderer(const TApplicationSettings& applicationSettings,
                       const TApplicationContext& applicationContext) : ApplicationSettings(applicationSettings), ApplicationContext(applicationContext) {}

    virtual ~TRenderer() = default;
    virtual auto Load() -> bool = 0;
    virtual auto Unload() -> void = 0;
    virtual auto Render(TRenderContext& renderContext,
                        TScene& scene) -> void = 0;
    virtual auto RenderUserInterface(TRenderContext& renderContext,
                                     TScene& scene) -> void = 0;
protected:
    TApplicationSettings ApplicationSettings = {};
    TApplicationContext ApplicationContext = {};
};

