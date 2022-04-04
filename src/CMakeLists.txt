add_executable(Client
    main.cpp
    Cube.cpp
    Client.cpp
    Shader.cpp
    Camera.cpp
    bindings/imgui_impl_glfw.cpp
    bindings/imgui_impl_opengl3.cpp
)

target_include_directories(Client PRIVATE
    bindings
    ${GRAPHICS_SOURCE_DIR}/include
)

target_link_libraries(Client
    ${CONAN_LIBS}
)
