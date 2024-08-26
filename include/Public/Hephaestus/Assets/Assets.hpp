#pragma once

#include <Hephaestus/VectorMath.hpp>

#include <expected>
#include <filesystem>
#include <string>

struct TScannedAsset {
    std::string Name;
    std::vector<std::string> Nodes;
    std::vector<std::string> Meshes;

    std::vector<std::string> Materials;
    std::vector<std::string> Images;
    std::vector<std::string> Samplers;

    std::vector<std::string> Scenes;
    std::vector<std::string> Cameras;
    std::vector<std::string> Lights;

    std::vector<std::string> Animations;
    std::vector<std::string> Skins;
};

struct TGpuVertexPosition;
struct TGpuVertexNormalUvTangent;

struct TAssetMesh {
    std::string_view Name;
    glm::mat4 InitialTransform;
    std::vector<TGpuVertexPosition> VertexPositions;
    std::vector<TGpuVertexNormalUvTangent> VertexNormalUvTangents;
    std::vector<uint32_t> Indices;
    std::string MaterialName;
};

auto GetSafeResourceName(
    const char* const baseName,
    const char* const text,
    const char* const resourceType,
    const std::size_t resourceIndex) -> std::string;
auto ScanAsset(const std::filesystem::path& assetFilePath,
               std::string_view baseName) -> std::expected<TScannedAsset, std::string>;
auto GetAssetMesh(const std::string& assetMeshName) -> TAssetMesh&;