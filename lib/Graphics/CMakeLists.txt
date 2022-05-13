add_library(Graphics STATIC
    src/Cube.cpp
    src/Maze.cpp
    src/Model.cpp
    src/Shader.cpp
    src/Skybox.cpp
    src/Camera.cpp
    src/FakeModel.cpp
    src/ObjectLoader.cpp
    src/TexturedMesh.cpp
    src/PrimitiveMesh.cpp
    src/ThirdPersonCamera.cpp
    src/CollisionDetector.cpp
    src/Tokenizer.cpp
    src/SceneLoader.cpp
    src/Bone.cpp
    src/Animation.cpp
    src/Animator.cpp
)

target_include_directories(Graphics PRIVATE
    include
)

# make header files visible in visual studio
target_sources(Graphics PRIVATE
    include/Cube.h
    include/Maze.h
    include/Model.h
    include/Shader.h
    include/Skybox.h
    include/Camera.h
    include/ShaderUtil.h
    include/FakeModel.h
    include/ObjectLoader.h
    include/TexturedMesh.h
    include/PrimitiveMesh.h
    include/GraphicObject.h
    include/ThirdPersonCamera.h
    include/CollisionDetector.h
    include/Tokenizer.h
    include/SceneLoader.h
    include/Bone.h
    include/Animation.h
    include/Animator.h
)

target_link_libraries(Graphics
    ${CONAN_LIBS}
)

#add filter named shaders in VS
file(GLOB_RECURSE SRC_SHADER
    "${CMAKE_SOURCE_DIR}/shaders/*"
)
target_sources(Graphics PRIVATE
    ${SRC_SHADER}
)

source_group("Shaders" FILES ${SRC_SHADER})
