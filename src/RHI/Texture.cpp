#include <Hephaestus/RHI/Texture.hpp>
#include <Hephaestus/RHI/Debug.hpp>

#include <cassert>
#include <vector>

#include <glad/gl.h>

std::vector<TTexture> g_textures;

constexpr auto TextureAddressModeToGL(TTextureAddressMode textureAddressMode) -> uint32_t {
    switch (textureAddressMode) {
        case TTextureAddressMode::ClampToBorder:
            return GL_CLAMP_TO_BORDER;
        case TTextureAddressMode::ClampToEdge:
            return GL_CLAMP_TO_EDGE;
        case TTextureAddressMode::ClampToEdgeMirrored:
            return GL_MIRROR_CLAMP_TO_EDGE;
        case TTextureAddressMode::Repeat:
            return GL_REPEAT;
        case TTextureAddressMode::RepeatMirrored:
            return GL_MIRRORED_REPEAT;
        default:
            std::unreachable();
    }
}

constexpr auto TextureMagFilterToGL(TTextureMagFilter textureMagFilter) -> uint32_t {
    switch (textureMagFilter) {
        case TTextureMagFilter::Linear:
            return GL_LINEAR;
        case TTextureMagFilter::Nearest:
            return GL_NEAREST;
        default:
            std::unreachable();
    }
}

constexpr auto TextureMinFilterToGL(TTextureMinFilter textureMinFilter) -> uint32_t {
    switch (textureMinFilter) {
        case TTextureMinFilter::Linear:
            return GL_LINEAR;
        case TTextureMinFilter::Nearest:
            return GL_NEAREST;
        case TTextureMinFilter::LinearMipmapLinear:
            return GL_LINEAR_MIPMAP_LINEAR;
        case TTextureMinFilter::LinearMipmapNearest:
            return GL_LINEAR_MIPMAP_NEAREST;
        case TTextureMinFilter::NearestMipmapLinear:
            return GL_NEAREST_MIPMAP_LINEAR;
        case TTextureMinFilter::NearestMipmapNearest:
            return GL_NEAREST_MIPMAP_NEAREST;
        default:
            std::unreachable();
    }
}

constexpr auto TextureTypeToGL(TTextureType textureType) -> uint32_t {

    switch (textureType) {
        case TTextureType::Texture1D:
            return GL_TEXTURE_1D;
        case TTextureType::Texture2D:
            return GL_TEXTURE_2D;
        case TTextureType::Texture3D:
            return GL_TEXTURE_3D;
        case TTextureType::Texture1DArray:
            return GL_TEXTURE_1D_ARRAY;
        case TTextureType::Texture2DArray:
            return GL_TEXTURE_2D_ARRAY;
        case TTextureType::TextureCube:
            return GL_TEXTURE_CUBE_MAP;
        case TTextureType::TextureCubeArray:
            return GL_TEXTURE_CUBE_MAP_ARRAY;
        case TTextureType::Texture2DMultisample:
            return GL_TEXTURE_2D_MULTISAMPLE;
        case TTextureType::Texture2DMultisampleArray:
            return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
        default:
            std::unreachable();
    }
}

constexpr auto UploadFormatToGL(TUploadFormat uploadFormat) -> uint32_t {

    switch (uploadFormat) {
        case TUploadFormat::R:
            return GL_RED;
        case TUploadFormat::Rg:
            return GL_RG;
        case TUploadFormat::Rgb:
            return GL_RGB;
        case TUploadFormat::Bgr:
            return GL_BGR;
        case TUploadFormat::Rgba:
            return GL_RGBA;
        case TUploadFormat::Bgra:
            return GL_BGRA;
        case TUploadFormat::RInteger:
            return GL_RED_INTEGER;
        case TUploadFormat::RgInteger:
            return GL_RG_INTEGER;
        case TUploadFormat::RgbInteger:
            return GL_RGB_INTEGER;
        case TUploadFormat::BgrInteger:
            return GL_BGR_INTEGER;
        case TUploadFormat::RgbaInteger:
            return GL_RGBA_INTEGER;
        case TUploadFormat::BgraInteger:
            return GL_BGRA_INTEGER;
        case TUploadFormat::Depth:
            return GL_DEPTH_COMPONENT;
        case TUploadFormat::StencilIndex:
            return GL_STENCIL_INDEX;
        case TUploadFormat::DepthStencilIndex:
            return GL_DEPTH_STENCIL;
        default:
            std::unreachable();
    }
}

