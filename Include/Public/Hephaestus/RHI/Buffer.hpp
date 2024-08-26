#pragma once

#include <cstdint>
#include <string_view>

enum class TBufferType {
    UniformBuffer,
    ShaderStorageBuffer
};

auto BindBufferAs(uint32_t buffer,
                  TBufferType bufferType,
                  int32_t bindingIndex) -> void;

auto CreateBuffer(std::string_view label,
                  int64_t sizeInBytes,
                  const void* data,
                  uint32_t flags) -> uint32_t;

auto UpdateBuffer(uint32_t buffer,
                  int64_t offsetInBytes,
                  int64_t sizeInBytes,
                  const void* data) -> void;

auto DeleteBuffer(uint32_t buffer) -> void;