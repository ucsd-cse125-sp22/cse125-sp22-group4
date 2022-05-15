add_library(Audio STATIC
    src/AudioEngine.cpp
)

target_include_directories(Audio PRIVATE
    include
)

target_sources(Audio PRIVATE
    include/AudioEngine.h
)