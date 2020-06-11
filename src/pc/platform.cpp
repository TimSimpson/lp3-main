#include <lp3/main/utils.hpp>

#include <optional>
#include <stdexcept>
#ifdef _WIN32
#include <windows.h>
#endif

namespace {

#if !defined(_WIN32)

std::optional<std::string> get_env_var(const char * name) {
    const char * const env_var_value = getenv(name);
    if (env_var_value) {
        return std::string{env_var_value};
    } else {
        return std::nullopt;
    }
}

#else

std::optional<std::string> get_env_var(const char * name) {
    char * env_var_value;
    std::size_t length;
    auto result = _dupenv_s(&env_var_value, &length, name);
    if (0 != result) {
        return std::nullopt;
    }
    std::unique_ptr<char> delete_env_var(env_var_value);
    if (env_var_value) {
        return std::string{env_var_value};
    } else {
        return std::nullopt;
    }
}

std::optional<std::string>
write_wchar_t_to_string(wchar_t const * src) noexcept {    
    const std::size_t total_length = wcslen(src);
    // The function may need one extra here according to the docs
    // as it always puts a \0, even if doing that means writing 
    // one past the argument sent in.
    const std::size_t buffer_size = total_length + 1;
    std::string result(buffer_size, ' ');

    std::size_t converted_count;
    const auto errors = wcstombs_s(&converted_count, result.data(),
                                   buffer_size, src, total_length);

    if (errors != 0 || converted_count < total_length || converted_count > buffer_size) {
        // Remove last byte, needed for the '\0' (if one was written)        
        return std::nullopt;
    } else {
        result.resize(total_length);
        return result;
    }
}

#endif

} // namespace

namespace lp3::main {

namespace {
    std::optional<int> loop_count() {
        auto value = get_env_var("LP3_LOOP_COUNT");
        if (!value) {
            return std::nullopt;
        } else {
            try {
                return std::stoi(*value);
            } catch (const std::logic_error &) {
                return std::nullopt;
            }
        }
    }
} // namespace

LP3_MAIN_API
PlatformLoop::PlatformLoop() : arguments() {
#if defined(_WIN32)
    int argLength;
    LPWSTR * windows_string
            = ::CommandLineToArgvW(GetCommandLineW(), &argLength);
    if (nullptr == windows_string) {
        return;
    }
    for (int i = 0; i < argLength; i++) {
        const auto new_string = write_wchar_t_to_string(windows_string[i]);
        if (new_string) {
            this->arguments.push_back(*new_string);
        }
    }
    LocalFree(windows_string);
#endif
}

LP3_MAIN_API
PlatformLoop::PlatformLoop(int argc, char ** argv) : arguments() {
    for (int i = 0; i < argc; i++) {
        this->arguments.push_back(argv[i]);
    }
}

LP3_MAIN_API
std::vector<std::string> PlatformLoop::command_line_args() const {
    return arguments;
}

LP3_MAIN_API
int PlatformLoop::run(std::function<bool()> iterate) {
    if (iterate) {
        const auto count = loop_count();
        if (count) {
            int c = *count;
            while (iterate() && (--c) < 0) {
            }
        } else {
            while (iterate()) {
            }
        }
    }
    return 0;
}

} // namespace lp3::main
