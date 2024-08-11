#include <Hephaestus/DefaultRenderer.hpp>

#include <glad/gl.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

auto DefaultRenderer::Load() -> bool {

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
        .VertexShaderFilePath = "data/shaders/Simple.vs.glsl",
        .FragmentShaderFilePath = "data/shaders/Simple.fs.glsl",
        .InputAssembly = {
            .PrimitiveTopology = EPrimitiveTopology::Triangles
        },
    });

    if (!geometryPassResult) {
        spdlog::error(geometryPassResult.error());
        return false;
    }

    _geometryPassPipeline = *geometryPassResult;

    auto fullscreenPassResult = CreateGraphicsPipeline({
        .Label = "FullscreenPass",
        .VertexShaderFilePath = "data/shaders/FST.vs.glsl",
        .FragmentShaderFilePath = "data/shaders/FST.fs.glsl",
        .InputAssembly = {
                .PrimitiveTopology = EPrimitiveTopology::Triangles
        }
    });

    _fullscreenPassPipeline = *fullscreenPassResult;

    return true;
}

auto DefaultRenderer::Unload() -> void {

    DestroyFramebuffers();
    DeleteGraphicsPipeline(_geometryPassPipeline);
    DeleteGraphicsPipeline(_fullscreenPassPipeline);
}

auto DefaultRenderer::Render(SRenderContext &renderContext,
                             IScene &scene) -> void {

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

    BindFramebuffer(_geometryPassFramebuffer);
}

auto DefaultRenderer::RenderUserInterface(SRenderContext &renderContext,
                                          IScene &scene) -> void {

    Renderer::RenderUserInterface(renderContext, scene);
    auto& fullscreenPipeline = GetGraphicsPipeline(_fullscreenPassPipeline);

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

auto DefaultRenderer::DestroyFramebuffers() -> void {

    DeleteFramebuffer(_geometryPassFramebuffer);
    //DeleteFramebuffer(_resolvePassFramebuffer);
}

auto DefaultRenderer::CreateFramebuffers(const glm::ivec2& scaledFramebufferSize) -> void {

    _geometryPassFramebuffer = CreateFramebuffer({
         .Label = "GeometryPass",
         .ColorAttachments = {
             SFramebufferColorAttachmentDescriptor{
                 .Label = "GeometryAlbedo",
                 .Format = EFormat::R8G8B8A8_SRGB,
                 .Extent = SExtent2D(scaledFramebufferSize.x,
                                     scaledFramebufferSize.y),
                 .LoadOperation = EFramebufferAttachmentLoadOperation::Clear,
                 .ClearColor = SFramebufferAttachmentClearColor{0.4f, 0.3f, 0.2f, 1.0f},
             },
             SFramebufferColorAttachmentDescriptor{
                 .Label = "GeometryNormals",
                 .Format = EFormat::R32G32B32A32_FLOAT,
                 .Extent = SExtent2D(scaledFramebufferSize.x,
                                     scaledFramebufferSize.y),
                 .LoadOperation = EFramebufferAttachmentLoadOperation::Clear,
                 .ClearColor = SFramebufferAttachmentClearColor{0.0f, 0.0f, 0.0f, 1.0f},
             },
         },
         .DepthStencilAttachment = SFramebufferDepthStencilAttachmentDescriptor{
             .Label = "GeometryDepth",
             .Format = EFormat::D24_UNORM_S8_UINT,
             .Extent = SExtent2D(scaledFramebufferSize.x,
                                 scaledFramebufferSize.y),
             .LoadOperation = EFramebufferAttachmentLoadOperation::Clear,
             .ClearDepthStencil = {1.0f, 0},
         }
    });

    /*
    _resolvePassFramebuffer = CreateFramebuffer({
        .Label = "ResolvePass"
    });
     */
}
