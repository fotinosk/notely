// utils.cpp
#include "utils.h"
#include <filesystem>

bool folderExists(const std::string& folderPath) {
    return std::filesystem::exists(folderPath) && std::filesystem::is_directory(folderPath);
}

bool isVerboseFlagPresent(int& argc, char* argv[]) {
    const char* verboseFlag = "-v";

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], verboseFlag) == 0) {
            // Remove the verbose flag from argv
            for (int j = i; j < argc - 1; ++j) {
                argv[j] = argv[j + 1];
            }
            // Decrement argc to reflect the removal
            --argc;
            return true;
        }
    }

    return false;
}