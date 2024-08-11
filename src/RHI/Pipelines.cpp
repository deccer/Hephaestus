#include <Hephaestus/RHI/Pipelines.hpp>
#include <Hephaestus/RHI/Debug.hpp>
#include <Hephaestus/RHI/Texture.hpp>

#include <filesystem>
#include <format>
#include <vector>

#include <glad/gl.h>

#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_GLSL
#include <stb_include.h>

uint32_t g_defaultInputLayout = 0;
uint32_t g_lastIndexBuffer = 0;

std::vector<GraphicsPipeline> g_graphicsPipelines = {};
std::vector<ComputePipeline> g_computePipelines = {};

auto GetGraphicsPipeline(SGraphicsPipelineId graphicsPipelineId) -> GraphicsPipeline& {
    assert(graphicsPipelineId != SGraphicsPipelineId::Invalid);
    return g_graphicsPipelines[static_cast<size_t>(graphicsPipelineId)];
}

auto GetComputePipeline(SComputePipelineId computePipelineId) -> ComputePipeline& {
    assert(computePipelineId != SComputePipelineId::Invalid);
    return g_computePipelines[static_cast<size_t>(computePipelineId)];
}

Pipeline::~Pipeline() {
}

auto Pipeline::Bind() -> void {
    glUseProgram(Id);
}

auto Pipeline::BindBufferAsUniformBuffer(uint32_t buffer,
                                          int32_t bindingIndex) -> void {
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, buffer);
}

auto Pipeline::BindBufferAsShaderStorageBuffer(uint32_t buffer,
                                                int32_t bindingIndex) -> void {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex, buffer);
}

auto Pipeline::BindTexture(int32_t bindingIndex,
                            uint32_t texture) -> void {
    glBindTextureUnit(bindingIndex, texture);
}

auto Pipeline::BindTextureAndSampler(int32_t bindingIndex,
                                      uint32_t texture,
                                      uint32_t sampler) -> void {
    glBindTextureUnit(bindingIndex, texture);
    glBindSampler(bindingIndex, sampler);
}

auto Pipeline::SetUniform(int32_t location,
                           float value) -> void {
    glProgramUniform1f(Id, location, value);
}

auto Pipeline::SetUniform(int32_t location,
                           int32_t value) -> void {
    glProgramUniform1i(Id, location, value);
}

auto Pipeline::SetUniform(int32_t location,
                           uint32_t value) -> void {
    glProgramUniform1ui(Id, location, value);
}

auto Pipeline::SetUniform(int32_t location,
                           uint64_t value) -> void {
    glProgramUniformHandleui64ARB(Id, location, value);
}

auto Pipeline::SetUniform(int32_t location,
                           const glm::vec2& value) -> void {
    glProgramUniform2fv(Id, location, 1, glm::value_ptr(value));
}

auto Pipeline::SetUniform(int32_t location,
                           const glm::vec3& value) -> void {
    glProgramUniform3fv(Id, location, 1, glm::value_ptr(value));
}

auto Pipeline::SetUniform(int32_t location,
                           float value1,
                           float value2,
                           float value3,
                           float value4) -> void {
    glProgramUniform4f(Id, location, value1, value2, value3, value4);
}

auto Pipeline::SetUniform(int32_t location,
                           int32_t value1,
                           int32_t value2,
                           int32_t value3,
                           int32_t value4) -> void {
    glProgramUniform4i(Id, location, value1, value2, value3, value4);
}

auto Pipeline::SetUniform(int32_t location,
                           const glm::vec4& value) -> void {
    glProgramUniform4fv(Id, location, 1, glm::value_ptr(value));
}

auto Pipeline::SetUniform(int32_t location,
                           const glm::mat4& value) -> void {
    glProgramUniformMatrix4fv(Id, location, 1, GL_FALSE, glm::value_ptr(value));
}

auto GraphicsPipeline::Bind() -> void {

    if (g_defaultInputLayout == 0) {
        glCreateVertexArrays(1, &g_defaultInputLayout);
    }

    Pipeline::Bind();
    glBindVertexArray(InputLayout.value_or(g_defaultInputLayout));
}

auto GraphicsPipeline::BindBufferAsVertexBuffer(uint32_t buffer,
                                                 uint32_t bindingIndex,
                                                 long offset,
                                                 int32_t stride) -> void {

    if (InputLayout.has_value()) {
        glVertexArrayVertexBuffer(*InputLayout, bindingIndex, buffer, offset, stride);
    }
}

auto GraphicsPipeline::DrawArrays(int32_t vertexOffset,
                                  int32_t vertexCount) -> void {

    glDrawArrays(PrimitiveTopology, vertexOffset, vertexCount);
}

auto GraphicsPipeline::DrawElements(uint32_t indexBuffer,
                                    int32_t elementCount) -> void {

    if (g_lastIndexBuffer != indexBuffer) {
        glVertexArrayElementBuffer(InputLayout.has_value() ? InputLayout.value() : g_defaultInputLayout, indexBuffer);
        g_lastIndexBuffer = indexBuffer;
    }

    glDrawElements(PrimitiveTopology, elementCount, GL_UNSIGNED_INT, nullptr);
}

