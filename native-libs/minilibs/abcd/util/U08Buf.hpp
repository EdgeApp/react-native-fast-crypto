/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */
/**
 * @file An old C-style data slice and associated helpers.
 */

#ifndef ABCD_UTIL_U08BUF_H
#define ABCD_UTIL_U08BUF_H

#include "Data.hpp"
#include <string.h>

namespace abcd {

/**
 * A slice of raw data.
 */
struct U08Buf
{
    U08Buf():
        p_(nullptr), end_(nullptr)
    {}
    U08Buf(unsigned char *data, size_t size):
        p_(data), end_(data + size)
    {}

    /**
     * Casts new-style data to an old-style U08Buf type.
     * DANGER! THIS IS NOT CONST-CORRECT!
     */
    U08Buf(DataSlice slice):
        p_(const_cast<uint8_t *>(slice.data())), end_(p_+ slice.size())
    {}

    size_t size() const { return end_ - p_; }
    unsigned char *data() const { return p_; }
    unsigned char *begin() const { return p_; }
    unsigned char *end() const { return end_; }

    unsigned char *p_;
    unsigned char *end_;
};

/**
 * Frees the buffer contents and sets the pointers to null.
 */
void U08BufFree(U08Buf &self);

/**
 * A data slice that can automatically free itself.
 */
struct AutoU08Buf:
    public U08Buf
{
    ~AutoU08Buf()
    {
        U08BufFree(*this);
    }
};

#define ABC_BUF_NEW(buf, count)             { \
                                                (buf).p_ = (unsigned char*)calloc(count, 1); \
                                                (buf).end_ = (buf).p_ + count; \
                                            }
#define ABC_BUF_APPEND_PTR(buf, ptr, count) { \
                                                auto total = (buf).size() + count; \
                                                (buf).p_ = (unsigned char*)realloc((buf).p_, total); \
                                                (buf).end_ = (buf).p_ + total; \
                                                memcpy((buf).end_ - count, ptr, count); \
                                            }
#define ABC_BUF_DUP(dst, src)               { \
                                                if (0 < (src).size()) \
                                                { \
                                                    (dst).p_ = (unsigned char*)malloc((src).size()); \
                                                    (dst).end_ = (dst).p_ + (src).size(); \
                                                    memcpy((dst).p_, (src).data(), (src).size()); \
                                                } \
                                            }
#define ABC_CHECK_NULL_BUF(arg)             { ABC_CHECK_ASSERT(arg.data() != NULL, ABC_CC_NULLPtr, "NULL ABC_Buf pointer"); }

/* example usage
{
    AutoU08Buf mybuf;                 // declares buf and inits pointers to NULL
    ABC_BUF_NEW(mybuf, 10);           // allocates buffer with 10 elements
    int count = myBuf.size();         // returns the count of elements in the buffer
}
*/

// Compatibility wrappers:
typedef U08Buf tABC_U08Buf;
#define ABC_BUF_FREE(x) U08BufFree(x);

} // namespace abcd

#endif
