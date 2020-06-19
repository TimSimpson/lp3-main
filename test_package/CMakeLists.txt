cmake_minimum_required(VERSION 3.5)
project(test_package)

set(CMAKE_CXX_STANDARD 17)

find_package(lp3-main REQUIRED)

add_executable(main_test ${CMAKE_CURRENT_SOURCE_DIR}/main_test.cpp)
target_link_libraries(main_test PUBLIC lp3-main::lp3-main)
# add_test(main_test main_test)
