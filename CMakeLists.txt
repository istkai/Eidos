cmake_minimum_required(VERSION 3.25)

project(Eidos LANGUAGES CXX)

add_executable(eidos
    src/main.cpp
    src/pixel_count.cpp
    src/extent.cpp
)

target_compile_features(eidos PRIVATE cxx_std_20)

set_target_properties(eidos PROPERTIES
    CXX_EXTENSIONS OFF
)
