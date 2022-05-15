add_executable(Client
    src/main.cpp
    src/Client.cpp
    src/ClientGame.cpp
    src/ClientNetwork.cpp
    bindings/imgui_impl_glfw.cpp
    bindings/imgui_impl_opengl3.cpp
)

target_include_directories(Client PRIVATE
    include
    bindings
    ${LIB_INCLUDE}
)

# make header files visible in visual studio
target_sources(Client PRIVATE
    include/main.h
    include/Client.h
    include/ClientGame.h
    include/ClientNetwork.h
)

target_link_libraries(Client
    ${CONAN_LIBS}
    Graphics
    Network
    Constants
    Audio
    ${LIB_INCLUDE}/Audio/fmodstudio_vc.lib
    ${LIB_INCLUDE}/Audio/fmod_vc.lib
)
