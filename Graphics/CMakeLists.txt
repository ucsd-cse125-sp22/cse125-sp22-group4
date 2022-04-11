add_executable(ClientGL
    src/main.cpp
    src/Client.cpp
    bindings/imgui_impl_glfw.cpp
    bindings/imgui_impl_opengl3.cpp
)

target_include_directories(ClientGL PRIVATE
    bindings
    include
    ${LIB_INCLUDE}
)

# make header files visible in visual studio
target_sources(ClientGL PRIVATE
    include/main.h
    include/Client.h
)

target_link_libraries(ClientGL
    ${CONAN_LIBS}
    Graphics
)
