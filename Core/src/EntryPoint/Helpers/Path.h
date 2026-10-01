// This header file contains reusable function that help with
// file path processing as needed
#include <filesystem>
#include <string>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
    #include <limits.h>
#endif


namespace Core {
    std::filesystem::path GetExecutableDirectory()
    {
        #ifdef _WIN32
            // Retrieve the full path of the loaded module(.exe)
            char buffer[MAX_PATH]{};
            GetModuleFileNameA(nullptr, buffer, MAX_PATH);
            // Get the parent directory of the loaded module
            return std::filesystem::path(buffer).parent_path();
        #else
            char buffer[PATH_MAX]{};
            ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
            
            if (len != -1) {
                buffer[len] = '\0';
                return std::filesystem::path(buffer).parent_path();
            }

            return {};
        #endif
    }
}
