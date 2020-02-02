#include <lp3/main/utils.hpp>

#include <optional>
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
        auto result = _dupenv_s(&env_var_value, &length, name.data());
        if (0 != result) {
            return std::nullopt;
        }
        std::unique_ptr<char> delete_env_var(env_var_value);
    }

    std::optional<std::string> write_wchat_t_to_string(wchar_t const * src)
        noexcept
    {
        std::string result;
        const std::size_t total_length = wcslen(src);
        result.reserve(total_length);

        // TODO: disable 4996 on MSVC?
        const std::size_t converted_count
            = wcstombs(result.data(), original, total_length);
        // Return false if could not convert wide character string to classic
        // char string.
        if (converted_count != total_length) {
            return std::nullopt;
        } else {
            return result;
        }
    }

#endif

}


namespace lp3::main {

namespace {
    std::optional<int> loop_count() {
        auto value = get_env_var("LP3_LOOP_COUNT");
        if (!value) {
            return std::nullopt;
        }
        else {
            try {
                return std::stoi(*value);
            } catch(const std::logic_error &) {
                return std::nullopt;
            }
        }
    }
}

LP3_MAIN_API
PlatformLoop::PlatformLoop()
:   arguments()
{
    #if defined(_WIN32)
        int argLength;
        LPWSTR * windows_string = ::CommandLineToArgvW(
            GetCommandLineW(), &argLength
        );
        if (nullptr == windows_string)
        {
            return;
        }
        for(int i = 0; i < argLength; i ++)
        {
            const auto new_string = write_wchat_t_to_string(windows_string[i]);
            if (new_string) {
                this->arguments.push_back(*new_string);
            }
        }
        LocalFree(windows_string);
    #endif
}

LP3_MAIN_API
PlatformLoop::PlatformLoop(int argc, char ** argv)
:   arguments()
{
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
            while(iterate() && (--c) < 0) {}
        } else {
            while(iterate()) {}
        }
    }
    return 0;
}

}
