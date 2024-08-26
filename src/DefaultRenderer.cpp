#include <Hephaestus/DefaultRenderer.hpp>
#include <Hephaestus/Scene.hpp>

#include <Hephaestus/Components/MeshComponent.hpp>
#include <Hephaestus/Components/MaterialComponent.hpp>
#include <Hephaestus/Components/TransformComponent.hpp>

#include <Hephaestus/Components/TagCreateGpuResourcesComponent.hpp>
#include <Hephaestus/Components/GpuMaterialComponent.hpp>
#include <Hephaestus/Components/GpuMeshComponent.hpp>

#include <Hephaestus/RHI/Debug.hpp>
#include <Hephaestus/RHI/VertexTypes.hpp>

#include <Hephaestus/Assets/Assets.hpp>

#include <glad/gl.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

#include <parallel_hashmap/phmap.h>

phmap::flat_hash_map<std::string, TGpuMeshComponent> g_gpuMeshComponents = {};
phmap::flat_hash_map<std::string, TGpuMaterialComponent> g_gpuMaterialComponents = {};

phmap::flat_hash_map<std::string, TGpuMesh> g_gpuMeshes = {};
phmap::flat_hash_map<std::string, TGpuMaterial> g_gpuMaterials = {};

auto TDefaultRenderer::Load() -> bool {

    ApplicationContext.WindowFramebufferScaledSize = glm::ivec2{
        ApplicationContext.WindowFramebufferSize.x * ApplicationSettings.ResolutionScale,
        ApplicationContext.WindowFramebufferSize.y * ApplicationSettings.ResolutionScale};
    ApplicationContext.SceneViewerScaledSize = glm::ivec2{
        ApplicationContext.SceneViewerSize.x * ApplicationSettings.ResolutionScale,
        ApplicationContext.SceneViewerSize.y * ApplicationSettings.ResolutionScale};

    glm::ivec2 scaledFramebufferSize = {};

    if (ApplicationContext.IsEditor) {
        scaledFramebufferSize = ApplicationContext.SceneViewerScaledSize;
    } else {
        scaledFramebufferSize = ApplicationContext.WindowFramebufferScaledSize;
    }
    CreateFramebuffers(scaledFramebufferSize);

    auto geometryPassResult = CreateGraphicsPipeline({
        .Label = "GeometryPass",
        .VertexShaderFilePath = "data/Shaders/Default/Scene.vs.glsl",
        .FragmentShaderFilePath = "data/Shaders/Default/Scene.fs.glsl",
        .InputAssembly = {
            .PrimitiveTopology = TPrimitiveTopology::Triangles
        },
    });

    if (!geometryPassResult) {
        spdlog::error(geometryPassResult.error());
        return false;
    }

    _geometryPassPipelineId = *geometryPassResult;

    auto fullscreenPassResult = CreateGraphicsPipeline({
        .Label = "FullscreenPass",
        .VertexShaderFilePath = "data/Shaders/FST.vs.glsl",
        .FragmentShaderFilePath = "data/Shaders/FST.fs.glsl",
        .InputAssembly = {
                .PrimitiveTopology = TPrimitiveTopology::Triangles
        }
    });

    _fullscreenPassPipelineId = *fullscreenPassResult;

    return true;
}

auto TDefaultRenderer::Unload() -> void {

    DestroyFramebuffers();
    DeleteGraphicsPipeline(_geometryPassPipelineId);
    DeleteGraphicsPipeline(_fullscreenPassPipelineId);
}

