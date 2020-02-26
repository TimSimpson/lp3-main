// --------------------------------------------------------------------
//      This example shows how to use LP3_MAIN to make a simple app.
// --------------------------------------------------------------------
#include <iostream>

#include <lp3/main.hpp>


int _main(lp3::main::PlatformLoop & loop) {
    std::cout << "Greetings from the MicroMain Demo.\n";

	int index = 0;
    for(const std::string & element : loop.command_line_args()) {
        std::cout << "[" << index << "] = " << element << "\n";
        ++ index;
    }

    const auto result = loop.run([&]() {
        // This gets called each frame until false is returned.
		return false;
    });

    std::cout << "Good bye.\n";
    return result;
}

LP3_MAIN(_main)
// ~end-doc
