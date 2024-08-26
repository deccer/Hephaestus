#include <Hephaestus/Input/Mouse.hpp>

std::array<bool, static_cast<std::size_t>(TMouseButton::ButtonCount)> TMouse::_buttons = {};
glm::ivec2 TMouse::_deltaCursorPosition = {};

auto TMouse::IsButtonPressed(TMouseButton button) -> bool {
    int index = static_cast<int>(button);
    if (index >= 0 && index < _buttons.size()) {
        return _buttons[index];
    }
    return false;
}

auto TMouse::SetDeltaCursorPosition(float deltaX, float deltaY) -> void {
    _deltaCursorPosition = {deltaX, deltaY};
}

auto TMouse::GetDeltaCursorPosition() -> glm::vec2 const {
    return _deltaCursorPosition;
};

auto TMouse::SetButton(TMouseButton button, bool isPressed) -> void {
    auto index = static_cast<std::size_t>(button);
    if (index >= 0 && index < _buttons.size()) {
        _buttons[index] = isPressed;
    }
}

