add_library(Logic STATIC
    src/Player.cpp
    src/Item.cpp
)

target_include_directories(Logic PRIVATE
    include
    ${LIB_INCLUDE}
)

# make header files visible in visual studio
target_sources(Logic PRIVATE
    include/Player.h
    include/Item.h
)

target_link_libraries(Logic
    Constants
)