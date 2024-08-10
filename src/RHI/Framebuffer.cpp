#include <Hephaestus/RHI/Framebuffer.hpp>
#include <Hephaestus/RHI/Debug.hpp>

#include <glad/gl.h>

#include <cassert>
#include <format>
#include <utility>

enum class EAttachmentType : uint32_t {
    ColorAttachment0 = 0u,
    ColorAttachment1,
    ColorAttachment2,
    ColorAttachment3,
    ColorAttachment4,
    ColorAttachment5,
    ColorAttachment6,
    ColorAttachment7,
    DepthAttachment,
    StencilAttachment
};

constexpr auto FormatToAttachmentType(EFormat attachmentFormat, size_t colorAttachmentIndex) -> EAttachmentType {

    switch (attachmentFormat) {
        case EFormat::D24_UNORM:
        case EFormat::D24_UNORM_S8_UINT:
        case EFormat::D32_FLOAT:
        case EFormat::D32_FLOAT_S8_UINT:
        case EFormat::D32_UNORM:
            return EAttachmentType::DepthAttachment;
        default:
            return static_cast<EAttachmentType>(
                static_cast<std::underlying_type<EAttachmentType>::type>(EAttachmentType::ColorAttachment0) +
                colorAttachmentIndex);
    }
}

auto AttachmentTypeToGL(EAttachmentType attachmentType) -> uint32_t {
    switch (attachmentType) {
        case EAttachmentType::ColorAttachment0:
            return GL_COLOR_ATTACHMENT0;
        case EAttachmentType::ColorAttachment1:
            return GL_COLOR_ATTACHMENT1;
        case EAttachmentType::ColorAttachment2:
            return GL_COLOR_ATTACHMENT2;
        case EAttachmentType::ColorAttachment3:
            return GL_COLOR_ATTACHMENT3;
        case EAttachmentType::ColorAttachment4:
            return GL_COLOR_ATTACHMENT4;
        case EAttachmentType::ColorAttachment5:
            return GL_COLOR_ATTACHMENT5;
        case EAttachmentType::ColorAttachment6:
            return GL_COLOR_ATTACHMENT6;
        case EAttachmentType::ColorAttachment7:
            return GL_COLOR_ATTACHMENT7;
        case EAttachmentType::DepthAttachment:
            return GL_DEPTH_ATTACHMENT;
        case EAttachmentType::StencilAttachment:
            return GL_STENCIL_ATTACHMENT;
        default:
            std::string message = "AttachmentType not mappable";
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH,
                                 message.size(), message.data());
            return -1;
    }
}

auto CreateFramebuffer(const SFramebufferDescriptor& framebufferDescriptor) -> SFramebuffer {

    SFramebuffer framebuffer = {};
    glCreateFramebuffers(1, &framebuffer.Id);
    if (!framebufferDescriptor.Label.empty()) {
        SetDebugLabel(framebuffer.Id, GL_FRAMEBUFFER, framebufferDescriptor.Label);
    }

    std::array<uint32_t, 8> drawBuffers = {};

    for (auto colorAttachmentIndex = 0; auto colorAttachmentDescriptorValue: framebufferDescriptor.ColorAttachments) {
        if (colorAttachmentDescriptorValue.has_value()) {
            auto& colorAttachmentDescriptor = *colorAttachmentDescriptorValue;
            auto colorAttachmentTextureId = CreateTexture({
                                                              .TextureType = ETextureType::Texture2D,
                                                              .Format = colorAttachmentDescriptor.Format,
                                                              .Extent = SExtent3D(
                                                                  colorAttachmentDescriptor.Extent.Width,
                                                                  colorAttachmentDescriptor.Extent.Height, 1),
                                                              .MipMapLevels = 1,
                                                              .Layers = 0,
                                                              .SampleCount = ESampleCount::One,
                                                              .Label = std::format("{}_{}x{}",
                                                                                   colorAttachmentDescriptor.Label,
                                                                                   colorAttachmentDescriptor.Extent.Width,
                                                                                   colorAttachmentDescriptor.Extent.Height)
                                                          });
            auto& colorAttachmentTexture = GetTexture(colorAttachmentTextureId);

            framebuffer.ColorAttachments[colorAttachmentIndex] = {
                .Texture = colorAttachmentTexture,
                .ClearColor = colorAttachmentDescriptor.ClearColor,
                .LoadOperation = colorAttachmentDescriptor.LoadOperation,
            };

            auto attachmentType = FormatToAttachmentType(colorAttachmentDescriptor.Format, colorAttachmentIndex);
            glNamedFramebufferTexture(framebuffer.Id, AttachmentTypeToGL(attachmentType), colorAttachmentTexture.Id, 0);

            drawBuffers[colorAttachmentIndex] = AttachmentTypeToGL(attachmentType);

        } else {
            framebuffer.ColorAttachments[colorAttachmentIndex] = std::nullopt;
            drawBuffers[colorAttachmentIndex] = GL_NONE;
        }

        colorAttachmentIndex++;
    }

    glNamedFramebufferDrawBuffers(framebuffer.Id, 8, drawBuffers.data());

    if (framebufferDescriptor.DepthStencilAttachment.has_value()) {
        auto& depthStencilAttachment = *framebufferDescriptor.DepthStencilAttachment;
        auto depthTextureId = CreateTexture({
                                                .TextureType = ETextureType::Texture2D,
                                                .Format = depthStencilAttachment.Format,
                                                .Extent = SExtent3D(depthStencilAttachment.Extent.Width,
                                                                    depthStencilAttachment.Extent.Height, 1),
                                                .MipMapLevels = 1,
                                                .Layers = 0,
                                                .SampleCount = ESampleCount::One,
                                                .Label = std::format("{}_{}x{}", depthStencilAttachment.Label,
                                                                     depthStencilAttachment.Extent.Width,
                                                                     depthStencilAttachment.Extent.Height)
                                            });
        auto& depthTexture = GetTexture(depthTextureId);

        auto attachmentType = FormatToAttachmentType(depthStencilAttachment.Format, 0);
        glNamedFramebufferTexture(framebuffer.Id, AttachmentTypeToGL(attachmentType), depthTexture.Id, 0);

        framebuffer.DepthStencilAttachment = {
            .Texture = depthTexture,
            .ClearDepthStencil = depthStencilAttachment.ClearDepthStencil,
            .LoadOperation = depthStencilAttachment.LoadOperation,
        };
    } else {
        framebuffer.DepthStencilAttachment = std::nullopt;
    }

    auto framebufferStatus = glCheckNamedFramebufferStatus(framebuffer.Id, GL_FRAMEBUFFER);
    if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
        auto message = std::format("Framebuffer {} is incomplete", framebufferDescriptor.Label);
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 1, GL_DEBUG_SEVERITY_HIGH,
                             message.size(), message.data());
    }

    return framebuffer;
}

