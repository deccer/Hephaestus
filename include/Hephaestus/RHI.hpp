#pragma once

#include <array>
#include <expected>
#include <filesystem>
#include <optional>
#include <string>
#include <variant>

#include <Hephaestus/Types.hpp>
#include <Hephaestus/VectorMath.hpp>

struct SExtent2D {
    uint32_t Width{};
    uint32_t Height{};

    bool operator==(const SExtent2D&) const noexcept = default;

    constexpr SExtent2D operator+(const SExtent2D& other) const { return { Width + other.Width, Height + other.Height }; }

    constexpr SExtent2D operator-(const SExtent2D& other) const { return { Width - other.Width, Height - other.Height }; }

    constexpr SExtent2D operator*(const SExtent2D& other) const { return { Width * other.Width, Height * other.Height }; }

    constexpr SExtent2D operator/(const SExtent2D& other) const { return { Width / other.Width, Height / other.Height }; }

    constexpr SExtent2D operator>>(const SExtent2D& other) const {
        return { Width >> other.Width, Height >> other.Height };
    }

    constexpr SExtent2D operator<<(const SExtent2D& other) const {
        return { Width << other.Width, Height << other.Height };
    }

    constexpr SExtent2D operator+(uint32_t value) const { return *this + SExtent2D{ value, value }; }

    constexpr SExtent2D operator-(uint32_t value) const { return *this - SExtent2D{ value, value }; }

    constexpr SExtent2D operator*(uint32_t value) const { return *this * SExtent2D{ value, value }; }

    constexpr SExtent2D operator/(uint32_t value) const { return *this / SExtent2D{ value, value }; }

    constexpr SExtent2D operator>>(uint32_t value) const { return *this >> SExtent2D{ value, value }; }

    constexpr SExtent2D operator<<(uint32_t value) const { return *this << SExtent2D{ value, value }; }
};

constexpr SExtent2D operator+(uint32_t value, SExtent2D extent) {
    return extent + value;
}

constexpr SExtent2D operator-(uint32_t value, SExtent2D extent) {
    return extent - value;
}

constexpr SExtent2D operator*(uint32_t value, SExtent2D extent) {
    return extent * value;
}

constexpr SExtent2D operator/(uint32_t value, SExtent2D extent) {
    return extent / value;
}

constexpr SExtent2D operator>>(uint32_t value, SExtent2D extent) {
    return extent >> value;
}

constexpr SExtent2D operator<<(uint32_t value, SExtent2D extent) {
    return extent << value;
}

struct SExtent3D {
    uint32_t Width{};
    uint32_t Height{};
    uint32_t Depth{};

    operator SExtent2D() const { return { Width, Height }; }

    bool operator==(const SExtent3D&) const noexcept = default;

    constexpr SExtent3D operator+(const SExtent3D& other) const {
        return { Width + other.Width, Height + other.Height, Depth + other.Depth };
    }

    constexpr SExtent3D operator-(const SExtent3D& other) const {
        return { Width - other.Width, Height - other.Height, Depth - other.Depth };
    }

    constexpr SExtent3D operator*(const SExtent3D& other) const {
        return { Width * other.Width, Height * other.Height, Depth * other.Depth };
    }

    constexpr SExtent3D operator/(const SExtent3D& other) const {
        return { Width / other.Width, Height / other.Height, Depth / other.Depth };
    }

    constexpr SExtent3D operator>>(const SExtent3D& other) const {
        return { Width >> other.Width, Height >> other.Height, Depth >> other.Depth };
    }

    constexpr SExtent3D operator<<(const SExtent3D& other) const {
        return { Width << other.Width, Height << other.Height, Depth << other.Depth };
    }

    constexpr SExtent3D operator+(uint32_t value) const { return *this + SExtent3D{ value, value, value }; }

    constexpr SExtent3D operator-(uint32_t value) const { return *this - SExtent3D{ value, value, value }; }

    constexpr SExtent3D operator*(uint32_t value) const { return *this * SExtent3D{ value, value, value }; }

    constexpr SExtent3D operator/(uint32_t value) const { return *this / SExtent3D{ value, value, value }; }

    constexpr SExtent3D operator>>(uint32_t value) const { return *this >> SExtent3D{ value, value, value }; }

