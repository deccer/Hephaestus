#pragma once

#include <Hephaestus/RHI/Extents.hpp>
#include <Hephaestus/RHI/Format.hpp>
#include <Hephaestus/RHI/Texture.hpp>
#include <Hephaestus/Id.hpp>

#include <array>
#include <cstdint>
#include <optional>
#include <variant>

enum class EFramebufferAttachmentLoadOperation {
    Load,
    Clear,
    DontCare
};

struct SFramebufferAttachmentClearColor {
    //SFramebufferAttachmentClearColor() = default;

    template<typename... Args>
    requires (sizeof...(Args) <= 4)
    explicit SFramebufferAttachmentClearColor(const Args& ...args)
        : Color(std::array<std::common_type_t<std::remove_cvref_t<Args>...>, 4>{ args... }) {
    }

    std::variant<std::array<float, 4>, std::array<uint32_t, 4>, std::array<int32_t, 4>> Color;
};

struct SFramebufferAttachmentClearDepthStencil {
    float Depth = {};
    int32_t Stencil = {};
};

struct SFramebufferColorAttachmentDescriptor {
    std::string_view Label = {};
    EFormat Format = {};
    SExtent2D Extent = {};
    EFramebufferAttachmentLoadOperation LoadOperation = {};
    SFramebufferAttachmentClearColor ClearColor;
};

struct SFramebufferDepthStencilAttachmentDescriptor {
    std::string_view Label = {};
    EFormat Format = {};
    SExtent2D Extent = {};
    EFramebufferAttachmentLoadOperation LoadOperation = {};
    SFramebufferAttachmentClearDepthStencil ClearDepthStencil = {};
};

struct SFramebufferDescriptor {
    std::string_view Label = {};
    std::array<std::optional<SFramebufferColorAttachmentDescriptor>, 8> ColorAttachments;
    std::optional<SFramebufferDepthStencilAttachmentDescriptor> DepthStencilAttachment;
};

struct SFramebufferColorAttachment {
    STexture Texture = {};
    SFramebufferAttachmentClearColor ClearColor;
    EFramebufferAttachmentLoadOperation LoadOperation;
};

struct SFramebufferDepthStencilAttachment {
    STexture Texture = {};
    SFramebufferAttachmentClearDepthStencil ClearDepthStencil;
    EFramebufferAttachmentLoadOperation LoadOperation;
};

struct SFramebuffer {
    uint32_t Id = 0;
    std::array<std::optional<SFramebufferColorAttachment>, 8> ColorAttachments;
    std::optional<SFramebufferDepthStencilAttachment> DepthStencilAttachment;
};

auto CreateFramebuffer(const SFramebufferDescriptor& framebufferDescriptor) -> SFramebuffer;

auto BindFramebuffer(const SFramebuffer& framebuffer) -> void;

auto DeleteFramebuffer(const SFramebuffer& framebuffer) -> void;