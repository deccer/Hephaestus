#pragma once

#include <Hephaestus/RHI/Extents.hpp>
#include <Hephaestus/RHI/Format.hpp>
#include <Hephaestus/RHI/Texture.hpp>
#include <Hephaestus/Id.hpp>

#include <array>
#include <cstdint>
#include <optional>
#include <variant>

enum class TFramebufferAttachmentLoadOperation {
    Load,
    Clear,
    DontCare
};

struct TFramebufferAttachmentClearColor {
    //TFramebufferAttachmentClearColor() = default;

    template<typename... Args>
    requires (sizeof...(Args) <= 4)
    explicit TFramebufferAttachmentClearColor(const Args& ...args)
        : Color(std::array<std::common_type_t<std::remove_cvref_t<Args>...>, 4>{ args... }) {
    }

    std::variant<std::array<float, 4>, std::array<uint32_t, 4>, std::array<int32_t, 4>> Color;
};

struct TFramebufferAttachmentClearDepthStencil {
    float Depth = {};
    int32_t Stencil = {};
};

struct TFramebufferColorAttachmentDescriptor {
    std::string_view Label = {};
    TFormat Format = {};
    TExtent2D Extent = {};
    TFramebufferAttachmentLoadOperation LoadOperation = {};
    TFramebufferAttachmentClearColor ClearColor;
};

struct TFramebufferDepthStencilAttachmentDescriptor {
    std::string_view Label = {};
    TFormat Format = {};
    TExtent2D Extent = {};
    TFramebufferAttachmentLoadOperation LoadOperation = {};
    TFramebufferAttachmentClearDepthStencil ClearDepthStencil = {};
};

struct TFramebufferDescriptor {
    std::string_view Label = {};
    std::array<std::optional<TFramebufferColorAttachmentDescriptor>, 8> ColorAttachments;
    std::optional<TFramebufferDepthStencilAttachmentDescriptor> DepthStencilAttachment;
};

struct TFramebufferColorAttachment {
    TTexture Texture = {};
    TFramebufferAttachmentClearColor ClearColor;
    TFramebufferAttachmentLoadOperation LoadOperation;
};

struct TFramebufferDepthStencilAttachment {
    TTexture Texture = {};
    TFramebufferAttachmentClearDepthStencil ClearDepthStencil;
    TFramebufferAttachmentLoadOperation LoadOperation;
};

struct TFramebuffer {
    uint32_t Id = 0;
    std::array<std::optional<TFramebufferColorAttachment>, 8> ColorAttachments;
    std::optional<TFramebufferDepthStencilAttachment> DepthStencilAttachment;
};

auto CreateFramebuffer(const TFramebufferDescriptor& framebufferDescriptor) -> TFramebuffer;

auto BindFramebuffer(const TFramebuffer& framebuffer) -> void;

auto DeleteFramebuffer(const TFramebuffer& framebuffer) -> void;