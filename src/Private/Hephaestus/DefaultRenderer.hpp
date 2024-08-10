#pragma once

#include <Hephaestus/ApplicationSettings.hpp>
#include <Hephaestus/Renderer.hpp>
#include <Hephaestus/RHI/Pipelines.hpp>
#include <Hephaestus/RHI/Framebuffer.hpp>

#include <memory>

class DefaultRenderer : public IRenderer {
public:
    explicit DefaultRenderer(const SApplicationSettings& applicationSettings,
                             const SApplicationContext& applicationContext);

    ~DefaultRenderer() override;
    auto Load() -> bool override;
    auto Unload() -> void override;
    auto Render(SRenderContext& renderContext,
                IScene& scene) -> void override;
private:
    auto DestroyFramebuffers() -> void;
    auto CreateFramebuffers(const glm::ivec2& scaledFramebufferSize) -> void;

    SApplicationSettings _applicationSettings = {};
    SApplicationContext _applicationContext = {};

    SFramebuffer _depthPrePassFramebuffer;
    SGraphicsPipelineId _depthPrePassPipeline = SGraphicsPipelineId::Invalid;

    SFramebuffer _geometryPassFramebuffer;
    SGraphicsPipelineId _geometryPassPipeline = SGraphicsPipelineId::Invalid;

    SFramebuffer _resolvePassFramebuffer;
    SGraphicsPipelineId _resolvePassPipeline = SGraphicsPipelineId::Invalid;
};