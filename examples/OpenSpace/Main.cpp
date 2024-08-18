#include <Hephaestus/Application.hpp>
#include <Hephaestus/ApplicationContext.hpp>

#include <cstdint>

class MyRenderer : public IRenderer {
public:
    ~MyRenderer() override {

    }

    auto Load() -> bool override {
        return true;
    }

    auto Unload() -> void override {

    }

    auto Render(SRenderContext& renderContext,
                IScene& scene) -> void override {

    }
};

class MyScene : public Scene {
public:

};

auto main(
    [[maybe_unused]] int32_t argc,
    [[maybe_unused]] char* argv[]) -> int32_t {

    //auto myRenderer = std::make_unique<MyRenderer>();
    Application application({
        .Settings = {
            .ResolutionWidth = 1920,
            .ResolutionHeight = 1080,
            .ResolutionScale = 1.0f,
            .ResolutionStartup = EResolutionStartup::NinetyPercentOfScreenSize,
            .WindowStyle = EWindowStyle::Windowed,
            .IsDebug = true,
            .IsVSyncEnabled = true,
        },
        //.Renderer = myRenderer.release(),
    });

    application.Run();

    return 0;
}