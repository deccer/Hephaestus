#include <Hephaestus/Game.hpp>
#include <Hephaestus/RHI.hpp>
#include <entt/entt.hpp>

#include <glad/gl.h>
#include <imgui.h>

SApplicationSettings g_applicationSettings = {};
SFramebuffer g_geometryFramebuffer = {};

glm::ivec2 g_scaledFramebufferSize = {};

auto DeleteRendererFramebuffers() -> void {

    DeleteFramebuffer(g_geometryFramebuffer);
}

auto CreateRendererFramebuffers(const glm::vec2& scaledFramebufferSize) -> void {

    g_geometryFramebuffer = CreateFramebuffer({
                                                  .Label = "Geometry",
                                                  .ColorAttachments = {
                                                      SFramebufferColorAttachmentDescriptor{
                                                          .Label = "GeometryAlbedo",
                                                          .Format = EFormat::R8G8B8A8_SRGB,
                                                          .Extent = SExtent2D(scaledFramebufferSize.x, scaledFramebufferSize.y),
                                                          .LoadOperation = EFramebufferAttachmentLoadOperation::Clear,
                                                          .ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f },
                                                      },
                                                      SFramebufferColorAttachmentDescriptor{
                                                          .Label = "GeometryNormals",
                                                          .Format = EFormat::R32G32B32A32_FLOAT,
                                                          .Extent = SExtent2D(scaledFramebufferSize.x, scaledFramebufferSize.y),
                                                          .LoadOperation = EFramebufferAttachmentLoadOperation::Clear,
                                                          .ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f },
                                                      },
                                                  },
                                                  .DepthStencilAttachment = SFramebufferDepthStencilAttachmentDescriptor{
                                                      .Label = "GeometryDepth",
                                                      .Format = EFormat::D24_UNORM_S8_UINT,
                                                      .Extent = SExtent2D(scaledFramebufferSize.x, scaledFramebufferSize.y),
                                                      .LoadOperation = EFramebufferAttachmentLoadOperation::Clear,
                                                      .ClearDepthStencil = { 1.0f, 0 },
                                                  }
                                              });
}

auto GameLoad(entt::registry& registry) -> bool {
    return true;
}

auto GameUpdate(entt::registry& registry, float deltaTime) -> void {

}

auto GameRender(entt::registry& registry, float deltaTime) -> void {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Resize if necessary
    if (g_applicationContext.WindowFramebufferResized || g_applicationContext.SceneViewerResized) {

        g_applicationContext.WindowFramebufferScaledSize = glm::ivec2{g_applicationContext.WindowFramebufferSize.x * g_applicationSettings.ResolutionScale,
                                                                      g_applicationContext.WindowFramebufferSize.y * g_applicationSettings.ResolutionScale};
        g_applicationContext.SceneViewerScaledSize = glm::ivec2{g_applicationContext.SceneViewerSize.x * g_applicationSettings.ResolutionScale,
                                                                g_applicationContext.SceneViewerSize.y * g_applicationSettings.ResolutionScale};

        if (g_applicationContext.IsEditor) {
            g_scaledFramebufferSize = g_applicationContext.SceneViewerScaledSize;
        } else {
            g_scaledFramebufferSize = g_applicationContext.WindowFramebufferScaledSize;
        }

        if (g_applicationContext.FrameCounter > 0) {
            DeleteRendererFramebuffers();
        }
        CreateRendererFramebuffers(g_scaledFramebufferSize);

        glViewport(0, 0, g_scaledFramebufferSize.x, g_scaledFramebufferSize.y);

        g_applicationContext.WindowFramebufferResized = false;
        g_applicationContext.SceneViewerResized = false;
    }
}

auto GameRenderUI(entt::registry& registry, float deltaTime) -> void {

    if (!g_applicationContext.IsEditor) {
        ImGui::SetNextWindowPos({32, 32});
        ImGui::SetNextWindowSize({168, 152});
        auto windowBackgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
        windowBackgroundColor.w = 0.4f;
        ImGui::PushStyleColor(ImGuiCol_WindowBg, windowBackgroundColor);
        if (ImGui::Begin("#InGameStatistics", nullptr, ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoDecoration)) {

            ImGui::TextColored(ImVec4{0.9f, 0.7f, 0.0f, 1.0f}, "F1 to toggle editor");
            ImGui::SeparatorText("Frame Statistics");

            auto framesPerSecond = 1.0f / deltaTime;
            ImGui::Text("afps: %.0f rad/s", glm::two_pi<float>() * framesPerSecond);
            ImGui::Text("dfps: %.0f °/s", glm::degrees(glm::two_pi<float>() * framesPerSecond));
            ImGui::Text("rfps: %.0f", framesPerSecond);
            ImGui::Text("rpms: %.0f", framesPerSecond * 60.0f);
            ImGui::Text("  ft: %.2f ms", deltaTime * 1000.0f);
            ImGui::Text("   f: %lu", g_applicationContext.FrameCounter);
        }
        ImGui::End();
        ImGui::PopStyleColor();
    }

}

auto GameUnload() -> void {

}

auto RunGame(const SApplicationSettings& applicationSettings) -> void {

    g_applicationSettings = applicationSettings;
    RunApplication(applicationSettings,
                   &GameLoad,
                   &GameUpdate,
                   &GameRender,
                   &GameRenderUI,
                   &GameUnload);
}