#include <Hephaestus/Renderer.hpp>
#include <Hephaestus/Application.hpp>
#include <Hephaestus/CpuComponents.hpp>
#include <Hephaestus/Assets.hpp>

#include <spdlog/spdlog.h>

#include <glad/gl.h>
#include <imgui.h>

#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_GLSL
#include <stb_include.h>

#include <expected>
#include <format>
#include <filesystem>
#include <string>
#include <utility>

constexpr ImVec2 g_imvec2UnitX = ImVec2(1, 0);
constexpr ImVec2 g_imvec2UnitY = ImVec2(0, 1);

// RHI Interface ////////////////////////////////////////////////////////////////

uint32_t g_defaultInputLayout = 0;
uint32_t g_lastIndexBuffer = 0;

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

    virtual ~SPipeline() = default;

    auto virtual Bind() -> void {
        glUseProgram(Id);
    }

    auto BindBufferAsUniformBuffer(uint32_t buffer, int32_t bindingIndex) -> void {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, buffer);
    }

    auto BindBufferAsShaderStorageBuffer(uint32_t buffer, int32_t bindingIndex) -> void {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex, buffer);
    }

    auto BindTexture(int32_t bindingIndex, uint32_t texture) -> void {
        glBindTextureUnit(bindingIndex, texture);
    }

    auto BindTextureAndSampler(int32_t bindingIndex, uint32_t texture, uint32_t sampler) -> void {
        glBindTextureUnit(bindingIndex, texture);
        glBindSampler(bindingIndex, sampler);
    }

    auto SetUniform(int32_t location, float value) -> void {
        glProgramUniform1f(Id, location, value);
    }

    auto SetUniform(int32_t location, int32_t value) -> void {
        glProgramUniform1i(Id, location, value);
    }

    auto SetUniform(int32_t location, uint32_t value) -> void {
        glProgramUniform1ui(Id, location, value);
    }

    auto SetUniform(int32_t location, uint64_t value) -> void {
        glProgramUniformHandleui64ARB(Id, location, value);
    }

    auto SetUniform(int32_t location, const glm::vec2& value) -> void {
        glProgramUniform2fv(Id, location, 1, glm::value_ptr(value));
    }

    auto SetUniform(int32_t location, const glm::vec3& value) -> void {
        glProgramUniform3fv(Id, location, 1, glm::value_ptr(value));
    }

    auto SetUniform(int32_t location, float value1, float value2, float value3, float value4) -> void {
        glProgramUniform4f(Id, location, value1, value2, value3, value4);
    }

    auto SetUniform(int32_t location, int32_t value1, int32_t value2, int32_t value3, int32_t value4) -> void {
        glProgramUniform4i(Id, location, value1, value2, value3, value4);
    }

    auto SetUniform(int32_t location, const glm::vec4& value) -> void {
        glProgramUniform4fv(Id, location, 1, glm::value_ptr(value));
    }

    auto SetUniform(int32_t location, const glm::mat4& value) -> void {
        glProgramUniformMatrix4fv(Id, location, 1, GL_FALSE, glm::value_ptr(value));
    }

    uint32_t Id;
};

struct SGraphicsPipeline : public SPipeline {

    auto Bind() -> void override {

        SPipeline::Bind();
        glBindVertexArray(InputLayout.value_or(g_defaultInputLayout));
    }

    auto BindBufferAsVertexBuffer(uint32_t buffer, uint32_t bindingIndex, long offset, int32_t stride) -> void {

        if (InputLayout.has_value()) {
            glVertexArrayVertexBuffer(*InputLayout, bindingIndex, buffer, offset, stride);
        }
    }

    auto DrawArrays(int32_t vertexOffset, int32_t vertexCount) -> void {

        glDrawArrays(PrimitiveTopology, vertexOffset, vertexCount);
    }

    auto DrawElements(uint32_t indexBuffer, int32_t elementCount) -> void {

        if (g_lastIndexBuffer != indexBuffer) {
            glVertexArrayElementBuffer(InputLayout.has_value() ? InputLayout.value() : g_defaultInputLayout, indexBuffer);
            g_lastIndexBuffer = indexBuffer;
        }

        glDrawElements(PrimitiveTopology, elementCount, GL_UNSIGNED_INT, nullptr);
    }

    auto DrawElementsInstanced(uint32_t indexBuffer, int32_t elementCount, int32_t instanceCount) -> void {
        if (g_lastIndexBuffer != indexBuffer) {
            glVertexArrayElementBuffer(InputLayout.has_value() ? InputLayout.value() : g_defaultInputLayout, indexBuffer);
            g_lastIndexBuffer = indexBuffer;
        }

        glDrawElementsInstanced(PrimitiveTopology, elementCount, GL_UNSIGNED_INT, nullptr, instanceCount);
    }

    std::optional<uint32_t> InputLayout;
    uint32_t PrimitiveTopology;
    bool IsPrimitiveRestartEnabled;
};

struct SComputePipeline : public SPipeline {

};

auto ReadShaderTextFromFile(const std::filesystem::path& filePath) -> std::expected<std::string, std::string> {

    std::string pathStr = filePath.string();
    std::string parentPathStr = filePath.parent_path().string();
    char error[256]{};
    auto processedSource = std::unique_ptr<char, decltype([](char* ptr) { free(ptr); })>(
        stb_include_file(pathStr.data(), nullptr, parentPathStr.data(), error));
    if (!processedSource) {
        return std::unexpected(std::format("Failed to process includes for {}", filePath.string()));
    }

    return processedSource.get();
}

constexpr auto PrimitiveTopologyToGL(EPrimitiveTopology primitiveTopology) -> uint32_t {
    switch (primitiveTopology) {
        case EPrimitiveTopology::Lines:
            return GL_LINES;
        case EPrimitiveTopology::TriangleFan:
            return GL_TRIANGLE_FAN;
        case EPrimitiveTopology::Triangles:
            return GL_TRIANGLES;
        case EPrimitiveTopology::TriangleStip:
            return GL_TRIANGLE_STRIP;
        default:
            std::unreachable();
    }
}

constexpr auto TextureAddressModeToGL(ETextureAddressMode textureAddressMode) -> uint32_t {
    switch (textureAddressMode) {
        case ETextureAddressMode::ClampToBorder :
            return GL_CLAMP_TO_BORDER;
        case ETextureAddressMode::ClampToEdge :
            return GL_CLAMP_TO_EDGE;
        case ETextureAddressMode::ClampToEdgeMirrored:
            return GL_MIRROR_CLAMP_TO_EDGE;
        case ETextureAddressMode::Repeat:
            return GL_REPEAT;
        case ETextureAddressMode::RepeatMirrored:
            return GL_MIRRORED_REPEAT;
        default:
            std::unreachable();
    }
}

constexpr auto TextureMagFilterToGL(ETextureMagFilter textureMagFilter) -> uint32_t {
    switch (textureMagFilter) {
        case ETextureMagFilter::Linear:
            return GL_LINEAR;
        case ETextureMagFilter::Nearest:
            return GL_NEAREST;
        default:
            std::unreachable();
    }
}

constexpr auto TextureMinFilterToGL(ETextureMinFilter textureMinFilter) -> uint32_t {
    switch (textureMinFilter) {
        case ETextureMinFilter::Linear:
            return GL_LINEAR;
        case ETextureMinFilter::Nearest:
            return GL_NEAREST;
        case ETextureMinFilter::LinearMipmapLinear:
            return GL_LINEAR_MIPMAP_LINEAR;
        case ETextureMinFilter::LinearMipmapNearest:
            return GL_LINEAR_MIPMAP_NEAREST;
        case ETextureMinFilter::NearestMipmapLinear:
            return GL_NEAREST_MIPMAP_LINEAR;
        case ETextureMinFilter::NearestMipmapNearest:
            return GL_NEAREST_MIPMAP_NEAREST;
        default:
            std::unreachable();
    }
}

constexpr auto TextureTypeToGL(ETextureType textureType) -> uint32_t {

    switch (textureType) {
        case ETextureType::Texture1D:
            return GL_TEXTURE_1D;
        case ETextureType::Texture2D:
            return GL_TEXTURE_2D;
        case ETextureType::Texture3D:
            return GL_TEXTURE_3D;
        case ETextureType::Texture1DArray:
            return GL_TEXTURE_1D_ARRAY;
        case ETextureType::Texture2DArray:
            return GL_TEXTURE_2D_ARRAY;
        case ETextureType::TextureCube:
            return GL_TEXTURE_CUBE_MAP;
        case ETextureType::TextureCubeArray:
            return GL_TEXTURE_CUBE_MAP_ARRAY;
        case ETextureType::Texture2DMultisample:
            return GL_TEXTURE_2D_MULTISAMPLE;
        case ETextureType::Texture2DMultisampleArray:
            return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
        default:
            std::unreachable();
    }
}