    constexpr SExtent3D operator<<(uint32_t value) const { return *this << SExtent3D{ value, value, value }; }
};

constexpr SExtent3D operator+(uint32_t value, SExtent3D extent) {
    return extent + value;
}

constexpr SExtent3D operator-(uint32_t value, SExtent3D extent) {
    return extent - value;
}

constexpr SExtent3D operator*(uint32_t value, SExtent3D extent) {
    return extent * value;
}

constexpr SExtent3D operator/(uint32_t value, SExtent3D extent) {
    return extent / value;
}

constexpr SExtent3D operator>>(uint32_t value, SExtent3D extent) {
    return extent >> value;
}

constexpr SExtent3D operator<<(uint32_t value, SExtent3D extent) {
    return extent << value;
}

struct SOffset2D {

    uint32_t X{};
    uint32_t Y{};

    bool operator==(const SOffset2D&) const noexcept = default;

    constexpr SOffset2D operator+(const SOffset2D& other) const { return { X + other.X, Y + other.Y }; }

    constexpr SOffset2D operator-(const SOffset2D& other) const { return { X - other.X, Y - other.Y }; }

    constexpr SOffset2D operator*(const SOffset2D& other) const { return { X * other.X, Y * other.Y }; }

    constexpr SOffset2D operator/(const SOffset2D& other) const { return { X / other.X, Y / other.Y }; }

    constexpr SOffset2D operator>>(const SOffset2D& other) const { return { X >> other.X, Y >> other.Y }; }

    constexpr SOffset2D operator<<(const SOffset2D& other) const { return { X << other.X, Y << other.Y }; }

    constexpr SOffset2D operator+(uint32_t value) const { return *this + SOffset2D{ value, value }; }

    constexpr SOffset2D operator-(uint32_t value) const { return *this - SOffset2D{ value, value }; }

    constexpr SOffset2D operator*(uint32_t value) const { return *this * SOffset2D{ value, value }; }

    constexpr SOffset2D operator/(uint32_t value) const { return *this / SOffset2D{ value, value }; }

    constexpr SOffset2D operator>>(uint32_t value) const { return *this >> SOffset2D{ value, value }; }

    constexpr SOffset2D operator<<(uint32_t value) const { return *this << SOffset2D{ value, value }; }
};

constexpr SOffset2D operator+(uint32_t value, SOffset2D offset) {
    return offset + value;
}

constexpr SOffset2D operator-(uint32_t value, SOffset2D offset) {
    return offset - value;
}

constexpr SOffset2D operator*(uint32_t value, SOffset2D offset) {
    return offset * value;
}

constexpr SOffset2D operator/(uint32_t value, SOffset2D offset) {
    return offset / value;
}

constexpr SOffset2D operator>>(uint32_t value, SOffset2D offset) {
    return offset >> value;
}

constexpr SOffset2D operator<<(uint32_t value, SOffset2D offset) {
    return offset << value;
}

struct SOffset3D {

    uint32_t X{};
    uint32_t Y{};
    uint32_t Z{};

    operator SOffset2D() const { return { X, Y }; }

    bool operator==(const SOffset3D&) const noexcept = default;

    constexpr SOffset3D operator+(const SOffset3D& other) const { return { X + other.X, Y + other.Y, Z + other.Z }; }

    constexpr SOffset3D operator-(const SOffset3D& other) const { return { X - other.X, Y - other.Y, Z - other.Z }; }

    constexpr SOffset3D operator*(const SOffset3D& other) const { return { X * other.X, Y * other.Y, Z * other.Z }; }

    constexpr SOffset3D operator/(const SOffset3D& other) const { return { X / other.X, Y / other.Y, Z / other.Z }; }

    constexpr SOffset3D operator>>(const SOffset3D& other) const { return { X >> other.X, Y >> other.Y, Z >> other.Z }; }

    constexpr SOffset3D operator<<(const SOffset3D& other) const { return { X << other.X, Y << other.Y, Z << other.Z }; }

    constexpr SOffset3D operator+(uint32_t value) const { return *this + SOffset3D{ value, value, value }; }