auto TDefaultRenderer::Render(TRenderContext& renderContext,
                              TScene& scene) -> void {

    ResizeIfNecessary(renderContext);

    auto& registry = scene.GetRegistry();

    ///////////////////////
    // Create Gpu Resources if necessary
    ///////////////////////

    auto createGpuResourcesNecessaryView = registry.view<TTagCreateGpuResourcesComponent>();
    for (auto& entity : createGpuResourcesNecessaryView) {

        auto& meshComponent = registry.get<TMeshComponent>(entity);
        auto& materialComponent = registry.get<TMaterialComponent>(entity);

        CreateGpuMesh(meshComponent.MeshName);
        CreateGpuMaterial(materialComponent.MaterialName);

        registry.emplace<TGpuMeshComponent>(entity, meshComponent.MeshName);
        registry.emplace<TGpuMaterialComponent>(entity, materialComponent.MaterialName);

        registry.remove<TTagCreateGpuResourcesComponent>(entity);
    }

    BindFramebuffer(_geometryPassFramebuffer);
    auto& geometryGraphicsPipeline = GetGraphicsPipeline(_geometryPassPipelineId);

    auto worldMatrix = glm::mat4(1.0f);
    auto materialIndex = 0u;

    geometryGraphicsPipeline.Bind();

    auto gpuResourcesView = registry.view<TGpuMeshComponent>();
    for (auto& entity : gpuResourcesView) {

        auto& gpuMeshComponent = registry.get<TGpuMeshComponent>(entity);
        auto& gpuMaterialComponent = registry.get<TGpuMaterialComponent>(entity);
        auto& transformComponent = registry.get<TTransformComponent>(entity);

        auto& gpuMesh = GetGpuMesh(gpuMeshComponent.MeshName);
        auto& gpuMaterial = GetGpuMaterial(gpuMaterialComponent.MaterialName);
    }
    geometryGraphicsPipeline.SetUniform(0, worldMatrix);
    geometryGraphicsPipeline.SetUniform(5, materialIndex);
}

auto TDefaultRenderer::RenderUserInterface(TRenderContext &renderContext,
                                           TScene &scene) -> void {

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    auto& fullscreenPipeline = GetGraphicsPipeline(_fullscreenPassPipelineId);

    fullscreenPipeline.Bind();
    fullscreenPipeline.BindTexture(0, _geometryPassFramebuffer.ColorAttachments[0].value().Texture.Id);
    fullscreenPipeline.DrawArrays(0, 3);

    if (!ApplicationContext.IsEditor) {
        ImGui::SetNextWindowPos({32, 32});
        ImGui::SetNextWindowSize({168, 176});
        auto windowBackgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
        windowBackgroundColor.w = 0.4f;
        ImGui::PushStyleColor(ImGuiCol_WindowBg, windowBackgroundColor);
        if (ImGui::Begin("#InGameStatistics", nullptr, ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoDecoration)) {

            ImGui::TextColored(ImVec4{0.9f, 0.7f, 0.0f, 1.0f}, "F1 to toggle editor");
            ImGui::SeparatorText("Frame Statistics");

            auto framesPerSecond = 1.0f / renderContext.DeltaTime;
            ImGui::Text("afps: %.0f rad/s", glm::two_pi<float>() * framesPerSecond);
            ImGui::Text("dfps: %.0f °/s", glm::degrees(glm::two_pi<float>() * framesPerSecond));
            ImGui::Text("rfps: %.0f", framesPerSecond);
            ImGui::Text("rpms: %.0f", framesPerSecond * 60.0f);
            ImGui::Text("  ft: %.2f ms", renderContext.DeltaTime * 1000.0f);
            ImGui::Text("   f: %lu", renderContext.FrameCounter);
        }
        ImGui::End();
        ImGui::PopStyleColor();
    }

}

auto TDefaultRenderer::DestroyFramebuffers() -> void {

    DeleteFramebuffer(_geometryPassFramebuffer);
}

auto TDefaultRenderer::CreateFramebuffers(const glm::ivec2& scaledFramebufferSize) -> void {

    _geometryPassFramebuffer = CreateFramebuffer({
         .Label = "GeometryPass",
         .ColorAttachments = {
             TFramebufferColorAttachmentDescriptor{
                 .Label = "GeometryAlbedo",
                 .Format = TFormat::R8G8B8A8_SRGB,
                 .Extent = TExtent2D(scaledFramebufferSize.x,
                                     scaledFramebufferSize.y),
                 .LoadOperation = TFramebufferAttachmentLoadOperation::Clear,
                 .ClearColor = TFramebufferAttachmentClearColor{0.4f, 0.3f, 0.2f, 1.0f},
             },
             TFramebufferColorAttachmentDescriptor{
                 .Label = "GeometryNormals",
                 .Format = TFormat::R32G32B32A32_FLOAT,
                 .Extent = TExtent2D(scaledFramebufferSize.x,
                                     scaledFramebufferSize.y),
                 .LoadOperation = TFramebufferAttachmentLoadOperation::Clear,
                 .ClearColor = TFramebufferAttachmentClearColor{0.0f, 0.0f, 0.0f, 1.0f},
             },
         },
         .DepthStencilAttachment = TFramebufferDepthStencilAttachmentDescriptor{
             .Label = "GeometryDepth",
             .Format = TFormat::D24_UNORM_S8_UINT,
             .Extent = TExtent2D(scaledFramebufferSize.x,
                                 scaledFramebufferSize.y),
             .LoadOperation = TFramebufferAttachmentLoadOperation::Clear,
             .ClearDepthStencil = {1.0f, 0},
         }
    });

    /*
    _resolvePassFramebuffer = CreateFramebuffer({
        .Label = "ResolvePass"
    });
     */
}

