/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#ifndef ABCD_UTIL_AUTO_FREE_HPP
#define ABCD_UTIL_AUTO_FREE_HPP

namespace abcd {

/**
 * A wrapper for C-style resources that need to be freed.
 * This is similar to the standard C++ unique_ptr,
 * but it is possible to mutate the underlying pointer through `get`.
 */
template<typename T, void Free(T *p)>
class AutoFree
{
public:
    ~AutoFree()
    {
        if (p_)
            Free(p_);
    }

    AutoFree():
        p_(nullptr)
    {}
    explicit AutoFree(T *p):
        p_(p)
    {}

    /**
     * Obtains a writable reference to the underlying pointer.
     */
    T *&
    get()
    {
        return p_;
    }

    /**
     * Returns the underlying pointer, passing ownership to the caller.
     */
    T *
    release()
    {
        auto out = p_;
        p_ = nullptr;
        return out;
    }

    operator T *() { return p_; }
    T *operator ->() { return p_; }

private:
    T *p_;
};

} // namespace abcd

#endif
