# lp3-main

Very basic library which helps writing apps with a "platform loop."

Here's an example:

```cpp
#include <lp3/main.hpp>  // Only ever include this once
// include <lp3/main/utils.hpp> for some of the other functionality

int _main(lp3::main::PlatformLoop & loop) {
    // Create all the important systems of your game or app (for argument's
    // sake pretend this initialization code is complex):
    MyGrossAppState state();

    // now, run the app:
    const auto result = loop.run([&]() {
        // This gets called each frame until false is returned.
        bool keep_running = state.run();
        return keep_running;
    });

    return result;
}

LP3_MAIN(_main)
```

As seen, "PlatformLoop" is created by the dangerous and cool "LP3_MAIN" macro and passed into the function you write. You then give PlatformLoop a function to "run" repeatedly in a loop.

Platform Specific Behaviors:

* In Emscripten, `PlatformLoop::run` calls `emscripten_set_main_loop`.
* In Windows, the `LP3_MAIN` macro sandwhiches the main method you give it between calls to `_CrtSetReportMode` and `_CrtDumpMemoryLeaks`.

`lp3/main/utils.hpp` also has some stuff to let you create global variables in a way that avoids false negatives with some leak detectors.


