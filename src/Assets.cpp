#include <Hephaestus/Assets.hpp>

#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>

#include <spdlog/spdlog.h>

#define POOLSTL_STD_SUPPLEMENT
#include <poolstl/poolstl.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fstream>
#include <iterator>
#include <ranges>
#include <stack>

std::unordered_map<std::string, SAssetImage> g_assetImages = {};
std::unordered_map<std::string, SAssetSampler> g_assetSamplers = {};
std::unordered_map<std::string, SAssetTexture> g_assetTextures = {};
std::unordered_map<std::string, SAssetMaterial> g_assetMaterials = {};
std::unordered_map<std::string, SAssetMesh> g_assetMeshes = {};
std::unordered_map<std::string, std::vector<std::string>> g_assetModelMeshes = {};

auto ReadBinaryFromFile(const std::filesystem::path& filePath) -> std::pair<std::unique_ptr<std::byte[]>, std::size_t> {
    std::size_t fileSize = std::filesystem::file_size(filePath);
    auto memory = std::make_unique<std::byte[]>(fileSize);
    std::ifstream file{filePath, std::ifstream::binary};
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), reinterpret_cast<char*>(memory.get()));
    return {std::move(memory), fileSize};
}

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

auto GetSafeResourceName(
    const char* const baseName,
    const char* const text,
    const char* const resourceType,
    const std::size_t resourceIndex) -> std::string {

    return (text == nullptr) || strlen(text) == 0
           ? std::format("{}-{}-{}", baseName, resourceType, resourceIndex)
           : std::format("{}-{}", baseName, text);
}

auto AddAssetMesh(
    const std::string& assetMeshName,
    const SAssetMesh& assetMesh) -> void {

    assert(!assetMeshName.empty());

    g_assetMeshes[assetMeshName] = assetMesh;
}

auto AddAssetImage(
    const std::string& assetImageName,
    SAssetImage&& assetImage) -> void {

    assert(!assetImageName.empty());

    g_assetImages[assetImageName] = std::move(assetImage);
}

auto AddAssetMaterial(
    const std::string& assetMaterialName,
    const SAssetMaterial& assetMaterial) -> void {

    assert(!assetMaterialName.empty());

    g_assetMaterials[assetMaterialName] = assetMaterial;
}

auto AddAssetTexture(
    const std::string& assetTextureName,
    const SAssetTexture& assetTexture) -> void {

    assert(!assetTextureName.empty());

    g_assetTextures[assetTextureName] = assetTexture;
}

auto AddAssetSampler(
    const std::string& assetSamplerName,
    const SAssetSampler& assetSampler) -> void {

    assert(!assetSamplerName.empty());

    g_assetSamplers[assetSamplerName] = assetSampler;
}

auto GetAssetImage(const std::string& assetImageName) -> SAssetImage& {

    assert(!assetImageName.empty() || g_assetImages.contains(assetImageName));

    return g_assetImages.at(assetImageName);
}

auto GetAssetMesh(const std::string& assetMeshName) -> SAssetMesh& {

    assert(!assetMeshName.empty() || g_assetMeshes.contains(assetMeshName));

    return g_assetMeshes.at(assetMeshName);
}

auto GetAssetMaterial(const std::string& assetMaterialName) -> SAssetMaterial& {

    assert(!assetMaterialName.empty() || g_assetMaterials.contains(assetMaterialName));

    return g_assetMaterials.at(assetMaterialName);
}

auto GetAssetModelMeshNames(const std::string& modelName) -> std::vector<std::string>& {

    assert(!modelName.empty() || g_assetModelMeshes.contains(modelName));

    return g_assetModelMeshes.at(modelName);
}

auto GetAssetSampler(const std::string& assetSamplerName) -> SAssetSampler& {

    assert(!assetSamplerName.empty() || g_assetSamplers.contains(assetSamplerName));

    return g_assetSamplers.at(assetSamplerName);
}

auto GetAssetTexture(const std::string& assetTextureName) -> SAssetTexture& {

    assert(!assetTextureName.empty());

    return g_assetTextures[assetTextureName];
}