auto GraphicsPipeline::DrawElementsInstanced(uint32_t indexBuffer,
                                              int32_t elementCount,
                                              int32_t instanceCount) -> void {
    if (g_lastIndexBuffer != indexBuffer) {
        glVertexArrayElementBuffer(InputLayout.has_value() ? InputLayout.value() : g_defaultInputLayout, indexBuffer);
        g_lastIndexBuffer = indexBuffer;
    }

    glDrawElementsInstanced(PrimitiveTopology, elementCount, GL_UNSIGNED_INT, nullptr, instanceCount);
}

auto DeleteGraphicsPipeline(const SGraphicsPipelineId& graphicsPipelineId) -> void {
    auto& graphicsPipeline = GetGraphicsPipeline(graphicsPipelineId);
    glDeleteProgram(graphicsPipeline.Id);
}

auto DeleteComputePipeline(const SComputePipelineId& computePipelineId) -> void {
    auto& computePipeline = GetComputePipeline(computePipelineId);
    glDeleteProgram(computePipeline.Id);
}

auto ReadShaderTextFromFile(const std::filesystem::path& filePath) -> std::expected<std::string, std::string> {

    std::string pathStr = filePath.string();
    std::string parentPathStr = filePath.parent_path().string();
    char error[256]{};
    auto processedSource = std::unique_ptr<char, decltype([](char* ptr) { free(ptr); })>(
            stb_include_file(pathStr.data(), nullptr, parentPathStr.data(), error));
    if (!processedSource) {
        return std::unexpected(std::format("Failed to process includes for {}", filePath.string()));
    }

    return processedSource.get();
}