    constexpr SOffset3D operator-(uint32_t value) const { return *this - SOffset3D{ value, value, value }; }

    constexpr SOffset3D operator*(uint32_t value) const { return *this * SOffset3D{ value, value, value }; }

    constexpr SOffset3D operator/(uint32_t value) const { return *this / SOffset3D{ value, value, value }; }

    constexpr SOffset3D operator>>(uint32_t value) const { return *this >> SOffset3D{ value, value, value }; }

    constexpr SOffset3D operator<<(uint32_t value) const { return *this << SOffset3D{ value, value, value }; }
};

constexpr SOffset3D operator+(uint32_t value, SOffset3D offset) {
    return offset + value;
}

constexpr SOffset3D operator-(uint32_t value, SOffset3D offset) {
    return offset - value;
}

constexpr SOffset3D operator*(uint32_t value, SOffset3D offset) {
    return offset * value;
}

constexpr SOffset3D operator/(uint32_t value, SOffset3D offset) {
    return offset / value;
}

constexpr SOffset3D operator>>(uint32_t value, SOffset3D offset) {
    return offset >> value;
}

constexpr SOffset3D operator<<(uint32_t value, SOffset3D offset) {
    return offset << value;
}

enum class EFormat : uint32_t {

    Undefined,

    // Color formats
    R8_UNORM,
    R8_SNORM,
    R16_UNORM,
    R16_SNORM,
    R8G8_UNORM,
    R8G8_SNORM,
    R16G16_UNORM,
    R16G16_SNORM,
    R3G3B2_UNORM,
    R4G4B4_UNORM,
    R5G5B5_UNORM,
    R8G8B8_UNORM,
    R8G8B8_SNORM,
    R10G10B10_UNORM,
    R12G12B12_UNORM,
    R16G16B16_SNORM,
    R2G2B2A2_UNORM,
    R4G4B4A4_UNORM,
    R5G5B5A1_UNORM,
    R8G8B8A8_UNORM,
    R8G8B8A8_SNORM,
    R10G10B10A2_UNORM,
    R10G10B10A2_UINT,
    R12G12B12A12_UNORM,
    R16G16B16A16_UNORM,
    R16G16B16A16_SNORM,
    R8G8B8_SRGB,
    R8G8B8A8_SRGB,
    R16_FLOAT,
    R16G16_FLOAT,
    R16G16B16_FLOAT,
    R16G16B16A16_FLOAT,
    R32_FLOAT,
    R32G32_FLOAT,
    R32G32B32_FLOAT,
    R32G32B32A32_FLOAT,
    R11G11B10_FLOAT,
    R9G9B9_E5,
    R8_SINT,
    R8_UINT,
    R16_SINT,
    R16_UINT,
    R32_SINT,
    R32_UINT,
    R8G8_SINT,
    R8G8_UINT,
    R16G16_SINT,
    R16G16_UINT,
    R32G32_SINT,
    R32G32_UINT,
    R8G8B8_SINT,
    R8G8B8_UINT,
    R16G16B16_SINT,
    R16G16B16_UINT,
    R32G32B32_SINT,
    R32G32B32_UINT,
    R8G8B8A8_SINT,
    R8G8B8A8_UINT,
    R16G16B16A16_SINT,
    R16G16B16A16_UINT,
    R32G32B32A32_SINT,
    R32G32B32A32_UINT,

    // Depth & stencil formats
    D32_FLOAT,
    D32_UNORM,
    D24_UNORM,
    D16_UNORM,
    D32_FLOAT_S8_UINT,
    D24_UNORM_S8_UINT,
    S8_UINT,

    // Compressed formats
    // DXT
    BC1_RGB_UNORM,
    BC1_RGB_SRGB,
    BC1_RGBA_UNORM,
    BC1_RGBA_SRGB,
    BC2_RGBA_UNORM,
    BC2_RGBA_SRGB,
    BC3_RGBA_UNORM,
    BC3_RGBA_SRGB,
    // RGTC
    BC4_R_UNORM,
    BC4_R_SNORM,
    BC5_RG_UNORM,
    BC5_RG_SNORM,
    // BPTC
    BC6H_RGB_UFLOAT,
    BC6H_RGB_SFLOAT,
    BC7_RGBA_UNORM,
    BC7_RGBA_SRGB
};

