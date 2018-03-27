/*
 * Copyright (c) 2015, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#ifndef ABC_UTIL_DEBUG_HPP
#define ABC_UTIL_DEBUG_HPP

#include "Status.hpp"
#include "Data.hpp"

#define DEBUG_LEVEL 1

#define ABC_DebugLevel(level, ...)  \
{                                   \
    if (DEBUG_LEVEL >= level)       \
    {                               \
        ABC_DebugLog(__VA_ARGS__);  \
    }                               \
}

#define ABC_Debug(level, STR)  \
{                                   \
    if (DEBUG_LEVEL >= level)       \
    {                               \
        logInfo(STR);  \
    }                               \
}
namespace abcd {

Status
debugInitialize();

void
debugTerminate();

DataChunk
debugLogLoad();

void ABC_DebugLog(const char *format, ...);

/**
 * Like `ABC_DebugLog`, but takes `std::string`.
 */
void
logInfo(const std::string &message);

} // namespace abcd

#endif
