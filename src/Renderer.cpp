#include <Hephaestus/Renderer.hpp>

#include <glad/gl.h>

Renderer::Renderer(const SApplicationSettings& applicationSettings,
                   const SApplicationContext& applicationContext)
    : ApplicationSettings(applicationSettings), ApplicationContext(applicationContext) {
}

auto Renderer::RenderUserInterface(SRenderContext& renderContext,
                                   IScene& scene) -> void {

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}