# *********************************************************************
# LP3_MAIN
#       Simplifies writing game "lp3-main.cpp" files.
# *********************************************************************
cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)

if(NOT DEFINED PROJECT_NAME)
    set(NOT_SUBPROJECT ON)
endif()

project(lp3-main CXX)

include(CTest)
include(CMakePackageConfigHelpers)

# Allow user to ask explicitly to build tests
option(LP3_MAIN_Build_Tests "Build tests when BUILD_TESTING is enabled."
       ${NOT_SUBPROJECT})

add_library(
    main
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/main.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/main/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/main/utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exit.cpp)
set_target_properties(main PROPERTIES OUTPUT_NAME "lp3-main")

# Mandate the use of at least C++17 by everything that uses this
target_compile_features(main PUBLIC cxx_std_17)

target_include_directories(
    main
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>
    PRIVATE src)

# This is built as a "shared library" in sarcastic air quotes. It's only
# made that way to make linking faster, and relies on consumers using the same
# version of the runtime it was built with. IIRC this used to not be that big
# of a problem with VS, but now it is, thus the disabled warnings.
if(BUILD_SHARED_LIBS)
    target_compile_definitions(
        main
        PUBLIC LP3_MAIN_API_DYNAMIC
        PRIVATE LP3_MAIN_API_CREATE)
    if(MSVC)
        target_compile_options(main PRIVATE /wd4251 /wd4275)
    endif()
endif()

if("${CMAKE_SYSTEM_NAME}" MATCHES "Emscripten")
    target_sources(main
                   PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/js/platform.cpp)
else()
    target_sources(main
                   PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pc/platform.cpp)
endif()

# *********************************************************************
# Tests and Drivers / Demos
# *********************************************************************

if(BUILD_TESTING AND LP3_MAIN_Build_Tests)
    find_package(Catch2 REQUIRED)

    function(make_test exe_target)
        if("${CMAKE_SYSTEM_NAME}" MATCHES "Emscripten")
            add_test(NAME "test_${exe_target}" COMMAND node
                                                       $<TARGET_FILE:${exe_target}>)
        else()
            add_test(NAME "test_${exe_target}" COMMAND ${exe_target})
        endif()
    endfunction()

    add_executable(ExitDemo WIN32
               ${CMAKE_CURRENT_SOURCE_DIR}/demos/ExitDemo.cpp)
    target_link_libraries(ExitDemo main)
    make_test(ExitDemo)

    add_executable(MainDemo WIN32
                   ${CMAKE_CURRENT_SOURCE_DIR}/demos/MainDemo.cpp)
    target_link_libraries(MainDemo main)

    add_executable(WinConsoleTest
                   ${CMAKE_CURRENT_SOURCE_DIR}/demos/WinConsoleTest.cpp)
    target_link_libraries(WinConsoleTest main)
    make_test(WinConsoleTest)

    if(LP3_SETUP_FOLDERS)
        set_target_properties(main PROPERTIES FOLDER lp3-main)
        set_target_properties(ExitDemo PROPERTIES FOLDER lp3-main)
        set_target_properties(MainDemo PROPERTIES FOLDER lp3-main)
        set_target_properties(WinConsoleTest PROPERTIES FOLDER lp3-main)
    endif()

    if(BUILD_SHARED_LIBS)
        if(MSVC)
            target_compile_options(ExitDemo PRIVATE /wd4251 /wd4275)
            target_compile_options(MainDemo PRIVATE /wd4251 /wd4275)
            target_compile_options(WinConsoleTest PRIVATE /wd4251 /wd4275)
        endif()
    endif()
endif()

# *********************************************************************
# Package / Install Stuff
# *********************************************************************

install(DIRECTORY include/ DESTINATION include)

add_library(lp3::main ALIAS main)

install(
    TARGETS main
    EXPORT lp3-main-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES
    DESTINATION include)

install(
    EXPORT lp3-main-targets
    FILE lp3-main-targets.cmake
    NAMESPACE lp3::
    DESTINATION lib/cmake/lp3-main)

file(
    WRITE "${PROJECT_BINARY_DIR}/lp3-mainConfig.cmake"
    "
include(CMakeFindDependencyMacro)
include(\"\${CMAKE_CURRENT_LIST_DIR}/lp3-main-targets.cmake\")
")

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/lp3-mainConfigVersion.cmake"
    VERSION 1.0.1
    COMPATIBILITY AnyNewerVersion)

install(FILES "${PROJECT_BINARY_DIR}/lp3-mainConfig.cmake"
              "${PROJECT_BINARY_DIR}/lp3-mainConfigVersion.cmake"
        DESTINATION lib/cmake/lp3-main)
