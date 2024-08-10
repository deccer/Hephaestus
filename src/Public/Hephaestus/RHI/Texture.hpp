#pragma once

#include <Hephaestus/RHI/Offsets.hpp>
#include <Hephaestus/RHI/Extents.hpp>
#include <Hephaestus/RHI/Format.hpp>
#include <Hephaestus/Id.hpp>

#include <cstdint>
#include <string>
#include <utility>

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

using STextureId = SId<struct TTextureId>;

struct SCreateTextureDescriptor {
    ETextureType TextureType = {};
    EFormat Format = {};
    SExtent3D Extent = {};
    int32_t MipMapLevels = 0;
    int32_t Layers = 0;
    ESampleCount SampleCount = {};
    std::string Label = {};
};

struct SUploadTextureDescriptor {
    uint32_t Level = {};
    SOffset3D Offset = {};
    SExtent3D Extent = {};
    EUploadFormat UploadFormat = EUploadFormat::Auto;
    EUploadType UploadType = EUploadType::Auto;
    const void* PixelData = nullptr;
};

struct STexture {
    uint32_t Id = {};
    EFormat Format = {};
    SExtent3D Extent = {};
    ETextureType TextureType = {};
};

auto FormatToBaseTypeClass(EFormat format) -> EBaseTypeClass;
auto FormatToUnderlyingOpenGLType(EFormat format) -> uint32_t;
auto FormatToComponentCount(EFormat format) -> int32_t;
auto IsFormatNormalized(EFormat format) -> int32_t;
auto FormatToFormatClass(EFormat format) -> EFormatClass;

auto GetTexture(STextureId id) -> STexture&;
auto CreateTexture(const SCreateTextureDescriptor& createTextureDescriptor) -> STextureId;
auto UploadTexture(const STextureId& textureId,
                   const SUploadTextureDescriptor& updateTextureDescriptor) -> void;
auto MakeTextureResident(const STextureId& textureId) -> uint64_t;
auto GenerateMipmaps(const STextureId& textureId) -> void;