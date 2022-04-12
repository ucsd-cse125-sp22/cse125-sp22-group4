add_library(Constants STATIC
    src/PlaceHolder.cpp
)

target_sources(Constants PRIVATE
    include/pch.h
    include/constants.h
)

target_include_directories(Constants PRIVATE
    ${LIB_INCLUDE}
)