enum class EFormatClass {
    Float,
    Integer,
    Long
};

enum class EBaseTypeClass {
    Float,
    Integer,
    UnsignedInteger
};

enum class ETextureType : uint32_t {
    Texture1D,
    Texture1DArray,
    Texture2D,
    Texture2DArray,
    Texture2DMultisample,
    Texture2DMultisampleArray,
    Texture3D,
    TextureCube,
    TextureCubeArray,
};

enum class ESampleCount : uint32_t {
    One = 1,
    Two = 2,
    Four = 4,
    Eight = 8,
    SixTeen = 16,
    ThirtyTwo = 32,
};

enum class EUploadFormat : uint32_t {
    Undefined,
    Auto,
    R,
    Rg,
    Rgb,
    Bgr,
    Rgba,
    Bgra,
    RInteger,
    RgInteger,
    RgbInteger,
    BgrInteger,
    RgbaInteger,
    BgraInteger,
    Depth,
    StencilIndex,
    DepthStencilIndex,
};

enum class EUploadType : uint32_t {
    Undefined,
    Auto,
    UnsignedByte,
    SignedByte,
    UnsignedShort,
    SignedShort,
    UnsignedInteger,
    SignedInteger,
    Float,
    UnsignedByte332,
    UnsignedByte233Reversed,
    UnsignedShort565,
    UnsignedShort565Reversed,
    UnsignedShort4444,
    UnsignedShort4444Reversed,
    UnsignedShort5551,
    UnsignedShort1555Reversed,
    UnsignedInteger8888,
    UnsignedInteger8888Reversed,
    UnsignedInteger1010102,
    UnsignedInteger2101010Reversed,
};

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

enum class ETextureAddressMode {
    Repeat,
    RepeatMirrored,
    ClampToEdge,
    ClampToBorder,
    ClampToEdgeMirrored,
};

enum class ETextureMinFilter {
    Nearest,
    NearestMipmapLinear,
    NearestMipmapNearest,
    Linear,
    LinearMipmapLinear,
    LinearMipmapNearest,
};

enum class ETextureMagFilter {
    Nearest,
    Linear
};

enum class EPrimitiveTopology {
    Triangles,
    TriangleStip,
    TriangleFan,
    Lines,
};

enum class EFramebufferAttachmentLoadOperation {
    Load,
    Clear,
    DontCare
};

using STextureId = SId<struct TTextureId>;
using SSamplerId = SId<struct TSamplerId>;

struct SCreateTextureDescriptor {
    ETextureType TextureType = {};
    EFormat Format = {};
    SExtent3D Extent = {};
    uint32_t MipMapLevels = 0;
    uint32_t Layers = 0;
    ESampleCount SampleCount = {};
    std::string Label = {};
};

struct SUploadTextureDescriptor {
    uint32_t Level;
    SOffset3D Offset;
    SExtent3D Extent;
    EUploadFormat UploadFormat = EUploadFormat::Auto;
    EUploadType UploadType = EUploadType::Auto;
    const void* PixelData = nullptr;
};

struct STexture {
    uint32_t Id;
    EFormat Format;
    SExtent3D Extent;
    ETextureType TextureType;
};

struct SFramebufferAttachmentClearColor {
    SFramebufferAttachmentClearColor() = default;

    template<typename... Args>
    requires (sizeof...(Args) <= 4)
    SFramebufferAttachmentClearColor(const Args& ... args)
        : Data(std::array<std::common_type_t<std::remove_cvref_t<Args>...>, 4>{ args... }) {
    }

    std::variant<std::array<float, 4>, std::array<uint32_t, 4>, std::array<int32_t, 4>> Data;
};

struct SFramebufferAttachmentClearDepthStencil {
    float Depth = {};
    int32_t Stencil = {};
};

struct SFramebufferAttachmendDescriptor {
    std::string_view Label;
    EFormat Format;
    SExtent2D Extent;
    EFramebufferAttachmentLoadOperation LoadOperation;
};

struct SFramebufferColorAttachmentDescriptor {
    std::string_view Label;
    EFormat Format;
    SExtent2D Extent;
    EFramebufferAttachmentLoadOperation LoadOperation;
    SFramebufferAttachmentClearColor ClearColor;
};

