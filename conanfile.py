import os.path

import conans


class Lp3Main(conans.ConanFile):
    name = "Lp3-Main"
    version = "1.0.4"
    license = "Zlib"
    author = "Tim Simpson"
    url = "https://github.com/TimSimpson/Lp3-Main"
    description = "simple game app helper library"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}

    requires = tuple()

    build_requires = (
        "catch2/2.4.1@bincrafters/stable"
    )
    generators = "cmake_paths", "cmake_find_package"

    exports_sources = (
        "src/*", "include/*", "demos/*", "tests/*", "CMakeLists.txt"
    )

    def _configed_cmake(self):
        cmake = conans.CMake(self)
        cmake.configure(defs={
            "CMAKE_FIND_PACKAGE_PREFER_CONFIG":"TRUE",
        })
        return cmake

    def build(self):
        cmake = self._configed_cmake()
        cmake.build()
        # # If SDL2 is shared, we won't be able to find it in most cases.
        # #if self.settings.os != "Emscripten" and not self.options.shared:
        # cmake.test()

    def package(self):
        cmake = self._configed_cmake()
        cmake.install()

        # self.copy("*.hpp", dst="include", src="src")
        # self.copy("*.lib", dst="lib", keep_path=False)
        # self.copy("*.dll", dst="bin", keep_path=False)
        # self.copy("*.dylib*", dst="lib", keep_path=False)
        # self.copy("*.so", dst="lib", keep_path=False)
        # self.copy("*.a", dst="lib", keep_path=False)
        # self.copy("Lp3_Main-config*.cmake", dst="lib/cmake", keep_path=False)

    def package_info(self):
        self.cpp_info.name = "lp3-main"
        self.cpp_info.libs = [ "lp3-main" ]
        # self.cpp_info.libs = ["lp3-main"]
        # # self.cpp_info.names["cmake_find_package"] = "Lp3-Main"
        # # self.cpp_info.names["cmake_find_package_multi"] = "Lp3"
        # self.cpp_info.components["main"].names["cmake"] = "main"
        # self.cpp_info.components["main"].names["cmake_find_package"] = "main"
        # self.cpp_info.components["main"].names["cmake_find_package_multi"] = "main"
        # self.cpp_info.components["main"].includedirs = ["include"]
        # self.cpp_info.components["main"].libs = ["lp3-main"]