constexpr auto UploadTypeToGL(TUploadType uploadType) -> uint32_t {

    switch (uploadType) {
        case TUploadType::UnsignedByte:
            return GL_UNSIGNED_BYTE;
        case TUploadType::SignedByte:
            return GL_BYTE;
        case TUploadType::UnsignedShort:
            return GL_UNSIGNED_SHORT;
        case TUploadType::SignedShort:
            return GL_SHORT;
        case TUploadType::UnsignedInteger:
            return GL_UNSIGNED_INT;
        case TUploadType::SignedInteger:
            return GL_INT;
        case TUploadType::Float:
            return GL_FLOAT;
        case TUploadType::UnsignedByte332:
            return GL_UNSIGNED_BYTE_3_3_2;
        case TUploadType::UnsignedByte233Reversed:
            return GL_UNSIGNED_BYTE_2_3_3_REV;
        case TUploadType::UnsignedShort565:
            return GL_UNSIGNED_SHORT_5_6_5;
        case TUploadType::UnsignedShort565Reversed:
            return GL_UNSIGNED_SHORT_5_6_5_REV;
        case TUploadType::UnsignedShort4444:
            return GL_UNSIGNED_SHORT_4_4_4_4;
        case TUploadType::UnsignedShort4444Reversed:
            return GL_UNSIGNED_SHORT_4_4_4_4_REV;
        case TUploadType::UnsignedShort5551:
            return GL_UNSIGNED_SHORT_5_5_5_1;
        case TUploadType::UnsignedShort1555Reversed:
            return GL_UNSIGNED_SHORT_1_5_5_5_REV;
        case TUploadType::UnsignedInteger8888:
            return GL_UNSIGNED_INT_8_8_8_8;
        case TUploadType::UnsignedInteger8888Reversed:
            return GL_UNSIGNED_INT_8_8_8_8_REV;
        case TUploadType::UnsignedInteger1010102:
            return GL_UNSIGNED_INT_10_10_10_2;
        case TUploadType::UnsignedInteger2101010Reversed:
            return GL_UNSIGNED_INT_2_10_10_10_REV;
        default:
            std::unreachable();
    }
}

constexpr auto SampleCountToGL(TSampleCount sampleCount) -> int32_t {

    switch (sampleCount) {
        case TSampleCount::One:
            return 1;
        case TSampleCount::Two:
            return 2;
        case TSampleCount::Four:
            return 4;
        case TSampleCount::Eight:
            return 8;
        case TSampleCount::SixTeen:
            return 16;
        case TSampleCount::ThirtyTwo:
            return 32;
        default:
            std::unreachable();
    }
}

