add_custom_target(copy_data ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)

add_executable(OpenSpace
    Main.cpp
)
add_dependencies(OpenSpace copy_data)

target_link_libraries(OpenSpace
    PRIVATE Hephaestus
    PRIVATE glm-header-only
    PRIVATE EnTT::EnTT
    PRIVATE phmap
)