auto CreateGraphicsProgram(
        std::string_view label,
        std::string_view vertexShaderFilePath,
        std::string_view fragmentShaderFilePath) -> std::expected<uint32_t, std::string> {

    auto vertexShaderSourceResult = ReadShaderTextFromFile(vertexShaderFilePath);
    if (!vertexShaderSourceResult) {
        return std::unexpected(vertexShaderSourceResult.error());
    }

    auto fragmentShaderSourceResult = ReadShaderTextFromFile(fragmentShaderFilePath);
    if (!fragmentShaderSourceResult) {
        return std::unexpected(fragmentShaderSourceResult.error());
    }

    auto vertexShaderSource = *vertexShaderSourceResult;
    auto fragmentShaderSource = *fragmentShaderSourceResult;

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto vertexShaderSourcePtr = vertexShaderSource.data();
    glShaderSource(vertexShader, 1, &vertexShaderSourcePtr, nullptr);
    glCompileShader(vertexShader);
    int32_t status = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {

        int32_t infoLength = 512;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLength);
        auto infoLog = std::string(infoLength + 1, '\0');
        glGetShaderInfoLog(vertexShader, infoLength, nullptr, infoLog.data());
        glDeleteShader(vertexShader);

        return std::unexpected(std::format("Vertex shader in program {} has errors\n{}", label, infoLog));
    }

    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragmentShaderSourcePtr = fragmentShaderSource.data();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourcePtr, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {

        int32_t infoLength = 512;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLength);
        auto infoLog = std::string(infoLength + 1, '\0');
        glGetShaderInfoLog(fragmentShader, infoLength, nullptr, infoLog.data());
        glDeleteShader(fragmentShader);

        return std::unexpected(std::format("Fragment shader in program {} has errors\n{}", label, infoLog));
    }

    auto program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {

        int32_t infoLength = 512;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        auto infoLog = std::string(infoLength + 1, '\0');
        glGetProgramInfoLog(program, infoLength, nullptr, infoLog.data());

        return std::unexpected(std::format("Graphics program {} has linking errors\n{}", label, infoLog));
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

auto CreateComputeProgram(
        std::string_view label,
        std::string_view computeShaderFilePath) -> std::expected<uint32_t, std::string> {

    auto computeShaderSourceResult = ReadShaderTextFromFile(computeShaderFilePath);
    if (!computeShaderSourceResult) {
        return std::unexpected(computeShaderSourceResult.error());
    }

    int32_t status = 0;

    auto computeShader = glCreateShader(GL_COMPUTE_SHADER);
    auto computeShaderSourcePtr = (*computeShaderSourceResult).data();
    glShaderSource(computeShader, 1, &computeShaderSourcePtr, nullptr);
    glCompileShader(computeShader);
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {

        int32_t infoLength = 512;
        glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &infoLength);
        auto infoLog = std::string(infoLength + 1, '\0');
        glGetShaderInfoLog(computeShader, infoLength, nullptr, infoLog.data());
        glDeleteShader(computeShader);

        return std::unexpected(std::format("Compute shader {} has errors\n{}", label, infoLog));
    }

    auto program = glCreateProgram();
    glAttachShader(program, computeShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {

        int32_t infoLength = 512;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        auto infoLog = std::string(infoLength + 1, '\0');
        glGetProgramInfoLog(program, infoLength, nullptr, infoLog.data());

        return std::unexpected(std::format("Compute program {} has linking errors\n{}", label, infoLog));
    }

    glDetachShader(program, computeShader);
    glDeleteShader(computeShader);

    return program;
}

constexpr auto PrimitiveTopologyToGL(EPrimitiveTopology primitiveTopology) -> uint32_t {
    switch (primitiveTopology) {
        case EPrimitiveTopology::Lines:
            return GL_LINES;
        case EPrimitiveTopology::TriangleFan:
            return GL_TRIANGLE_FAN;
        case EPrimitiveTopology::Triangles:
            return GL_TRIANGLES;
        case EPrimitiveTopology::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        default:
            std::unreachable();
    }
}

auto CreateGraphicsPipeline(const SGraphicsPipelineDescriptor& graphicsPipelineDescriptor) -> std::expected<SGraphicsPipelineId, std::string> {

    const auto id = SGraphicsPipelineId(g_graphicsPipelines.size());
    auto& pipeline = g_graphicsPipelines.emplace_back();

    auto graphicsProgramResult = CreateGraphicsProgram(graphicsPipelineDescriptor.Label,
                                                       graphicsPipelineDescriptor.VertexShaderFilePath,
                                                       graphicsPipelineDescriptor.FragmentShaderFilePath);
    if (!graphicsProgramResult) {
        return std::unexpected(std::format("RHI: Unable to build GraphicsPipeline {}\n{}", graphicsPipelineDescriptor.Label,
                            graphicsProgramResult.error()));
    }

    pipeline.Id = *graphicsProgramResult;

    if (graphicsPipelineDescriptor.VertexInput.has_value()) {

        uint32_t inputLayout = 0;
        glCreateVertexArrays(1, &inputLayout);

        auto& vertexInput = *graphicsPipelineDescriptor.VertexInput;
        for (auto inputAttributeIndex = 0; auto& inputAttribute: vertexInput.VertexInputAttributes) {
            if (inputAttribute.has_value()) {
                auto& inputAttributeValue = *inputAttribute;

                glEnableVertexArrayAttrib(inputLayout, inputAttributeValue.Location);
                glVertexArrayAttribBinding(inputLayout, inputAttributeValue.Location, inputAttributeValue.Binding);

                auto type = FormatToUnderlyingOpenGLType(inputAttributeValue.Format);
                auto componentCount = FormatToComponentCount(inputAttributeValue.Format);
                auto isFormatNormalized = IsFormatNormalized(inputAttributeValue.Format);
                auto formatClass = FormatToFormatClass(inputAttributeValue.Format);
                switch (formatClass) {
                    case EFormatClass::Float:
                        glVertexArrayAttribFormat(inputLayout, inputAttributeValue.Location, componentCount, type,
                                                  isFormatNormalized, inputAttributeValue.Offset);
                        break;
                    case EFormatClass::Integer:
                        glVertexArrayAttribIFormat(inputLayout, inputAttributeValue.Location, componentCount, type,
                                                   inputAttributeValue.Offset);
                        break;
                    case EFormatClass::Long:
                        glVertexArrayAttribLFormat(inputLayout, inputAttributeValue.Location, componentCount, type,
                                                   inputAttributeValue.Offset);
                        break;
                    default:
                        std::string message = "Unsupported Format Class";
                        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0,
                                             GL_DEBUG_SEVERITY_HIGH, message.size(), message.data());
                        break;
                }
            }

            inputAttributeIndex++;
        }

        SetDebugLabel(inputLayout, GL_VERTEX_ARRAY, std::format("InputLayout-{}", graphicsPipelineDescriptor.Label));

        pipeline.InputLayout = inputLayout;
    } else {
        pipeline.InputLayout = std::nullopt;
    }

    pipeline.PrimitiveTopology = PrimitiveTopologyToGL(graphicsPipelineDescriptor.InputAssembly.PrimitiveTopology);
    pipeline.IsPrimitiveRestartEnabled = graphicsPipelineDescriptor.InputAssembly.IsPrimitiveRestartEnabled;

    return id;
}

auto CreateComputePipeline(const SComputePipelineDescriptor& computePipelineDescriptor) -> std::expected<SComputePipelineId, std::string> {

    const auto id = SComputePipelineId(g_graphicsPipelines.size());
    auto& pipeline = g_computePipelines.emplace_back();

    auto computeProgramResult = CreateComputeProgram(computePipelineDescriptor.Label,
                                                     computePipelineDescriptor.ComputeShaderFilePath);
    if (!computeProgramResult) {
        return std::unexpected(
                std::format("RHI: Unable to build ComputePipeline {}\n{}", computePipelineDescriptor.Label,
                            computeProgramResult.error()));
    }

    pipeline.Id = *computeProgramResult;

    return id;
}