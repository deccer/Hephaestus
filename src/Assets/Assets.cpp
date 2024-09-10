#include <Hephaestus/Assets/Assets.hpp>
#include <Hephaestus/RHI/VertexTypes.hpp>

#include <parallel_hashmap/phmap.h>

#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>

#include <spdlog/spdlog.h>

phmap::flat_hash_map<std::string, std::filesystem::path> g_scannedAssets = {};
phmap::flat_hash_map<std::string, TAssetMesh> g_assetMeshes = {};
phmap::flat_hash_map<std::string, TAssetMaterial> g_assetMaterials = {};

auto GetSafeResourceName(
    const char* const text,
    const char* const resourceType,
    std::size_t resourceIndex) -> std::string {

    return (text == nullptr) || strlen(text) == 0
           ? std::format("{}-{}", resourceType, resourceIndex)
           : std::format("{}", text);
}

auto ScanAsset(const std::string& baseName,
               const std::filesystem::path& filePath) -> std::expected<TScannedAsset, std::string> {

    fastgltf::Parser parser;

    auto fgFile = fastgltf::MappedGltfFile::FromPath(filePath);
    if (fgFile.error() != fastgltf::Error::None) {
        return std::unexpected(std::format("fastgltf: Failed to load glTF data: {}", fastgltf::getErrorMessage(fgFile.error())));
    }

    auto parentPath = filePath.parent_path();
    auto loadResult = parser.loadGltf(fgFile.get(), parentPath, fastgltf::Options::None);
    if (loadResult.error() != fastgltf::Error::None)
    {
        return std::unexpected(std::format("fastgltf: Failed to parse glTF: {}", fastgltf::getErrorMessage(loadResult.error())));
    }

    TScannedAsset assetScan = {
        .Name = baseName
    };

    auto& fgAsset = loadResult.get();
    assetScan.Images.resize(fgAsset.images.size());
    for (std::size_t i = 0, end = fgAsset.images.size(); i < end; ++i) {
        assetScan.Images[i] = GetSafeResourceName(fgAsset.images[i].name.data(), "image", i);
    }

    assetScan.Samplers.resize(fgAsset.samplers.size());
    for (std::size_t i = 0, end = fgAsset.samplers.size(); i < end; ++i) {
        assetScan.Samplers[i] = GetSafeResourceName(fgAsset.samplers[i].name.data(), "sampler", i);
    }

    assetScan.Materials.resize(fgAsset.materials.size());
    for (std::size_t i = 0, end = fgAsset.materials.size(); i < end; ++i) {
        assetScan.Materials[i] = GetSafeResourceName(fgAsset.materials[i].name.data(), "material", i);
    }

    assetScan.Cameras.resize(fgAsset.cameras.size());
    for (std::size_t i = 0, end = fgAsset.cameras.size(); i < end; ++i) {
        assetScan.Cameras[i] = GetSafeResourceName(fgAsset.cameras[i].name.data(), "camera", i);
    }

    assetScan.Lights.resize(fgAsset.lights.size());
    for (std::size_t i = 0, end = fgAsset.lights.size(); i < end; ++i) {
        assetScan.Lights[i] = GetSafeResourceName(fgAsset.lights[i].name.data(), "light", i);
    }

    assetScan.Meshes.resize(fgAsset.meshes.size());
    for (std::size_t i = 0, end = fgAsset.meshes.size(); i < end; ++i) {
        assetScan.Meshes[i] = GetSafeResourceName(fgAsset.meshes[i].name.data(), "mesh", i);
    }

    assetScan.Nodes.resize(fgAsset.nodes.size());
    for (std::size_t i = 0, end = fgAsset.nodes.size(); i < end; ++i) {
        assetScan.Nodes[i] = GetSafeResourceName(fgAsset.nodes[i].name.data(), "node", i);
    }

    assetScan.Skins.resize(fgAsset.skins.size());
    for (std::size_t i = 0, end = fgAsset.skins.size(); i < end; ++i) {
        assetScan.Skins[i] = GetSafeResourceName(fgAsset.skins[i].name.data(), "skin", i);
    }

    assetScan.Animations.resize(fgAsset.animations.size());
    for (std::size_t i = 0, end = fgAsset.animations.size(); i < end; ++i) {
        assetScan.Animations[i] = GetSafeResourceName(fgAsset.animations[i].name.data(), "animation", i);
    }

    assetScan.Scenes.resize(fgAsset.scenes.size());
    for (std::size_t i = 0, end = fgAsset.scenes.size(); i < end; ++i) {
        assetScan.Scenes[i] = GetSafeResourceName(fgAsset.scenes[i].name.data(), "scene", i);
    }

    g_scannedAssets[baseName] = filePath;

    return assetScan;
}

auto CreateAssetMesh(std::string_view assetMeshName) -> void {

    const auto& filePath = g_scannedAssets.at(assetMeshName);

    constexpr auto parserOptions =
        fastgltf::Extensions::EXT_mesh_gpu_instancing |
        fastgltf::Extensions::KHR_mesh_quantization |
        fastgltf::Extensions::EXT_meshopt_compression |
        fastgltf::Extensions::KHR_lights_punctual |
        fastgltf::Extensions::EXT_texture_webp |
        fastgltf::Extensions::KHR_texture_transform |
        fastgltf::Extensions::KHR_texture_basisu |
        fastgltf::Extensions::MSFT_texture_dds |
        fastgltf::Extensions::KHR_materials_specular |
        fastgltf::Extensions::KHR_materials_ior |
        fastgltf::Extensions::KHR_materials_iridescence |
        fastgltf::Extensions::KHR_materials_volume |
        fastgltf::Extensions::KHR_materials_transmission |
        fastgltf::Extensions::KHR_materials_clearcoat |
        fastgltf::Extensions::KHR_materials_emissive_strength |
        fastgltf::Extensions::KHR_materials_sheen |
        fastgltf::Extensions::KHR_materials_unlit;
    fastgltf::Parser parser(parserOptions);

    auto dataResult = fastgltf::GltfDataBuffer::FromPath(filePath);
    if (dataResult.error() != fastgltf::Error::None) {
        spdlog::error("fastgltf: Failed to load glTF data: {}", fastgltf::getErrorMessage(dataResult.error()));
        return;
    }

    constexpr auto gltfOptions =
        fastgltf::Options::DontRequireValidAssetMember |
        fastgltf::Options::AllowDouble |
        fastgltf::Options::LoadExternalBuffers |
        fastgltf::Options::LoadExternalImages;
    const auto parentPath = filePath.parent_path();
    auto loadResult = parser.loadGltf(dataResult.get(), parentPath, gltfOptions);
    if (loadResult.error() != fastgltf::Error::None)
    {
        spdlog::error("fastgltf: Failed to parse glTF: {}", fastgltf::getErrorMessage(loadResult.error()));
        return;
    }

    TAssetMesh assetMesh;

    auto& fgAsset = loadResult.get();

    ProcessTextures();
    ProcessMaterials();
    ProcessMeshes();
    ProcessNodes();
}

auto GetAssetMesh(const std::string& assetMeshName) -> TAssetMesh& {

    assert(!assetMeshName.empty() || g_assetMeshes.contains(assetMeshName));
    return g_assetMeshes.at(assetMeshName);
}

auto GetAssetMaterial(const std::string& assetMaterialName) -> TAssetMaterial& {

    assert(!assetMaterialName.empty() || g_assetMaterials.contains(assetMaterialName));
    return g_assetMaterials.at(assetMaterialName);
}