#pragma once

#include <Hephaestus/ApplicationSettings.hpp>
#include <Hephaestus/Renderer.hpp>
#include <Hephaestus/RHI/Pipelines.hpp>
#include <Hephaestus/RHI/Framebuffer.hpp>

#include <memory>

class DefaultRenderer : public Renderer {
public:
    DefaultRenderer(const SApplicationSettings& applicationSettings,
                    const SApplicationContext& applicationContext) : Renderer(applicationSettings, applicationContext) {}
    ~DefaultRenderer() override = default;

    auto Load() -> bool override;
    auto Unload() -> void override;
    auto Render(SRenderContext& renderContext,
                IScene& scene) -> void override;
    auto RenderUserInterface(SRenderContext& renderContext,
                             IScene& scene) -> void override;
private:
    auto DestroyFramebuffers() -> void;
    auto CreateFramebuffers(const glm::ivec2& scaledFramebufferSize) -> void;

    SFramebuffer _geometryPassFramebuffer;
    SGraphicsPipelineId _geometryPassPipeline = SGraphicsPipelineId::Invalid;

    SFramebuffer _fullscreenPassFramebuffer;
    SGraphicsPipelineId _fullscreenPassPipeline = SGraphicsPipelineId::Invalid;
};