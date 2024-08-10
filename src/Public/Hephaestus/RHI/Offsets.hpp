#pragma once

#include <cstdint>

struct SOffset2D {

uint32_t X{};
uint32_t Y{};

bool operator==(const SOffset2D&) const noexcept = default;

constexpr SOffset2D operator+(const SOffset2D& other) const { return { X + other.X, Y + other.Y }; }

constexpr SOffset2D operator-(const SOffset2D& other) const { return { X - other.X, Y - other.Y }; }

constexpr SOffset2D operator*(const SOffset2D& other) const { return { X * other.X, Y * other.Y }; }

constexpr SOffset2D operator/(const SOffset2D& other) const { return { X / other.X, Y / other.Y }; }

constexpr SOffset2D operator>>(const SOffset2D& other) const { return { X >> other.X, Y >> other.Y }; }

constexpr SOffset2D operator<<(const SOffset2D& other) const { return { X << other.X, Y << other.Y }; }

constexpr SOffset2D operator+(uint32_t value) const { return *this + SOffset2D{ value, value }; }

constexpr SOffset2D operator-(uint32_t value) const { return *this - SOffset2D{ value, value }; }

constexpr SOffset2D operator*(uint32_t value) const { return *this * SOffset2D{ value, value }; }

constexpr SOffset2D operator/(uint32_t value) const { return *this / SOffset2D{ value, value }; }

constexpr SOffset2D operator>>(uint32_t value) const { return *this >> SOffset2D{ value, value }; }

constexpr SOffset2D operator<<(uint32_t value) const { return *this << SOffset2D{ value, value }; }
};

constexpr SOffset2D operator+(uint32_t value, SOffset2D offset) {
    return offset + value;
}

constexpr SOffset2D operator-(uint32_t value, SOffset2D offset) {
    return offset - value;
}

constexpr SOffset2D operator*(uint32_t value, SOffset2D offset) {
    return offset * value;
}

constexpr SOffset2D operator/(uint32_t value, SOffset2D offset) {
    return offset / value;
}

constexpr SOffset2D operator>>(uint32_t value, SOffset2D offset) {
    return offset >> value;
}

constexpr SOffset2D operator<<(uint32_t value, SOffset2D offset) {
    return offset << value;
}

struct SOffset3D {

    uint32_t X{};
    uint32_t Y{};
    uint32_t Z{};

    operator SOffset2D() const { return { X, Y }; }

    bool operator==(const SOffset3D&) const noexcept = default;

    constexpr SOffset3D operator+(const SOffset3D& other) const { return { X + other.X, Y + other.Y, Z + other.Z }; }

    constexpr SOffset3D operator-(const SOffset3D& other) const { return { X - other.X, Y - other.Y, Z - other.Z }; }

    constexpr SOffset3D operator*(const SOffset3D& other) const { return { X * other.X, Y * other.Y, Z * other.Z }; }

    constexpr SOffset3D operator/(const SOffset3D& other) const { return { X / other.X, Y / other.Y, Z / other.Z }; }

    constexpr SOffset3D operator>>(const SOffset3D& other) const { return { X >> other.X, Y >> other.Y, Z >> other.Z }; }

    constexpr SOffset3D operator<<(const SOffset3D& other) const { return { X << other.X, Y << other.Y, Z << other.Z }; }

    constexpr SOffset3D operator+(uint32_t value) const { return *this + SOffset3D{ value, value, value }; }

    constexpr SOffset3D operator-(uint32_t value) const { return *this - SOffset3D{ value, value, value }; }

    constexpr SOffset3D operator*(uint32_t value) const { return *this * SOffset3D{ value, value, value }; }

    constexpr SOffset3D operator/(uint32_t value) const { return *this / SOffset3D{ value, value, value }; }

    constexpr SOffset3D operator>>(uint32_t value) const { return *this >> SOffset3D{ value, value, value }; }

    constexpr SOffset3D operator<<(uint32_t value) const { return *this << SOffset3D{ value, value, value }; }
};

constexpr SOffset3D operator+(uint32_t value, SOffset3D offset) {
    return offset + value;
}

constexpr SOffset3D operator-(uint32_t value, SOffset3D offset) {
    return offset - value;
}

constexpr SOffset3D operator*(uint32_t value, SOffset3D offset) {
    return offset * value;
}

constexpr SOffset3D operator/(uint32_t value, SOffset3D offset) {
    return offset / value;
}

constexpr SOffset3D operator>>(uint32_t value, SOffset3D offset) {
    return offset >> value;
}

constexpr SOffset3D operator<<(uint32_t value, SOffset3D offset) {
    return offset << value;
}