constexpr auto UploadFormatToGL(EUploadFormat uploadFormat) -> uint32_t {

    switch (uploadFormat) {
        case EUploadFormat::R:
            return GL_RED;
        case EUploadFormat::Rg:
            return GL_RG;
        case EUploadFormat::Rgb:
            return GL_RGB;
        case EUploadFormat::Bgr:
            return GL_BGR;
        case EUploadFormat::Rgba:
            return GL_RGBA;
        case EUploadFormat::Bgra:
            return GL_BGRA;
        case EUploadFormat::RInteger:
            return GL_RED_INTEGER;
        case EUploadFormat::RgInteger:
            return GL_RG_INTEGER;
        case EUploadFormat::RgbInteger:
            return GL_RGB_INTEGER;
        case EUploadFormat::BgrInteger:
            return GL_BGR_INTEGER;
        case EUploadFormat::RgbaInteger:
            return GL_RGBA_INTEGER;
        case EUploadFormat::BgraInteger:
            return GL_BGRA_INTEGER;
        case EUploadFormat::Depth:
            return GL_DEPTH_COMPONENT;
        case EUploadFormat::StencilIndex:
            return GL_STENCIL_INDEX;
        case EUploadFormat::DepthStencilIndex:
            return GL_DEPTH_STENCIL;
        default:
            std::unreachable();
    }
}

constexpr auto UploadTypeToGL(EUploadType uploadType) -> uint32_t {

    switch (uploadType) {
        case EUploadType::UnsignedByte:
            return GL_UNSIGNED_BYTE;
        case EUploadType::SignedByte:
            return GL_BYTE;
        case EUploadType::UnsignedShort:
            return GL_UNSIGNED_SHORT;
        case EUploadType::SignedShort:
            return GL_SHORT;
        case EUploadType::UnsignedInteger:
            return GL_UNSIGNED_INT;
        case EUploadType::SignedInteger:
            return GL_INT;
        case EUploadType::Float:
            return GL_FLOAT;
        case EUploadType::UnsignedByte332:
            return GL_UNSIGNED_BYTE_3_3_2;
        case EUploadType::UnsignedByte233Reversed:
            return GL_UNSIGNED_BYTE_2_3_3_REV;
        case EUploadType::UnsignedShort565:
            return GL_UNSIGNED_SHORT_5_6_5;
        case EUploadType::UnsignedShort565Reversed:
            return GL_UNSIGNED_SHORT_5_6_5_REV;
        case EUploadType::UnsignedShort4444:
            return GL_UNSIGNED_SHORT_4_4_4_4;
        case EUploadType::UnsignedShort4444Reversed:
            return GL_UNSIGNED_SHORT_4_4_4_4_REV;
        case EUploadType::UnsignedShort5551:
            return GL_UNSIGNED_SHORT_5_5_5_1;
        case EUploadType::UnsignedShort1555Reversed:
            return GL_UNSIGNED_SHORT_1_5_5_5_REV;
        case EUploadType::UnsignedInteger8888:
            return GL_UNSIGNED_INT_8_8_8_8;
        case EUploadType::UnsignedInteger8888Reversed:
            return GL_UNSIGNED_INT_8_8_8_8_REV;
        case EUploadType::UnsignedInteger1010102:
            return GL_UNSIGNED_INT_10_10_10_2;
        case EUploadType::UnsignedInteger2101010Reversed:
            return GL_UNSIGNED_INT_2_10_10_10_REV;
        default:
            std::unreachable();
    }
}

constexpr auto SampleCountToGL(ESampleCount sampleCount) -> uint32_t {

    switch (sampleCount) {
        case ESampleCount::One:
            return 1;
        case ESampleCount::Two:
            return 2;
        case ESampleCount::Four:
            return 4;
        case ESampleCount::Eight:
            return 8;
        case ESampleCount::SixTeen:
            return 16;
        case ESampleCount::ThirtyTwo:
            return 32;
        default:
            std::unreachable();
    }
}

