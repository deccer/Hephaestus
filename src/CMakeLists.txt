add_library(Xacor STATIC
    RHI/Debug.cpp
    RHI/Buffer.cpp
    RHI/Texture.cpp

    DefaultRenderer.cpp
    DefaultScene.cpp

    Input/Mouse.cpp
    Input/Keyboard.cpp
    Application.cpp
)
target_include_directories(Xacor
    PUBLIC Public
    PRIVATE Private
)
target_link_libraries(Xacor
    PRIVATE glfw
    PRIVATE glad
    PRIVATE glm
    PRIVATE imgui
    PRIVATE debugbreak
    PRIVATE spdlog::spdlog_header_only
)