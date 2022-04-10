add_executable(Client
    src/Client.cpp
    src/ClientGame.cpp
    src/ClientNetwork.cpp
)

target_include_directories(Client PRIVATE
    include
    ${LIB_INCLUDE}
)

# make header files visible in visual studio
target_sources(Client PRIVATE
    include/ClientGame.h
    include/ClientNetwork.h
)

target_link_libraries(Client
    NETWORK
)