auto FormatToGL(TFormat format) -> uint32_t {
    switch (format) {
        case TFormat::R8_UNORM:
            return GL_R8;
        case TFormat::R8_SNORM:
            return GL_R8_SNORM;
        case TFormat::R16_UNORM:
            return GL_R16;
        case TFormat::R16_SNORM:
            return GL_R16_SNORM;
        case TFormat::R8G8_UNORM:
            return GL_RG8;
        case TFormat::R8G8_SNORM:
            return GL_RG8_SNORM;
        case TFormat::R16G16_UNORM:
            return GL_RG16;
        case TFormat::R16G16_SNORM:
            return GL_RG16_SNORM;
        case TFormat::R3G3B2_UNORM:
            return GL_R3_G3_B2;
        case TFormat::R4G4B4_UNORM:
            return GL_RGB4;
        case TFormat::R5G5B5_UNORM:
            return GL_RGB5;
        case TFormat::R8G8B8_UNORM:
            return GL_RGB8;
        case TFormat::R8G8B8_SNORM:
            return GL_RGB8_SNORM;
        case TFormat::R10G10B10_UNORM:
            return GL_RGB10;
        case TFormat::R12G12B12_UNORM:
            return GL_RGB12;
            // GL_RG16?
        case TFormat::R16G16B16_SNORM:
            return GL_RGB16_SNORM;
        case TFormat::R2G2B2A2_UNORM:
            return GL_RGBA2;
        case TFormat::R4G4B4A4_UNORM:
            return GL_RGBA4;
        case TFormat::R5G5B5A1_UNORM:
            return GL_RGB5_A1;
        case TFormat::R8G8B8A8_UNORM:
            return GL_RGBA8;
        case TFormat::R8G8B8A8_SNORM:
            return GL_RGBA8_SNORM;
        case TFormat::R10G10B10A2_UNORM:
            return GL_RGB10_A2;
        case TFormat::R10G10B10A2_UINT:
            return GL_RGB10_A2UI;
        case TFormat::R12G12B12A12_UNORM:
            return GL_RGBA12;
        case TFormat::R16G16B16A16_UNORM:
            return GL_RGBA16;
        case TFormat::R16G16B16A16_SNORM:
            return GL_RGBA16_SNORM;
        case TFormat::R8G8B8_SRGB:
            return GL_SRGB8;
        case TFormat::R8G8B8A8_SRGB:
            return GL_SRGB8_ALPHA8;
        case TFormat::R16_FLOAT:
            return GL_R16F;
        case TFormat::R16G16_FLOAT:
            return GL_RG16F;
        case TFormat::R16G16B16_FLOAT:
            return GL_RGB16F;
        case TFormat::R16G16B16A16_FLOAT:
            return GL_RGBA16F;
        case TFormat::R32_FLOAT:
            return GL_R32F;
        case TFormat::R32G32_FLOAT:
            return GL_RG32F;
        case TFormat::R32G32B32_FLOAT:
            return GL_RGB32F;
        case TFormat::R32G32B32A32_FLOAT:
            return GL_RGBA32F;
        case TFormat::R11G11B10_FLOAT:
            return GL_R11F_G11F_B10F;
        case TFormat::R9G9B9_E5:
            return GL_RGB9_E5;
        case TFormat::R8_SINT:
            return GL_R8I;
        case TFormat::R8_UINT:
            return GL_R8UI;
        case TFormat::R16_SINT:
            return GL_R16I;
        case TFormat::R16_UINT:
            return GL_R16UI;
        case TFormat::R32_SINT:
            return GL_R32I;
        case TFormat::R32_UINT:
            return GL_R32UI;
        case TFormat::R8G8_SINT:
            return GL_RG8I;
        case TFormat::R8G8_UINT:
            return GL_RG8UI;
        case TFormat::R16G16_SINT:
            return GL_RG16I;
        case TFormat::R16G16_UINT:
            return GL_RG16UI;
        case TFormat::R32G32_SINT:
            return GL_RG32I;
        case TFormat::R32G32_UINT:
            return GL_RG32UI;
        case TFormat::R8G8B8_SINT:
            return GL_RGB8I;
        case TFormat::R8G8B8_UINT:
            return GL_RGB8UI;
        case TFormat::R16G16B16_SINT:
            return GL_RGB16I;
        case TFormat::R16G16B16_UINT:
            return GL_RGB16UI;
        case TFormat::R32G32B32_SINT:
            return GL_RGB32I;
        case TFormat::R32G32B32_UINT:
            return GL_RGB32UI;
        case TFormat::R8G8B8A8_SINT:
            return GL_RGBA8I;
        case TFormat::R8G8B8A8_UINT:
            return GL_RGBA8UI;
        case TFormat::R16G16B16A16_SINT:
            return GL_RGBA16I;
        case TFormat::R16G16B16A16_UINT:
            return GL_RGBA16UI;
        case TFormat::R32G32B32A32_SINT:
            return GL_RGBA32I;
        case TFormat::R32G32B32A32_UINT:
            return GL_RGBA32UI;
        case TFormat::D32_FLOAT:
            return GL_DEPTH_COMPONENT32F;
        case TFormat::D32_UNORM:
            return GL_DEPTH_COMPONENT32;
        case TFormat::D24_UNORM:
            return GL_DEPTH_COMPONENT24;
        case TFormat::D16_UNORM:
            return GL_DEPTH_COMPONENT16;
        case TFormat::D32_FLOAT_S8_UINT:
            return GL_DEPTH32F_STENCIL8;
        case TFormat::D24_UNORM_S8_UINT:
            return GL_DEPTH24_STENCIL8;
        case TFormat::S8_UINT:
            return GL_STENCIL_INDEX8;
            /*
            case TFormat::BC1_RGB_UNORM: return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
            case TFormat::BC1_RGBA_UNORM: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            case TFormat::BC1_RGB_SRGB: return GL_COMPRESSED_SRGB_S3TC_DXT1_EXT;
            case TFormat::BC1_RGBA_SRGB: return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
            case TFormat::BC2_RGBA_UNORM: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            case TFormat::BC2_RGBA_SRGB: return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
            case TFormat::BC3_RGBA_UNORM: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            case TFormat::BC3_RGBA_SRGB: return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
            */
        case TFormat::BC4_R_UNORM:
            return GL_COMPRESSED_RED_RGTC1;
        case TFormat::BC4_R_SNORM:
            return GL_COMPRESSED_SIGNED_RED_RGTC1;
        case TFormat::BC5_RG_UNORM:
            return GL_COMPRESSED_RG_RGTC2;
        case TFormat::BC5_RG_SNORM:
            return GL_COMPRESSED_SIGNED_RG_RGTC2;
        case TFormat::BC6H_RGB_UFLOAT:
            return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
        case TFormat::BC6H_RGB_SFLOAT:
            return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
        case TFormat::BC7_RGBA_UNORM:
            return GL_COMPRESSED_RGBA_BPTC_UNORM;
        case TFormat::BC7_RGBA_SRGB:
            return GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
        default:
            std::string message = "Format not mappable";
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH,
                                 message.size(), message.data());
            return -1;
    }
}

