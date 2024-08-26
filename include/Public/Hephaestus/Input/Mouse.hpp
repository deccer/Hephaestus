#pragma once

#include <array>
#include <cstdint>
#include <glm/vec2.hpp>

enum class TMouseButton {
    None = 0,
    Left,
    Right,
    Middle,
    ButtonCount
};

class TApplication;
class TApplicationAccess;

class TMouse {
public:
    static auto IsButtonPressed(TMouseButton button) -> bool;
    static auto GetDeltaCursorPosition() -> glm::vec2 const;

private:
    friend class TApplication;
    friend class TApplicationAccess;

    static auto SetButton(TMouseButton button, bool isPressed) -> void;
    static auto SetDeltaCursorPosition(float deltaX, float deltaY) -> void;

    static std::array<bool, static_cast<std::size_t>(TMouseButton::ButtonCount)> _buttons;
    static glm::ivec2 _deltaCursorPosition;
};
