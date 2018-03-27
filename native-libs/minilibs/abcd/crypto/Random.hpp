/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#ifndef ABCD_CRYPTO_RANDOM_HPP
#define ABCD_CRYPTO_RANDOM_HPP

#include "../util/Data.hpp"
#include "../util/Status.hpp"

namespace abcd {

#define DATA_KEY_LENGTH 32
#define BITCOIN_SEED_LENGTH 32

/**
 * Sets the seed for the random number generator.
 */
Status
randomInitialize(DataSlice seed);

/**
 * Generates cryptographically-secure random data.
 */
Status
randomData(DataChunk &result, size_t size);

/**
 * Creates a random version 4 UUID.
 */
Status
randomUuid(std::string &result);

} // namespace abcd

#endif
