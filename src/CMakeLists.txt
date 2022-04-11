add_executable(Client
    main.cpp
    Cube.cpp
    Client.cpp
    Shader.cpp
    Camera.cpp
    ObjectLoader.cpp
    TexturedMesh.cpp
    PrimitiveMesh.cpp
    ThirdPersonCamera.cpp
    bindings/imgui_impl_glfw.cpp
    bindings/imgui_impl_opengl3.cpp
)

target_include_directories(Client PRIVATE
    bindings
    ${GRAPHICS_INCLUDE}
)

# make header files visible in visual studio
target_sources(Client PRIVATE
    ${GRAPHICS_INCLUDE}/main.h
    ${GRAPHICS_INCLUDE}/Cube.h
    ${GRAPHICS_INCLUDE}/Model.h
    ${GRAPHICS_INCLUDE}/Client.h
    ${GRAPHICS_INCLUDE}/Shader.h
    ${GRAPHICS_INCLUDE}/Camera.h
    ${GRAPHICS_INCLUDE}/ShaderUtil.h
    ${GRAPHICS_INCLUDE}/ObjectLoader.h
    ${GRAPHICS_INCLUDE}/TexturedMesh.h
    ${GRAPHICS_INCLUDE}/PrimitiveMesh.h
    ${GRAPHICS_INCLUDE}/GraphicObject.h
    ${GRAPHICS_INCLUDE}/ThirdPersonCamera.h
)

target_link_libraries(Client
    ${CONAN_LIBS}
)
