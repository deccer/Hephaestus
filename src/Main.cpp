#include <Hephaestus/Game.hpp>

auto main([[maybe_unused]] int32_t argc,
          [[maybe_unused]] char* argv[]) -> int32_t {

    RunGame({
        .ResolutionWidth = 1920,
        .ResolutionHeight = 720,
        .ResolutionScale = 1.0f,
        .ResolutionStartup = EResolutionStartup::UseApplicationSettings,
        .WindowStyle = EWindowStyle::Windowed,
        .IsDebug = true,
        .IsVSyncEnabled = true,
    });

    return 0;
}