auto BindFramebuffer(const SFramebuffer& framebuffer) -> void {

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.Id);

    for (auto colorAttachmentIndex = 0; auto colorAttachmentValue: framebuffer.ColorAttachments) {
        if (colorAttachmentValue.has_value()) {
            auto& colorAttachment = *colorAttachmentValue;
            if (colorAttachment.LoadOperation == EFramebufferAttachmentLoadOperation::Clear) {
                auto baseTypeClass = FormatToBaseTypeClass(colorAttachment.Texture.Format);
                switch (baseTypeClass) {
                    case EBaseTypeClass::Float:
                        glClearNamedFramebufferfv(framebuffer.Id, GL_COLOR, colorAttachmentIndex, std::get_if<std::array<float, 4>>(
                            &colorAttachment.ClearColor.Color)->data());
                        break;
                    case EBaseTypeClass::Integer:
                        glClearNamedFramebufferiv(framebuffer.Id, GL_COLOR, colorAttachmentIndex,
                                                  std::get_if<std::array<int32_t, 4>>(
                                                      &colorAttachment.ClearColor.Color)->data());
                        break;
                    case EBaseTypeClass::UnsignedInteger:
                        glClearNamedFramebufferuiv(framebuffer.Id, GL_COLOR, colorAttachmentIndex,
                                                   std::get_if<std::array<uint32_t, 4>>(
                                                       &colorAttachment.ClearColor.Color)->data());
                        break;
                    default:
                        std::unreachable;
                }
            }
        }
        colorAttachmentIndex++;
    }

    if (framebuffer.DepthStencilAttachment.has_value()) {
        auto& depthStencilAttachment = *framebuffer.DepthStencilAttachment;
        if (depthStencilAttachment.LoadOperation == EFramebufferAttachmentLoadOperation::Clear) {
            glClearNamedFramebufferfi(framebuffer.Id, GL_DEPTH_STENCIL, 0,
                                      depthStencilAttachment.ClearDepthStencil.Depth,
                                      depthStencilAttachment.ClearDepthStencil.Stencil);
        }
    }
}

auto DeleteFramebuffer(const SFramebuffer& framebuffer) -> void {

    assert(framebuffer.Id != 0);

    for (auto colorAttachment: framebuffer.ColorAttachments) {
        if (colorAttachment.has_value()) {
            glDeleteTextures(1, &(*colorAttachment).Texture.Id);
        }
    }

    if (framebuffer.DepthStencilAttachment.has_value()) {
        glDeleteTextures(1, &(*framebuffer.DepthStencilAttachment).Texture.Id);
    }

    glDeleteFramebuffers(1, &framebuffer.Id);
}