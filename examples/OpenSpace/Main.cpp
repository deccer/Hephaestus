#include <Xacor/Application.hpp>

#include <cstdint>

auto main(
    [[maybe_unused]] int32_t argc,
    [[maybe_unused]] char* argv[]) -> int32_t {

    Application application({
        .Settings = {
            .ResolutionWidth = 1920,
            .ResolutionHeight = 1080,
            .ResolutionScale = 1.0f,
            .ResolutionStartup = EResolutionStartup::NinetyPercentOfScreenSize,
            .WindowStyle = EWindowStyle::Windowed,
            .IsDebug = true,
            .IsVSyncEnabled = true,
        }
    });

    application.Run();

    return 0;
}