auto FormatToBaseTypeClass(TFormat format) -> TBaseTypeClass {
    switch (format) {
        case TFormat::R8_UNORM:
        case TFormat::R8_SNORM:
        case TFormat::R16_UNORM:
        case TFormat::R16_SNORM:
        case TFormat::R8G8_UNORM:
        case TFormat::R8G8_SNORM:
        case TFormat::R16G16_UNORM:
        case TFormat::R16G16_SNORM:
        case TFormat::R3G3B2_UNORM:
        case TFormat::R4G4B4_UNORM:
        case TFormat::R5G5B5_UNORM:
        case TFormat::R8G8B8_UNORM:
        case TFormat::R8G8B8_SNORM:
        case TFormat::R10G10B10_UNORM:
        case TFormat::R12G12B12_UNORM:
        case TFormat::R16G16B16_SNORM:
        case TFormat::R2G2B2A2_UNORM:
        case TFormat::R4G4B4A4_UNORM:
        case TFormat::R5G5B5A1_UNORM:
        case TFormat::R8G8B8A8_UNORM:
        case TFormat::R8G8B8A8_SNORM:
        case TFormat::R10G10B10A2_UNORM:
        case TFormat::R12G12B12A12_UNORM:
        case TFormat::R16G16B16A16_UNORM:
        case TFormat::R8G8B8_SRGB:
        case TFormat::R8G8B8A8_SRGB:
        case TFormat::R16_FLOAT:
        case TFormat::R16G16_FLOAT:
        case TFormat::R16G16B16_FLOAT:
        case TFormat::R16G16B16A16_FLOAT:
        case TFormat::R32_FLOAT:
        case TFormat::R32G32_FLOAT:
        case TFormat::R32G32B32_FLOAT:
        case TFormat::R32G32B32A32_FLOAT:
        case TFormat::R11G11B10_FLOAT:
        case TFormat::R9G9B9_E5:
            return TBaseTypeClass::Float;
        case TFormat::R8_SINT:
        case TFormat::R16_SINT:
        case TFormat::R32_SINT:
        case TFormat::R8G8_SINT:
        case TFormat::R16G16_SINT:
        case TFormat::R32G32_SINT:
        case TFormat::R8G8B8_SINT:
        case TFormat::R16G16B16_SINT:
        case TFormat::R32G32B32_SINT:
        case TFormat::R8G8B8A8_SINT:
        case TFormat::R16G16B16A16_SINT:
        case TFormat::R32G32B32A32_SINT:
            return TBaseTypeClass::Integer;
        case TFormat::R10G10B10A2_UINT:
        case TFormat::R8_UINT:
        case TFormat::R16_UINT:
        case TFormat::R32_UINT:
        case TFormat::R8G8_UINT:
        case TFormat::R16G16_UINT:
        case TFormat::R32G32_UINT:
        case TFormat::R8G8B8_UINT:
        case TFormat::R16G16B16_UINT:
        case TFormat::R32G32B32_UINT:
        case TFormat::R8G8B8A8_UINT:
        case TFormat::R16G16B16A16_UINT:
        case TFormat::R32G32B32A32_UINT:
            return TBaseTypeClass::UnsignedInteger;
        default:
            std::unreachable();
    }
}