auto GetLocalTransform(const fastgltf::Node& node) -> glm::mat4 {

    glm::mat4 transform{1.0};

    if (auto* trs = std::get_if<fastgltf::TRS>(&node.transform))
    {
        auto rotation = glm::quat{trs->rotation[3], trs->rotation[0], trs->rotation[1], trs->rotation[2]};
        auto scale = glm::make_vec3(trs->scale.data());
        auto translation = glm::make_vec3(trs->translation.data());

        auto rotationMatrix = glm::mat4_cast(rotation);

        // T * R * S
        transform = glm::scale(glm::translate(glm::mat4(1.0f), translation) * rotationMatrix, scale);
    }
    else if (auto* mat = std::get_if<fastgltf::Node::TransformMatrix>(&node.transform))
    {
        const auto& m = *mat;
        transform = glm::make_mat4x4(m.data());
    }

    return transform;
}

auto SignNotZero(glm::vec2 v) -> glm::vec2 {

    return glm::vec2((v.x >= 0.0f) ? +1.0f : -1.0f, (v.y >= 0.0f) ? +1.0f : -1.0f);
}

auto EncodeNormal(glm::vec3 normal) -> glm::vec2 {

    glm::vec2 encodedNormal = glm::vec2{normal.x, normal.y} * (1.0f / (abs(normal.x) + abs(normal.y) + abs(normal.z)));
    return (normal.z <= 0.0f)
           ? ((1.0f - glm::abs(glm::vec2{encodedNormal.y, encodedNormal.x})) * SignNotZero(encodedNormal))
           : encodedNormal;
}

auto GetVertices(
    const fastgltf::Asset& asset,
    const fastgltf::Primitive& primitive) -> std::pair<std::vector<SGpuVertexPosition>, std::vector<SGpuVertexNormalUvTangent>> {

    std::vector<glm::vec3> positions;
    auto& positionAccessor = asset.accessors[primitive.findAttribute("POSITION")->second];
    positions.resize(positionAccessor.count);
    fastgltf::iterateAccessorWithIndex<glm::vec3>(asset,
                                                  positionAccessor,
                                                  [&](glm::vec3 position, std::size_t index) { positions[index] = position; });

    std::vector<glm::vec3> normals;
    auto& normalAccessor = asset.accessors[primitive.findAttribute("NORMAL")->second];
    normals.resize(normalAccessor.count);
    fastgltf::iterateAccessorWithIndex<glm::vec3>(asset,
                                                  normalAccessor,
                                                  [&](glm::vec3 normal, std::size_t index) { normals[index] = normal; });

    std::vector<glm::vec4> tangents;
    auto& tangentAccessor = asset.accessors[primitive.findAttribute("TANGENT")->second];
    tangents.resize(tangentAccessor.count);
    fastgltf::iterateAccessorWithIndex<glm::vec4>(asset,
                                                  tangentAccessor,
                                                  [&](glm::vec4 tangent, std::size_t index) { tangents[index] = tangent; });

    std::vector<glm::vec3> uvs;
    if (primitive.findAttribute("TEXCOORD_0") != primitive.attributes.end())
    {
        auto& uvAccessor = asset.accessors[primitive.findAttribute("TEXCOORD_0")->second];
        uvs.resize(uvAccessor.count);
        fastgltf::iterateAccessorWithIndex<glm::vec2>(asset,
                                                      uvAccessor,
                                                      [&](glm::vec2 uv, std::size_t index) { uvs[index] = glm::vec3{uv, 0.0f}; });
    }
    else
    {
        uvs.resize(positions.size(), {});
    }

    std::vector<SGpuVertexPosition> verticesPosition;
    std::vector<SGpuVertexNormalUvTangent> verticesNormalUvTangent;
    verticesPosition.resize(positions.size());
    verticesNormalUvTangent.resize(positions.size());

    for (size_t i = 0; i < positions.size(); i++) {
        verticesPosition[i] = {
            positions[i],
        };
        uvs[i].z = tangents[i].w;
        verticesNormalUvTangent[i] = {
            glm::packSnorm2x16(EncodeNormal(normals[i])),
            glm::packSnorm2x16(EncodeNormal(tangents[i].xyz())),
            uvs[i]
        };
    }

    return {verticesPosition, verticesNormalUvTangent};
}

