#include <Hephaestus/Game.hpp>

auto main([[maybe_unused]] int32_t argc,
          [[maybe_unused]] char* argv[]) -> int32_t {

    RunGame({
        .ResolutionWidth = 1920,
        .ResolutionHeight = 1080,
        .ResolutionScale = 1.0f,
        .WindowStyle = EWindowStyle::Windowed,
        .IsDebug = true,
        .IsVSyncEnabled = true,
    });

    return 0;
}
