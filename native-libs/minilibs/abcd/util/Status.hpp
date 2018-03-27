/*
 * Copyright (c) 2015, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#ifndef ABCD_UTIL_STATUS_HPP
#define ABCD_UTIL_STATUS_HPP

// We need tABC_CC and tABC_Error:
#include "../../src/ABC.h"
#include <list>
#include <ostream>
#include <string>

namespace abcd {

/**
 * A source location for reporting errors.
 * The members can be plain pointers,
 * since the strings are statically allocated by the compiler.
 */
struct ErrorLocation
{
    const char *function;
    const char *file;
    size_t line;
};

std::ostream &operator<<(std::ostream &output, const ErrorLocation &s);

#define ABC_HERE() ErrorLocation{__FUNCTION__, __FILE__, __LINE__}

/**
 * Describes the results of calling a core function,
 * which can be either success or failure.
 */
class Status
{
public:
    /**
     * Constructs a success status.
     */
    Status();

    /**
     * Constructs an error status.
     */
    Status(tABC_CC value, std::string message, ErrorLocation here);

    // Read accessors:
    tABC_CC value()             const { return value_; }
    std::string message()       const { return message_; }
    const std::list<ErrorLocation> &backtrace() const { return backtrace_; }

    /**
     * Returns true if the status code represents success.
     */
    explicit operator bool() const { return value_ == ABC_CC_Ok; }

    /**
     * Unpacks this status into a tABC_Error structure.
     */
    void toError(tABC_Error &error, ErrorLocation here) const;

    /**
     * Converts a tABC_Error structure into a Status.
     */
    static Status fromError(const tABC_Error &error, ErrorLocation here);

    /**
     * Prints this status to the ABC log if it is an error.
     */
    const Status &
    log() const;

    /**
     * Adds a source location to the backtrace.
     */
    Status &
    at(ErrorLocation here);

private:
    // Error information:
    tABC_CC value_;
    std::string message_;

    std::list<ErrorLocation> backtrace_;
};

std::ostream &operator<<(std::ostream &output, const Status &s);

/**
 * Constructs an error status using the current source location.
 */
#define ABC_ERROR(value, message) \
    Status(value, message, ABC_HERE())

/**
 * Checks a status code, and returns if it represents an error.
 */
#define ABC_CHECK(f) \
    do { \
        Status ABC_status = (f); \
        if (!ABC_status) \
            return ABC_status.at(ABC_HERE()); \
    } while (false)

/**
 * Use when a new-style function calls an old-style tABC_Error function.
 */
#define ABC_CHECK_OLD(f) \
    do { \
        tABC_CC cc; \
        tABC_Error error; \
        error.code = ABC_CC_Ok; \
        cc = f; \
        if (ABC_CC_Ok != cc) \
            return Status::fromError(error, ABC_HERE()); \
    } while (false)

/**
 * Use when an old-style function calls a new-style abcd::Status function.
 */
#define ABC_CHECK_NEW(f) \
    do { \
        Status ABC_status = (f); \
        if (!ABC_status) { \
            ABC_status.toError(*pError, ABC_HERE()); \
            cc = ABC_status.value(); \
            goto exit; \
        } \
    } while (false)

} // namespace abcd

#endif