auto FormatToUploadFormat(TFormat format) -> TUploadFormat {

    switch (format) {
        case TFormat::R8_UNORM:
        case TFormat::R8_SNORM:
        case TFormat::R16_UNORM:
        case TFormat::R16_SNORM:
        case TFormat::R16_FLOAT:
        case TFormat::R32_FLOAT:
            return TUploadFormat::R;
        case TFormat::R8_SINT:
        case TFormat::R8_UINT:
        case TFormat::R16_SINT:
        case TFormat::R16_UINT:
        case TFormat::R32_SINT:
        case TFormat::R32_UINT:
            return TUploadFormat::RInteger;
        case TFormat::R8G8_UNORM:
        case TFormat::R8G8_SNORM:
        case TFormat::R16G16_UNORM:
        case TFormat::R16G16_SNORM:
        case TFormat::R16G16_FLOAT:
        case TFormat::R32G32_FLOAT:
            return TUploadFormat::Rg;
        case TFormat::R8G8_SINT:
        case TFormat::R8G8_UINT:
        case TFormat::R16G16_SINT:
        case TFormat::R16G16_UINT:
        case TFormat::R32G32_SINT:
        case TFormat::R32G32_UINT:
            return TUploadFormat::RgInteger;
        case TFormat::R3G3B2_UNORM:
        case TFormat::R4G4B4_UNORM:
        case TFormat::R5G5B5_UNORM:
        case TFormat::R8G8B8_UNORM:
        case TFormat::R8G8B8_SNORM:
        case TFormat::R10G10B10_UNORM:
        case TFormat::R12G12B12_UNORM:
        case TFormat::R16G16B16_SNORM:
        case TFormat::R8G8B8_SRGB:
        case TFormat::R16G16B16_FLOAT:
        case TFormat::R9G9B9_E5:
        case TFormat::R32G32B32_FLOAT:
        case TFormat::R11G11B10_FLOAT:
            return TUploadFormat::Rgb;
        case TFormat::R8G8B8_SINT:
        case TFormat::R8G8B8_UINT:
        case TFormat::R16G16B16_SINT:
        case TFormat::R16G16B16_UINT:
        case TFormat::R32G32B32_SINT:
        case TFormat::R32G32B32_UINT:
            return TUploadFormat::RgbInteger;
        case TFormat::R2G2B2A2_UNORM:
        case TFormat::R4G4B4A4_UNORM:
        case TFormat::R5G5B5A1_UNORM:
        case TFormat::R8G8B8A8_UNORM:
        case TFormat::R8G8B8A8_SNORM:
        case TFormat::R10G10B10A2_UNORM:
        case TFormat::R12G12B12A12_UNORM:
        case TFormat::R16G16B16A16_UNORM:
        case TFormat::R16G16B16A16_SNORM:
        case TFormat::R8G8B8A8_SRGB:
        case TFormat::R16G16B16A16_FLOAT:
        case TFormat::R32G32B32A32_FLOAT:
            return TUploadFormat::Rgba;
        case TFormat::R10G10B10A2_UINT:
        case TFormat::R8G8B8A8_SINT:
        case TFormat::R8G8B8A8_UINT:
        case TFormat::R16G16B16A16_SINT:
        case TFormat::R16G16B16A16_UINT:
        case TFormat::R32G32B32A32_SINT:
        case TFormat::R32G32B32A32_UINT:
            return TUploadFormat::RgbaInteger;
        case TFormat::D32_FLOAT:
        case TFormat::D32_UNORM:
        case TFormat::D24_UNORM:
        case TFormat::D16_UNORM:
            return TUploadFormat::Depth;
        case TFormat::D32_FLOAT_S8_UINT:
        case TFormat::D24_UNORM_S8_UINT:
            return TUploadFormat::DepthStencilIndex;
        case TFormat::S8_UINT:
            return TUploadFormat::StencilIndex;
        default:
            std::unreachable();
    }
}