auto GetIndices(
    const fastgltf::Asset& asset,
    const fastgltf::Primitive& primitive) -> std::vector<uint32_t> {

    auto indices = std::vector<uint32_t>();
    auto& accessor = asset.accessors[primitive.indicesAccessor.value()];
    indices.resize(accessor.count);
    fastgltf::iterateAccessorWithIndex<uint32_t>(asset, accessor, [&](uint32_t value, size_t index)
    {
        indices[index] = value;
    });
    return indices;
}

auto CreateAssetMesh(
    std::string_view name,
    glm::mat4 initialTransform,
    const std::pair<std::vector<SGpuVertexPosition>, std::vector<SGpuVertexNormalUvTangent>>& vertices,
    const std::vector<uint32_t> indices,
    const std::string& materialName) -> SAssetMesh {

    return SAssetMesh{
        .Name = name,
        .InitialTransform = initialTransform,
        .VertexPositions = std::move(vertices.first),
        .VertexNormalUvTangents = std::move(vertices.second),
        .Indices = std::move(indices),
        .MaterialName = materialName
    };
}

auto CreateAssetImage(
    const void* data,
    std::size_t dataSize,
    fastgltf::MimeType mimeType,
    std::string_view name) -> SAssetImage {

    auto dataCopy = std::make_unique<std::byte[]>(dataSize);
    std::copy_n(static_cast<const std::byte*>(data), dataSize, dataCopy.get());

    return SAssetImage {
        .Name = std::string(name),
        .EncodedData = std::move(dataCopy),
        .EncodedDataSize = dataSize,
    };
}

auto CreateAssetMaterial(
    const std::string& modelName,
    const fastgltf::Asset& asset,
    const fastgltf::Material& fgMaterial,
    size_t assetMaterialIndex) -> void {

    SAssetMaterial assetMaterial = {};
    assetMaterial.BaseColorFactor = glm::make_vec4(fgMaterial.pbrData.baseColorFactor.data());
    assetMaterial.MetallicFactor = fgMaterial.pbrData.metallicFactor;
    assetMaterial.EmissiveFactor = glm::make_vec3(fgMaterial.emissiveFactor.data());
    assetMaterial.EmissiveStrength = fgMaterial.emissiveStrength;

    if (fgMaterial.pbrData.baseColorTexture.has_value()) {
        auto& fgBaseColorTexture = fgMaterial.pbrData.baseColorTexture.value();
        auto& fgTexture = asset.textures[fgBaseColorTexture.textureIndex];
        auto textureName = asset.images[fgTexture.imageIndex.value_or(0)].name.data();
        auto fgTextureImageName = GetSafeResourceName(modelName.data(), textureName, "image", fgTexture.imageIndex.value_or(0));
        auto fgTextureSamplerName = GetSafeResourceName(modelName.data(), textureName, "sampler", fgTexture.samplerIndex.value_or(0));

        assetMaterial.BaseColorChannel = SAssetMaterialChannel{
            .Usage = EMaterialChannelUsage::SRgb,
            .Image = fgTextureImageName,
            .Sampler = fgTextureSamplerName,
        };
    }

    if (fgMaterial.normalTexture.has_value()) {
        auto& fgNormalTexture = fgMaterial.normalTexture.value();
        auto& fgTexture = asset.textures[fgNormalTexture.textureIndex];
        auto textureName = asset.images[fgTexture.imageIndex.value_or(0)].name.data();
        auto fgTextureImageName = GetSafeResourceName(modelName.data(), textureName, "image", fgTexture.imageIndex.value_or(0));
        auto fgTextureSamplerName = GetSafeResourceName(modelName.data(), textureName, "sampler", fgTexture.samplerIndex.value_or(0));

        assetMaterial.BaseColorChannel = SAssetMaterialChannel{
            .Usage = EMaterialChannelUsage::Normal,
            .Image = fgTextureImageName,
            .Sampler = fgTextureSamplerName,
        };
    }

    if (fgMaterial.pbrData.metallicRoughnessTexture.has_value()) {
        auto& fgMetallicRoughnessTexture = fgMaterial.pbrData.metallicRoughnessTexture.value();
        auto& fgTexture = asset.textures[fgMetallicRoughnessTexture.textureIndex];
        auto textureName = asset.images[fgTexture.imageIndex.value_or(0)].name.data();
        auto fgTextureImageName = GetSafeResourceName(modelName.data(), textureName, "image", fgTexture.imageIndex.value_or(0));
        auto fgTextureSamplerName = GetSafeResourceName(modelName.data(), textureName, "sampler", fgTexture.samplerIndex.value_or(0));

        assetMaterial.OcclusionRoughnessMetallnessChannel = SAssetMaterialChannel{
            .Usage = EMaterialChannelUsage::MetalnessRoughness,
            .Image = fgTextureImageName,
            .Sampler = fgTextureSamplerName,
        };
    }

    if (fgMaterial.emissiveTexture.has_value()) {
        auto& fgEmissiveTexture = fgMaterial.emissiveTexture.value();
        auto& fgTexture = asset.textures[fgEmissiveTexture.textureIndex];
        auto textureName = asset.images[fgTexture.imageIndex.value_or(0)].name.data();
        auto fgTextureImageName = GetSafeResourceName(modelName.data(), textureName, "image", fgTexture.imageIndex.value_or(0));
        auto fgTextureSamplerName = GetSafeResourceName(modelName.data(), textureName, "sampler", fgTexture.samplerIndex.value_or(0));

        assetMaterial.BaseColorChannel = SAssetMaterialChannel{
            .Usage = EMaterialChannelUsage::SRgb,
            .Image = fgTextureImageName,
            .Sampler = fgTextureSamplerName,
        };
    }

    auto materialName = GetSafeResourceName(modelName.data(), fgMaterial.name.data(), "material", assetMaterialIndex);
    return AddAssetMaterial(materialName, assetMaterial);
}

