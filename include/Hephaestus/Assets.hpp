#pragma once

#include <Hephaestus/Renderer.hpp>
#include <Hephaestus/Types.hpp>

#include <format>
#include <unordered_map>

#include <fastgltf/types.hpp>

using SAssetMeshId = SId<struct TAssetMeshId>;
using SAssetImageId = SId<struct TAssetImageId>;
using SAssetTextureId = SId<struct TAssetTextureId>;
using SAssetSamplerId = SId<struct TAssetSamplerId>;
using SAssetMaterialId = SId<struct TAssetMaterialId>;

enum class EMaterialChannelUsage {
    SRgb, // use sRGB Format
    Normal, // use RG-format or bc7 when compressed
    MetalnessRoughness // single channelism or also RG?
};

struct SAssetMesh {
    std::string_view Name;
    glm::mat4 InitialTransform;
    std::vector<SGpuVertexPosition> VertexPositions;
    std::vector<SGpuVertexNormalUvTangent> VertexNormalUvTangents;
    std::vector<uint32_t> Indices;
    std::string MaterialName;
};

enum class EAssetSamplerWrap {
    ClampToEdge,
    MirroredRepeat,
    Repeat
};

enum class EAssetSamplerFilter {
    Linear,
    LinearMipmapLinear,
    LinearMipmapNearest,
    Nearest,
    NearestMipmapLinear,
    NearestMipmapNearest
};

struct SAssetSampler {
    EAssetSamplerWrap AddressModeU;
    EAssetSamplerWrap AddressModeV;
    EAssetSamplerFilter MagFilter;
    EAssetSamplerFilter MinFilter;
};

struct SAssetImage {

    int32_t Width = 0;
    int32_t Height = 0;
    int32_t PixelType = 0;
    int32_t Bits = 8;
    int32_t Components = 0;
    std::string Name;

    std::unique_ptr<std::byte[]> EncodedData = {};
    std::size_t EncodedDataSize = 0;

    std::unique_ptr<unsigned char[]> DecodedData = {};

    size_t Index = 0;
};

struct SAssetTexture {
    std::string ImageName;
    std::string SamplerName;
};

struct SAssetMaterialChannel {
    EMaterialChannelUsage Usage;
    std::string Image;
    std::string Sampler;
};

struct SAssetMaterial {
    glm::vec4 BaseColorFactor;
    float MetallicFactor;
    glm::vec3 EmissiveFactor;
    float EmissiveStrength;
    std::optional<SAssetMaterialChannel> BaseColorChannel;
    std::optional<SAssetMaterialChannel> NormalsChannel;
    std::optional<SAssetMaterialChannel> OcclusionRoughnessMetallnessChannel;
    std::optional<SAssetMaterialChannel> EmissiveChannel;
};

auto GetSafeResourceName(
    const char* const baseName,
    const char* const text,
    const char* const resourceType,
    const std::size_t resourceIndex) -> std::string;

auto AddAssetMesh(
    const std::string& assetMeshName,
    const SAssetMesh& assetMesh) -> void;

auto AddAssetImage(
    const std::string& assetImageName,
    SAssetImage&& assetImage) -> void;

auto AddAssetMaterial(
    const std::string& assetMaterialName,
    const SAssetMaterial& assetMaterial) -> void;

auto AddAssetTexture(
    const std::string& assetTextureName,
    const SAssetTexture& assetTexture) -> void;

auto AddAssetSampler(
    const std::string& assetSamplerName,
    const SAssetSampler& assetSampler) -> void;

auto GetAssetImage(const std::string& assetImageName) -> SAssetImage&;

auto GetAssetMesh(const std::string& assetMeshName) -> SAssetMesh&;

auto GetAssetMaterial(const std::string& assetMaterialName) -> SAssetMaterial&;

auto GetAssetModelMeshNames(const std::string& modelName) -> std::vector<std::string>&;

auto GetAssetSampler(const std::string& assetSamplerName) -> SAssetSampler&;

auto GetAssetTexture(const std::string& assetTextureName) -> SAssetTexture&;

auto GetLocalTransform(const fastgltf::Node& node) -> glm::mat4;

auto CreateAssetMesh(
    std::string_view name,
    glm::mat4 initialTransform,
    const std::pair<std::vector<SGpuVertexPosition>, std::vector<SGpuVertexNormalUvTangent>>& vertices,
    const std::vector<uint32_t> indices,
    const std::string& materialName) -> SAssetMesh;

auto CreateAssetImage(
    const void* data,
    std::size_t dataSize,
    fastgltf::MimeType mimeType,
    std::string_view name) -> SAssetImage;

auto CreateAssetMaterial(
    const std::string& modelName,
    const fastgltf::Asset& asset,
    const fastgltf::Material& fgMaterial,
    size_t assetMaterialIndex) -> void;

auto LoadModelFromFile(
    const std::string& modelName,
    const std::filesystem::path& filePath) -> void;