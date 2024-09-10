#include <Hephaestus/Application.hpp>
#include <Hephaestus/ApplicationContext.hpp>

#include <cstdint>

class TMyRenderer : public TRenderer {
public:
    ~TMyRenderer() override {

    }

    auto Load() -> bool override {

        return true;
    }

    auto Unload() -> void override {

    }

    auto Render(TRenderContext& renderContext,
                TScene& scene) -> void override {

    }
};

class TMyScene : public TScene {
public:

};

auto main(
    [[maybe_unused]] int32_t argc,
    [[maybe_unused]] char* argv[]) -> int32_t {

    //auto myRenderer = std::make_unique<TMyRenderer>();
    TApplication application({
        .Settings = {
            .ResolutionWidth = 1920,
            .ResolutionHeight = 1080,
            .ResolutionScale = 1.0f,
            .ResolutionStartup = TResolutionStartup::NinetyPercentOfScreenSize,
            .WindowStyle = TWindowStyle::Windowed,
            .IsDebug = true,
            .IsVSyncEnabled = true,
        },
        //.Renderer = myRenderer.release(),
    });

    application.Run();

    return 0;
}