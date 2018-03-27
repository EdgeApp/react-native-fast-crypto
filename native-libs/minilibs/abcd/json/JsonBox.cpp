/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "JsonBox.hpp"
#include "../crypto/Crypto.hpp"
#include "../crypto/Encoding.hpp"

namespace abcd {

enum CryptoType
{
    AES256_CBC_AIRBITZ = 0,
};

Status
JsonBox::encrypt(DataSlice data, DataSlice key)
{
    DataChunk nonce;
    AutoU08Buf cyphertext;
    ABC_CHECK_OLD(ABC_CryptoEncryptAES256Package(data, key,
                  &cyphertext, nonce, &error));

    ABC_CHECK(typeSet(AES256_CBC_AIRBITZ));
    ABC_CHECK(nonceSet(base16Encode(nonce)));
    ABC_CHECK(cyphertextSet(base64Encode(cyphertext)));

    return Status();
}

Status
JsonBox::decrypt(DataChunk &result, DataSlice key)
{
    DataChunk nonce;
    ABC_CHECK(nonceOk());
    ABC_CHECK(base16Decode(nonce, this->nonce()));

    DataChunk cyphertext;
    ABC_CHECK(cyphertextOk());
    ABC_CHECK(base64Decode(cyphertext, this->cyphertext()));

    switch (type())
    {
    case AES256_CBC_AIRBITZ:
    {
        ABC_CHECK_OLD(ABC_CryptoDecryptAES256Package(result,
                      cyphertext, key, nonce,
                      &error));
        return Status();
    }

    default:
        return ABC_ERROR(ABC_CC_DecryptError, "Unknown encryption type");
    }
}

} // namespace abcd
