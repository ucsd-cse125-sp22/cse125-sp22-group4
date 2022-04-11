add_executable(Graphics
    src/main.cpp
    src/Cube.cpp
    src/Model.cpp
    src/Client.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/ObjectLoader.cpp
    src/TexturedMesh.cpp
    src/PrimitiveMesh.cpp
    src/ThirdPersonCamera.cpp
    bindings/imgui_impl_glfw.cpp
    bindings/imgui_impl_opengl3.cpp
)

target_include_directories(Graphics PRIVATE
    bindings
    include
)

# make header files visible in visual studio
target_sources(Graphics PRIVATE
    include/main.h
    include/Cube.h
    include/Model.h
    include/Client.h
    include/Shader.h
    include/Camera.h
    include/ShaderUtil.h
    include/ObjectLoader.h
    include/TexturedMesh.h
    include/PrimitiveMesh.h
    include/GraphicObject.h
    include/ThirdPersonCamera.h
)

target_link_libraries(Graphics
    ${CONAN_LIBS}
)