struct SFramebufferDepthStencilAttachmentDescriptor {
    std::string_view Label;
    EFormat Format;
    SExtent2D Extent;
    EFramebufferAttachmentLoadOperation LoadOperation;
    SFramebufferAttachmentClearDepthStencil ClearDepthStencil;
};

struct SFramebufferDescriptor {
    std::string_view Label;
    std::array<std::optional<SFramebufferColorAttachmentDescriptor>, 8> ColorAttachments;
    std::optional<SFramebufferDepthStencilAttachmentDescriptor> DepthStencilAttachment;
};

struct SFramebufferColorAttachment {
    STexture Texture;
    SFramebufferAttachmentClearColor ClearColor;
    EFramebufferAttachmentLoadOperation LoadOperation;
};

struct SFramebufferDepthStencilAttachment {
    STexture Texture;
    SFramebufferAttachmentClearDepthStencil ClearDepthStencil;
    EFramebufferAttachmentLoadOperation LoadOperation;
};

struct SFramebuffer {
    uint32_t Id;
    std::array<std::optional<SFramebufferColorAttachment>, 8> ColorAttachments;
    std::optional<SFramebufferDepthStencilAttachment> DepthStencilAttachment;
};

struct SSamplerDescriptor {
    std::string_view Label;
    ETextureAddressMode AddressModeU = ETextureAddressMode::ClampToEdge;
    ETextureAddressMode AddressModeV = ETextureAddressMode::ClampToEdge;
    ETextureAddressMode AddressModeW = ETextureAddressMode::ClampToEdge;
    ETextureMagFilter MagFilter = ETextureMagFilter::Linear;
    ETextureMinFilter MinFilter = ETextureMinFilter::Linear;
    float LodBias = 0;
    float LodMin = -1000;
    float LodMax = 1000;

    bool operator==(const SSamplerDescriptor& rhs) const {
        return Label == rhs.Label &&
               AddressModeU == rhs.AddressModeU &&
               AddressModeV == rhs.AddressModeV &&
               AddressModeW == rhs.AddressModeW &&
               MagFilter == rhs.MagFilter &&
               MinFilter == rhs.MinFilter &&
               LodBias == rhs.LodBias &&
               LodMin == rhs.LodMin &&
               LodMax == rhs.LodMax;
    }
};

namespace std {
    template<>
    struct hash<SSamplerDescriptor> {
        size_t operator()(const SSamplerDescriptor& samplerDescriptor) const {
            size_t seed = 0;
            hash_combine(seed, samplerDescriptor.Label);
            hash_combine(seed, samplerDescriptor.AddressModeU);
            hash_combine(seed, samplerDescriptor.AddressModeV);
            hash_combine(seed, samplerDescriptor.AddressModeW);
            hash_combine(seed, samplerDescriptor.MagFilter);
            hash_combine(seed, samplerDescriptor.MinFilter);
            hash_combine(seed, samplerDescriptor.LodBias);
            hash_combine(seed, samplerDescriptor.LodMax);
            hash_combine(seed, samplerDescriptor.LodMin);
            return seed;
        }

    private:
        template<typename T>
        void hash_combine(size_t& seed, const T& v) const {
            std::hash<T> hasher;
            seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    };
}

struct SSampler {
    uint32_t Id;
};

struct SInputAssemblyDescriptor {
    EPrimitiveTopology PrimitiveTopology = EPrimitiveTopology::Triangles;
    bool IsPrimitiveRestartEnabled = false;
};

struct SVertexInputAttributeDescriptor {
    uint32_t Location;
    uint32_t Binding;
    EFormat Format;
    uint32_t Offset;
};

struct SVertexInputDescriptor {
    std::array<std::optional<const SVertexInputAttributeDescriptor>, 8> VertexInputAttributes = {};
};

struct SGraphicsPipelineDescriptor {
    std::string_view Label;
    std::string_view VertexShaderFilePath;
    std::string_view FragmentShaderFilePath;

    SInputAssemblyDescriptor InputAssembly;
    std::optional<SVertexInputDescriptor> VertexInput;
};

struct SComputePipelineDescriptor {
    std::string_view Label;
    std::string_view ComputeShaderFilePath;
};

struct SPipeline {

