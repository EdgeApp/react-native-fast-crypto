/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "Util.hpp"

namespace abcd {

void
stringFree(char *string)
{
    if (string)
    {
        ABC_UtilGuaranteedMemset(string, 0, strlen(string));
        free(string);
    }
}

char *
stringCopy(const char *string)
{
    auto out = strdup(string);
    if (!out)
        throw std::bad_alloc();
    return out;
}

char *
stringCopy(const std::string &string)
{
    return stringCopy(string.c_str());
}

/**
 * For security reasons, it is important that we always make sure memory is set the way we expect
 * this function should ensure that
 * reference: http://www.dwheeler.com/secure-programs/Secure-Programs-HOWTO/protect-secrets.html
 */
void *ABC_UtilGuaranteedMemset(void *v, int c, size_t n)
{
    if (v)
    {
        volatile char *p = (char *)v;
        while (n--)
        {
            *p++ = c;
        }
    }

    return v;
}

} // namespace abcd
