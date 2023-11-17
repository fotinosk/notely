// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>

bool folderExists(const std::string& folderPath);
bool isVerboseFlagPresent(int& argc, char* argv[]);

#endif // UTILS_H