    auto virtual Bind() -> void;

    auto BindBufferAsUniformBuffer(uint32_t buffer, int32_t bindingIndex) -> void;

    auto BindBufferAsShaderStorageBuffer(uint32_t buffer, int32_t bindingIndex) -> void;

    auto BindTexture(int32_t bindingIndex, uint32_t texture) -> void;

    auto BindTextureAndSampler(int32_t bindingIndex, uint32_t texture, uint32_t sampler) -> void;

    auto SetUniform(int32_t location, float value) -> void;

    auto SetUniform(int32_t location, int32_t value) -> void;

    auto SetUniform(int32_t location, uint32_t value) -> void;

    auto SetUniform(int32_t location, uint64_t value) -> void;

    auto SetUniform(int32_t location, const glm::vec2& value) -> void;

    auto SetUniform(int32_t location, const glm::vec3& value) -> void;

    auto SetUniform(int32_t location, float value1, float value2, float value3, float value4) -> void;

    auto SetUniform(int32_t location, int32_t value1, int32_t value2, int32_t value3, int32_t value4) -> void;

    auto SetUniform(int32_t location, const glm::vec4 value) -> void;

    auto SetUniform(int32_t location, const glm::mat4& value) -> void;

    uint32_t Id;
};

struct SGraphicsPipeline : public SPipeline {

    auto Bind() -> void override;

    auto BindBufferAsVertexBuffer(uint32_t buffer, uint32_t bindingIndex, size_t offset, size_t stride) -> void;

    auto DrawArrays(int32_t vertexOffset, size_t vertexCount) -> void;

    auto DrawElements(uint32_t indexBuffer, size_t elementCount) -> void;

    auto DrawElementsInstanced(uint32_t indexBuffer, size_t elementCount, size_t instanceCount) -> void;

    std::optional<uint32_t> InputLayout;
    uint32_t PrimitiveTopology;
    bool IsPrimitiveRestartEnabled;
};

struct SComputePipeline : public SPipeline {

};

auto CreateGraphicsPipeline(const SGraphicsPipelineDescriptor& graphicsPipelineDescriptor) -> std::expected<SGraphicsPipeline, std::string>;

auto CreateComputePipeline(const SComputePipelineDescriptor& computePipelineDescriptor) -> std::expected<SComputePipeline, std::string>;

auto DeletePipeline(SPipeline pipeline) -> void;

auto LoadImageFromFile(const std::filesystem::path& filePath,
                       int32_t* width,
                       int32_t* height,
                       int32_t* components) -> unsigned char*;

auto LoadImageFromMemory(std::byte* encodedData,
                         size_t encodedDataSize,
                         int32_t* width,
                         int32_t* height,
                         int32_t* components) -> unsigned char*;

auto FreeImage(void* pixels) -> void;

auto CreateTexture(const SCreateTextureDescriptor& createTextureDescriptor) -> STextureId;

auto UploadTexture(const STextureId& textureId,
                   const SUploadTextureDescriptor& updateTextureDescriptor) -> void;

auto MakeTextureResident(const STextureId& textureId) -> uint64_t;

auto GenerateMipmaps(const STextureId textureId) -> void;

auto UnloadTextures() -> void;

auto CreateFramebuffer(const SFramebufferDescriptor& framebufferDescriptor) -> SFramebuffer;

auto BindFramebuffer(const SFramebuffer& framebuffer) -> void;

auto DeleteFramebuffer(const SFramebuffer& framebuffer) -> void;

auto SetDebugLabel(const uint32_t object,
                   const uint32_t objectType,
                   const std::string_view label) -> void;

auto PushDebugGroup(const std::string_view label) -> void;

auto PopDebugGroup() -> void;

auto CreateBuffer(std::string_view label,
                  size_t sizeInBytes,
                  const void* data,
                  uint32_t flags) -> uint32_t;

auto UpdateBuffer(uint32_t buffer,
                  size_t offsetInBytes,
                  size_t sizeInBytes,
                  const void* data) -> void;

auto DeleteBuffer(uint32_t buffer) -> void;