auto FormatToGL(EFormat format) -> uint32_t {
    switch (format) {
        case EFormat::R8_UNORM:
            return GL_R8;
        case EFormat::R8_SNORM:
            return GL_R8_SNORM;
        case EFormat::R16_UNORM:
            return GL_R16;
        case EFormat::R16_SNORM:
            return GL_R16_SNORM;
        case EFormat::R8G8_UNORM:
            return GL_RG8;
        case EFormat::R8G8_SNORM:
            return GL_RG8_SNORM;
        case EFormat::R16G16_UNORM:
            return GL_RG16;
        case EFormat::R16G16_SNORM:
            return GL_RG16_SNORM;
        case EFormat::R3G3B2_UNORM:
            return GL_R3_G3_B2;
        case EFormat::R4G4B4_UNORM:
            return GL_RGB4;
        case EFormat::R5G5B5_UNORM:
            return GL_RGB5;
        case EFormat::R8G8B8_UNORM:
            return GL_RGB8;
        case EFormat::R8G8B8_SNORM:
            return GL_RGB8_SNORM;
        case EFormat::R10G10B10_UNORM:
            return GL_RGB10;
        case EFormat::R12G12B12_UNORM:
            return GL_RGB12;
            // GL_RG16?
        case EFormat::R16G16B16_SNORM:
            return GL_RGB16_SNORM;
        case EFormat::R2G2B2A2_UNORM:
            return GL_RGBA2;
        case EFormat::R4G4B4A4_UNORM:
            return GL_RGBA4;
        case EFormat::R5G5B5A1_UNORM:
            return GL_RGB5_A1;
        case EFormat::R8G8B8A8_UNORM:
            return GL_RGBA8;
        case EFormat::R8G8B8A8_SNORM:
            return GL_RGBA8_SNORM;
        case EFormat::R10G10B10A2_UNORM:
            return GL_RGB10_A2;
        case EFormat::R10G10B10A2_UINT:
            return GL_RGB10_A2UI;
        case EFormat::R12G12B12A12_UNORM:
            return GL_RGBA12;
        case EFormat::R16G16B16A16_UNORM:
            return GL_RGBA16;
        case EFormat::R16G16B16A16_SNORM:
            return GL_RGBA16_SNORM;
        case EFormat::R8G8B8_SRGB:
            return GL_SRGB8;
        case EFormat::R8G8B8A8_SRGB:
            return GL_SRGB8_ALPHA8;
        case EFormat::R16_FLOAT:
            return GL_R16F;
        case EFormat::R16G16_FLOAT:
            return GL_RG16F;
        case EFormat::R16G16B16_FLOAT:
            return GL_RGB16F;
        case EFormat::R16G16B16A16_FLOAT:
            return GL_RGBA16F;
        case EFormat::R32_FLOAT:
            return GL_R32F;
        case EFormat::R32G32_FLOAT:
            return GL_RG32F;
        case EFormat::R32G32B32_FLOAT:
            return GL_RGB32F;
        case EFormat::R32G32B32A32_FLOAT:
            return GL_RGBA32F;
        case EFormat::R11G11B10_FLOAT:
            return GL_R11F_G11F_B10F;
        case EFormat::R9G9B9_E5:
            return GL_RGB9_E5;
        case EFormat::R8_SINT:
            return GL_R8I;
        case EFormat::R8_UINT:
            return GL_R8UI;
        case EFormat::R16_SINT:
            return GL_R16I;
        case EFormat::R16_UINT:
            return GL_R16UI;
        case EFormat::R32_SINT:
            return GL_R32I;
        case EFormat::R32_UINT:
            return GL_R32UI;
        case EFormat::R8G8_SINT:
            return GL_RG8I;
        case EFormat::R8G8_UINT:
            return GL_RG8UI;
        case EFormat::R16G16_SINT:
            return GL_RG16I;
        case EFormat::R16G16_UINT:
            return GL_RG16UI;
        case EFormat::R32G32_SINT:
            return GL_RG32I;
        case EFormat::R32G32_UINT:
            return GL_RG32UI;
        case EFormat::R8G8B8_SINT:
            return GL_RGB8I;
        case EFormat::R8G8B8_UINT:
            return GL_RGB8UI;
        case EFormat::R16G16B16_SINT:
            return GL_RGB16I;
        case EFormat::R16G16B16_UINT:
            return GL_RGB16UI;
        case EFormat::R32G32B32_SINT:
            return GL_RGB32I;
        case EFormat::R32G32B32_UINT:
            return GL_RGB32UI;
        case EFormat::R8G8B8A8_SINT:
            return GL_RGBA8I;
        case EFormat::R8G8B8A8_UINT:
            return GL_RGBA8UI;
        case EFormat::R16G16B16A16_SINT:
            return GL_RGBA16I;
        case EFormat::R16G16B16A16_UINT:
            return GL_RGBA16UI;
        case EFormat::R32G32B32A32_SINT:
            return GL_RGBA32I;
        case EFormat::R32G32B32A32_UINT:
            return GL_RGBA32UI;
        case EFormat::D32_FLOAT:
            return GL_DEPTH_COMPONENT32F;
        case EFormat::D32_UNORM:
            return GL_DEPTH_COMPONENT32;
        case EFormat::D24_UNORM:
            return GL_DEPTH_COMPONENT24;
        case EFormat::D16_UNORM:
            return GL_DEPTH_COMPONENT16;
        case EFormat::D32_FLOAT_S8_UINT:
            return GL_DEPTH32F_STENCIL8;
        case EFormat::D24_UNORM_S8_UINT:
            return GL_DEPTH24_STENCIL8;
        case EFormat::S8_UINT:
            return GL_STENCIL_INDEX8;
            /*
            case EFormat::BC1_RGB_UNORM: return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
            case EFormat::BC1_RGBA_UNORM: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            case EFormat::BC1_RGB_SRGB: return GL_COMPRESSED_SRGB_S3TC_DXT1_EXT;
            case EFormat::BC1_RGBA_SRGB: return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
            case EFormat::BC2_RGBA_UNORM: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            case EFormat::BC2_RGBA_SRGB: return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
            case EFormat::BC3_RGBA_UNORM: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            case EFormat::BC3_RGBA_SRGB: return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
            */
        case EFormat::BC4_R_UNORM:
            return GL_COMPRESSED_RED_RGTC1;
        case EFormat::BC4_R_SNORM:
            return GL_COMPRESSED_SIGNED_RED_RGTC1;
        case EFormat::BC5_RG_UNORM:
            return GL_COMPRESSED_RG_RGTC2;
        case EFormat::BC5_RG_SNORM:
            return GL_COMPRESSED_SIGNED_RG_RGTC2;
        case EFormat::BC6H_RGB_UFLOAT:
            return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
        case EFormat::BC6H_RGB_SFLOAT:
            return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
        case EFormat::BC7_RGBA_UNORM:
            return GL_COMPRESSED_RGBA_BPTC_UNORM;
        case EFormat::BC7_RGBA_SRGB:
            return GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
        default:
            std::string message = "Format not mappable";
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH,
                                 message.size(), message.data());
            return -1;
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

auto FormatToUploadFormat(EFormat format) -> EUploadFormat {

    switch (format) {
        case EFormat::R8_UNORM:
        case EFormat::R8_SNORM:
        case EFormat::R16_UNORM:
        case EFormat::R16_SNORM:
        case EFormat::R16_FLOAT:
        case EFormat::R32_FLOAT:
            return EUploadFormat::R;
        case EFormat::R8_SINT:
        case EFormat::R8_UINT:
        case EFormat::R16_SINT:
        case EFormat::R16_UINT:
        case EFormat::R32_SINT:
        case EFormat::R32_UINT:
            return EUploadFormat::RInteger;
        case EFormat::R8G8_UNORM:
        case EFormat::R8G8_SNORM:
        case EFormat::R16G16_UNORM:
        case EFormat::R16G16_SNORM:
        case EFormat::R16G16_FLOAT:
        case EFormat::R32G32_FLOAT:
            return EUploadFormat::Rg;
        case EFormat::R8G8_SINT:
        case EFormat::R8G8_UINT:
        case EFormat::R16G16_SINT:
        case EFormat::R16G16_UINT:
        case EFormat::R32G32_SINT:
        case EFormat::R32G32_UINT:
            return EUploadFormat::RgInteger;
        case EFormat::R3G3B2_UNORM:
        case EFormat::R4G4B4_UNORM:
        case EFormat::R5G5B5_UNORM:
        case EFormat::R8G8B8_UNORM:
        case EFormat::R8G8B8_SNORM:
        case EFormat::R10G10B10_UNORM:
        case EFormat::R12G12B12_UNORM:
        case EFormat::R16G16B16_SNORM:
        case EFormat::R8G8B8_SRGB:
        case EFormat::R16G16B16_FLOAT:
        case EFormat::R9G9B9_E5:
        case EFormat::R32G32B32_FLOAT:
        case EFormat::R11G11B10_FLOAT:
            return EUploadFormat::Rgb;
        case EFormat::R8G8B8_SINT:
        case EFormat::R8G8B8_UINT:
        case EFormat::R16G16B16_SINT:
        case EFormat::R16G16B16_UINT:
        case EFormat::R32G32B32_SINT:
        case EFormat::R32G32B32_UINT:
            return EUploadFormat::RgbInteger;
        case EFormat::R2G2B2A2_UNORM:
        case EFormat::R4G4B4A4_UNORM:
        case EFormat::R5G5B5A1_UNORM:
        case EFormat::R8G8B8A8_UNORM:
        case EFormat::R8G8B8A8_SNORM:
        case EFormat::R10G10B10A2_UNORM:
        case EFormat::R12G12B12A12_UNORM:
        case EFormat::R16G16B16A16_UNORM:
        case EFormat::R16G16B16A16_SNORM:
        case EFormat::R8G8B8A8_SRGB:
        case EFormat::R16G16B16A16_FLOAT:
        case EFormat::R32G32B32A32_FLOAT:
            return EUploadFormat::Rgba;
        case EFormat::R10G10B10A2_UINT:
        case EFormat::R8G8B8A8_SINT:
        case EFormat::R8G8B8A8_UINT:
        case EFormat::R16G16B16A16_SINT:
        case EFormat::R16G16B16A16_UINT:
        case EFormat::R32G32B32A32_SINT:
        case EFormat::R32G32B32A32_UINT:
            return EUploadFormat::RgbaInteger;
        case EFormat::D32_FLOAT:
        case EFormat::D32_UNORM:
        case EFormat::D24_UNORM:
        case EFormat::D16_UNORM:
            return EUploadFormat::Depth;
        case EFormat::D32_FLOAT_S8_UINT:
        case EFormat::D24_UNORM_S8_UINT:
            return EUploadFormat::DepthStencilIndex;
        case EFormat::S8_UINT:
            return EUploadFormat::StencilIndex;
        default:
            std::unreachable();
    }
}

auto FormatToUnderlyingOpenGLType(EFormat format) -> uint32_t {

    switch (format) {
        case EFormat::R8_UNORM:
        case EFormat::R8G8_UNORM:
        case EFormat::R8G8B8_UNORM:
        case EFormat::R8G8B8A8_UNORM:
        case EFormat::R8_UINT:
        case EFormat::R8G8_UINT:
        case EFormat::R8G8B8_UINT:
        case EFormat::R8G8B8A8_UINT:
        case EFormat::R8G8B8A8_SRGB:
        case EFormat::R8G8B8_SRGB:
            return GL_UNSIGNED_BYTE;
        case EFormat::R8_SNORM:
        case EFormat::R8G8_SNORM:
        case EFormat::R8G8B8_SNORM:
        case EFormat::R8G8B8A8_SNORM:
        case EFormat::R8_SINT:
        case EFormat::R8G8_SINT:
        case EFormat::R8G8B8_SINT:
        case EFormat::R8G8B8A8_SINT:
            return GL_BYTE;
        case EFormat::R16_UNORM:
        case EFormat::R16G16_UNORM:
        case EFormat::R16G16B16A16_UNORM:
        case EFormat::R16_UINT:
        case EFormat::R16G16_UINT:
        case EFormat::R16G16B16_UINT:
        case EFormat::R16G16B16A16_UINT:
            return GL_UNSIGNED_SHORT;
        case EFormat::R16_SNORM:
        case EFormat::R16G16_SNORM:
        case EFormat::R16G16B16_SNORM:
        case EFormat::R16G16B16A16_SNORM:
        case EFormat::R16_SINT:
        case EFormat::R16G16_SINT:
        case EFormat::R16G16B16_SINT:
        case EFormat::R16G16B16A16_SINT:
            return GL_SHORT;
        case EFormat::R16_FLOAT:
        case EFormat::R16G16_FLOAT:
        case EFormat::R16G16B16_FLOAT:
        case EFormat::R16G16B16A16_FLOAT:
            return GL_HALF_FLOAT;
        case EFormat::R32_FLOAT:
        case EFormat::R32G32_FLOAT:
        case EFormat::R32G32B32_FLOAT:
        case EFormat::R32G32B32A32_FLOAT:
            return GL_FLOAT;
        case EFormat::R32_SINT:
        case EFormat::R32G32_SINT:
        case EFormat::R32G32B32_SINT:
        case EFormat::R32G32B32A32_SINT:
            return GL_INT;
        case EFormat::R32_UINT:
        case EFormat::R32G32_UINT:
        case EFormat::R32G32B32_UINT:
        case EFormat::R32G32B32A32_UINT:
            return GL_UNSIGNED_INT;
        default:
            std::string message = "Format not mappable to opengl type";
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH,
                                 message.size(), message.data());
            return -1;
    }
}

constexpr auto TextureTypeToDimension(ETextureType textureType) -> uint32_t {

    switch (textureType) {
        case ETextureType::Texture1D:
            return 1;
        case ETextureType::Texture2D:
        case ETextureType::Texture2DMultisample:
        case ETextureType::Texture1DArray:
            return 2;
        case ETextureType::Texture3D:
        case ETextureType::Texture2DArray:
        case ETextureType::Texture2DMultisampleArray:
        case ETextureType::TextureCube:
        case ETextureType::TextureCubeArray:
            return 3;
        default:
            std::unreachable();
    }
}

