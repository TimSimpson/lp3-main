cmake_minimum_required(VERSION 3.5)
project(test_package)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Lp3_Main CONFIG REQUIRED)

add_executable(main_test ${CMAKE_CURRENT_SOURCE_DIR}/main_test.cpp)
target_link_libraries(main_test PUBLIC lp3::lp3_main)
# add_test(main_test main_test)
