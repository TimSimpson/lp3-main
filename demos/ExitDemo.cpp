// ---------------------------------------------------------------------------
// GlobalVar example
// ---------------------------------------------------------------------------
// Everyone knows that global variables are bad, but they also allow us to
// do neato reflection type things as the "RegisterData" struct in this
// example shows.
//
// Unfortunately Microsoft Visual C++'s leak detector is triggered if any
// global variables are added to the program (supposedly it's not that severe
// and has to do with config options but I haven't been able to make it work).
//
// So I've written a few custom things to create globals using pointers and
// make sure they are deleted, as shown here.
// ---------------------------------------------------------------------------
#include <iostream>
#include <map>
#include <string>

#include <lp3/main.hpp>

struct Data {
    const char * name;
    const char * desc;
};

namespace {
lp3::main::GlobalVar<std::map<std::string, Data>> data;
}

struct RegisterData {
    RegisterData(const char * name, const char * desc) {
        data.get()[std::string(name)] = Data{name, desc};
    }

    ~RegisterData() { std::cout << "Register data is being deleted...\n"; }
};

RegisterData data_1("Tim", "Author");
RegisterData data_2("C++", "Language");

int _main(lp3::main::PlatformLoop &) {
    std::cout << "Let's play with crazy global data.\n";

    for (auto & e : data.get()) {
        if (e.first != e.second.name) {
            std::cerr << "Error! :";
        }
        std::cout << e.first << " - " << e.second.desc << "\n";
    }

    std::cout << "Good bye.\n";
    return 0;
}

LP3_MAIN(_main)
// ~end-doc
