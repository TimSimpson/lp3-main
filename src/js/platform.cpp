#include <GL/glfw.h>
///#include <emscripten/emscripten.h>
#include <emscripten.h>

#include <lp3/main/utils.hpp>

namespace lp3::main {

namespace {
    static int global_instances = 0;

    static std::function<bool()> * global_iterate = nullptr;

    class SetGlobalIterateToNullOnDestruct {
    public:
        ~SetGlobalIterateToNullOnDestruct() {
            global_iterate = nullptr;
        }
    };

    void do_loop() {
        if (global_iterate) {
            (*global_iterate)();  // return value is ignored
        }
    }

}

PlatformLoop::PlatformLoop()
:   arguments()
{
    if (global_instances >= 1) {
        throw std::logic_error("Platform Loop cannot be created twice.");
    }
    ++global_instances;
}

PlatformLoop::PlatformLoop(int argc, char ** argv)
:   arguments()
{
    if (global_instances >= 1) {
        throw std::logic_error("Platform Loop cannot be created twice.");
    }
    ++global_instances;

    for (int i = 0; i < argc; i++) {
        this->arguments.push_back(argv[i]);
    }
}

std::vector<std::string> PlatformLoop::command_line_args() const {
    return arguments;
}

int PlatformLoop::run(std::function<bool()> iterate) {
    SetGlobalIterateToNullOnDestruct set_it;
    if (iterate) {
        global_iterate = &iterate;
    }
    emscripten_set_main_loop(do_loop, 0, 1);

    return 0;
}

}
