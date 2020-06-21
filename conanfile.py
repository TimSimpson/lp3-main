import os.path

import conans


class Lp3Main(conans.ConanFile):
    name = "Lp3-Main"
    version = "1.0.5"
    license = "Zlib"
    author = "Tim Simpson"
    url = "https://github.com/TimSimpson/Lp3-Main"
    description = "simple game app helper library"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}

    requires = tuple()

    build_requires = []

    test_requires = [
        "Catch2/2.11.1@catchorg/stable",
    ]

    @property
    def tests_enabled(self):
        return (
            self.develop
            and (os.environ.get("CONAN_SKIP_TESTS") or "").lower() != 'true'
        )

    def build_requirements(self):
        if self.tests_enabled:
            for tr in self.test_requires:
                self.build_requires(tr)

    generators = "cmake_find_package"

    exports_sources = (
        "src/*", "include/*", "demos/*", "tests/*", "CMakeLists.txt"
    )

    def _configed_cmake(self):
        cmake = conans.CMake(self)
        cmake.configure(defs={
            "LP3_MAIN_Build_Tests": self.tests_enabled,
        })
        return cmake

    def build(self):
        cmake = self._configed_cmake()
        cmake.build()

    def package(self):
        cmake = self._configed_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.name = "lp3-main"
        self.cpp_info.libs = [ "lp3-main" ]