auto FormatToUnderlyingOpenGLType(TFormat format) -> uint32_t {

    switch (format) {
        case TFormat::R8_UNORM:
        case TFormat::R8G8_UNORM:
        case TFormat::R8G8B8_UNORM:
        case TFormat::R8G8B8A8_UNORM:
        case TFormat::R8_UINT:
        case TFormat::R8G8_UINT:
        case TFormat::R8G8B8_UINT:
        case TFormat::R8G8B8A8_UINT:
        case TFormat::R8G8B8A8_SRGB:
        case TFormat::R8G8B8_SRGB:
            return GL_UNSIGNED_BYTE;
        case TFormat::R8_SNORM:
        case TFormat::R8G8_SNORM:
        case TFormat::R8G8B8_SNORM:
        case TFormat::R8G8B8A8_SNORM:
        case TFormat::R8_SINT:
        case TFormat::R8G8_SINT:
        case TFormat::R8G8B8_SINT:
        case TFormat::R8G8B8A8_SINT:
            return GL_BYTE;
        case TFormat::R16_UNORM:
        case TFormat::R16G16_UNORM:
        case TFormat::R16G16B16A16_UNORM:
        case TFormat::R16_UINT:
        case TFormat::R16G16_UINT:
        case TFormat::R16G16B16_UINT:
        case TFormat::R16G16B16A16_UINT:
            return GL_UNSIGNED_SHORT;
        case TFormat::R16_SNORM:
        case TFormat::R16G16_SNORM:
        case TFormat::R16G16B16_SNORM:
        case TFormat::R16G16B16A16_SNORM:
        case TFormat::R16_SINT:
        case TFormat::R16G16_SINT:
        case TFormat::R16G16B16_SINT:
        case TFormat::R16G16B16A16_SINT:
            return GL_SHORT;
        case TFormat::R16_FLOAT:
        case TFormat::R16G16_FLOAT:
        case TFormat::R16G16B16_FLOAT:
        case TFormat::R16G16B16A16_FLOAT:
            return GL_HALF_FLOAT;
        case TFormat::R32_FLOAT:
        case TFormat::R32G32_FLOAT:
        case TFormat::R32G32B32_FLOAT:
        case TFormat::R32G32B32A32_FLOAT:
            return GL_FLOAT;
        case TFormat::R32_SINT:
        case TFormat::R32G32_SINT:
        case TFormat::R32G32B32_SINT:
        case TFormat::R32G32B32A32_SINT:
            return GL_INT;
        case TFormat::R32_UINT:
        case TFormat::R32G32_UINT:
        case TFormat::R32G32B32_UINT:
        case TFormat::R32G32B32A32_UINT:
            return GL_UNSIGNED_INT;
        default:
            std::string message = "Format not mappable to opengl type";
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH,
                                 message.size(), message.data());
            return -1;
    }
}

auto FormatToComponentCount(TFormat format) -> int32_t {
    switch (format) {
        case TFormat::R8_UNORM:
        case TFormat::R8_SNORM:
        case TFormat::R16_UNORM:
        case TFormat::R16_SNORM:
        case TFormat::R16_FLOAT:
        case TFormat::R32_FLOAT:
        case TFormat::R8_SINT:
        case TFormat::R16_SINT:
        case TFormat::R32_SINT:
        case TFormat::R8_UINT:
        case TFormat::R16_UINT:
        case TFormat::R32_UINT:
            return 1;
        case TFormat::R8G8_UNORM:
        case TFormat::R8G8_SNORM:
        case TFormat::R16G16_FLOAT:
        case TFormat::R16G16_UNORM:
        case TFormat::R16G16_SNORM:
        case TFormat::R32G32_FLOAT:
        case TFormat::R8G8_SINT:
        case TFormat::R16G16_SINT:
        case TFormat::R32G32_SINT:
        case TFormat::R8G8_UINT:
        case TFormat::R16G16_UINT:
        case TFormat::R32G32_UINT:
            return 2;
        case TFormat::R8G8B8_UNORM:
        case TFormat::R8G8B8_SNORM:
        case TFormat::R16G16B16_SNORM:
        case TFormat::R16G16B16_FLOAT:
        case TFormat::R32G32B32_FLOAT:
        case TFormat::R8G8B8_SINT:
        case TFormat::R16G16B16_SINT:
        case TFormat::R32G32B32_SINT:
        case TFormat::R8G8B8_UINT:
        case TFormat::R16G16B16_UINT:
        case TFormat::R32G32B32_UINT:
            return 3;
        case TFormat::R8G8B8A8_UNORM:
        case TFormat::R8G8B8A8_SNORM:
        case TFormat::R16G16B16A16_UNORM:
        case TFormat::R16G16B16A16_FLOAT:
        case TFormat::R32G32B32A32_FLOAT:
        case TFormat::R8G8B8A8_SINT:
        case TFormat::R16G16B16A16_SINT:
        case TFormat::R32G32B32A32_SINT:
        case TFormat::R10G10B10A2_UINT:
        case TFormat::R8G8B8A8_UINT:
        case TFormat::R16G16B16A16_UINT:
        case TFormat::R32G32B32A32_UINT:
            return 4;
        default:
            std::unreachable();
    }
}

