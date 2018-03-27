/*
 * Copyright (c) 2015, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "JsonSnrp.hpp"
#include "../crypto/Encoding.hpp"

namespace abcd {

Status
JsonSnrp::snrpGet(ScryptSnrp &result) const
{
    ABC_CHECK(saltOk());
    ABC_CHECK(nOk());
    ABC_CHECK(rOk());
    ABC_CHECK(pOk());

    ABC_CHECK(base16Decode(result.salt, salt()));
    result.n = n();
    result.r = r();
    result.p = p();
    return Status();
}

Status
JsonSnrp::snrpSet(const ScryptSnrp &snrp)
{
    ABC_CHECK(saltSet(base16Encode(snrp.salt)));
    ABC_CHECK(nSet(snrp.n));
    ABC_CHECK(rSet(snrp.r));
    ABC_CHECK(pSet(snrp.p));
    return Status();
}

Status
JsonSnrp::create()
{
    ScryptSnrp snrp;
    ABC_CHECK(snrp.create());
    ABC_CHECK(snrpSet(snrp));
    return Status();
}

Status
JsonSnrp::hash(DataChunk &result, DataSlice data) const
{
    ScryptSnrp snrp;
    ABC_CHECK(snrpGet(snrp));
    ABC_CHECK(snrp.hash(result, data));
    return Status();
}

} // namespace abcd
