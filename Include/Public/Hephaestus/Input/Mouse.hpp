#pragma once

#include <array>
#include <cstdint>
#include <glm/vec2.hpp>

enum class EMouseButton {
    None = 0,
    Left,
    Right,
    Middle,
    ButtonCount
};

class Application;
class ApplicationAccess;

class Mouse {
public:
    static auto IsButtonPressed(EMouseButton button) -> bool;
    static auto GetDeltaCursorPosition() -> glm::vec2 const;

private:
    friend class Application;
    friend class ApplicationAccess;

    static auto SetButton(EMouseButton button, bool isPressed) -> void;
    static auto SetDeltaCursorPosition(float deltaX, float deltaY) -> void;

    static std::array<bool, static_cast<std::size_t>(EMouseButton::ButtonCount)> _buttons;
    static glm::ivec2 _deltaCursorPosition;
};
