/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */
/**
 * @file
 * General-purpose data types and utility macros.
 */

#ifndef ABC_Util_h
#define ABC_Util_h

#include "../../src/ABC.h"
#include "AutoFree.hpp"
#include "Debug.hpp"
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <new>
#include <string>

namespace abcd {

/**
 * Frees a C-style string and sets the pointer to null.
 */
void
stringFree(char *string);

/**
 * Copies a C-style string, throwing an exception if something goes wrong.
 */
char *
stringCopy(const char *string);

char *
stringCopy(const std::string &string);

typedef AutoFree<char, stringFree> AutoString;

/**
 * Allocates a C-style structure.
 */
template<typename T> T *
structAlloc()
{
    auto out = static_cast<T *>(calloc(1, sizeof(T)));
    if (!out)
        throw std::bad_alloc();
    return out;
}

/**
 * Allocates a C-style array.
 */
template<typename T> T *
arrayAlloc(size_t count)
{
    auto out = static_cast<T *>(calloc(count, sizeof(T)));
    if (!out)
        throw std::bad_alloc();
    return out;
}

#ifdef DEBUG
#define ABC_LOG_ERROR(code, err_string) \
    { \
        ABC_DebugLog("Error: %s, code: %d, func: %s, source: %s, line: %d", err_string, code, __FUNCTION__, __FILE__, __LINE__); \
    }
#else
#define ABC_LOG_ERROR(code, err_string) { }
#endif

#define ABC_SET_ERR_CODE(err, set_code) \
    if (err != NULL) { \
        err->code = set_code; \
    }

#define ABC_RET_ERROR(err, desc) \
    { \
        if (pError) \
        { \
            pError->code = err; \
            strcpy(pError->szDescription, desc); \
            strcpy(pError->szSourceFunc, __FUNCTION__); \
            strcpy(pError->szSourceFile, __FILE__); \
            pError->nSourceLine = __LINE__; \
        } \
        cc = err; \
        ABC_LOG_ERROR(cc, desc); \
        goto exit; \
    }

#define ABC_CHECK_ASSERT(assert, err, desc) \
    { \
        if (!(assert)) \
        { \
            ABC_RET_ERROR(err, desc); \
        } \
    } \

#define ABC_CHECK_NULL(arg) \
    { \
        ABC_CHECK_ASSERT(arg != NULL, ABC_CC_NULLPtr, "NULL pointer"); \
    } \

#define ABC_CHECK_RET(err) \
    { \
        cc = err; \
        if (ABC_CC_Ok != cc) \
        { \
            ABC_LOG_ERROR(cc, #err); \
            goto exit; \
        } \
    }

#define ABC_STR_NEW(ptr, count) \
    { \
        ptr = (char*)calloc(count, sizeof(char)); \
        ABC_CHECK_ASSERT(ptr != NULL, ABC_CC_NULLPtr, "calloc failed (returned NULL)"); \
    }

#define ABC_ARRAY_NEW(ptr, count, type) \
    { \
        ptr = (type*)calloc(count, sizeof(type)); \
        ABC_CHECK_ASSERT(ptr != NULL, ABC_CC_NULLPtr, "calloc failed (returned NULL)"); \
    }

#define ABC_ARRAY_RESIZE(ptr, count, type) \
    { \
        ptr = (type*)realloc(ptr, (count)*sizeof(type)); \
        ABC_CHECK_ASSERT(ptr != NULL, ABC_CC_NULLPtr, "realloc failed (returned NULL)"); \
    }

#define ABC_STRLEN(string) (string == NULL ? 0 : strlen(string))

#define ABC_FREE(ptr) \
    { \
        if (ptr != NULL) \
        { \
            free(ptr); \
            ptr = NULL; \
        } \
    }

#define ABC_FREE_STR(str) \
    { \
        stringFree(const_cast<char*>(str)); \
        str = nullptr; \
    }

#define ABC_CLEAR_FREE(ptr, len) \
    { \
        if (ptr != NULL) \
        { \
            if ((len) > 0) \
            { \
                ABC_UtilGuaranteedMemset(ptr, 0, (len)); \
            } \
            free(ptr); \
            ptr = NULL; \
        } \
    }

void *ABC_UtilGuaranteedMemset(void *v, int c, size_t n);

} // namespace abcd

#endif
