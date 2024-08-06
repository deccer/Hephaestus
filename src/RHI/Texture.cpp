#include <Xacor/RHI/Texture.hpp>
#include <Xacor/RHI/Debug.hpp>

#include <cassert>
#include <utility>
#include <vector>

#include <glad/gl.h>

std::vector<STexture> g_textures;
STextureId g_textureCounter = STextureId::Invalid;

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