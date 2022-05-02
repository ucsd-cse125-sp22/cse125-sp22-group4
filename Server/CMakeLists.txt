add_executable(Server
    src/Server.cpp
    src/ServerGame.cpp
    src/ServerNetwork.cpp
    src/Util.cpp
)

target_include_directories(Server PRIVATE
    include
    ${LIB_INCLUDE}
)

# make header files visible in visual studio
target_sources(Server PRIVATE
    include/ServerGame.h
    include/ServerNetwork.h
    include/Util.h
)

target_link_libraries(Server
    ${CONAN_LIBS}
    Network
    Constants
    Graphics
)