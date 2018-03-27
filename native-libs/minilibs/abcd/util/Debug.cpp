/*
 * Copyright (c) 2015, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "Debug.hpp"
#include "FileIO.hpp"
#include "../Context.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#ifdef ANDROID
#include <android/log.h>
#endif
#include <iomanip>
#include <mutex>
#include <sstream>
#include <vector>

namespace abcd {

#define MAX_LOG_SIZE (1 << 19) // Max size 512 KiB

static std::mutex gDebugMutex;
static FILE *gLogFile = nullptr;

Status
debugLogRotate()
{
    if (gLogFile)
    {
        fclose(gLogFile);
        gLogFile = nullptr;
    }

    auto path = gContext->paths.logPath();
    if (fileExists(path))
        rename(path.c_str(), gContext->paths.logPrevPath().c_str());

    gLogFile = fopen(path.c_str(), "w");
    if (!gLogFile)
        return ABC_ERROR(ABC_CC_SysError, "Cannot open " + path);

    return Status();
}

Status
debugInitialize()
{
#ifdef DEBUG
    std::lock_guard<std::mutex> lock(gDebugMutex);
    ABC_CHECK(debugLogRotate());
#endif

    return Status();
}

void
debugTerminate()
{
    std::lock_guard<std::mutex> lock(gDebugMutex);
    if (gLogFile)
    {
        fclose(gLogFile);
        gLogFile = nullptr;
    }
}

DataChunk
debugLogLoad()
{
    DataChunk out1;
    fileLoad(out1, gContext->paths.logPrevPath()).log();

    DataChunk out2;
    fileLoad(out2, gContext->paths.logPath()).log();

    return buildData({out1, out2});
}

void ABC_DebugLog(const char *format, ...)
{
#ifdef DEBUG
    time_t t = time(nullptr);
    struct tm *utc = gmtime(&t);

    std::stringstream date;
    date << std::setfill('0');
    date << std::setw(4) << utc->tm_year + 1900 << '-';
    date << std::setw(2) << utc->tm_mon + 1 << '-';
    date << std::setw(2) << utc->tm_mday << ' ';
    date << std::setw(2) << utc->tm_hour << ':';
    date << std::setw(2) << utc->tm_min << ':';
    date << std::setw(2) << utc->tm_sec << " ABC_Log: ";

    // Get the message length:
    va_list args;
    va_start(args, format);
    char temp[1];
    int size = vsnprintf(temp, sizeof(temp), format, args);
    va_end(args);
    if (size < 0)
        return;

    // Format the message:
    va_start(args, format);
    std::vector<char> message(size + 1);
    vsnprintf(message.data(), message.size(), format, args);
    va_end(args);

    // Put the pieces together:
    std::string out = date.str();
    out.append(message.begin(), message.end() - 1);
    if (out.back() != '\n')
        out.append(1, '\n');

#ifdef ANDROID
    __android_log_print(ANDROID_LOG_DEBUG, "ABC", "%s", out.c_str());
#else
    fprintf(stderr, "%s", out.c_str());
#endif

    std::lock_guard<std::mutex> lock(gDebugMutex);
    if (gLogFile && MAX_LOG_SIZE < ftell(gLogFile))
        debugLogRotate().log();

    if (gLogFile)
    {
        fwrite(out.c_str(), 1, out.size(), gLogFile);
        fflush(gLogFile);
    }
#endif
}

void
logInfo(const std::string &message)
{
    ABC_DebugLog("%s", message.c_str());
}

} // namespace abcd
