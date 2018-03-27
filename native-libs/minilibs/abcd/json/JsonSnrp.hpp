/*
 * Copyright (c) 2015, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#ifndef ABCD_JSON_JSON_SNRP_HPP
#define ABCD_JSON_JSON_SNRP_HPP

#include "JsonObject.hpp"
#include "../crypto/Scrypt.hpp"

namespace abcd {

/**
 * The scrypt parameter serialization format.
 */
struct JsonSnrp:
    public JsonObject
{
    ABC_JSON_CONSTRUCTORS(JsonSnrp, JsonObject)

    ABC_JSON_STRING(salt, "salt_hex", nullptr)
    ABC_JSON_INTEGER(n, "n", 0)
    ABC_JSON_INTEGER(r, "r", 0)
    ABC_JSON_INTEGER(p, "p", 0)

    Status
    snrpGet(ScryptSnrp &result) const;

    Status
    snrpSet(const ScryptSnrp &snrp);

    Status
    create();

    Status
    hash(DataChunk &result, DataSlice data) const;
};

} // namespace abcd

#endif
