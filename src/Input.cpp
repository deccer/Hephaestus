#include <Hephaestus/Input.hpp>

SInputState g_inputState = {};

auto GetInputState() -> SInputState& {
    return g_inputState;
}