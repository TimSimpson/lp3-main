# Change Log

### 1.0.3 2020-06-11 bug fix

🐞 Fixed a gross bug in PlatformLoop on Windows which triggered an assert error in debug mode (who knows what really happened in release) whenever it read the command line arguments (so, like, all the time 😬). `wcstombs_s` is a harsh mistress.

