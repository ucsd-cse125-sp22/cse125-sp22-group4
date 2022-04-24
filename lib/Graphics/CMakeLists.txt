add_library(Graphics STATIC
    src/Cube.cpp
    src/Model.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/ObjectLoader.cpp
    src/TexturedMesh.cpp
    src/PrimitiveMesh.cpp
    src/ThirdPersonCamera.cpp
    src/Maze.cpp
    src/Skybox.cpp
)

target_include_directories(Graphics PRIVATE
    include
)

# make header files visible in visual studio
target_sources(Graphics PRIVATE
    include/Cube.h
    include/Model.h
    include/Shader.h
    include/Camera.h
    include/ShaderUtil.h
    include/ObjectLoader.h
    include/TexturedMesh.h
    include/PrimitiveMesh.h
    include/GraphicObject.h
    include/ThirdPersonCamera.h
    include/Maze.h
    include/Skybox.h
)

target_link_libraries(Graphics
    ${CONAN_LIBS}
)
