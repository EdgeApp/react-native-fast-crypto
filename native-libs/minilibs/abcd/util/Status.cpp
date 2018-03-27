/*
 * Copyright (c) 2015, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "Status.hpp"
#include "Debug.hpp"
#include <string.h>
#include <sstream>

namespace abcd {

std::ostream &operator<<(std::ostream &output, const ErrorLocation &s)
{
    output << s.file << ":" << s.line << ": " << s.function;
    return output;
}

Status::Status() :
    value_(ABC_CC_Ok)
{
}

Status::Status(tABC_CC value, std::string message, ErrorLocation here):
    value_(value),
    message_(message)
{
    backtrace_.push_back(here);
}

void Status::toError(tABC_Error &error, ErrorLocation here) const
{
    log();

    error.code = value_;
    strncpy(error.szDescription, message_.c_str(), ABC_MAX_STRING_LENGTH);
    strncpy(error.szSourceFunc, here.function, ABC_MAX_STRING_LENGTH);
    strncpy(error.szSourceFile, here.file, ABC_MAX_STRING_LENGTH);
    error.nSourceLine = here.line;

    error.szDescription[ABC_MAX_STRING_LENGTH] = 0;
    error.szSourceFunc[ABC_MAX_STRING_LENGTH] = 0;
    error.szSourceFile[ABC_MAX_STRING_LENGTH] = 0;
}

Status Status::fromError(const tABC_Error &error, ErrorLocation here)
{
    static char file[ABC_MAX_STRING_LENGTH + 1];
    static char function[ABC_MAX_STRING_LENGTH + 1];
    strncpy(file, error.szSourceFile, ABC_MAX_STRING_LENGTH);
    strncpy(function, error.szSourceFunc, ABC_MAX_STRING_LENGTH);
    file[ABC_MAX_STRING_LENGTH] = 0;
    function[ABC_MAX_STRING_LENGTH] = 0;
    ErrorLocation location{function, file, static_cast<size_t>(error.nSourceLine)};

    return Status(error.code, error.szDescription, location).at(here);
}

const Status &
Status::log() const
{
    if (!*this)
    {
        std::stringstream s;
        s << *this;
        ABC_DebugLog("%s", s.str().c_str());
    }
    return *this;
}

Status &
Status::at(ErrorLocation here)
{
    backtrace_.push_back(here);
    return *this;
}

std::ostream &operator<<(std::ostream &output, const Status &s)
{
    output << "Error " << s.value() << " (" << s.message() << ")";
    for (const auto &i: s.backtrace())
        output << std::endl << "  at " << i;
    return output;
}

} // namespace abcd
