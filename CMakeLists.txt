# *********************************************************************
# LP3_MAIN
#       Simplifies writing game "main.cpp" files.
# *********************************************************************
project(Lp3_Main CXX)
cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)
include(CMakePackageConfigHelpers)

enable_testing()

include("${CMAKE_BINARY_DIR}/conan_paths.cmake"
        OPTIONAL
        RESULT_VARIABLE using_conan)

if(using_conan)
    set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
endif()

add_library(lp3_main
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/main.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/main/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/main/utils.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/exit.cpp
)
set_target_properties(lp3_main PROPERTIES OUTPUT_NAME "Lp3_Main")

# Mandate the use of at least C++17 by everything that uses this
target_compile_features(lp3_main
    PUBLIC
    cxx_std_17
)

target_include_directories(lp3_main
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

# This is built as a "shared library" in sarcastic air quotes. It's only
# made that way to make linking faster, and relies on consumers using the same
# version of the runtime it was built with. IIRC this used to not be that big
# of a problem with VS, but now it is, thus the disabled warnings.
if(BUILD_SHARED_LIBS)
    target_compile_definitions(lp3_main
        PUBLIC  LP3_MAIN_API_DYNAMIC
        PRIVATE LP3_MAIN_API_CREATE
    )
    if (MSVC)
        target_compile_options(lp3_main PRIVATE /wd4251 /wd4275)
    endif()
endif()

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Emscripten")
    target_sources(lp3_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/js/platform.cpp)
else()
    target_sources(lp3_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pc/platform.cpp)
endif()


# *********************************************************************
# Package / Install Stuff
# *********************************************************************

install(DIRECTORY include/ DESTINATION include)

install(TARGETS lp3_main EXPORT Lp3_Main-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

add_library(lp3::main ALIAS lp3_main)
install(EXPORT Lp3_Main-targets
    FILE Lp3_Main-targets.cmake
    NAMESPACE lp3::
    DESTINATION lib/cmake/Lp3_Main
)

file(WRITE "${PROJECT_BINARY_DIR}/Lp3_MainConfig.cmake" "
include(CMakeFindDependencyMacro)
include(\"\${CMAKE_CURRENT_LIST_DIR}/Lp3_Main-targets.cmake\")
")

write_basic_package_version_file("${PROJECT_BINARY_DIR}/Lp3_MainConfigVersion.cmake"
    VERSION 1.0.1
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${PROJECT_BINARY_DIR}/Lp3_MainConfig.cmake"
    "${PROJECT_BINARY_DIR}/Lp3_MainConfigVersion.cmake"
    DESTINATION lib/cmake/Lp3_Main
)

# *********************************************************************
# Tests and Drivers / Demos
# *********************************************************************

get_directory_property(is_child PARENT_DIRECTORY)
if (NOT is_child)
    set(LP3_MAIN_FULL True)
endif()

if (LP3_MAIN_FULL)
    add_executable(ExitDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/ExitDemo.cpp)
    target_link_libraries(ExitDemo lp3_main)

    add_executable(MainDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/MainDemo.cpp)
    target_link_libraries(MainDemo lp3_main)

    if (LP3_SETUP_FOLDERS)
        set_target_properties (lp3_main PROPERTIES FOLDER main)
        set_target_properties (ExitDemo PROPERTIES FOLDER main)
        set_target_properties (MainDemo PROPERTIES FOLDER main)
    endif()
    if(BUILD_SHARED_LIBS)
        if (MSVC)
            target_compile_options(ExitDemo PRIVATE /wd4251 /wd4275)
            target_compile_options(MainDemo PRIVATE /wd4251 /wd4275)
        endif()
    endif()
endif()

