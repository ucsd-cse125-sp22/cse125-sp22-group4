add_library(Network STATIC
    src/Network.cpp
    src/NetworkServices.cpp
    src/Packets.cpp
    src/pch.cpp
)

target_include_directories(Network PRIVATE
    include
    ${LIB_INCLUDE}
)

# make header files visible in visual studio
target_sources(Network PRIVATE
    include/framework.h
    include/NetworkData.h
    include/NetworkServices.h
    include/Packets.h
    include/pch.h
)

target_link_libraries(Network
    wsock32
    ws2_32
)