// --------------------------------------------------------------------
//      This exists to test lp3::main::PlatformLoop's
//      command_line_args() function on Windows, since a WIN32 app
//      won't have access to the console.
// --------------------------------------------------------------------
#include <iostream>

#include <lp3/main.hpp>

int main(int argc, const char ** argv) {
    std::cout << "Console test.\n";

    lp3::main::PlatformLoop loop;

    int index = 0;
    for (const std::string & element : loop.command_line_args()) {
        std::cout << "[" << index << "] = " << element << "\n";
        ++index;
    }

    const auto result = loop.run([&]() {
        // This gets called each frame until false is returned.
        return false;
    });

    std::cout << "Good bye.\n";
    return result;
}
