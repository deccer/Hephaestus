add_library(Hephaestus STATIC
    RHI/Debug.cpp
    RHI/Buffer.cpp
    RHI/Texture.cpp
    RHI/Framebuffer.cpp
    RHI/Pipelines.cpp
    Renderer.cpp

    DefaultRenderer.cpp
    DefaultScene.cpp

    Input/Mouse.cpp
    Input/Keyboard.cpp
    Application.cpp
)
target_include_directories(Hephaestus
    PUBLIC Public
    PRIVATE Private
)
target_link_libraries(Hephaestus
    PRIVATE glfw
    PRIVATE glad
    PRIVATE glm
    PRIVATE imgui
    PRIVATE debugbreak
    PRIVATE spdlog::spdlog_header_only
    PRIVATE stb
)