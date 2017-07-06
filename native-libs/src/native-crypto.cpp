/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "native-crypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../minilibs/scrypt/crypto_scrypt.h"

void fast_crypto_scrypt (const uint8_t *passwd, size_t passwdlen, const uint8_t *salt, size_t saltlen, uint64_t N,
    uint32_t r, uint32_t p, uint8_t *buf, size_t buflen)
{
    crypto_scrypt(passwd, passwdlen, salt, saltlen, N, r, p, buf, buflen);
}
