#pragma once

#include <array>

#include <glm/vec2.hpp>
#include <spdlog/spdlog.h>

enum class EKey {
    KeyNone = 0,
    KeyEscape,
    KeyF1,
    KeyF2,
    KeyF3,
    KeyF4,
    KeyF5,
    KeyF6,
    KeyF7,
    KeyF8,
    KeyF9,
    KeyF10,
    KeyF11,
    KeyF12,
    KeyPrint,
    KeyScrollLock,
    KeyPause,

    KeyGraveAccent,
    Key1,
    Key2,
    Key3,
    Key4,
    Key5,
    Key6,
    Key7,
    Key8,
    Key9,
    Key0,
    KeyMinus,
    KeyEqual,
    KeyBackspace,
    KeyTab,
    KeyQ,
    KeyW,
    KeyE,
    KeyR,
    KeyT,
    KeyY,
    KeyU,
    KeyI,
    KeyO,
    KeyP,
    KeyLeftBracket,
    KeyRightBracket,
    KeyBackslash,
    KeyCapsLock,
    KeyA,
    KeyS,
    KeyD,
    KeyF,
    KeyG,
    KeyH,
    KeyJ,
    KeyK,
    KeyL,
    KeySemicolon,
    KeyApostrophe,
    KeyEnter,
    KeyLeftShift,
    KeyZ,
    KeyX,
    KeyC,
    KeyV,
    KeyB,
    KeyN,
    KeyM,
    KeyComma,
    KeyPeriod,
    KeySlash,
    KeyRightShift,
    KeyLeftControl,
    KeyLeftMeta,
    KeyLeftAlt,
    KeySpace,
    KeyRightAlt,
    KeyRightMeta,
    KeyFn,
    KeyRightControl,
    KeyCount
};

enum class EMouseButton {
    None = 0,
    Left,
    Right,
    Middle,
    ButtonCount
};

enum class EGamepadButton {
    None = 0,
    ButtonA,
    ButtonB,
    ButtonX,
    ButtonY,
    ButtonBack,
    ButtonCircle,
    ButtonCross,
    ButtonStart
};

struct SMouseState {
public:
    auto SetButton(EMouseButton button, bool isPressed) -> void {
        auto index = static_cast<size_t>(button);
        if (index >= 0 && index < _buttons.size()) {
            _buttons[index] = isPressed;
        }
    }

    auto SetDeltaCursorPosition(float deltaX, float deltaY) -> void {
        _deltaCursorPosition = {deltaX, deltaY};
    }

    auto GetDeltaCursorPosition() -> glm::vec2 const {
        return _deltaCursorPosition;
    };

    [[nodiscard]] auto IsButtonPressed(EMouseButton button) const -> bool {
        int index = static_cast<int>(button);
        if (index >= 0 && index < _buttons.size()) {
            return _buttons[index];
        }
        return false;
    }
private:
    std::array<bool, static_cast<size_t>(EMouseButton::ButtonCount)> _buttons = {};
    glm::vec2 _deltaCursorPosition = {0, 0};
};

struct SKeyboardState {
public:
    auto SetKey(EKey key, bool pressed) -> void {
        auto keyIndex = static_cast<size_t>(key);
        if (keyIndex < _keys.size()) {
            _keys[keyIndex] = pressed;
        }
    }

    [[nodiscard]] auto IsKeyPressed(EKey key) const -> bool {
        auto keyIndex = static_cast<size_t>(key);
        return keyIndex < _keys.size() && _keys[keyIndex];
    }
private:
    std::array<bool, static_cast<size_t>(EKey::KeyCount)> _keys;
};

struct SInputState {
    SMouseState Mouse = {};
    SKeyboardState Keyboard = {};
};

auto GetInputState() -> SInputState&;
