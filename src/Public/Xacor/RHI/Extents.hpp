#pragma once

#include <cstdint>

struct SExtent2D {
    uint32_t Width{};
    uint32_t Height{};

    bool operator==(const SExtent2D&) const noexcept = default;

    constexpr SExtent2D operator+(const SExtent2D& other) const { return { Width + other.Width, Height + other.Height }; }

    constexpr SExtent2D operator-(const SExtent2D& other) const { return { Width - other.Width, Height - other.Height }; }

    constexpr SExtent2D operator*(const SExtent2D& other) const { return { Width * other.Width, Height * other.Height }; }

    constexpr SExtent2D operator/(const SExtent2D& other) const { return { Width / other.Width, Height / other.Height }; }

    constexpr SExtent2D operator>>(const SExtent2D& other) const {
        return { Width >> other.Width, Height >> other.Height };
    }

    constexpr SExtent2D operator<<(const SExtent2D& other) const {
        return { Width << other.Width, Height << other.Height };
    }

    constexpr SExtent2D operator+(uint32_t value) const { return *this + SExtent2D{ value, value }; }

    constexpr SExtent2D operator-(uint32_t value) const { return *this - SExtent2D{ value, value }; }

    constexpr SExtent2D operator*(uint32_t value) const { return *this * SExtent2D{ value, value }; }

    constexpr SExtent2D operator/(uint32_t value) const { return *this / SExtent2D{ value, value }; }

    constexpr SExtent2D operator>>(uint32_t value) const { return *this >> SExtent2D{ value, value }; }

    constexpr SExtent2D operator<<(uint32_t value) const { return *this << SExtent2D{ value, value }; }
};

constexpr SExtent2D operator+(uint32_t value, SExtent2D extent) {
    return extent + value;
}

constexpr SExtent2D operator-(uint32_t value, SExtent2D extent) {
    return extent - value;
}

constexpr SExtent2D operator*(uint32_t value, SExtent2D extent) {
    return extent * value;
}

constexpr SExtent2D operator/(uint32_t value, SExtent2D extent) {
    return extent / value;
}

constexpr SExtent2D operator>>(uint32_t value, SExtent2D extent) {
    return extent >> value;
}

constexpr SExtent2D operator<<(uint32_t value, SExtent2D extent) {
    return extent << value;
}

struct SExtent3D {
    uint32_t Width{};
    uint32_t Height{};
    uint32_t Depth{};

    operator SExtent2D() const { return { Width, Height }; }

    bool operator==(const SExtent3D&) const noexcept = default;

    constexpr SExtent3D operator+(const SExtent3D& other) const {
        return { Width + other.Width, Height + other.Height, Depth + other.Depth };
    }

    constexpr SExtent3D operator-(const SExtent3D& other) const {
        return { Width - other.Width, Height - other.Height, Depth - other.Depth };
    }

    constexpr SExtent3D operator*(const SExtent3D& other) const {
        return { Width * other.Width, Height * other.Height, Depth * other.Depth };
    }

    constexpr SExtent3D operator/(const SExtent3D& other) const {
        return { Width / other.Width, Height / other.Height, Depth / other.Depth };
    }

    constexpr SExtent3D operator>>(const SExtent3D& other) const {
        return { Width >> other.Width, Height >> other.Height, Depth >> other.Depth };
    }

    constexpr SExtent3D operator<<(const SExtent3D& other) const {
        return { Width << other.Width, Height << other.Height, Depth << other.Depth };
    }

    constexpr SExtent3D operator+(uint32_t value) const { return *this + SExtent3D{ value, value, value }; }

    constexpr SExtent3D operator-(uint32_t value) const { return *this - SExtent3D{ value, value, value }; }

    constexpr SExtent3D operator*(uint32_t value) const { return *this * SExtent3D{ value, value, value }; }

    constexpr SExtent3D operator/(uint32_t value) const { return *this / SExtent3D{ value, value, value }; }

    constexpr SExtent3D operator>>(uint32_t value) const { return *this >> SExtent3D{ value, value, value }; }

    constexpr SExtent3D operator<<(uint32_t value) const { return *this << SExtent3D{ value, value, value }; }
};

constexpr SExtent3D operator+(uint32_t value, SExtent3D extent) {
    return extent + value;
}

constexpr SExtent3D operator-(uint32_t value, SExtent3D extent) {
    return extent - value;
}

constexpr SExtent3D operator*(uint32_t value, SExtent3D extent) {
    return extent * value;
}

constexpr SExtent3D operator/(uint32_t value, SExtent3D extent) {
    return extent / value;
}

constexpr SExtent3D operator>>(uint32_t value, SExtent3D extent) {
    return extent >> value;
}

constexpr SExtent3D operator<<(uint32_t value, SExtent3D extent) {
    return extent << value;
}