auto IsFormatNormalized(TFormat format) -> int32_t {

    switch (format) {
        case TFormat::R8_UNORM:
        case TFormat::R8_SNORM:
        case TFormat::R16_UNORM:
        case TFormat::R16_SNORM:
        case TFormat::R8G8_UNORM:
        case TFormat::R8G8_SNORM:
        case TFormat::R16G16_UNORM:
        case TFormat::R16G16_SNORM:
        case TFormat::R8G8B8_UNORM:
        case TFormat::R8G8B8_SNORM:
        case TFormat::R16G16B16_SNORM:
        case TFormat::R8G8B8A8_UNORM:
        case TFormat::R8G8B8A8_SNORM:
        case TFormat::R16G16B16A16_UNORM:
            return GL_TRUE;
        case TFormat::R16_FLOAT:
        case TFormat::R32_FLOAT:
        case TFormat::R8_SINT:
        case TFormat::R16_SINT:
        case TFormat::R32_SINT:
        case TFormat::R8_UINT:
        case TFormat::R16_UINT:
        case TFormat::R32_UINT:
        case TFormat::R16G16_FLOAT:
        case TFormat::R32G32_FLOAT:
        case TFormat::R8G8_SINT:
        case TFormat::R16G16_SINT:
        case TFormat::R32G32_SINT:
        case TFormat::R8G8_UINT:
        case TFormat::R16G16_UINT:
        case TFormat::R32G32_UINT:
        case TFormat::R16G16B16_FLOAT:
        case TFormat::R32G32B32_FLOAT:
        case TFormat::R8G8B8_SINT:
        case TFormat::R16G16B16_SINT:
        case TFormat::R32G32B32_SINT:
        case TFormat::R8G8B8_UINT:
        case TFormat::R16G16B16_UINT:
        case TFormat::R32G32B32_UINT:
        case TFormat::R16G16B16A16_FLOAT:
        case TFormat::R32G32B32A32_FLOAT:
        case TFormat::R8G8B8A8_SINT:
        case TFormat::R16G16B16A16_SINT:
        case TFormat::R32G32B32A32_SINT:
        case TFormat::R10G10B10A2_UINT:
        case TFormat::R8G8B8A8_UINT:
        case TFormat::R16G16B16A16_UINT:
        case TFormat::R32G32B32A32_UINT:
            return GL_FALSE;
        default:
            std::unreachable();
    }
}

auto FormatToFormatClass(TFormat format) -> TFormatClass {
    switch (format) {
        case TFormat::R8_UNORM:
        case TFormat::R8_SNORM:
        case TFormat::R16_UNORM:
        case TFormat::R16_SNORM:
        case TFormat::R8G8_UNORM:
        case TFormat::R8G8_SNORM:
        case TFormat::R16G16_UNORM:
        case TFormat::R16G16_SNORM:
        case TFormat::R8G8B8_UNORM:
        case TFormat::R8G8B8_SNORM:
        case TFormat::R16G16B16_SNORM:
        case TFormat::R8G8B8A8_UNORM:
        case TFormat::R8G8B8A8_SNORM:
        case TFormat::R16G16B16A16_UNORM:
        case TFormat::R16_FLOAT:
        case TFormat::R16G16_FLOAT:
        case TFormat::R16G16B16_FLOAT:
        case TFormat::R16G16B16A16_FLOAT:
        case TFormat::R32_FLOAT:
        case TFormat::R32G32_FLOAT:
        case TFormat::R32G32B32_FLOAT:
        case TFormat::R32G32B32A32_FLOAT:
            return TFormatClass::Float;
        case TFormat::R8_SINT:
        case TFormat::R16_SINT:
        case TFormat::R32_SINT:
        case TFormat::R8G8_SINT:
        case TFormat::R16G16_SINT:
        case TFormat::R32G32_SINT:
        case TFormat::R8G8B8_SINT:
        case TFormat::R16G16B16_SINT:
        case TFormat::R32G32B32_SINT:
        case TFormat::R8G8B8A8_SINT:
        case TFormat::R16G16B16A16_SINT:
        case TFormat::R32G32B32A32_SINT:
        case TFormat::R10G10B10A2_UINT:
        case TFormat::R8_UINT:
        case TFormat::R16_UINT:
        case TFormat::R32_UINT:
        case TFormat::R8G8_UINT:
        case TFormat::R16G16_UINT:
        case TFormat::R32G32_UINT:
        case TFormat::R8G8B8_UINT:
        case TFormat::R16G16B16_UINT:
        case TFormat::R32G32B32_UINT:
        case TFormat::R8G8B8A8_UINT:
        case TFormat::R16G16B16A16_UINT:
        case TFormat::R32G32B32A32_UINT:
            return TFormatClass::Integer;
        default:
            return TFormatClass::Long;
    }
}

