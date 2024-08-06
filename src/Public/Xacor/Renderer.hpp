#pragma once

struct IScene;

struct SRenderContext {
    float DeltaTime;
};

struct IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual auto Load() -> bool = 0;
    virtual auto Unload() -> void = 0;
    virtual auto Render(SRenderContext& renderContext, IScene& scene) -> void = 0;
};
