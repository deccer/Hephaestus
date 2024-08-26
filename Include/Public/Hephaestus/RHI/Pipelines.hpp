#pragma once

#include <Hephaestus/RHI/Format.hpp>
#include <Hephaestus/VectorMath.hpp>
#include <Hephaestus/Id.hpp>

#include <array>
#include <cstdint>
#include <expected>
#include <optional>
#include <string_view>

enum class TPrimitiveTopology {
    Triangles,
    TriangleStrip,
    TriangleFan,
    Lines,
};

struct TInputAssemblyDescriptor {
    TPrimitiveTopology PrimitiveTopology = TPrimitiveTopology::Triangles;
    bool IsPrimitiveRestartEnabled = false;
};

struct TVertexInputAttributeDescriptor {
    uint32_t Location;
    uint32_t Binding;
    TFormat Format;
    uint32_t Offset;
};

struct TVertexInputDescriptor {
    std::array<std::optional<const TVertexInputAttributeDescriptor>, 8> VertexInputAttributes = {};
};

struct TGraphicsPipelineDescriptor {
    std::string_view Label;
    std::string_view VertexShaderFilePath;
    std::string_view FragmentShaderFilePath;

    TInputAssemblyDescriptor InputAssembly;
    std::optional<TVertexInputDescriptor> VertexInput;
};

struct TComputePipelineDescriptor {
    std::string_view Label;
    std::string_view ComputeShaderFilePath;
};

using TGraphicsPipelineId = SId<struct TTagGraphicsPipelineId>;
using TComputePipelineId = SId<struct TTagComputePipelineId>;

class TPipeline {
public:
    virtual ~TPipeline();

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

class TGraphicsPipeline : public TPipeline {
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

class TComputePipeline : public TPipeline {
public:
private:
};

auto GetGraphicsPipeline(TGraphicsPipelineId graphicsPipelineId) -> TGraphicsPipeline&;
auto GetComputePipeline(TComputePipelineId computePipelineId) -> TComputePipeline&;
auto DeleteGraphicsPipeline(const TGraphicsPipelineId& graphicsPipelineId) -> void;
auto DeleteComputePipeline(const TComputePipelineId& computePipelineId) -> void;
auto CreateGraphicsPipeline(const TGraphicsPipelineDescriptor& graphicsPipelineDescriptor) -> std::expected<TGraphicsPipelineId, std::string>;
auto CreateComputePipeline(const TComputePipelineDescriptor& computePipelineDescriptor) -> std::expected<TComputePipelineId, std::string>;