auto FormatToBaseTypeClass(EFormat format) -> EBaseTypeClass {
    switch (format) {
        case EFormat::R8_UNORM:
        case EFormat::R8_SNORM:
        case EFormat::R16_UNORM:
        case EFormat::R16_SNORM:
        case EFormat::R8G8_UNORM:
        case EFormat::R8G8_SNORM:
        case EFormat::R16G16_UNORM:
        case EFormat::R16G16_SNORM:
        case EFormat::R3G3B2_UNORM:
        case EFormat::R4G4B4_UNORM:
        case EFormat::R5G5B5_UNORM:
        case EFormat::R8G8B8_UNORM:
        case EFormat::R8G8B8_SNORM:
        case EFormat::R10G10B10_UNORM:
        case EFormat::R12G12B12_UNORM:
        case EFormat::R16G16B16_SNORM:
        case EFormat::R2G2B2A2_UNORM:
        case EFormat::R4G4B4A4_UNORM:
        case EFormat::R5G5B5A1_UNORM:
        case EFormat::R8G8B8A8_UNORM:
        case EFormat::R8G8B8A8_SNORM:
        case EFormat::R10G10B10A2_UNORM:
        case EFormat::R12G12B12A12_UNORM:
        case EFormat::R16G16B16A16_UNORM:
        case EFormat::R8G8B8_SRGB:
        case EFormat::R8G8B8A8_SRGB:
        case EFormat::R16_FLOAT:
        case EFormat::R16G16_FLOAT:
        case EFormat::R16G16B16_FLOAT:
        case EFormat::R16G16B16A16_FLOAT:
        case EFormat::R32_FLOAT:
        case EFormat::R32G32_FLOAT:
        case EFormat::R32G32B32_FLOAT:
        case EFormat::R32G32B32A32_FLOAT:
        case EFormat::R11G11B10_FLOAT:
        case EFormat::R9G9B9_E5:
            return EBaseTypeClass::Float;
        case EFormat::R8_SINT:
        case EFormat::R16_SINT:
        case EFormat::R32_SINT:
        case EFormat::R8G8_SINT:
        case EFormat::R16G16_SINT:
        case EFormat::R32G32_SINT:
        case EFormat::R8G8B8_SINT:
        case EFormat::R16G16B16_SINT:
        case EFormat::R32G32B32_SINT:
        case EFormat::R8G8B8A8_SINT:
        case EFormat::R16G16B16A16_SINT:
        case EFormat::R32G32B32A32_SINT:
            return EBaseTypeClass::Integer;
        case EFormat::R10G10B10A2_UINT:
        case EFormat::R8_UINT:
        case EFormat::R16_UINT:
        case EFormat::R32_UINT:
        case EFormat::R8G8_UINT:
        case EFormat::R16G16_UINT:
        case EFormat::R32G32_UINT:
        case EFormat::R8G8B8_UINT:
        case EFormat::R16G16B16_UINT:
        case EFormat::R32G32B32_UINT:
        case EFormat::R8G8B8A8_UINT:
        case EFormat::R16G16B16A16_UINT:
        case EFormat::R32G32B32A32_UINT:
            return EBaseTypeClass::UnsignedInteger;
        default:
            std::unreachable();
    }
}

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

auto FormatToComponentCount(EFormat format) -> int32_t {
    switch (format) {
        case EFormat::R8_UNORM:
        case EFormat::R8_SNORM:
        case EFormat::R16_UNORM:
        case EFormat::R16_SNORM:
        case EFormat::R16_FLOAT:
        case EFormat::R32_FLOAT:
        case EFormat::R8_SINT:
        case EFormat::R16_SINT:
        case EFormat::R32_SINT:
        case EFormat::R8_UINT:
        case EFormat::R16_UINT:
        case EFormat::R32_UINT:
            return 1;
        case EFormat::R8G8_UNORM:
        case EFormat::R8G8_SNORM:
        case EFormat::R16G16_FLOAT:
        case EFormat::R16G16_UNORM:
        case EFormat::R16G16_SNORM:
        case EFormat::R32G32_FLOAT:
        case EFormat::R8G8_SINT:
        case EFormat::R16G16_SINT:
        case EFormat::R32G32_SINT:
        case EFormat::R8G8_UINT:
        case EFormat::R16G16_UINT:
        case EFormat::R32G32_UINT:
            return 2;
        case EFormat::R8G8B8_UNORM:
        case EFormat::R8G8B8_SNORM:
        case EFormat::R16G16B16_SNORM:
        case EFormat::R16G16B16_FLOAT:
        case EFormat::R32G32B32_FLOAT:
        case EFormat::R8G8B8_SINT:
        case EFormat::R16G16B16_SINT:
        case EFormat::R32G32B32_SINT:
        case EFormat::R8G8B8_UINT:
        case EFormat::R16G16B16_UINT:
        case EFormat::R32G32B32_UINT:
            return 3;
        case EFormat::R8G8B8A8_UNORM:
        case EFormat::R8G8B8A8_SNORM:
        case EFormat::R16G16B16A16_UNORM:
        case EFormat::R16G16B16A16_FLOAT:
        case EFormat::R32G32B32A32_FLOAT:
        case EFormat::R8G8B8A8_SINT:
        case EFormat::R16G16B16A16_SINT:
        case EFormat::R32G32B32A32_SINT:
        case EFormat::R10G10B10A2_UINT:
        case EFormat::R8G8B8A8_UINT:
        case EFormat::R16G16B16A16_UINT:
        case EFormat::R32G32B32A32_UINT:
            return 4;
        default:
            std::unreachable();
    }
}

auto IsFormatNormalized(EFormat format) -> int32_t {

    switch (format) {
        case EFormat::R8_UNORM:
        case EFormat::R8_SNORM:
        case EFormat::R16_UNORM:
        case EFormat::R16_SNORM:
        case EFormat::R8G8_UNORM:
        case EFormat::R8G8_SNORM:
        case EFormat::R16G16_UNORM:
        case EFormat::R16G16_SNORM:
        case EFormat::R8G8B8_UNORM:
        case EFormat::R8G8B8_SNORM:
        case EFormat::R16G16B16_SNORM:
        case EFormat::R8G8B8A8_UNORM:
        case EFormat::R8G8B8A8_SNORM:
        case EFormat::R16G16B16A16_UNORM:
            return GL_TRUE;
        case EFormat::R16_FLOAT:
        case EFormat::R32_FLOAT:
        case EFormat::R8_SINT:
        case EFormat::R16_SINT:
        case EFormat::R32_SINT:
        case EFormat::R8_UINT:
        case EFormat::R16_UINT:
        case EFormat::R32_UINT:
        case EFormat::R16G16_FLOAT:
        case EFormat::R32G32_FLOAT:
        case EFormat::R8G8_SINT:
        case EFormat::R16G16_SINT:
        case EFormat::R32G32_SINT:
        case EFormat::R8G8_UINT:
        case EFormat::R16G16_UINT:
        case EFormat::R32G32_UINT:
        case EFormat::R16G16B16_FLOAT:
        case EFormat::R32G32B32_FLOAT:
        case EFormat::R8G8B8_SINT:
        case EFormat::R16G16B16_SINT:
        case EFormat::R32G32B32_SINT:
        case EFormat::R8G8B8_UINT:
        case EFormat::R16G16B16_UINT:
        case EFormat::R32G32B32_UINT:
        case EFormat::R16G16B16A16_FLOAT:
        case EFormat::R32G32B32A32_FLOAT:
        case EFormat::R8G8B8A8_SINT:
        case EFormat::R16G16B16A16_SINT:
        case EFormat::R32G32B32A32_SINT:
        case EFormat::R10G10B10A2_UINT:
        case EFormat::R8G8B8A8_UINT:
        case EFormat::R16G16B16A16_UINT:
        case EFormat::R32G32B32A32_UINT:
            return GL_FALSE;
        default:
            std::unreachable();
    }
}

