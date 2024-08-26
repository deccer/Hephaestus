#pragma once

#include <Hephaestus/ApplicationSettings.hpp>
#include <Hephaestus/Renderer.hpp>

#include <Hephaestus/RHI/Pipelines.hpp>
#include <Hephaestus/RHI/Framebuffer.hpp>

#include <Hephaestus/GpuMesh.hpp>
#include <Hephaestus/GpuMaterial.hpp>

#include <memory>

class TDefaultRenderer : public TRenderer {
public:
    TDefaultRenderer(const TApplicationSettings& applicationSettings,
                     const TApplicationContext& applicationContext)
                     : TRenderer(applicationSettings, applicationContext) {}
    ~TDefaultRenderer() override = default;

    auto Load() -> bool override;
    auto Unload() -> void override;
    auto Render(TRenderContext& renderContext,
                TScene& scene) -> void override;
    auto RenderUserInterface(TRenderContext& renderContext,
                             TScene& scene) -> void override;
private:
    auto DestroyFramebuffers() -> void;
    auto CreateFramebuffers(const glm::ivec2& scaledFramebufferSize) -> void;
    auto ResizeIfNecessary(const TRenderContext& renderContext) -> void;

    auto CreateGpuMesh(const std::string& meshName) -> void;
    auto CreateGpuMaterial(const std::string& materialName) -> void;

    auto GetGpuMesh(const std::string& meshName) -> TGpuMesh&;
    auto GetGpuMaterial(const std::string& materialName) -> TGpuMaterial&;

    TFramebuffer _geometryPassFramebuffer;
    TGraphicsPipelineId _geometryPassPipelineId = TGraphicsPipelineId::Invalid;
    TGraphicsPipelineId _fullscreenPassPipelineId = TGraphicsPipelineId::Invalid;
};