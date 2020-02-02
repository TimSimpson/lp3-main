import conans

class TestPackage(conans.ConanFile):

    generators = "cmake_paths"

    def build(self):
        cmake = conans.CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        self.run(f"{self.build_folder}/main_test", run_environment=True)
