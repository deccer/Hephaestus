#pragma once

#include <Hephaestus/ApplicationSettings.hpp>
#include <Hephaestus/Renderer.hpp>

#include <Hephaestus/RHI/Pipelines.hpp>
#include <Hephaestus/RHI/Framebuffer.hpp>

#include <Hephaestus/GpuMesh.hpp>
#include <Hephaestus/GpuMaterial.hpp>

#include <memory>

class CDefaultRenderer : public CRenderer {
public:
    CDefaultRenderer(const SApplicationSettings& applicationSettings,
                    const SApplicationContext& applicationContext) : CRenderer(applicationSettings, applicationContext) {}
    ~CDefaultRenderer() override = default;

    auto Load() -> bool override;
    auto Unload() -> void override;
    auto Render(SRenderContext& renderContext,
                TScene& scene) -> void override;
    auto RenderUserInterface(SRenderContext& renderContext,
                             TScene& scene) -> void override;
private:
    auto DestroyFramebuffers() -> void;
    auto CreateFramebuffers(const glm::ivec2& scaledFramebufferSize) -> void;
    auto ResizeIfNecessary(const SRenderContext& renderContext) -> void;

    auto CreateGpuMesh(const std::string& meshName) -> void;
    auto CreateGpuMaterial(const std::string& materialName) -> void;

    auto GetGpuMesh(const std::string& meshName) -> SGpuMesh&;
    auto GetGpuMaterial(const std::string& materialName) -> SGpuMaterial&;

    SFramebuffer _geometryPassFramebuffer;
    SGraphicsPipelineId _geometryPassPipelineId = SGraphicsPipelineId::Invalid;
    SGraphicsPipelineId _fullscreenPassPipelineId = SGraphicsPipelineId::Invalid;
};