#include <Hephaestus/RHI/Buffer.hpp>
#include <Hephaestus/RHI/Debug.hpp>

#include <glad/gl.h>

#include <utility>

constexpr uint32_t BufferTypeToGL(EBufferType bufferType) {
    switch (bufferType) {
        case EBufferType::UniformBuffer: return GL_UNIFORM_BUFFER;
        case EBufferType::ShaderStorageBuffer: return GL_SHADER_STORAGE_BUFFER;
        default: std::unreachable();
    }
}

auto BindBufferAs(uint32_t buffer,
                  EBufferType bufferType,
                  int32_t bindingIndex) -> void {
    glBindBufferBase(BufferTypeToGL(bufferType), bindingIndex, buffer);
}

auto CreateBuffer(std::string_view label,
                  int64_t sizeInBytes,
                  const void* data,
                  uint32_t flags) -> uint32_t {

    uint32_t buffer = 0;
    glCreateBuffers(1, &buffer);
    SetDebugLabel(buffer, GL_BUFFER, label);
    glNamedBufferStorage(buffer, sizeInBytes, data, flags);
    return buffer;
}

auto UpdateBuffer(uint32_t buffer,
                  int64_t offsetInBytes,
                  int64_t sizeInBytes,
                  const void* data) -> void {

    glNamedBufferSubData(buffer, offsetInBytes, sizeInBytes, data);
}

auto DeleteBuffer(uint32_t buffer) -> void {

    glDeleteBuffers(1, &buffer);
}