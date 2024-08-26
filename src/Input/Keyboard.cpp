#include <Hephaestus/Input/Keyboard.hpp>

std::array<bool, static_cast<std::size_t>(TKey::KeyCount)> TKeyboard::_keys = {};

auto TKeyboard::IsKeyPressed(TKey key) -> bool {
    auto keyIndex = static_cast<std::size_t>(key);
    return keyIndex < _keys.size() && _keys[keyIndex];
}

auto TKeyboard::SetKey(TKey key, bool pressed) -> void {
    auto keyIndex = static_cast<std::size_t>(key);
    if (keyIndex < _keys.size()) {
        _keys[keyIndex] = pressed;
    }
}