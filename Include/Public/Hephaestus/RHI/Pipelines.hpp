#pragma once

#include <Hephaestus/RHI/Format.hpp>
#include <Hephaestus/VectorMath.hpp>
#include <Hephaestus/Id.hpp>

#include <array>
#include <cstdint>
#include <expected>
#include <optional>
#include <string_view>

enum class EPrimitiveTopology {
    Triangles,
    TriangleStrip,
    TriangleFan,
    Lines,
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

using SGraphicsPipelineId = SId<struct TGraphicsPipelineId>;
using SComputePipelineId = SId<struct TComputePipelineId>;

class Pipeline {
public:
    virtual ~Pipeline();

    virtual auto Bind() -> void;

    auto BindBufferAsUniformBuffer(uint32_t buffer,
                                   int32_t bindingIndex) -> void;

    auto BindBufferAsShaderStorageBuffer(uint32_t buffer,
                                         int32_t bindingIndex) -> void;

    auto BindTexture(int32_t bindingIndex,
                     uint32_t texture) -> void;

    auto BindTextureAndSampler(int32_t bindingIndex,
                               uint32_t texture,
                               uint32_t sampler) -> void;

    auto SetUniform(int32_t location,
                    float value) -> void;

    auto SetUniform(int32_t location,
                    int32_t value) -> void;

    auto SetUniform(int32_t location,
                    uint32_t value) -> void;

    auto SetUniform(int32_t location,
                    uint64_t value) -> void;

    auto SetUniform(int32_t location,
                    const glm::vec2& value) -> void;

    auto SetUniform(int32_t location,
                    const glm::vec3& value) -> void;

    auto SetUniform(int32_t location,
                    float value1,
                    float value2,
                    float value3,
                    float value4) -> void;

    auto SetUniform(int32_t location,
                    int32_t value1,
                    int32_t value2,
                    int32_t value3,
                    int32_t value4) -> void;

    auto SetUniform(int32_t location,
                    const glm::vec4& value) -> void;

    auto SetUniform(int32_t location,
                    const glm::mat4& value) -> void;

    uint32_t Id;
};

class GraphicsPipeline : public Pipeline {
public:
    auto Bind() -> void override;

    auto BindBufferAsVertexBuffer(uint32_t buffer,
                                  uint32_t bindingIndex,
                                  long offset,
                                  int32_t stride) -> void;

    auto DrawArrays(int32_t vertexOffset,
                    int32_t vertexCount) -> void;

    auto DrawElements(uint32_t indexBuffer,
                      int32_t elementCount) -> void;

    auto DrawElementsInstanced(uint32_t indexBuffer,
                               int32_t elementCount,
                               int32_t instanceCount) -> void;

    std::optional<uint32_t> InputLayout;
    uint32_t PrimitiveTopology;
    bool IsPrimitiveRestartEnabled;
};

class ComputePipeline : public Pipeline {
public:
private:
};

auto GetGraphicsPipeline(SGraphicsPipelineId graphicsPipelineId) -> GraphicsPipeline&;
auto GetComputePipeline(SComputePipelineId computePipelineId) -> ComputePipeline&;
auto DeleteGraphicsPipeline(const SGraphicsPipelineId& graphicsPipelineId) -> void;
auto DeleteComputePipeline(const SComputePipelineId& computePipelineId) -> void;
auto CreateGraphicsPipeline(const SGraphicsPipelineDescriptor& graphicsPipelineDescriptor) -> std::expected<SGraphicsPipelineId, std::string>;
auto CreateComputePipeline(const SComputePipelineDescriptor& computePipelineDescriptor) -> std::expected<SComputePipelineId, std::string>;