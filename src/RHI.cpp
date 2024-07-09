#include <Hephaestus/RHI.hpp>

#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_GLSL

#include <stb_include.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include <expected>
#include <format>
#include <filesystem>
#include <string>
#include <utility>

#include <glad/gl.h>

uint32_t g_defaultInputLayout = 0;
uint32_t g_lastIndexBuffer = 0;

std::vector<SSampler> g_samplers;
SSamplerId g_samplerCounter = SSamplerId::Invalid;

std::vector<STexture> g_textures;
STextureId g_textureCounter = STextureId::Invalid;

std::unordered_map<SSamplerDescriptor, SSamplerId> g_samplerDescriptors;

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

inline auto GetTexture(STextureId id) -> STexture& {

    assert(id != STextureId::Invalid);
    return g_textures[size_t(id)];
}

/////////////////////////////////////////////////////////////

auto FreeImage(void* pixels) -> void {
    if (pixels != nullptr) {
        stbi_image_free(pixels);
    }
}

auto LoadImageFromMemory(std::byte* encodedData,
                         size_t encodedDataSize,
                         int32_t* width,
                         int32_t* height,
                         int32_t* components) -> unsigned char* {

    return stbi_load_from_memory(
        reinterpret_cast<const unsigned char*>(encodedData),
        static_cast<int32_t>(encodedDataSize),
        width,
        height,
        components,
        4);
}

auto LoadImageFromFile(const std::filesystem::path& filePath,
                       int32_t* width,
                       int32_t* height,
                       int32_t* components) -> unsigned char* {

    auto imageFile = fopen(filePath.c_str(), "rb");
    if (imageFile != nullptr) {
        auto* pixels = stbi_load_from_file(imageFile, width, height, components, 4);
        fclose(imageFile);
        return pixels;
    } else {
        return nullptr;
    }
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
                        glClearNamedFramebufferfv(0, GL_COLOR, colorAttachmentIndex, std::get_if<std::array<float, 4>>(
                            &colorAttachment.ClearColor.Data)->data());
                        break;
                    case EBaseTypeClass::Integer:
                        glClearNamedFramebufferiv(0, GL_COLOR, colorAttachmentIndex,
                                                  std::get_if<std::array<int32_t, 4>>(
                                                      &colorAttachment.ClearColor.Data)->data());
                        break;
                    case EBaseTypeClass::UnsignedInteger:
                        glClearNamedFramebufferuiv(0, GL_COLOR, colorAttachmentIndex,
                                                   std::get_if<std::array<uint32_t, 4>>(
                                                       &colorAttachment.ClearColor.Data)->data());
                        break;
                    default:
                        std::unreachable_sentinel;
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

auto SPipeline::Bind() -> void {
    glUseProgram(Id);
}

auto SPipeline::BindBufferAsUniformBuffer(uint32_t buffer, int32_t bindingIndex) -> void {
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, buffer);
}

auto SPipeline::BindBufferAsShaderStorageBuffer(uint32_t buffer, int32_t bindingIndex) -> void {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex, buffer);
}

auto SPipeline::BindTexture(int32_t bindingIndex, uint32_t texture) -> void {
    glBindTextureUnit(bindingIndex, texture);
}

auto SPipeline::BindTextureAndSampler(int32_t bindingIndex, uint32_t texture, uint32_t sampler) -> void {
    glBindTextureUnit(bindingIndex, texture);
    glBindSampler(bindingIndex, sampler);
}

auto SPipeline::SetUniform(int32_t location, float value) -> void {
    glProgramUniform1f(Id, location, value);
}

auto SPipeline::SetUniform(int32_t location, int32_t value) -> void {
    glProgramUniform1i(Id, location, value);
}

auto SPipeline::SetUniform(int32_t location, uint32_t value) -> void {
    glProgramUniform1ui(Id, location, value);
}

auto SPipeline::SetUniform(int32_t location, uint64_t value) -> void {
    glProgramUniformHandleui64ARB(Id, location, value);
}

auto SPipeline::SetUniform(int32_t location, const glm::vec2& value) -> void {
    glProgramUniform2fv(Id, location, 1, glm::value_ptr(value));
}

auto SPipeline::SetUniform(int32_t location, const glm::vec3& value) -> void {
    glProgramUniform3fv(Id, location, 1, glm::value_ptr(value));
}

auto SPipeline::SetUniform(int32_t location, float value1, float value2, float value3, float value4) -> void {
    glProgramUniform4f(Id, location, value1, value2, value3, value4);
}

auto SPipeline::SetUniform(int32_t location, int32_t value1, int32_t value2, int32_t value3, int32_t value4) -> void {
    glProgramUniform4i(Id, location, value1, value2, value3, value4);
}

auto SPipeline::SetUniform(int32_t location, const glm::vec4 value) -> void {
    glProgramUniform4fv(Id, location, 1, glm::value_ptr(value));
}

auto SPipeline::SetUniform(int32_t location, const glm::mat4& value) -> void {
    glProgramUniformMatrix4fv(Id, location, 1, GL_FALSE, glm::value_ptr(value));
}

auto SGraphicsPipeline::Bind() -> void {
    SPipeline::Bind();
    glBindVertexArray(InputLayout.has_value() ? *InputLayout : g_defaultInputLayout);
}

auto SGraphicsPipeline::BindBufferAsVertexBuffer(uint32_t buffer, uint32_t bindingIndex, size_t offset,
                                                 size_t stride) -> void {

    if (InputLayout.has_value()) {
        glVertexArrayVertexBuffer(*InputLayout, bindingIndex, buffer, offset, stride);
    }
}

auto SGraphicsPipeline::DrawArrays(int32_t vertexOffset, size_t vertexCount) -> void {

    glDrawArrays(PrimitiveTopology, vertexOffset, vertexCount);
}

auto SGraphicsPipeline::DrawElements(uint32_t indexBuffer, size_t elementCount) -> void {

    if (g_lastIndexBuffer != indexBuffer) {
        glVertexArrayElementBuffer(InputLayout.has_value() ? InputLayout.value() : g_defaultInputLayout, indexBuffer);
        g_lastIndexBuffer = indexBuffer;
    }

    glDrawElements(PrimitiveTopology, elementCount, GL_UNSIGNED_INT, nullptr);
}

auto SGraphicsPipeline::DrawElementsInstanced(uint32_t indexBuffer, size_t elementCount, size_t instanceCount) -> void {

    if (g_lastIndexBuffer != indexBuffer) {
        glVertexArrayElementBuffer(InputLayout.has_value() ? InputLayout.value() : g_defaultInputLayout, indexBuffer);
        g_lastIndexBuffer = indexBuffer;
    }

    glDrawElementsInstanced(PrimitiveTopology, elementCount, GL_UNSIGNED_INT, nullptr, instanceCount);
}

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