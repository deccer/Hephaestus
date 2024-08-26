add_library(Hephaestus STATIC
    RHI/Debug.cpp
    RHI/Buffer.cpp
    RHI/Texture.cpp
    RHI/Framebuffer.cpp
    RHI/Pipelines.cpp
    Scene.cpp
    Assets/Assets.cpp

    DefaultRenderer.cpp
    DefaultScene.cpp

    Input/Mouse.cpp
    Input/Keyboard.cpp
    Application.cpp
)
target_include_directories(Hephaestus
    PUBLIC ../include/Public
    PRIVATE ../include/Private
)
target_link_libraries(Hephaestus
    PRIVATE glfw
    PRIVATE glad
    PRIVATE glm
    PRIVATE imgui
    PRIVATE debugbreak
    PRIVATE spdlog::spdlog_header_only
    PRIVATE stb
    PRIVATE EnTT::EnTT
    PRIVATE phmap
    PRIVATE fastgltf
)