auto TDefaultRenderer::ResizeIfNecessary(const TRenderContext& renderContext) -> void {

    if (ApplicationContext.WindowFramebufferResized || ApplicationContext.SceneViewerResized) {
        ApplicationContext.WindowFramebufferScaledSize = glm::ivec2{
            ApplicationContext.WindowFramebufferSize.x * ApplicationSettings.ResolutionScale,
            ApplicationContext.WindowFramebufferSize.y * ApplicationSettings.ResolutionScale};
        ApplicationContext.SceneViewerScaledSize = glm::ivec2{
            ApplicationContext.SceneViewerSize.x * ApplicationSettings.ResolutionScale,
            ApplicationContext.SceneViewerSize.y * ApplicationSettings.ResolutionScale};

        glm::ivec2 scaledFramebufferSize = {};

        if (ApplicationContext.IsEditor) {
            scaledFramebufferSize = ApplicationContext.SceneViewerScaledSize;
        } else {
            scaledFramebufferSize = ApplicationContext.WindowFramebufferScaledSize;
        }

        if (renderContext.FrameCounter > 0) {
            DestroyFramebuffers();
            if (scaledFramebufferSize.x + scaledFramebufferSize.y <= glm::epsilon<float>()) {
                scaledFramebufferSize = ApplicationContext.WindowFramebufferScaledSize;
            }
        }
        CreateFramebuffers(scaledFramebufferSize);

        glViewport(0, 0, scaledFramebufferSize.x, scaledFramebufferSize.y);

        ApplicationContext.WindowFramebufferResized = false;
        ApplicationContext.SceneViewerResized = false;
    }
}

auto TDefaultRenderer::CreateGpuMesh(const std::string& meshName) -> void {

    auto& assetMesh = GetAssetMesh(meshName);

    uint32_t buffers[3] = {};
    {
        glCreateBuffers(3, buffers);

        SetDebugLabel(buffers[0], GL_BUFFER, std::format("{}_TGpuVertexPosition", meshName));
        SetDebugLabel(buffers[1], GL_BUFFER, std::format("{}_TGpuVertexNormalUvTangent", meshName));
        SetDebugLabel(buffers[2], GL_BUFFER, std::format("{}_Indices", meshName));

        glNamedBufferStorage(buffers[0], sizeof(TGpuVertexPosition) * assetMesh.VertexPositions.size(),
                             assetMesh.VertexPositions.data(), 0);
        glNamedBufferStorage(buffers[1], sizeof(TGpuVertexNormalUvTangent) * assetMesh.VertexNormalUvTangents.size(),
                             assetMesh.VertexNormalUvTangents.data(), 0);
        glNamedBufferStorage(buffers[2], sizeof(uint32_t) * assetMesh.Indices.size(), assetMesh.Indices.data(), 0);
    }

    g_gpuMeshes[meshName] = TGpuMesh{
        .VertexPositionBuffer = buffers[0],
        .VertexNormalUvTangentBuffer = buffers[1],
        .IndexBuffer = buffers[2],

        .VertexCount = assetMesh.VertexPositions.size(),
        .IndexCount = assetMesh.Indices.size(),

        .InitialTransform = assetMesh.InitialTransform,
    };
}

auto TDefaultRenderer::CreateGpuMaterial(const std::string& materialName) -> void {


}

auto TDefaultRenderer::GetGpuMesh(const std::string& meshName) -> TGpuMesh& {

    return g_gpuMeshes[meshName];
}

auto TDefaultRenderer::GetGpuMaterial(const std::string& materialName) -> TGpuMaterial& {

    return g_gpuMaterials[materialName];
}