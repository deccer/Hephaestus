#pragma once

#include <cstdint>

struct TExtent2D {
    uint32_t Width{};
    uint32_t Height{};

    bool operator==(const TExtent2D&) const noexcept = default;

    constexpr TExtent2D operator+(const TExtent2D& other) const { return { Width + other.Width, Height + other.Height }; }

    constexpr TExtent2D operator-(const TExtent2D& other) const { return { Width - other.Width, Height - other.Height }; }

    constexpr TExtent2D operator*(const TExtent2D& other) const { return { Width * other.Width, Height * other.Height }; }

    constexpr TExtent2D operator/(const TExtent2D& other) const { return { Width / other.Width, Height / other.Height }; }

    constexpr TExtent2D operator>>(const TExtent2D& other) const {
        return { Width >> other.Width, Height >> other.Height };
    }

    constexpr TExtent2D operator<<(const TExtent2D& other) const {
        return { Width << other.Width, Height << other.Height };
    }

    constexpr TExtent2D operator+(uint32_t value) const { return *this + TExtent2D{ value, value }; }

    constexpr TExtent2D operator-(uint32_t value) const { return *this - TExtent2D{ value, value }; }

    constexpr TExtent2D operator*(uint32_t value) const { return *this * TExtent2D{ value, value }; }

    constexpr TExtent2D operator/(uint32_t value) const { return *this / TExtent2D{ value, value }; }

    constexpr TExtent2D operator>>(uint32_t value) const { return *this >> TExtent2D{ value, value }; }

    constexpr TExtent2D operator<<(uint32_t value) const { return *this << TExtent2D{ value, value }; }
};

constexpr TExtent2D operator+(uint32_t value, TExtent2D extent) {
    return extent + value;
}

constexpr TExtent2D operator-(uint32_t value, TExtent2D extent) {
    return extent - value;
}

constexpr TExtent2D operator*(uint32_t value, TExtent2D extent) {
    return extent * value;
}

constexpr TExtent2D operator/(uint32_t value, TExtent2D extent) {
    return extent / value;
}

constexpr TExtent2D operator>>(uint32_t value, TExtent2D extent) {
    return extent >> value;
}

constexpr TExtent2D operator<<(uint32_t value, TExtent2D extent) {
    return extent << value;
}

struct TExtent3D {
    uint32_t Width{};
    uint32_t Height{};
    uint32_t Depth{};

    operator TExtent2D() const { return { Width, Height }; }

    bool operator==(const TExtent3D&) const noexcept = default;

    constexpr TExtent3D operator+(const TExtent3D& other) const {
        return { Width + other.Width, Height + other.Height, Depth + other.Depth };
    }

    constexpr TExtent3D operator-(const TExtent3D& other) const {
        return { Width - other.Width, Height - other.Height, Depth - other.Depth };
    }

    constexpr TExtent3D operator*(const TExtent3D& other) const {
        return { Width * other.Width, Height * other.Height, Depth * other.Depth };
    }

    constexpr TExtent3D operator/(const TExtent3D& other) const {
        return { Width / other.Width, Height / other.Height, Depth / other.Depth };
    }

    constexpr TExtent3D operator>>(const TExtent3D& other) const {
        return { Width >> other.Width, Height >> other.Height, Depth >> other.Depth };
    }

    constexpr TExtent3D operator<<(const TExtent3D& other) const {
        return { Width << other.Width, Height << other.Height, Depth << other.Depth };
    }

    constexpr TExtent3D operator+(uint32_t value) const { return *this + TExtent3D{ value, value, value }; }

    constexpr TExtent3D operator-(uint32_t value) const { return *this - TExtent3D{ value, value, value }; }

    constexpr TExtent3D operator*(uint32_t value) const { return *this * TExtent3D{ value, value, value }; }

    constexpr TExtent3D operator/(uint32_t value) const { return *this / TExtent3D{ value, value, value }; }

    constexpr TExtent3D operator>>(uint32_t value) const { return *this >> TExtent3D{ value, value, value }; }

    constexpr TExtent3D operator<<(uint32_t value) const { return *this << TExtent3D{ value, value, value }; }
};

constexpr TExtent3D operator+(uint32_t value, TExtent3D extent) {
    return extent + value;
}

constexpr TExtent3D operator-(uint32_t value, TExtent3D extent) {
    return extent - value;
}

constexpr TExtent3D operator*(uint32_t value, TExtent3D extent) {
    return extent * value;
}

constexpr TExtent3D operator/(uint32_t value, TExtent3D extent) {
    return extent / value;
}

constexpr TExtent3D operator>>(uint32_t value, TExtent3D extent) {
    return extent >> value;
}

constexpr TExtent3D operator<<(uint32_t value, TExtent3D extent) {
    return extent << value;
}