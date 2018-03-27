/*
 * Copyright (c) 2015, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#ifndef ABCD_CRYPTO_ENCODING_HPP
#define ABCD_CRYPTO_ENCODING_HPP

#include "../util/Data.hpp"
#include "../util/Status.hpp"

namespace abcd {

/**
 * Encodes data into a hex string.
 */
std::string
base16Encode(DataSlice data);

/**
 * Decodes a hex string.
 */
Status
base16Decode(DataChunk &result, const std::string &in);

/**
 * Encodes data into a base-32 string according to rfc4648.
 */
std::string
base32Encode(DataSlice data);

/**
 * Decodes a base-32 string as defined by rfc4648.
 */
Status
base32Decode(DataChunk &result, const std::string &in);

/**
 * Encodes data into a base-58 string as used by Bitcoin.
 */
std::string
base58Encode(DataSlice data);

/**
 * Decodes a base-58 string as used by Bitcoin.
 */
Status
base58Decode(DataChunk &result, const std::string &in);

/**
 * Encodes data into a base-64 string according to rfc4648.
 */
std::string
base64Encode(DataSlice data);

/**
 * Decodes a base-64 string as defined by rfc4648.
 */
Status
base64Decode(DataChunk &result, const std::string &in);

} // namespace abcd

#endif