auto LoadModelFromFile(
    const std::string& modelName,
    const std::filesystem::path& filePath) -> void {

    fastgltf::Parser parser(fastgltf::Extensions::KHR_mesh_quantization);

    constexpr auto gltfOptions =
        fastgltf::Options::DontRequireValidAssetMember |
        fastgltf::Options::AllowDouble |
        fastgltf::Options::LoadGLBBuffers |
        fastgltf::Options::LoadExternalBuffers |
        fastgltf::Options::LoadExternalImages;

    fastgltf::GltfDataBuffer data;
    data.loadFromFile(filePath);

    auto assetResult = parser.loadGltf(&data, filePath.parent_path(), gltfOptions);
    if (assetResult.error() != fastgltf::Error::None)
    {
        spdlog::error("fastgltf: Failed to load glTF: {}", fastgltf::getErrorMessage(assetResult.error()));
        return;
    }

    auto& fgAsset = assetResult.get();

    // images

    auto assetImageIds = std::vector<SAssetImageId>(fgAsset.images.size());
    const auto assetImageIndices = std::ranges::iota_view{(size_t)0, fgAsset.images.size()};

    std::transform(
        poolstl::execution::par,
        assetImageIndices.begin(),
        assetImageIndices.end(),
        assetImageIds.begin(),
        [&](size_t imageIndex) {

            const auto& fgImage = fgAsset.images[imageIndex];

            auto imageData = [&] {

                if (const auto* filePathUri = std::get_if<fastgltf::sources::URI>(&fgImage.data)) {
                    auto filePathFixed = std::filesystem::path(filePathUri->uri.path());
                    auto filePathParent = filePath.parent_path();
                    if (filePathFixed.is_relative()) {
                        filePathFixed = filePath.parent_path() / filePathFixed;
                    }
                    auto fileData = ReadBinaryFromFile(filePathFixed);
                    return CreateAssetImage(fileData.first.get(), fileData.second, filePathUri->mimeType, fgImage.name);
                }
                if (const auto* array = std::get_if<fastgltf::sources::Array>(&fgImage.data)) {
                    return CreateAssetImage(array->bytes.data(), array->bytes.size(), array->mimeType, fgImage.name);
                }
                if (const auto* vector = std::get_if<fastgltf::sources::Vector>(&fgImage.data)) {
                    return CreateAssetImage(vector->bytes.data(), vector->bytes.size(), vector->mimeType, fgImage.name);
                }
                if (const auto* view = std::get_if<fastgltf::sources::BufferView>(&fgImage.data)) {
                    auto& bufferView = fgAsset.bufferViews[view->bufferViewIndex];
                    auto& buffer = fgAsset.buffers[bufferView.bufferIndex];
                    if (const auto* array = std::get_if<fastgltf::sources::Array>(&buffer.data)) {
                        return CreateAssetImage(
                            array->bytes.data() + bufferView.byteOffset,
                            bufferView.byteLength,
                            view->mimeType,
                            fgImage.name);
                    }
                }

                return SAssetImage{};
            }();

            auto assetImageId = g_assetImages.size() + imageIndex;

            int32_t width = 0;
            int32_t height = 0;
            int32_t components = 0;
            auto pixels = LoadImageFromMemory(
                imageData.EncodedData.get(),
                imageData.EncodedDataSize,
                &width,
                &height,
                &components);

            imageData.Width = width;
            imageData.Height = height;
            imageData.Components = 4; // LoadImageFromMemory is forcing 4 components
            imageData.DecodedData.reset(pixels);
            imageData.Index = assetImageId;

            AddAssetImage(GetSafeResourceName(modelName.data(), imageData.Name.data(), "image", imageIndex), std::move(imageData));
            return static_cast<SAssetImageId>(assetImageId);
        });

    // samplers

    auto samplerIds = std::vector<SAssetSamplerId>(fgAsset.samplers.size());
    const auto samplerIndices = std::ranges::iota_view{(size_t)0, fgAsset.samplers.size()};
    std::transform(poolstl::execution::par, samplerIndices.begin(), samplerIndices.end(), samplerIds.begin(), [&](size_t samplerIndex) {

        const fastgltf::Sampler& fgSampler = fgAsset.samplers[samplerIndex];

        const auto getAddressMode = [](const fastgltf::Wrap& wrap) {
            switch (wrap) {
                case fastgltf::Wrap::ClampToEdge: return EAssetSamplerWrap::ClampToEdge;
                case fastgltf::Wrap::MirroredRepeat: return EAssetSamplerWrap::MirroredRepeat;
                case fastgltf::Wrap::Repeat: return EAssetSamplerWrap::Repeat;
                default:
                    return EAssetSamplerWrap::ClampToEdge;
            }
        };

        //TODO(deccer) reinvent min/mag filterisms, this sucks here
        const auto getMinFilter = [](const fastgltf::Filter& minFilter) {
            switch (minFilter) {
                case fastgltf::Filter::Linear: return EAssetSamplerFilter::Linear;
                case fastgltf::Filter::LinearMipMapLinear: return EAssetSamplerFilter::LinearMipmapLinear;
                case fastgltf::Filter::LinearMipMapNearest: return EAssetSamplerFilter::LinearMipmapNearest;
                case fastgltf::Filter::Nearest: return EAssetSamplerFilter::Nearest;
                case fastgltf::Filter::NearestMipMapLinear: return EAssetSamplerFilter::NearestMipmapLinear;
                case fastgltf::Filter::NearestMipMapNearest: return EAssetSamplerFilter::NearestMipmapNearest;
                default: std::unreachable();
            }
        };

        const auto getMagFilter = [](const fastgltf::Filter& magFilter) {
            switch (magFilter) {
                case fastgltf::Filter::Linear: return EAssetSamplerFilter::Linear;
                case fastgltf::Filter::Nearest: return EAssetSamplerFilter::Nearest;
                default: std::unreachable();
            }
        };

        auto assetSampler = SAssetSampler {
            .AddressModeU = getAddressMode(fgSampler.wrapS),
            .AddressModeV = getAddressMode(fgSampler.wrapT),
            .MagFilter = getMagFilter(fgSampler.magFilter.value_or(fastgltf::Filter::Nearest)),
            .MinFilter = getMinFilter(fgSampler.minFilter.value_or(fastgltf::Filter::Nearest)),
        };

        auto assetSamplerId = g_assetSamplers.size() + samplerIndex;
        AddAssetSampler(GetSafeResourceName(modelName.data(), fgSampler.name.data(), "sampler", assetSamplerId), assetSampler);
        return static_cast<SAssetSamplerId>(assetSamplerId);
    });

    // textures

    auto assetTextures = std::vector<SAssetTextureId>(fgAsset.textures.size());
    const auto assetTextureIndices = std::ranges::iota_view{(size_t)0, fgAsset.textures.size()};
    for (auto assetTextureIndex : assetTextureIndices) {

        auto& fgTexture = fgAsset.textures[assetTextureIndex];
        auto textureName = GetSafeResourceName(modelName.data(), fgTexture.name.data(), "texture", assetTextureIndex);

        AddAssetTexture(textureName, SAssetTexture{
            .ImageName = GetSafeResourceName(modelName.data(), nullptr, "image", fgTexture.imageIndex.value_or(0)),
            .SamplerName = GetSafeResourceName(modelName.data(), nullptr, "sampler", fgTexture.samplerIndex.value_or(0))
        });
    }

    // materials

    auto assetMaterialIds = std::vector<SAssetMaterialId>(fgAsset.materials.size());
    const auto assetMaterialIndices = std::ranges::iota_view{(size_t)0, fgAsset.materials.size()};
    for (auto assetMaterialIndex : assetMaterialIndices) {

        auto& fgMaterial = fgAsset.materials[assetMaterialIndex];

        CreateAssetMaterial(modelName, fgAsset, fgMaterial, assetMaterialIndex);
    }

    std::stack<std::pair<const fastgltf::Node*, glm::mat4>> nodeStack;
    glm::mat4 rootTransform = glm::mat4(1.0f);

    for (auto nodeIndex : fgAsset.scenes[0].nodeIndices)
    {
        nodeStack.emplace(&fgAsset.nodes[nodeIndex], rootTransform);
    }

    while (!nodeStack.empty())
    {
        decltype(nodeStack)::value_type top = nodeStack.top();
        const auto& [fgNode, parentGlobalTransform] = top;
        nodeStack.pop();

        glm::mat4 localTransform = GetLocalTransform(*fgNode);
        glm::mat4 globalTransform = parentGlobalTransform * localTransform;

        for (auto childNodeIndex : fgNode->children)
        {
            nodeStack.emplace(&fgAsset.nodes[childNodeIndex], globalTransform);
        }

        if (!fgNode->meshIndex.has_value()) {
            continue;
        }

        auto meshIndex = fgNode->meshIndex.value();
        auto& fgMesh = fgAsset.meshes[meshIndex];

        for (auto primitiveIndex = 0; const auto& fgPrimitive : fgMesh.primitives)
        {
            const auto primitiveMaterialIndex = fgPrimitive.materialIndex.value_or(0);

            auto vertices = GetVertices(fgAsset, fgPrimitive);
            auto indices = GetIndices(fgAsset, fgPrimitive);

            auto meshName = GetSafeResourceName(modelName.data(), fgMesh.name.data(), "mesh", primitiveIndex);

            auto materialIndex = fgPrimitive.materialIndex.value_or(0);
            auto& fgMaterial = fgAsset.materials[materialIndex];
            auto materialName = GetSafeResourceName(modelName.data(), fgMaterial.name.data(), "material", g_assetMaterials.size() + materialIndex);

            auto assetMesh = CreateAssetMesh(meshName, globalTransform, std::move(vertices), std::move(indices), materialName);
            AddAssetMesh(meshName, assetMesh);
            g_assetModelMeshes[modelName].push_back(meshName);

            primitiveIndex++;
        }
    }
}