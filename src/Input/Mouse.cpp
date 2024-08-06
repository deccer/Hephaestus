#include <Xacor/Input/Mouse.hpp>

std::array<bool, static_cast<std::size_t>(EMouseButton::ButtonCount)> Mouse::_buttons = {};
glm::ivec2 Mouse::_deltaCursorPosition = {};

auto Mouse::IsButtonPressed(EMouseButton button) -> bool {
    int index = static_cast<int>(button);
    if (index >= 0 && index < _buttons.size()) {
        return _buttons[index];
    }
    return false;
}

auto Mouse::SetDeltaCursorPosition(float deltaX, float deltaY) -> void {
    _deltaCursorPosition = {deltaX, deltaY};
}

auto Mouse::GetDeltaCursorPosition() -> glm::vec2 const {
    return _deltaCursorPosition;
};

auto Mouse::SetButton(EMouseButton button, bool isPressed) -> void {
    auto index = static_cast<std::size_t>(button);
    if (index >= 0 && index < _buttons.size()) {
        _buttons[index] = isPressed;
    }
}