auto FormatToFormatClass(EFormat format) -> EFormatClass {
    switch (format) {
        case EFormat::R8_UNORM:
        case EFormat::R8_SNORM:
        case EFormat::R16_UNORM:
        case EFormat::R16_SNORM:
        case EFormat::R8G8_UNORM:
        case EFormat::R8G8_SNORM:
        case EFormat::R16G16_UNORM:
        case EFormat::R16G16_SNORM:
        case EFormat::R8G8B8_UNORM:
        case EFormat::R8G8B8_SNORM:
        case EFormat::R16G16B16_SNORM:
        case EFormat::R8G8B8A8_UNORM:
        case EFormat::R8G8B8A8_SNORM:
        case EFormat::R16G16B16A16_UNORM:
        case EFormat::R16_FLOAT:
        case EFormat::R16G16_FLOAT:
        case EFormat::R16G16B16_FLOAT:
        case EFormat::R16G16B16A16_FLOAT:
        case EFormat::R32_FLOAT:
        case EFormat::R32G32_FLOAT:
        case EFormat::R32G32B32_FLOAT:
        case EFormat::R32G32B32A32_FLOAT:
            return EFormatClass::Float;
        case EFormat::R8_SINT:
        case EFormat::R16_SINT:
        case EFormat::R32_SINT:
        case EFormat::R8G8_SINT:
        case EFormat::R16G16_SINT:
        case EFormat::R32G32_SINT:
        case EFormat::R8G8B8_SINT:
        case EFormat::R16G16B16_SINT:
        case EFormat::R32G32B32_SINT:
        case EFormat::R8G8B8A8_SINT:
        case EFormat::R16G16B16A16_SINT:
        case EFormat::R32G32B32A32_SINT:
        case EFormat::R10G10B10A2_UINT:
        case EFormat::R8_UINT:
        case EFormat::R16_UINT:
        case EFormat::R32_UINT:
        case EFormat::R8G8_UINT:
        case EFormat::R16G16_UINT:
        case EFormat::R32G32_UINT:
        case EFormat::R8G8B8_UINT:
        case EFormat::R16G16B16_UINT:
        case EFormat::R32G32B32_UINT:
        case EFormat::R8G8B8A8_UINT:
        case EFormat::R16G16B16A16_UINT:
        case EFormat::R32G32B32A32_UINT:
            return EFormatClass::Integer;
        default:
            return EFormatClass::Long;
    }
}

// RHI Implementation ///////////////////////////////////////////////////////////

std::unordered_map<std::string, SGpuMesh> g_gpuMeshes = {};
std::unordered_map<std::string, SSampler> g_gpuSamplers = {};
std::unordered_map<std::string, SGpuMaterial> g_gpuMaterials = {};

std::vector<SSampler> g_samplers;
SSamplerId g_samplerCounter = SSamplerId::Invalid;

std::vector<STexture> g_textures;
STextureId g_textureCounter = STextureId::Invalid;

std::unordered_map<SSamplerDescriptor, SSamplerId> g_samplerDescriptors;

auto SetDebugLabel(
    const uint32_t object,
    const uint32_t objectType,
    const std::string_view label) -> void {

    glObjectLabel(objectType, object, static_cast<GLsizei>(label.size()), label.data());
}

auto PushDebugGroup(const std::string_view label) -> void {
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, label.size(), label.data());
}

auto PopDebugGroup() -> void {
    glPopDebugGroup();
}

inline auto GetTexture(STextureId id) -> STexture& {

    assert(id != STextureId::Invalid);
    return g_textures[size_t(id)];
}

auto CreateTexture(const SCreateTextureDescriptor& createTextureDescriptor) -> STextureId {

    STexture texture = {};

    glCreateTextures(TextureTypeToGL(createTextureDescriptor.TextureType), 1, &texture.Id);
    if (!createTextureDescriptor.Label.empty()) {
        SetDebugLabel(texture.Id, GL_TEXTURE, createTextureDescriptor.Label);
    }

    texture.Extent = createTextureDescriptor.Extent;
    texture.Format = createTextureDescriptor.Format;
    texture.TextureType = createTextureDescriptor.TextureType;

    switch (createTextureDescriptor.TextureType) {
        case ETextureType::Texture1D:
            glTextureStorage1D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               createTextureDescriptor.Extent.Width);
            break;
        case ETextureType::Texture2D:
            glTextureStorage2D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               createTextureDescriptor.Extent.Width,
                               createTextureDescriptor.Extent.Height);
            break;
        case ETextureType::Texture3D:
            glTextureStorage3D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               createTextureDescriptor.Extent.Width,
                               createTextureDescriptor.Extent.Height,
                               createTextureDescriptor.Extent.Depth);
            break;
        case ETextureType::Texture1DArray:
            glTextureStorage2D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               createTextureDescriptor.Extent.Width,
                               createTextureDescriptor.Layers);
            break;
        case ETextureType::Texture2DArray:
            glTextureStorage3D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               createTextureDescriptor.Extent.Width,
                               createTextureDescriptor.Extent.Height,
                               createTextureDescriptor.Layers);
            break;
        case ETextureType::TextureCube:
            glTextureStorage2D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               createTextureDescriptor.Extent.Width,
                               createTextureDescriptor.Extent.Height);
            break;
        case ETextureType::TextureCubeArray:
            glTextureStorage3D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               createTextureDescriptor.Extent.Width,
                               createTextureDescriptor.Extent.Height,
                               createTextureDescriptor.Layers);
            break;
        case ETextureType::Texture2DMultisample:
            glTextureStorage2DMultisample(texture.Id,
                                          SampleCountToGL(createTextureDescriptor.SampleCount),
                                          FormatToGL(createTextureDescriptor.Format),
                                          createTextureDescriptor.Extent.Width,
                                          createTextureDescriptor.Extent.Height,
                                          GL_TRUE);
            break;
        case ETextureType::Texture2DMultisampleArray:
            glTextureStorage3DMultisample(texture.Id,
                                          SampleCountToGL(createTextureDescriptor.SampleCount),
                                          FormatToGL(createTextureDescriptor.Format),
                                          createTextureDescriptor.Extent.Width,
                                          createTextureDescriptor.Extent.Height,
                                          createTextureDescriptor.Layers,
                                          GL_TRUE);
            break;
        default:
            std::unreachable();
    }

    const auto textureId = STextureId(g_textures.size());
    g_textures.emplace_back(texture);

    return textureId;
}

auto UploadTexture(const STextureId& textureId,
                   const SUploadTextureDescriptor& updateTextureDescriptor) -> void {

    auto& texture = GetTexture(textureId);

    uint32_t format = 0;
    if (updateTextureDescriptor.UploadFormat == EUploadFormat::Auto) {
        format = UploadFormatToGL(FormatToUploadFormat(texture.Format));
    } else {
        format = UploadFormatToGL(updateTextureDescriptor.UploadFormat);
    }

    uint32_t type = 0;
    if (updateTextureDescriptor.UploadType == EUploadType::Auto) {
        type = FormatToUnderlyingOpenGLType(texture.Format);
    } else {
        type = UploadTypeToGL(updateTextureDescriptor.UploadType);
    }

    switch (TextureTypeToDimension(texture.TextureType)) {
        case 1:
            glTextureSubImage1D(texture.Id,
                                updateTextureDescriptor.Level,
                                updateTextureDescriptor.Offset.X,
                                updateTextureDescriptor.Extent.Width,
                                format,
                                type,
                                updateTextureDescriptor.PixelData);
            break;
        case 2:
            glTextureSubImage2D(texture.Id,
                                updateTextureDescriptor.Level,
                                updateTextureDescriptor.Offset.X,
                                updateTextureDescriptor.Offset.Y,
                                updateTextureDescriptor.Extent.Width,
                                updateTextureDescriptor.Extent.Height,
                                format,
                                type,
                                updateTextureDescriptor.PixelData);
            break;
        case 3:
            glTextureSubImage3D(texture.Id,
                                updateTextureDescriptor.Level,
                                updateTextureDescriptor.Offset.X,
                                updateTextureDescriptor.Offset.Y,
                                updateTextureDescriptor.Offset.Z,
                                updateTextureDescriptor.Extent.Width,
                                updateTextureDescriptor.Extent.Height,
                                updateTextureDescriptor.Extent.Depth,
                                format,
                                type,
                                updateTextureDescriptor.PixelData);
            break;
    }
}

auto MakeTextureResident(const STextureId& textureId) -> uint64_t {

    auto& texture = GetTexture(textureId);

    auto textureHandle = glGetTextureHandleARB(texture.Id);
    glMakeTextureHandleResidentARB(textureHandle);

    return textureHandle;
}

auto GenerateMipmaps(const STextureId textureId) -> void {

    auto& texture = GetTexture(textureId);
    glGenerateTextureMipmap(texture.Id);
}

auto UnloadTextures() -> void {
    for (auto& texture: g_textures) {
        glDeleteTextures(1, &texture.Id);
    }
}

