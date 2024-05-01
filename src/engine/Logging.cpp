#include "Logging.hpp"

#include <cstdio>
#include <ctime>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cstdarg>

namespace Logging {
    std::ofstream logFile;
    void log(std::string level, std::string fmt, va_list args);
}

bool Logging::init() {
    logFile = std::ofstream("./log.txt", std::ios::trunc);
    if (!logFile.good()) {
        printf("Failed to initialize logging file.");
        return false;
    }
    return true;
}

std::string getDateTimeString() {
    time_t currentTime = time(NULL);
    tm* timeInfo = localtime(&currentTime);

    std::stringstream ss;
    ss << "[" << (timeInfo->tm_year + 1900) << "/" << std::setw(2) << std::setfill('0') << timeInfo->tm_mon << "/" << std::setw(2) << timeInfo->tm_mday << " " << std::setw(2) << timeInfo->tm_hour << ":" << std::setw(2) << timeInfo->tm_min << ":" << std::setw(2) << timeInfo->tm_sec << "]" << std::setfill(' ');

    return ss.str();
}

void Logging::log(std::string level, std::string fmt, std::va_list args) {
    char formatBuf[512];

    vsprintf(formatBuf, fmt.c_str(), args);

    std::stringstream ss;
    ss << getDateTimeString() << " " << level.c_str() << " " << formatBuf << "\n";
    std::string buf = ss.str();
    printf(buf.c_str());
    logFile << buf.c_str();
}

void Logging::info(std::string fmt, ...) {
    std::va_list args;
    va_start(args, fmt);
    log("[I]", fmt, args);
    va_end(args);
}

void Logging::error(std::string fmt, ...) {
    std::va_list args;
    va_start(args, fmt);
    log("[E]", fmt, args);
    va_end(args);
}