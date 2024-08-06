#include <Xacor/DefaultRenderer.hpp>

#include <glad/gl.h>

DefaultRenderer::~DefaultRenderer() {

}

auto DefaultRenderer::Load() -> bool{

    glClearColor(0.3f, 0.4f, 0.2f, 1.0f);
    return true;
}

auto DefaultRenderer::Unload() -> void {

}

auto DefaultRenderer::Render(SRenderContext& renderContext, IScene& scene) -> void {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}