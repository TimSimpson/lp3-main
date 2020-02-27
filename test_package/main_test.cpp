#include <iostream>
#include <lp3/main.hpp>

int _main(lp3::main::PlatformLoop & loop) {
    int index = 0;
    for (const std::string & element : loop.command_line_args()) {
        std::cout << "[" << index << "] = " << element << "\n";
        ++index;
    }

    const auto result = loop.run([&]() { return false; });

    return result;
}

LP3_MAIN(_main)
