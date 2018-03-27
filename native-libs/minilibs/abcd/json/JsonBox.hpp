/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#ifndef ABCD_JSON_JSON_BOX_HPP
#define ABCD_JSON_JSON_BOX_HPP

#include "JsonObject.hpp"
#include "../util/Data.hpp"

namespace abcd {

/**
 * A json object holding encrypted data.
 */
class JsonBox:
    public JsonObject
{
public:
    ABC_JSON_CONSTRUCTORS(JsonBox, JsonObject)

    /**
     * Puts a value into the box, encrypting it with the given key.
     */
    Status
    encrypt(DataSlice data, DataSlice key);

    /**
     * Extracts the value from the box, decrypting it with the given key.
     */
    Status
    decrypt(DataChunk &result, DataSlice key);

private:
    ABC_JSON_INTEGER(type, "encryptionType", 0)
    ABC_JSON_STRING(nonce, "iv_hex", nullptr)
    ABC_JSON_STRING(cyphertext, "data_base64", nullptr)
};

} // namespace abcd

#endif