auto GetOrCreateSampler(const SSamplerDescriptor& samplerDescriptor) -> SSamplerId {

    if (auto existingSamplerDescriptor = g_samplerDescriptors.find(samplerDescriptor); existingSamplerDescriptor !=
                                                                                       g_samplerDescriptors.end()) {
        return existingSamplerDescriptor->second;
    }

    SSampler sampler = {};
    glCreateSamplers(1, &sampler.Id);
    SetDebugLabel(sampler.Id, GL_SAMPLER, samplerDescriptor.Label);
    glSamplerParameteri(sampler.Id, GL_TEXTURE_WRAP_S, TextureAddressModeToGL(samplerDescriptor.AddressModeU));
    glSamplerParameteri(sampler.Id, GL_TEXTURE_WRAP_T, TextureAddressModeToGL(samplerDescriptor.AddressModeV));
    glSamplerParameteri(sampler.Id, GL_TEXTURE_WRAP_R, TextureAddressModeToGL(samplerDescriptor.AddressModeW));
    glSamplerParameteri(sampler.Id, GL_TEXTURE_MAG_FILTER, TextureMagFilterToGL(samplerDescriptor.MagFilter));
    glSamplerParameteri(sampler.Id, GL_TEXTURE_MIN_FILTER, TextureMinFilterToGL(samplerDescriptor.MinFilter));
    glSamplerParameterf(sampler.Id, GL_TEXTURE_LOD_BIAS, samplerDescriptor.LodBias);
    glSamplerParameteri(sampler.Id, GL_TEXTURE_MIN_LOD, samplerDescriptor.LodMin);
    glSamplerParameteri(sampler.Id, GL_TEXTURE_MAX_LOD, samplerDescriptor.LodMax);

    const auto samplerId = SSamplerId(g_samplers.size());
    g_samplerDescriptors.insert({ samplerDescriptor, samplerId });
    g_samplers.emplace_back(sampler);

    return samplerId;
};

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
                            &colorAttachment.ClearColor.Data)->data());
                        break;
                    case EBaseTypeClass::Integer:
                        glClearNamedFramebufferiv(framebuffer.Id, GL_COLOR, colorAttachmentIndex,
                                                  std::get_if<std::array<int32_t, 4>>(
                                                      &colorAttachment.ClearColor.Data)->data());
                        break;
                    case EBaseTypeClass::UnsignedInteger:
                        glClearNamedFramebufferuiv(framebuffer.Id, GL_COLOR, colorAttachmentIndex,
                                                   std::get_if<std::array<uint32_t, 4>>(
                                                       &colorAttachment.ClearColor.Data)->data());
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

auto DeletePipeline(const SPipeline& pipeline) -> void {
    glDeleteProgram(pipeline.Id);
}

auto CreateGraphicsProgram(
    std::string_view label,
    std::string_view vertexShaderFilePath,
    std::string_view fragmentShaderFilePath) -> std::expected<uint32_t, std::string> {

    auto vertexShaderSourceResult = ReadShaderTextFromFile(vertexShaderFilePath);
    if (!vertexShaderSourceResult) {
        return std::unexpected(vertexShaderSourceResult.error());
    }

    auto fragmentShaderSourceResult = ReadShaderTextFromFile(fragmentShaderFilePath);
    if (!fragmentShaderSourceResult) {
        return std::unexpected(fragmentShaderSourceResult.error());
    }

    auto vertexShaderSource = *vertexShaderSourceResult;
    auto fragmentShaderSource = *fragmentShaderSourceResult;

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto vertexShaderSourcePtr = vertexShaderSource.data();
    glShaderSource(vertexShader, 1, &vertexShaderSourcePtr, nullptr);
    glCompileShader(vertexShader);
    int32_t status = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {

        int32_t infoLength = 512;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLength);
        auto infoLog = std::string(infoLength + 1, '\0');
        glGetShaderInfoLog(vertexShader, infoLength, nullptr, infoLog.data());
        glDeleteShader(vertexShader);

        return std::unexpected(std::format("Vertex shader in program {} has errors\n{}", label, infoLog));
    }

    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragmentShaderSourcePtr = fragmentShaderSource.data();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourcePtr, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {

        int32_t infoLength = 512;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLength);
        auto infoLog = std::string(infoLength + 1, '\0');
        glGetShaderInfoLog(fragmentShader, infoLength, nullptr, infoLog.data());
        glDeleteShader(fragmentShader);

        return std::unexpected(std::format("Fragment shader in program {} has errors\n{}", label, infoLog));
    }

    auto program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {

        int32_t infoLength = 512;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        auto infoLog = std::string(infoLength + 1, '\0');
        glGetProgramInfoLog(program, infoLength, nullptr, infoLog.data());

        return std::unexpected(std::format("Graphics program {} has linking errors\n{}", label, infoLog));
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

auto CreateComputeProgram(
    std::string_view label,
    std::string_view computeShaderFilePath) -> std::expected<uint32_t, std::string> {

    auto computeShaderSourceResult = ReadShaderTextFromFile(computeShaderFilePath);
    if (!computeShaderSourceResult) {
        return std::unexpected(computeShaderSourceResult.error());
    }

    int32_t status = 0;

    auto computeShader = glCreateShader(GL_COMPUTE_SHADER);
    auto computeShaderSourcePtr = (*computeShaderSourceResult).data();
    glShaderSource(computeShader, 1, &computeShaderSourcePtr, nullptr);
    glCompileShader(computeShader);
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {

        int32_t infoLength = 512;
        glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &infoLength);
        auto infoLog = std::string(infoLength + 1, '\0');
        glGetShaderInfoLog(computeShader, infoLength, nullptr, infoLog.data());
        glDeleteShader(computeShader);

        return std::unexpected(std::format("Compute shader in program {} has errors\n{}", label, infoLog));
    }

    auto program = glCreateProgram();
    glAttachShader(program, computeShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {

        int32_t infoLength = 512;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        auto infoLog = std::string(infoLength + 1, '\0');
        glGetProgramInfoLog(program, infoLength, nullptr, infoLog.data());

        return std::unexpected(std::format("Compute program {} has linking errors\n{}", label, infoLog));
    }

    glDetachShader(program, computeShader);
    glDeleteShader(computeShader);

    return program;
}

auto CreateGraphicsPipeline(
    const SGraphicsPipelineDescriptor& graphicsPipelineDescriptor) -> std::expected<SGraphicsPipeline, std::string> {

    SGraphicsPipeline pipeline = {};

    auto graphicsProgramResult = CreateGraphicsProgram(graphicsPipelineDescriptor.Label,
                                                       graphicsPipelineDescriptor.VertexShaderFilePath,
                                                       graphicsPipelineDescriptor.FragmentShaderFilePath);
    if (!graphicsProgramResult) {
        return std::unexpected(
            std::format("RHI: Unable to build GraphicsPipeline {}\n{}", graphicsPipelineDescriptor.Label,
                        graphicsProgramResult.error()));
    }

    pipeline.Id = *graphicsProgramResult;

    if (graphicsPipelineDescriptor.VertexInput.has_value()) {

        uint32_t inputLayout = 0;
        glCreateVertexArrays(1, &inputLayout);

        auto& vertexInput = *graphicsPipelineDescriptor.VertexInput;
        for (auto inputAttributeIndex = 0; auto& inputAttribute: vertexInput.VertexInputAttributes) {
            if (inputAttribute.has_value()) {
                auto& inputAttributeValue = *inputAttribute;

                glEnableVertexArrayAttrib(inputLayout, inputAttributeValue.Location);
                glVertexArrayAttribBinding(inputLayout, inputAttributeValue.Location, inputAttributeValue.Binding);

                auto type = FormatToUnderlyingOpenGLType(inputAttributeValue.Format);
                auto componentCount = FormatToComponentCount(inputAttributeValue.Format);
                auto isFormatNormalized = IsFormatNormalized(inputAttributeValue.Format);
                auto formatClass = FormatToFormatClass(inputAttributeValue.Format);
                switch (formatClass) {
                    case EFormatClass::Float:
                        glVertexArrayAttribFormat(inputLayout, inputAttributeValue.Location, componentCount, type,
                                                  isFormatNormalized, inputAttributeValue.Offset);
                        break;
                    case EFormatClass::Integer:
                        glVertexArrayAttribIFormat(inputLayout, inputAttributeValue.Location, componentCount, type,
                                                   inputAttributeValue.Offset);
                        break;
                    case EFormatClass::Long:
                        glVertexArrayAttribLFormat(inputLayout, inputAttributeValue.Location, componentCount, type,
                                                   inputAttributeValue.Offset);
                        break;
                    default:
                        std::string message = "Unsupported Format Class";
                        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0,
                                             GL_DEBUG_SEVERITY_HIGH, message.size(), message.data());
                        break;
                }
            }

            inputAttributeIndex++;
        }

        SetDebugLabel(inputLayout, GL_VERTEX_ARRAY, std::format("InputLayout-{}", graphicsPipelineDescriptor.Label));

        pipeline.InputLayout = inputLayout;
    } else {
        pipeline.InputLayout = std::nullopt;
    }

    pipeline.PrimitiveTopology = PrimitiveTopologyToGL(graphicsPipelineDescriptor.InputAssembly.PrimitiveTopology);
    pipeline.IsPrimitiveRestartEnabled = graphicsPipelineDescriptor.InputAssembly.IsPrimitiveRestartEnabled;

    return pipeline;
}

auto CreateComputePipeline(
    const SComputePipelineDescriptor& computePipelineDescriptor) -> std::expected<SComputePipeline, std::string> {

    SComputePipeline pipeline = {};

    auto computeProgramResult = CreateComputeProgram(computePipelineDescriptor.Label,
                                                     computePipelineDescriptor.ComputeShaderFilePath);
    if (!computeProgramResult) {
        return std::unexpected(
            std::format("RHI: Unable to build ComputePipeline {}\n{}", computePipelineDescriptor.Label,
                        computeProgramResult.error()));
    }

    pipeline.Id = *computeProgramResult;

    return pipeline;
}