constexpr auto TextureTypeToDimension(TTextureType textureType) -> uint32_t {

    switch (textureType) {
        case TTextureType::Texture1D:
            return 1;
        case TTextureType::Texture2D:
        case TTextureType::Texture2DMultisample:
        case TTextureType::Texture1DArray:
            return 2;
        case TTextureType::Texture3D:
        case TTextureType::Texture2DArray:
        case TTextureType::Texture2DMultisampleArray:
        case TTextureType::TextureCube:
        case TTextureType::TextureCubeArray:
            return 3;
        default:
            std::unreachable();
    }
}

inline auto GetTexture(TTextureId id) -> TTexture& {

    assert(id != TTextureId::Invalid);
    return g_textures[size_t(id)];
}

auto CreateTexture(const TCreateTextureDescriptor& createTextureDescriptor) -> TTextureId {

    const auto textureId = TTextureId(g_textures.size());
    auto& texture = g_textures.emplace_back();

    glCreateTextures(TextureTypeToGL(createTextureDescriptor.TextureType), 1, &texture.Id);
    if (!createTextureDescriptor.Label.empty()) {
        SetDebugLabel(texture.Id, GL_TEXTURE, createTextureDescriptor.Label);
    }

    texture.Extent = createTextureDescriptor.Extent;
    texture.Format = createTextureDescriptor.Format;
    texture.TextureType = createTextureDescriptor.TextureType;

    switch (createTextureDescriptor.TextureType) {
        case TTextureType::Texture1D:
            glTextureStorage1D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Width));
            break;
        case TTextureType::Texture2D:
            glTextureStorage2D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Width),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Height));
            break;
        case TTextureType::Texture3D:
            glTextureStorage3D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Width),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Height),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Depth));
            break;
        case TTextureType::Texture1DArray:
            glTextureStorage2D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Width),
                               createTextureDescriptor.Layers);
            break;
        case TTextureType::Texture2DArray:
            glTextureStorage3D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Width),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Height),
                               createTextureDescriptor.Layers);
            break;
        case TTextureType::TextureCube:
            glTextureStorage2D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Width),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Height));
            break;
        case TTextureType::TextureCubeArray:
            glTextureStorage3D(texture.Id,
                               createTextureDescriptor.MipMapLevels,
                               FormatToGL(createTextureDescriptor.Format),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Width),
                               static_cast<int32_t>(createTextureDescriptor.Extent.Height),
                               static_cast<int32_t>(createTextureDescriptor.Layers));
            break;
        case TTextureType::Texture2DMultisample:
            glTextureStorage2DMultisample(texture.Id,
                                          SampleCountToGL(createTextureDescriptor.SampleCount),
                                          FormatToGL(createTextureDescriptor.Format),
                                          static_cast<int32_t>(createTextureDescriptor.Extent.Width),
                                          static_cast<int32_t>(createTextureDescriptor.Extent.Height),
                                          GL_TRUE);
            break;
        case TTextureType::Texture2DMultisampleArray:
            glTextureStorage3DMultisample(texture.Id,
                                          SampleCountToGL(createTextureDescriptor.SampleCount),
                                          FormatToGL(createTextureDescriptor.Format),
                                          static_cast<int32_t>(createTextureDescriptor.Extent.Width),
                                          static_cast<int32_t>(createTextureDescriptor.Extent.Height),
                                          createTextureDescriptor.Layers,
                                          GL_TRUE);
            break;
        default:
            std::unreachable();
    }

    return textureId;
}

auto UploadTexture(const TTextureId& textureId,
                   const TUploadTextureDescriptor& updateTextureDescriptor) -> void {

    auto& texture = GetTexture(textureId);

    uint32_t format = 0;
    if (updateTextureDescriptor.UploadFormat == TUploadFormat::Auto) {
        format = UploadFormatToGL(FormatToUploadFormat(texture.Format));
    } else {
        format = UploadFormatToGL(updateTextureDescriptor.UploadFormat);
    }

    uint32_t type = 0;
    if (updateTextureDescriptor.UploadType == TUploadType::Auto) {
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

auto MakeTextureResident(const TTextureId& textureId) -> uint64_t {

    auto& texture = GetTexture(textureId);

    auto textureHandle = glGetTextureHandleARB(texture.Id);
    glMakeTextureHandleResidentARB(textureHandle);

    return textureHandle;
}

auto GenerateMipmaps(const TTextureId& textureId) -> void {

    auto& texture = GetTexture(textureId);
    glGenerateTextureMipmap(texture.Id);
}