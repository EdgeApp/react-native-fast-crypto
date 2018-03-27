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
#include <openssl/evp.h>
#include <openssl/sha.h>
// crypto.h used for the version
#include <openssl/crypto.h>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/math/secp256k1_initializer.hpp>

#include "../minilibs/scrypt/crypto_scrypt.h"

#define COMPRESSED_PUBKEY_LENGTH 33
#define DECOMPRESSED_PUBKEY_LENGTH 65
#define PRIVKEY_LENGTH 64


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


// Must pass a privateKey of length 64 bytes
void fast_crypto_secp256k1_ec_pubkey_create(const char *szPrivateKeyHex, char *szPublicKeyHex, int compressed)
{
    const auto signing_context = libbitcoin::signing.context();

    // int flags = compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED;
    uint8_t privateKey[PRIVKEY_LENGTH];
    szPublicKeyHex[0] = 0;
    int publicKeyLen;

    bool success = hexToBytes(szPrivateKeyHex, privateKey);
    if (!success) {
        return;
    }

    unsigned char public_key[DECOMPRESSED_PUBKEY_LENGTH];
    if (secp256k1_ec_pubkey_create(signing_context, public_key, &publicKeyLen, privateKey, compressed) == 0) {
        return;
    }

    bytesToHex(public_key, publicKeyLen, szPublicKeyHex);
}

// secp256k1_pubkey public_key;

void fast_crypto_secp256k1_ec_privkey_tweak_add(char *szPrivateKeyHex, const char *szTweak) {
    const auto verification_context = libbitcoin::verification.context();

    int privateKeyLen = strlen(szPrivateKeyHex) / 2;
    unsigned char privateKey[DECOMPRESSED_PUBKEY_LENGTH];
    unsigned char tweak[DECOMPRESSED_PUBKEY_LENGTH];

    bool success = hexToBytes(szPrivateKeyHex, privateKey);
    if (!success) {
        return;
    }
    success = hexToBytes(szTweak, tweak);
    if (!success) {
        return;
    }
    if (secp256k1_ec_privkey_tweak_add(verification_context, privateKey, (unsigned char *) tweak) == 1) {
        bytesToHex((uint8_t *)privateKey, privateKeyLen, szPrivateKeyHex);
    }
}

void fast_crypto_secp256k1_ec_pubkey_tweak_add(char *szPublicKeyHex, const char *szTweak, int compressed) {
    if (compressed != 1) {
        szPublicKeyHex[0] = 0;
        return;
    }

    const auto verification_context = libbitcoin::verification.context();

    int publicKeyLen = compressed ? COMPRESSED_PUBKEY_LENGTH : DECOMPRESSED_PUBKEY_LENGTH;

    unsigned char publicKey[DECOMPRESSED_PUBKEY_LENGTH];
    unsigned char tweak[DECOMPRESSED_PUBKEY_LENGTH];

    bool success = hexToBytes(szPublicKeyHex, publicKey);
    if (!success) {
        szPublicKeyHex[0] = 0;
        return;
    }

    success = hexToBytes(szTweak, tweak);
    if (!success) {
        szPublicKeyHex[0] = 0;
        return;
    }


    if (secp256k1_ec_pubkey_tweak_add(verification_context, publicKey, publicKeyLen, tweak) == 0) {
        szPublicKeyHex[0] = 0;
        return;
    }

    bytesToHex((uint8_t *)publicKey, publicKeyLen, szPublicKeyHex);
}

void fast_crypto_pbkdf2_sha512(const char *szPassHex, const char *szSaltHex, int iterations, int outputBytes, char* szResultHex) {
     unsigned int i;
     unsigned char digest[outputBytes];

     int passlen = strlen(szPassHex) / 2;
     int saltlen = strlen(szSaltHex) / 2;
     uint8_t pass[passlen];
     uint8_t salt[saltlen];
     hexToBytes(szPassHex, pass);
     hexToBytes(szSaltHex, salt);

     PKCS5_PBKDF2_HMAC((const char *) pass, passlen, (const unsigned char *) salt, saltlen, iterations, EVP_sha512(), outputBytes, digest);
     bytesToHex(digest, sizeof(digest), szResultHex);
}