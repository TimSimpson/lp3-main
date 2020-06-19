# Change Log

### 1.0.4 2020-06-19 find_package name changed

The CMake targets have changed to the following:

    find_package(lp3-main)
    target_link_libraries(main lp3-main::lp3-main)

I **really** wanted to use `find_package(Lp3-Main)` with the target then being `lp3::main`, but even though Conan has [dramatically increased support for matching esoteric CMake target names](https://github.com/TimSimpson/lp3-sdl/issues/5) it still can't pull this off, and wants the first part of the target (ie the part before `::`) to be identical to the argument to `find_package` (so `find_package(Lp3-Main)` had to have targets like `Lp3-Main::main`). Nuts.

For the record, I'm not sure if it's a good idea to share the prefix amongst targets brought in by `find_package` even when using nothing but CMake. Anyway it's probably not worth caring about... though man it sure is ugly.

### 1.0.3 2020-06-11 bug fix

🐞 Fixed a gross bug in PlatformLoop on Windows which triggered an assert error in debug mode (who knows what really happened in release) whenever it read the command line arguments (so, like, all the time 😬). `wcstombs_s` is a harsh mistress.