auto CreateBuffer(std::string_view label,
                  size_t sizeInBytes,
                  const void* data,
                  uint32_t flags) -> uint32_t {

    uint32_t buffer = 0;
    glCreateBuffers(1, &buffer);
    SetDebugLabel(buffer, GL_BUFFER, label);
    glNamedBufferStorage(buffer, sizeInBytes, data, flags);
    return buffer;
}

auto UpdateBuffer(uint32_t buffer,
                  size_t offsetInBytes,
                  size_t sizeInBytes,
                  const void* data) -> void {

    glNamedBufferSubData(buffer, offsetInBytes, sizeInBytes, data);
}

auto DeleteBuffer(uint32_t buffer) -> void {

    glDeleteBuffers(1, &buffer);
}

// Renderer /////////////////////////////////////////////////

struct SComponentGpuMesh {
    std::string Mesh;
};

struct SComponentGpuMaterial {
    std::string Material;
};

glm::ivec2 g_scaledFramebufferSize = {};

SFramebuffer g_geometryFramebuffer = {};
SGraphicsPipeline g_geometryGraphicsPipeline = {};
std::vector<SGpuGlobalLight> g_gpuGlobalLights;

bool g_drawDebugLines = true;
std::vector<SDebugLine> g_debugLines;
uint32_t g_debugInputLayout = 0;
uint32_t g_debugLinesVertexBuffer = 0;
SGraphicsPipeline g_debugLinesGraphicsPipeline = {};

SGraphicsPipeline g_fullscreenTriangleGraphicsPipeline = {};
SSampler g_fullscreenSamplerNearestClampToEdge = {};

SGpuGlobalUniforms g_globalUniforms = {};
uint32_t g_globalUniformsBuffer = 0;

uint32_t g_objectsBuffer = 0;

auto DrawFullscreenTriangleWithTexture(const STexture& texture) -> void {

    g_fullscreenTriangleGraphicsPipeline.Bind();
    g_fullscreenTriangleGraphicsPipeline.BindTextureAndSampler(0, texture.Id, g_fullscreenSamplerNearestClampToEdge.Id);
    g_fullscreenTriangleGraphicsPipeline.DrawArrays(0, 3);
}

auto LoadRenderer() -> bool {

    glCreateVertexArrays(1, &g_defaultInputLayout);
    SetDebugLabel(g_defaultInputLayout, GL_VERTEX_ARRAY, "InputLayout-Default");

    auto geometryGraphicsPipelineResult = CreateGraphicsPipeline({
                                                                     .Label = "GeometryPipeline",
                                                                     .VertexShaderFilePath = "data/shaders/Simple.vs.glsl",
                                                                     .FragmentShaderFilePath = "data/shaders/Simple.fs.glsl",
                                                                     .InputAssembly = {
                                                                         .PrimitiveTopology = EPrimitiveTopology::Triangles
                                                                     },
                                                                 });
    if (!geometryGraphicsPipelineResult) {
        spdlog::error(geometryGraphicsPipelineResult.error());
        return false;
    }
    g_geometryGraphicsPipeline = *geometryGraphicsPipelineResult;

    auto fullscreenTriangleGraphicsPipelineResult = CreateGraphicsPipeline({
                                                                               .Label = "FullscreenTrianglePipeline",
                                                                               .VertexShaderFilePath = "data/shaders/FST.vs.glsl",
                                                                               .FragmentShaderFilePath = "data/shaders/FST.GammaCorrected.fs.glsl",
                                                                               .InputAssembly = {
                                                                                   .PrimitiveTopology = EPrimitiveTopology::Triangles
                                                                               },
                                                                           });

    if (!fullscreenTriangleGraphicsPipelineResult) {
        spdlog::error(fullscreenTriangleGraphicsPipelineResult.error());
        return false;
    }
    g_fullscreenTriangleGraphicsPipeline = *fullscreenTriangleGraphicsPipelineResult;

    auto debugLinesGraphicsPipelineResult = CreateGraphicsPipeline({
                                                                       .Label = "DebugLinesPipeline",
                                                                       .VertexShaderFilePath = "data/shaders/DebugLines.vs.glsl",
                                                                       .FragmentShaderFilePath = "data/shaders/DebugLines.fs.glsl",
                                                                       .InputAssembly = {
                                                                           .PrimitiveTopology = EPrimitiveTopology::Lines,
                                                                           .IsPrimitiveRestartEnabled = false,
                                                                       },
                                                                       .VertexInput = SVertexInputDescriptor{
                                                                           .VertexInputAttributes = {
                                                                               SVertexInputAttributeDescriptor{
                                                                                   .Location = 0,
                                                                                   .Binding = 0,
                                                                                   .Format = EFormat::R32G32B32_FLOAT,
                                                                                   .Offset = offsetof(SDebugLine, StartPosition),
                                                                               },
                                                                               SVertexInputAttributeDescriptor{
                                                                                   .Location = 1,
                                                                                   .Binding = 0,
                                                                                   .Format = EFormat::R32G32B32A32_FLOAT,
                                                                                   .Offset = offsetof(SDebugLine, StartColor),
                                                                               },
                                                                           }
                                                                       }
                                                                   });

    if (!debugLinesGraphicsPipelineResult) {
        spdlog::error(debugLinesGraphicsPipelineResult.error());
        return false;
    }

    g_debugLinesGraphicsPipeline = *debugLinesGraphicsPipelineResult;
    g_debugLinesVertexBuffer = CreateBuffer("VertexBuffer-DebugLines", sizeof(SDebugLine) * 16384, nullptr, GL_DYNAMIC_STORAGE_BIT);

    g_globalUniformsBuffer = CreateBuffer("SGpuGlobalUniforms", sizeof(SGpuGlobalUniforms), &g_globalUniforms, GL_DYNAMIC_STORAGE_BIT);
    g_objectsBuffer = CreateBuffer("SGpuObjects", sizeof(SGpuObject) * 16384, nullptr, GL_DYNAMIC_STORAGE_BIT);


    return true;
}

auto UnloadRenderer() -> void {
    glDeleteVertexArrays(1, &g_defaultInputLayout);
    DeleteBuffer(g_debugLinesVertexBuffer);
    DeleteFramebuffer(g_geometryFramebuffer);
    DeletePipeline(g_geometryGraphicsPipeline);
    DeletePipeline(g_debugLinesGraphicsPipeline);
}

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
                                                          .ClearColor = { 0.1f, 0.1f, 0.2f, 1.0f },
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

auto CreateGpuMesh(const std::string& assetMeshName) -> void {

    auto& assetMesh = GetAssetMesh(assetMeshName);

    uint32_t buffers[3] = {};
    {
        glCreateBuffers(3, buffers);

        SetDebugLabel(buffers[0], GL_BUFFER, std::format("{}_SGpuVertexPosition", assetMeshName));
        SetDebugLabel(buffers[1], GL_BUFFER, std::format("{}_SGpuVertexNormalUvTangent", assetMeshName));
        SetDebugLabel(buffers[2], GL_BUFFER, std::format("{}_Indices", assetMeshName));

        glNamedBufferStorage(buffers[0], sizeof(SGpuVertexPosition) * assetMesh.VertexPositions.size(),
                             assetMesh.VertexPositions.data(), 0);
        glNamedBufferStorage(buffers[1], sizeof(SGpuVertexNormalUvTangent) * assetMesh.VertexNormalUvTangents.size(),
                             assetMesh.VertexNormalUvTangents.data(), 0);
        glNamedBufferStorage(buffers[2], sizeof(uint32_t) * assetMesh.Indices.size(), assetMesh.Indices.data(), 0);
    }

    auto gpuMesh = SGpuMesh{
        .Name = assetMesh.Name,
        .VertexPositionBuffer = buffers[0],
        .VertexNormalUvTangentBuffer = buffers[1],
        .IndexBuffer = buffers[2],

        .VertexCount = assetMesh.VertexPositions.size(),
        .IndexCount = assetMesh.Indices.size(),

        .InitialTransform = assetMesh.InitialTransform,
    };

    {
        g_gpuMeshes[assetMeshName] = gpuMesh;
    }
}

auto GetGpuMesh(const std::string& assetMeshName) -> SGpuMesh& {
    assert(!assetMeshName.empty() && g_gpuMeshes.contains(assetMeshName));

    return g_gpuMeshes[assetMeshName];
}

auto GetGpuMaterial(const std::string& assetMaterialName) -> SGpuMaterial& {
    assert(!assetMaterialName.empty() && g_gpuMaterials.contains(assetMaterialName));

    return g_gpuMaterials[assetMaterialName];
}

