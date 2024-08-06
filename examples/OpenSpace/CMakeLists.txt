add_executable(OpenSpace
    Main.cpp
)
target_link_libraries(OpenSpace
    PRIVATE Xacor
    PRIVATE glm-header-only
)