#include <Hephaestus/DefaultRenderer.hpp>

#include <glad/gl.h>

#include <spdlog/spdlog.h>

DefaultRenderer::DefaultRenderer(const SApplicationSettings& applicationSettings,
                                 const SApplicationContext& applicationContext) {
    _applicationSettings = applicationSettings;
    _applicationContext = applicationContext;
}

DefaultRenderer::~DefaultRenderer() {

}

auto DefaultRenderer::Load() -> bool {

    _applicationContext.WindowFramebufferScaledSize = glm::ivec2{
        _applicationContext.WindowFramebufferSize.x * _applicationSettings.ResolutionScale,
        _applicationContext.WindowFramebufferSize.y * _applicationSettings.ResolutionScale};
    _applicationContext.SceneViewerScaledSize = glm::ivec2{
        _applicationContext.SceneViewerSize.x * _applicationSettings.ResolutionScale,
        _applicationContext.SceneViewerSize.y * _applicationSettings.ResolutionScale};

    glm::ivec2 scaledFramebufferSize = {};

    if (_applicationContext.IsEditor) {
        scaledFramebufferSize = _applicationContext.SceneViewerScaledSize;
    } else {
        scaledFramebufferSize = _applicationContext.WindowFramebufferScaledSize;
    }
    CreateFramebuffers(scaledFramebufferSize);

    /*
    auto depthPrePassResult = CreateGraphicsPipeline({});
    if (!depthPrePassResult) {
        spdlog::error(depthPrePassResult.error());
        return false;
    }
    _depthPrePassPipeline.reset(&depthPrePassResult.value());
     */

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

    return true;
}

auto DefaultRenderer::Unload() -> void {

    DestroyFramebuffers();
    //DeletePipeline(*_depthPrePassPipeline);
    DeletePipeline(_geometryPassPipeline);
    //DeletePipeline(*_resolvePassPipeline);
}

auto DefaultRenderer::Render(SRenderContext &renderContext,
                             IScene &scene) -> void {

    if (_applicationContext.WindowFramebufferResized || _applicationContext.SceneViewerResized) {
        _applicationContext.WindowFramebufferScaledSize = glm::ivec2{
            _applicationContext.WindowFramebufferSize.x * _applicationSettings.ResolutionScale,
            _applicationContext.WindowFramebufferSize.y * _applicationSettings.ResolutionScale};
        _applicationContext.SceneViewerScaledSize = glm::ivec2{
            _applicationContext.SceneViewerSize.x * _applicationSettings.ResolutionScale,
            _applicationContext.SceneViewerSize.y * _applicationSettings.ResolutionScale};

        glm::ivec2 scaledFramebufferSize = {};

        if (_applicationContext.IsEditor) {
            scaledFramebufferSize = _applicationContext.SceneViewerScaledSize;
        } else {
            scaledFramebufferSize = _applicationContext.WindowFramebufferScaledSize;
        }

        if (renderContext.FrameCounter > 0) {
            DestroyFramebuffers();
            if (scaledFramebufferSize.x + scaledFramebufferSize.y <= glm::epsilon<float>()) {
                scaledFramebufferSize = _applicationContext.WindowFramebufferScaledSize;
            }
        }
        CreateFramebuffers(scaledFramebufferSize);

        glViewport(0, 0, scaledFramebufferSize.x, scaledFramebufferSize.y);

        _applicationContext.WindowFramebufferResized = false;
        _applicationContext.SceneViewerResized = false;
    }

    BindFramebuffer(_geometryPassFramebuffer);
}

auto DefaultRenderer::DestroyFramebuffers() -> void {

    //DeleteFramebuffer(_depthPrePassFramebuffer);
    DeleteFramebuffer(_geometryPassFramebuffer);
    //DeleteFramebuffer(_resolvePassFramebuffer);
}

auto DefaultRenderer::CreateFramebuffers(const glm::ivec2& scaledFramebufferSize) -> void {

    /*
    _depthPrePassFramebuffer = CreateFramebuffer({
         .Label = "DepthPrePass",
    });
     */

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
