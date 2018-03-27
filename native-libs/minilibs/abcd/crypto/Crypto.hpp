/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */
/**
 * @file
 * AirBitz cryptographic function wrappers.
 */

#ifndef ABCD_CRYPTO_CRYPTO_HPP
#define ABCD_CRYPTO_CRYPTO_HPP

#include "../util/U08Buf.hpp"
#include "../../src/ABC.h"
#include <jansson.h>

namespace abcd {

#define AES_256_IV_LENGTH       16
#define AES_256_BLOCK_LENGTH    16
#define AES_256_KEY_LENGTH      32

DataChunk
hmacSha256(DataSlice data, DataSlice key);

/**
 * Creates a cryptographically secure filename from a meaningful name
 * and a secret key.
 * This prevents the filename from leaking information about its contents
 * to anybody but the key holder.
 */
std::string
cryptoFilename(DataSlice key, const std::string &name);

tABC_CC ABC_CryptoEncryptAES256Package(DataSlice         Data,
                                       DataSlice         Key,
                                       tABC_U08Buf       *pEncData,
                                       DataChunk         &IV,
                                       tABC_Error        *pError);

tABC_CC ABC_CryptoDecryptAES256Package(DataChunk &result,
                                       DataSlice EncData,
                                       DataSlice Key,
                                       DataSlice IV,
                                       tABC_Error *pError);

} // namespace abcd

#endif
