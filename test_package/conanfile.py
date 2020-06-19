import conans

class TestPackage(conans.ConanFile):

    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"

    def build(self):
        cmake = conans.CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        if self.settings.os != "Emscripten":
            self.run(f"{self.build_folder}/main_test", run_environment=True)
