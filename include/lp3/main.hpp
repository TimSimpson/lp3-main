// ----------------------------------------------------------------------------
// Using LP3_MAIN to create the Platform Loop
// ============================================================================
//      The signature of ``main`` can differ between platforms. Besides, what
//      games want isn't a function entry point, but some kind of loop
//      (Emscripten actually prefers you give it a function which it can call
//      repeatedly).
//
//      Lp3::Core allows you to write a main by writing a function which
//      accepts ``lp3::main::PlatformLoop &`` as it's sole argument and returns
//      an integer exit code that may or may not be used depending on the
//      platform. PlatformLoop's ``run`` method can then be called with a
//      lambda to run your game logic. Writing a lambda that captures arguments
//      in a function is arguably easier to write and read than writing a
//      callable class or struct.
//
//      This function you write must then be called by the LP3_MAIN macro which
//      will generate the platform specific main function for you. It will also
//      do things like automatically enable the leak detector in Visual Studio.
//
// ~see-file "../../demos/MainDemo.cpp"
// ~end-doc summary
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// LP3_MAIN(main_function)
// -----------------------
//      Given a function that accepts ``lp3::main::PlatformLoop`` and
//      returns ``int``, this will generate the appropriate main function
//      for you.
//
//      As you might expect, this should only be used once for any program
//      you plan on building.
//
//      Other than easing development a bit, this also sets up the leak
//      detector stuff for Visual C++ automatically (see
//      ``lp3::main::GlobalVar`` for more info).
//
//      Note: unlike other components, this macro is only available if you
//      explicitly include ``<lp3/main.hpp>``.
// ---------------------------------------------------------------------------/
#ifdef FILE_LP3_MAIN_HPP
#error "This file should only be included once."
#endif
#define FILE_LP3_MAIN_HPP
#pragma once

// Handle VC++ leak checker stuff
#if defined(_WIN32) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#pragma warning(push, 0)
#include <crtdbg.h>
#pragma warning(pop)
#define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include "main/utils.hpp"
#define SDL_MAIN_HANDLED

#if !defined(_WIN32)
// Non-Windows ---------------------------------------------------------------
#define LP3_MAIN(main_function)                                                \
    int main(int argc, char * argv[]) {                                        \
        lp3::main::OnExitCleanUp clean_up;                                     \
        lp3::main::PlatformLoop loop(argc, argv);                              \
        return main_function(loop);                                            \
    }
#else
#include <windows.h>

// Just use "WIN32" flag in CMake
// #pragma comment( linker, "/subsystem:windows" )

#if _DEBUG
#define LP3_MAIN(main_function)                                                \
    int WinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {                           \
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);                      \
        int result;                                                            \
        {                                                                      \
            lp3::main::OnExitCleanUp clean_up;                                 \
            lp3::main::PlatformLoop loop;                                      \
            result = main_function(loop);                                      \
        }                                                                      \
        if (_CrtDumpMemoryLeaks()) {                                           \
            __debugbreak();                                                    \
            return 1;                                                          \
        }                                                                      \
        return result;                                                         \
    }
#else
#define LP3_MAIN(main_function)                                                \
    int WinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {                           \
        lp3::main::OnExitCleanUp clean_up;                                     \
        lp3::main::PlatformLoop loop;                                          \
        return main_function(loop);                                            \
    }
#endif
#endif
