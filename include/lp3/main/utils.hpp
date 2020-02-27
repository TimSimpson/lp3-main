#ifndef LP3_MAIN_UTILS_HPP
#define LP3_MAIN_UTILS_HPP
#pragma once

#include "config.hpp"
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace lp3 {
namespace main {

    // --------------------------------------------------------------------
    // class PlatformLoop
    // --------------------------------------------------------------------
    //     Contains code to run the main loop on any given platform.
    //     While it's possible to create this yourself, you should instead
    //     use the macro LP3_MAIN from <lp3/main.hpp> which creates this
    //     for you.
    // --------------------------------------------------------------------
    class LP3_MAIN_API PlatformLoop {
      public:
        PlatformLoop();

        PlatformLoop(int argc, char ** argv);

        PlatformLoop(const PlatformLoop & other) = delete;
        PlatformLoop & operator=(const PlatformLoop & other) = delete;

        std::vector<std::string> command_line_args() const;

        int run(std::function<bool()> iterate);

      private:
        std::vector<std::string> arguments;
    };

    typedef void (*ExitCall)();

    // This is called automatically by main.hpp.
    class LP3_MAIN_API OnExitCleanUp {
      public:
        OnExitCleanUp();
        ~OnExitCleanUp();
    };

    struct LP3_MAIN_API GlobalResource {
        virtual ~GlobalResource() {}
    };

    LP3_MAIN_API
    void on_exit_clean_up(GlobalResource *);

    // --------------------------------------------------------------------
    // class GlobalVar
    // --------------------------------------------------------------------
    // In C++11 and above you can declare certain global variables and be
    // certain it will be initialized correctly without suffering from
    // race conditions or other historical problems.
    //
    // Unfortunately if you want to use Visual C++'s otherwise excellent
    // leak detector it's not that easy, as it will wrongly flag data that
    // is created before the ``main`` is called.
    //
    // GlobalVar works in conjunction with the LP3_MAIN macro to get
    // around this problem by storing a pointer to global data which is
    // then deleted right before ``main`` exits, allowing you to use the
    // leak detector without triggering false negatives.
    // --------------------------------------------------------------------
    template <typename T> class GlobalVar {
      public:
        template <typename... Params>
        GlobalVar(Params &&... args)
            : ptr(new T(std::forward<Params>(args)...)) {
            on_exit_clean_up(new GR{ptr});
        }

        ~GlobalVar() {
            // Handled elsewhere.
        }

        T & get() { return *ptr; }

      private:
        T * ptr;

        struct GR : public GlobalResource {
            T * ptr;

            GR(T * _ptr) : ptr(_ptr) {}

            GR(GR && other) { std::swap(this->ptr, other.ptr); }

            ~GR() override {
                if (ptr) {
                    delete ptr;
                }
            }
        };
    };
    // ~end-doc

} // namespace main
} // namespace lp3

#endif
