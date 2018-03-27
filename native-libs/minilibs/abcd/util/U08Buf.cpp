/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "U08Buf.hpp"
#include "Util.hpp"

namespace abcd {

void U08BufFree(U08Buf &self)
{
    if (self.data())
    {
        ABC_UtilGuaranteedMemset(self.data(), 0, self.size());
        free(self.data());
    }
    self = U08Buf();
}

} // namespace abcd
