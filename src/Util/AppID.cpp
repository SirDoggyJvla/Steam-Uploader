#include "AppID.h"

#include "spdlog/spdlog.h"

#include <fstream>
#include <filesystem>
#include <system_error>

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

namespace {
    std::filesystem::path getAppFolder()
    {
    #if defined(_WIN32)
        char buffer[MAX_PATH] = {};
        const DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        if (length == 0 || length >= MAX_PATH) {
            return std::filesystem::current_path();
        }
        return std::filesystem::path(buffer).parent_path();
    #elif defined(__APPLE__)
        uint32_t size = 0;
        _NSGetExecutablePath(nullptr, &size);
        if (size == 0) {
            return std::filesystem::current_path();
        }

        std::string buffer(size, '\0');
        if (_NSGetExecutablePath(buffer.data(), &size) != 0) {
            return std::filesystem::current_path();
        }

        return std::filesystem::path(buffer.c_str()).parent_path();
    #elif defined(__linux__)
        std::array<char, 4096> buffer{};
        const ssize_t length = readlink("/proc/self/exe", buffer.data(), buffer.size() - 1);
        if (length <= 0) {
            return std::filesystem::current_path();
        }

        buffer[static_cast<size_t>(length)] = '\0';
        return std::filesystem::path(buffer.data()).parent_path();
    #else
        return std::filesystem::current_path();
    #endif
    }
} // namespace

// source:
// https://github.com/TechnologicNick/SteamChangePreview/blob/master/SteamChangePreview/SteamChangePreview.cpp
//! Updates the steam_appid.txt file which the steam API reads to know what appID to use
void setAppID(const AppId_t appID)
{
    spdlog::debug("Writing appID to steam_appid.txt file...");

    const std::filesystem::path appFolder = getAppFolder();

    spdlog::debug("old working directory: {}", std::filesystem::current_path().string());
    std::error_code ec;
    std::filesystem::current_path(appFolder, ec);
    if (ec) {
        spdlog::error("Failed to set working directory to app folder ({}): {}",
            appFolder.string(), ec.message());
    } else {
        spdlog::debug("new working directory: {}", std::filesystem::current_path().string());
    }

    std::ofstream file("steam_appid.txt", std::ios::out | std::ios::trunc);
    if (!file.is_open() || !file.good()) {
        spdlog::error("Failed to write appID to steam_appid file.");
        return;
    }

    file << appID;
    file.close();
}
