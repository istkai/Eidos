cmake_minimum_required(VERSION 3.25)

project(Eidos LANGUAGES CXX)

find_package(SDL3 REQUIRED CONFIG)

add_executable(eidos
    src/main.cpp
    src/pixel_count.cpp
    src/extent.cpp
    src/draw_scene.cpp
    src/motion.cpp
)

target_link_libraries(eidos PRIVATE SDL3::SDL3)

target_compile_features(eidos PRIVATE cxx_std_20)

set_target_properties(eidos PROPERTIES
    CXX_EXTENSIONS OFF
)

enable_testing()

add_executable(extent_tests
    tests/extent_tests.cpp
    src/extent.cpp
)

target_include_directories(extent_tests PRIVATE src)
target_compile_features(extent_tests PRIVATE cxx_std_20)

set_target_properties(extent_tests PROPERTIES
    CXX_EXTENSIONS OFF
)

add_test(NAME extent_scaling COMMAND extent_tests)

add_executable(motion_tests
    tests/motion_tests.cpp
    src/motion.cpp
)

target_include_directories(motion_tests PRIVATE src)
target_compile_features(motion_tests PRIVATE cxx_std_20)

set_target_properties(motion_tests PROPERTIES CXX_EXTENSIONS OFF)

add_test(NAME horizontal_motion COMMAND motion_tests)
