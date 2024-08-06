#pragma once

#include <Xacor/Renderer.hpp>

class DefaultRenderer : public IRenderer {
public:
    ~DefaultRenderer() override;
    auto Load() -> bool override;
    auto Unload() -> void override;
    auto Render(SRenderContext& renderContext, IScene& scene) -> void override;
};