auto CreateTextureForMaterialChannel(SAssetMaterialChannel& assetMaterialChannel) -> int64_t {

    auto& image = GetAssetImage(assetMaterialChannel.Image);

    auto textureId = CreateTexture(SCreateTextureDescriptor{
        .TextureType = ETextureType::Texture2D,
        .Format = EFormat::R8G8B8A8_UNORM,
        .Extent = SExtent3D{static_cast<uint32_t>(image.Width), static_cast<uint32_t>(image.Height), 1u},
        .MipMapLevels = static_cast<uint32_t>(glm::floor(glm::log2(glm::max(static_cast<float>(image.Width), static_cast<float>(image.Height))))),
        .Layers = 1,
        .SampleCount = ESampleCount::One,
        .Label = image.Name,
    });

    UploadTexture(textureId, SUploadTextureDescriptor{
        .Level = 0,
        .Offset = SOffset3D{0, 0, 0},
        .Extent = SExtent3D{static_cast<uint32_t>(image.Width), static_cast<uint32_t>(image.Height), 1u},
        .UploadFormat = EUploadFormat::Auto,
        .UploadType = EUploadType::Auto,
        .PixelData = image.DecodedData.get()
    });

    GenerateMipmaps(textureId);

    //auto& sampler = GetAssetSampler(assetMaterialChannel.Sampler);

    return MakeTextureResident(textureId);
}

auto CreateGpuMaterial(const std::string& assetMaterialName) -> void {

    auto& assetMaterial = GetAssetMaterial(assetMaterialName);
    uint64_t baseColorTexture = assetMaterial.BaseColorChannel.has_value()
                                ? CreateTextureForMaterialChannel(assetMaterial.BaseColorChannel.value())
                                : 0;

    uint64_t normalTexture = assetMaterial.NormalsChannel.has_value()
                             ? CreateTextureForMaterialChannel(assetMaterial.NormalsChannel.value())
                             : 0;

    auto gpuMaterial = SGpuMaterial{
        .BaseColor = assetMaterial.BaseColorFactor,
        .BaseColorTexture = baseColorTexture,
        .NormalTexture = normalTexture,
    };

    g_gpuMaterials[assetMaterialName] = gpuMaterial;
}

auto ResizeFramebuffersIfNecessary(const SApplicationSettings& applicationSettings) -> void {

    if (g_applicationContext.WindowFramebufferResized || g_applicationContext.SceneViewerResized) {

        g_applicationContext.WindowFramebufferScaledSize = glm::ivec2{g_applicationContext.WindowFramebufferSize.x * applicationSettings.ResolutionScale,
                                                                      g_applicationContext.WindowFramebufferSize.y * applicationSettings.ResolutionScale};
        g_applicationContext.SceneViewerScaledSize = glm::ivec2{g_applicationContext.SceneViewerSize.x * applicationSettings.ResolutionScale,
                                                                g_applicationContext.SceneViewerSize.y * applicationSettings.ResolutionScale};

        if (g_applicationContext.IsEditor) {
            g_scaledFramebufferSize = g_applicationContext.SceneViewerScaledSize;
        } else {
            g_scaledFramebufferSize = g_applicationContext.WindowFramebufferScaledSize;
        }

        if (g_applicationContext.FrameCounter > 0) {
            DeleteRendererFramebuffers();
            if (g_scaledFramebufferSize.x + g_scaledFramebufferSize.y <= glm::epsilon<float>()) {
                g_scaledFramebufferSize = g_applicationContext.WindowFramebufferScaledSize;
            }
        }
        CreateRendererFramebuffers(g_scaledFramebufferSize);

        glViewport(0, 0, g_scaledFramebufferSize.x, g_scaledFramebufferSize.y);

        g_applicationContext.WindowFramebufferResized = false;
        g_applicationContext.SceneViewerResized = false;
    }
}

auto Render(entt::registry& registry, float deltaTime, SCamera& camera) -> void {

    if (g_drawDebugLines) {
        g_debugLines.clear();

        g_debugLines.push_back(SDebugLine{
            .StartPosition = glm::vec3{-150, 30, 4},
            .StartColor = glm::vec4{0.3f, 0.95f, 0.1f, 1.0f},
            .EndPosition = glm::vec3{150, -40, -4},
            .EndColor = glm::vec4{0.6f, 0.1f, 0.0f, 1.0f}
        });
    }

    ///////////////////////
    // Create Gpu Resources if necessary
    ///////////////////////
    auto createGpuResourcesNecessaryView = registry.view<SComponentCreateGpuResources>();
    for (auto& entity : createGpuResourcesNecessaryView) {

        auto& meshComponent = registry.get<SComponentMesh>(entity);
        auto& materialComponent = registry.get<SComponentMaterial>(entity);

        CreateGpuMesh(meshComponent.Mesh);
        CreateGpuMaterial(materialComponent.Material);

        registry.emplace<SComponentGpuMesh>(entity, meshComponent.Mesh);
        registry.emplace<SComponentGpuMaterial>(entity, materialComponent.Material);

        registry.remove<SComponentCreateGpuResources>(entity);
    }

    g_globalUniforms.ProjectionMatrix = glm::infinitePerspective(glm::radians(60.0f), (float)g_scaledFramebufferSize.x / (float)g_scaledFramebufferSize.y, 0.1f);
    g_globalUniforms.ViewMatrix = camera.GetViewMatrix();
    g_globalUniforms.CameraPosition = glm::vec4(camera.Position, 0.0f);
    UpdateBuffer(g_globalUniformsBuffer, 0, sizeof(SGpuGlobalUniforms), &g_globalUniforms);

    g_geometryGraphicsPipeline.Bind();
    BindFramebuffer(g_geometryFramebuffer);

    g_geometryGraphicsPipeline.BindBufferAsUniformBuffer(g_globalUniformsBuffer, 0);

    auto renderablesView = registry.view<SComponentGpuMesh, SComponentGpuMaterial, SComponentTransform>();
    renderablesView.each([](const auto& meshComponent, const auto& materialComponent, auto& initialTransform) {

        auto& gpuMaterial = GetGpuMaterial(materialComponent.Material);
        auto& gpuMesh = GetGpuMesh(meshComponent.Mesh);

        //glBindSampler(1, g_fullscreenSamplerNearestClampToEdge.Id);

        g_geometryGraphicsPipeline.BindBufferAsShaderStorageBuffer(gpuMesh.VertexPositionBuffer, 1);
        g_geometryGraphicsPipeline.BindBufferAsShaderStorageBuffer(gpuMesh.VertexNormalUvTangentBuffer, 2);
        g_geometryGraphicsPipeline.SetUniform(0, initialTransform * gpuMesh.InitialTransform);
        g_geometryGraphicsPipeline.SetUniform(8, gpuMaterial.BaseColorTexture);
        g_geometryGraphicsPipeline.SetUniform(9, gpuMaterial.NormalTexture);

        g_geometryGraphicsPipeline.DrawElements(gpuMesh.IndexBuffer, gpuMesh.IndexCount);

    });

    if (g_drawDebugLines && !g_debugLines.empty()) {

        PushDebugGroup("Debug Lines");
        glDisable(GL_CULL_FACE);

        UpdateBuffer(g_debugLinesVertexBuffer, 0, sizeof(SDebugLine) * g_debugLines.size(), g_debugLines.data());

        g_debugLinesGraphicsPipeline.Bind();
        g_debugLinesGraphicsPipeline.BindBufferAsVertexBuffer(g_debugLinesVertexBuffer, 0, 0, sizeof(SDebugLine) / 2);
        g_debugLinesGraphicsPipeline.BindBufferAsUniformBuffer(g_globalUniformsBuffer, 0);
        g_debugLinesGraphicsPipeline.DrawArrays(0, static_cast<int32_t>(g_debugLines.size() * 2));

        glEnable(GL_CULL_FACE);
        PopDebugGroup();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (!g_applicationContext.IsEditor) {
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        g_fullscreenTriangleGraphicsPipeline.Bind();
        DrawFullscreenTriangleWithTexture(g_geometryFramebuffer.ColorAttachments[0].value().Texture);
    }
}

auto RenderUi(entt::registry& registry, float deltaTime) -> void {

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

    if (g_applicationContext.IsEditor) {
        ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

        if (ImGui::BeginMainMenuBar()) {
            //ImGui::Image(reinterpret_cast<ImTextureID>(g_iconPackageGreen), ImVec2(16, 16), g_imvec2UnitY, g_imvec2UnitX);
            ImGui::SetCursorPosX(20.0f);
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Quit")) {

                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // Scene Viewer
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        if (ImGui::Begin("Scene")) {
            auto availableSceneWindowSize = ImGui::GetContentRegionAvail();
            if (availableSceneWindowSize.x != g_applicationContext.SceneViewerSize.x || availableSceneWindowSize.y != g_applicationContext.SceneViewerSize.y) {
                g_applicationContext.SceneViewerSize = glm::ivec2(availableSceneWindowSize.x, availableSceneWindowSize.y);
                g_applicationContext.SceneViewerResized = true;
            }

            auto texture = g_geometryFramebuffer.ColorAttachments[0].value().Texture.Id;
            auto imagePosition = ImGui::GetCursorPos();
            ImGui::Image(reinterpret_cast<ImTextureID>(texture), availableSceneWindowSize, g_imvec2UnitY, g_imvec2UnitX);
            ImGui::SetCursorPos(imagePosition);
            if (ImGui::BeginChild(1, ImVec2{192, -1})) {
                if (ImGui::CollapsingHeader("Statistics")) {
                    ImGui::Text("Text");
                }
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleVar();
        ImGui::End();
    }
}