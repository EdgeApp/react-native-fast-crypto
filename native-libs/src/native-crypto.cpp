/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "native-crypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <secp256k1.h>
#include "../minilibs/scrypt/crypto_scrypt.h"

void fast_crypto_scrypt (const uint8_t *passwd, size_t passwdlen, const uint8_t *salt, size_t saltlen, uint64_t N,
    uint32_t r, uint32_t p, uint8_t *buf, size_t buflen)
{
    crypto_scrypt(passwd, passwdlen, salt, saltlen, N, r, p, buf, buflen);
}

void bytesToHex(uint8_t * in, int inlen, char * out)
{
    uint8_t * pin = in;
    const char * hex = "0123456789abcdef";
    char * pout = out;
    for(; pin < in+inlen; pout +=2, pin++){
        pout[0] = hex[(*pin>>4) & 0xF];
        pout[1] = hex[ *pin     & 0xF];
    }
    pout[0] = 0;
}

bool hexToBytes(const char * string, uint8_t *outBytes) {
    uint8_t *data = outBytes;

    if(string == NULL) 
       return false;

    size_t slength = strlen(string);
    if(slength % 2 != 0) // must be even
       return false;

    size_t dlength = slength / 2;

    memset(data, 0, dlength);

    size_t index = 0;
    while (index < slength) {
        char c = string[index];
        int value = 0;
        if(c >= '0' && c <= '9')
            value = (c - '0');
        else if (c >= 'A' && c <= 'F') 
            value = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')
             value = (10 + (c - 'a'));
        else
            return false;

        data[(index/2)] += value << (((index + 1) % 2) * 4);

        index++;
    }

    return true;
}

secp256k1_context_t *ctx = NULL;

// Must pass a privateKey of length 64
void fast_crypto_secp256k1_ec_pubkey_create(const char *szPrivateKeyHex, char *szPublicKeyHex, int compressed)
{
    if (ctx == NULL) {
        ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
    }

    uint8_t *privateKey = NULL;
    szPublicKeyHex[0] = 0; 

    privateKey = (uint8_t *) malloc(sizeof(uint8_t) * strlen(szPrivateKeyHex) / 2);
    bool success = hexToBytes(szPrivateKeyHex, privateKey);
    if (!success) {
        return;
    }

    unsigned char pubKey[65];
    int pubKeyLen;
    if (secp256k1_ec_pubkey_create(ctx, pubKey, &pubKeyLen, privateKey, compressed) == 1) {
        bytesToHex(pubKey, pubKeyLen, szPublicKeyHex);
    }
    if (privateKey) {
        free(privateKey);
    }
}

void fast_crypto_secp256k1_ec_privkey_tweak_add(char *szPrivateKeyHex, const char *szTweak) {
    if (ctx == NULL) {
        ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
    }

    int privateKeyLen = strlen(szPrivateKeyHex) / 2;
    unsigned char *privateKey = NULL;
    unsigned char *tweak = NULL;

    privateKey = (unsigned char *) malloc(sizeof(char *) * privateKeyLen);
    tweak = (unsigned char *) malloc(sizeof(char *) * strlen(szTweak) / 2);

    bool success = hexToBytes(szPrivateKeyHex, privateKey);
    if (!success) {
        return;
    }
    success = hexToBytes(szTweak, tweak);
    if (!success) {
        return;
    }
    if (secp256k1_ec_privkey_tweak_add(ctx, privateKey, (unsigned char *) tweak) == 1) {
        bytesToHex((uint8_t *)privateKey, privateKeyLen, szPrivateKeyHex);
    }
    if (privateKey) {
        free(privateKey);
    }
    if (tweak) {
        free(tweak);
    }
}
