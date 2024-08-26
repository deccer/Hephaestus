#pragma once

#include <array>
#include <cstdint>

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

class ApplicationAccess;

class Keyboard {
public:
    static auto IsKeyPressed(EKey key) -> bool;

private:
    friend class ApplicationAccess;

    static auto SetKey(EKey key, bool pressed) -> void;

    static std::array<bool, static_cast<std::size_t>(EKey::KeyCount)> _keys;
};