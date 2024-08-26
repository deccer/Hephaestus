#pragma once

#include <cstdint>

struct TOffset2D {

uint32_t X{};
uint32_t Y{};

bool operator==(const TOffset2D&) const noexcept = default;

constexpr TOffset2D operator+(const TOffset2D& other) const { return { X + other.X, Y + other.Y }; }

constexpr TOffset2D operator-(const TOffset2D& other) const { return { X - other.X, Y - other.Y }; }

constexpr TOffset2D operator*(const TOffset2D& other) const { return { X * other.X, Y * other.Y }; }

constexpr TOffset2D operator/(const TOffset2D& other) const { return { X / other.X, Y / other.Y }; }

constexpr TOffset2D operator>>(const TOffset2D& other) const { return { X >> other.X, Y >> other.Y }; }

constexpr TOffset2D operator<<(const TOffset2D& other) const { return { X << other.X, Y << other.Y }; }

constexpr TOffset2D operator+(uint32_t value) const { return *this + TOffset2D{ value, value }; }

constexpr TOffset2D operator-(uint32_t value) const { return *this - TOffset2D{ value, value }; }

constexpr TOffset2D operator*(uint32_t value) const { return *this * TOffset2D{ value, value }; }

constexpr TOffset2D operator/(uint32_t value) const { return *this / TOffset2D{ value, value }; }

constexpr TOffset2D operator>>(uint32_t value) const { return *this >> TOffset2D{ value, value }; }

constexpr TOffset2D operator<<(uint32_t value) const { return *this << TOffset2D{ value, value }; }
};

constexpr TOffset2D operator+(uint32_t value, TOffset2D offset) {
    return offset + value;
}

constexpr TOffset2D operator-(uint32_t value, TOffset2D offset) {
    return offset - value;
}

constexpr TOffset2D operator*(uint32_t value, TOffset2D offset) {
    return offset * value;
}

constexpr TOffset2D operator/(uint32_t value, TOffset2D offset) {
    return offset / value;
}

constexpr TOffset2D operator>>(uint32_t value, TOffset2D offset) {
    return offset >> value;
}

constexpr TOffset2D operator<<(uint32_t value, TOffset2D offset) {
    return offset << value;
}

struct TOffset3D {

    uint32_t X{};
    uint32_t Y{};
    uint32_t Z{};

    operator TOffset2D() const { return { X, Y }; }

    bool operator==(const TOffset3D&) const noexcept = default;

    constexpr TOffset3D operator+(const TOffset3D& other) const { return { X + other.X, Y + other.Y, Z + other.Z }; }

    constexpr TOffset3D operator-(const TOffset3D& other) const { return { X - other.X, Y - other.Y, Z - other.Z }; }

    constexpr TOffset3D operator*(const TOffset3D& other) const { return { X * other.X, Y * other.Y, Z * other.Z }; }

    constexpr TOffset3D operator/(const TOffset3D& other) const { return { X / other.X, Y / other.Y, Z / other.Z }; }

    constexpr TOffset3D operator>>(const TOffset3D& other) const { return { X >> other.X, Y >> other.Y, Z >> other.Z }; }

    constexpr TOffset3D operator<<(const TOffset3D& other) const { return { X << other.X, Y << other.Y, Z << other.Z }; }

    constexpr TOffset3D operator+(uint32_t value) const { return *this + TOffset3D{ value, value, value }; }

    constexpr TOffset3D operator-(uint32_t value) const { return *this - TOffset3D{ value, value, value }; }

    constexpr TOffset3D operator*(uint32_t value) const { return *this * TOffset3D{ value, value, value }; }

    constexpr TOffset3D operator/(uint32_t value) const { return *this / TOffset3D{ value, value, value }; }

    constexpr TOffset3D operator>>(uint32_t value) const { return *this >> TOffset3D{ value, value, value }; }

    constexpr TOffset3D operator<<(uint32_t value) const { return *this << TOffset3D{ value, value, value }; }
};

constexpr TOffset3D operator+(uint32_t value, TOffset3D offset) {
    return offset + value;
}

constexpr TOffset3D operator-(uint32_t value, TOffset3D offset) {
    return offset - value;
}

constexpr TOffset3D operator*(uint32_t value, TOffset3D offset) {
    return offset * value;
}

constexpr TOffset3D operator/(uint32_t value, TOffset3D offset) {
    return offset / value;
}

constexpr TOffset3D operator>>(uint32_t value, TOffset3D offset) {
    return offset >> value;
}

constexpr TOffset3D operator<<(uint32_t value, TOffset3D offset) {
    return offset << value;
}