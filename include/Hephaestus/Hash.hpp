#pragma once

#include <cstdint>
#include <string_view>

constexpr auto HashString(std::string_view str) {
    auto hash = 2166136261u;
    for (auto ch: str) {
        hash ^= ch;
        hash *= 16777619u;
    }
    return hash;
}

constexpr uint32_t operator"" _hash(const char* str, size_t) {
    return HashString(str);
}