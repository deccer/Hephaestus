#pragma once

#include <Hephaestus/RHI/Offsets.hpp>
#include <Hephaestus/RHI/Extents.hpp>
#include <Hephaestus/RHI/Format.hpp>
#include <Hephaestus/Id.hpp>

#include <cstdint>
#include <string>
#include <utility>

enum class TFormatClass {
    Float,
    Integer,
    Long
};

enum class TBaseTypeClass {
    Float,
    Integer,
    UnsignedInteger
};

enum class TTextureType : uint32_t {
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

enum class TSampleCount : uint32_t {
    One = 1,
    Two = 2,
    Four = 4,
    Eight = 8,
    SixTeen = 16,
    ThirtyTwo = 32,
};

enum class TUploadFormat : uint32_t {
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

enum class TUploadType : uint32_t {
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

enum class TTextureAddressMode {
    Repeat,
    RepeatMirrored,
    ClampToEdge,
    ClampToBorder,
    ClampToEdgeMirrored,
};

enum class TTextureMinFilter {
    Nearest,
    NearestMipmapLinear,
    NearestMipmapNearest,
    Linear,
    LinearMipmapLinear,
    LinearMipmapNearest,
};

enum class TTextureMagFilter {
    Nearest,
    Linear
};

using TTextureId = SId<struct TTagTextureId>;

struct TCreateTextureDescriptor {
    TTextureType TextureType = {};
    TFormat Format = {};
    TExtent3D Extent = {};
    int32_t MipMapLevels = 0;
    int32_t Layers = 0;
    TSampleCount SampleCount = {};
    std::string Label = {};
};

struct TUploadTextureDescriptor {
    uint32_t Level = {};
    TOffset3D Offset = {};
    TExtent3D Extent = {};
    TUploadFormat UploadFormat = TUploadFormat::Auto;
    TUploadType UploadType = TUploadType::Auto;
    const void* PixelData = nullptr;
};

struct TTexture {
    uint32_t Id = {};
    TFormat Format = {};
    TExtent3D Extent = {};
    TTextureType TextureType = {};
};

auto FormatToBaseTypeClass(TFormat format) -> TBaseTypeClass;
auto FormatToUnderlyingOpenGLType(TFormat format) -> uint32_t;
auto FormatToComponentCount(TFormat format) -> int32_t;
auto IsFormatNormalized(TFormat format) -> int32_t;
auto FormatToFormatClass(TFormat format) -> TFormatClass;

auto GetTexture(TTextureId id) -> TTexture&;
auto CreateTexture(const TCreateTextureDescriptor& createTextureDescriptor) -> TTextureId;
auto UploadTexture(const TTextureId& textureId,
                   const TUploadTextureDescriptor& updateTextureDescriptor) -> void;
auto MakeTextureResident(const TTextureId& textureId) -> uint64_t;
auto GenerateMipmaps(const TTextureId& textureId) -> void;