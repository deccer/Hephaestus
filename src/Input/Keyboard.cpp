#include <Hephaestus/Input/Keyboard.hpp>

std::array<bool, static_cast<std::size_t>(EKey::KeyCount)> Keyboard::_keys = {};

auto Keyboard::IsKeyPressed(EKey key) -> bool {
    auto keyIndex = static_cast<std::size_t>(key);
    return keyIndex < _keys.size() && _keys[keyIndex];
}

auto Keyboard::SetKey(EKey key, bool pressed) -> void {
    auto keyIndex = static_cast<std::size_t>(key);
    if (keyIndex < _keys.size()) {
        _keys[keyIndex] = pressed;
    }
}