#pragma once

#include <cstdint>
#include <string_view>

auto SetDebugLabel(
    const uint32_t object,
    const uint32_t objectType,
    const std::string_view label) -> void;

auto PushDebugGroup(const std::string_view label) -> void;

auto PopDebugGroup() -> void;