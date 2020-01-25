# LP3 Core

Very basic utility library that handles the following:

* A class to abstract the platform loop via a passed in std::function (useful for handling differences with Emscripten).
* Basic RAII helper classes to deal with SDL2 resources.
* Establishes a rudimentary logging system compatible with SDL2's log system and [the Squirrel Console](https://github.com/TimSimpson/SquirrelConsole). It uses boost::format to support a printf syntax and completely disappears outside of debug builds.
* Has code for raising errors or calling std::abort if exceptions are disabled, and creates a base exception class for other libraries to extend.
* Defines some compiler symbols to detect what platform code is running on (see le.hpp) (this is the descendant of code I wrote 15 years ago and should probably go away).
* `GlobalVar`, a class which avoids false negatives with Visual C++'s leak detector.
