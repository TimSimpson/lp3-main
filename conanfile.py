import os.path

import conans


class Lp3Core(conans.ConanFile):
    name = "Lp3-Core"
    version = "1.0.1"
    license = "Zlib"
    author = "Tim Simpson"
    url = "https://github.com/TimSimpson/Lp3-Core"
    description = "simple game app helper library"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}

    requires = (
        "boost/1.71.0@conan/stable",
        "gsl_microsoft/2.0.0@bincrafters/stable",
        "zlib/1.2.11@conan/stable",
    )

    sdl2_requires = (
        "sdl2/2.0.9@bincrafters/stable",
        "sdl2_image/2.0.5@bincrafters/stable",
    )

    def requirements(self):
        if self.settings.os != "Emscripten":
            for r in self.sdl2_requires:
                self.requires.add(r)

    build_requires = (
        "glm/0.9.8.5@g-truc/stable",
        "catch2/2.4.1@bincrafters/stable"
    )
    generators = "cmake_paths", "cmake_find_package"

    exports_sources = (
        "src/*", "include/*", "demos/*", "tests/*", "CMakeLists.txt"
    )

    def build(self):
        cmake = conans.CMake(self)
        cmake.configure(defs={
            "CMAKE_FIND_PACKAGE_PREFER_CONFIG":"TRUE",
        })
        cmake.build()

    def package(self):
        self.copy("*.hpp", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["lp3_core"]
