#pragma once

#include <cstdint>

template<class TTag>
struct SIdImpl {
    enum class Id : std::size_t { Invalid = SIZE_MAX };

    bool operator==(const SIdImpl&) const noexcept = default;
};

template<class TTag>
using SId = typename SIdImpl<TTag>::Id;

template<class TTag>
struct SIdGenerator {
public:
    auto Next() -> SId<TTag> {
        _counter += 1;
        return { _counter };
    }
private:
    size_t _counter = 0;
};

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