//fastCrypto.cpp
#include <android/log.h>
#include <jni.h>
#include "fast_crypto.h"

#define LOG_TAG "crypto_bridge-JNI"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern "C" {

/*
 * Copyright (c) 2003 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */
/* ====================================================================
 * Copyright (c) 1995-1999 The Apache Group.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the Apache Group
 *    for use in the Apache HTTP server project (http://www.apache.org/)."
 *
 * 4. The names "Apache Server" and "Apache Group" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache"
 *    nor may "Apache" appear in their names without prior written
 *    permission of the Apache Group.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the Apache Group
 *    for use in the Apache HTTP server project (http://www.apache.org/)."
 *
 * THIS SOFTWARE IS PROVIDED BY THE APACHE GROUP ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE APACHE GROUP OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Group and was originally based
 * on public domain software written at the National Center for
 * Supercomputing Applications, University of Illinois, Urbana-Champaign.
 * For more information on the Apache Group and the Apache HTTP server
 * project, please see <http://www.apache.org/>.
 *
 */

/* Base64 encoder/decoder. Originally Apache file ap_base64.c
 */

#include <string.h>

//#include "base64.h"

/* aaaack but it's fast and const should make it shared text page. */
static const unsigned char pr2six[256] =
        {
                /* ASCII table */
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
                52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
                64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
                64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
        };

int Base64decode_len(const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register int nprbytes;

    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);

    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;

    return nbytesdecoded + 1;
}

int Base64decode(char *bufplain, const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register unsigned char *bufout;
    register int nprbytes;

    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;

    bufout = (unsigned char *) bufplain;
    bufin = (const unsigned char *) bufcoded;

    while (nprbytes > 4) {
        *(bufout++) =
                (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
        *(bufout++) =
                (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
        *(bufout++) =
                (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
        bufin += 4;
        nprbytes -= 4;
    }

    /* Note: (nprbytes == 1) would be an error, so just ingore that case */
    if (nprbytes > 1) {
        *(bufout++) =
                (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    }
    if (nprbytes > 2) {
        *(bufout++) =
                (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    }
    if (nprbytes > 3) {
        *(bufout++) =
                (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    }

    *(bufout++) = '\0';
    nbytesdecoded -= (4 - nprbytes) & 3;
    return nbytesdecoded;
}

static const char basis_64[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int Base64encode_len(int len)
{
    return ((len + 2) / 3 * 4) + 1;
}

int Base64encode(char *encoded, const char *string, int len)
{
    int i;
    char *p;

    p = encoded;
    for (i = 0; i < len - 2; i += 3) {
        *p++ = basis_64[(string[i] >> 2) & 0x3F];
        *p++ = basis_64[((string[i] & 0x3) << 4) |
                        ((int) (string[i + 1] & 0xF0) >> 4)];
        *p++ = basis_64[((string[i + 1] & 0xF) << 2) |
                        ((int) (string[i + 2] & 0xC0) >> 6)];
        *p++ = basis_64[string[i + 2] & 0x3F];
    }
    if (i < len) {
        *p++ = basis_64[(string[i] >> 2) & 0x3F];
        if (i == (len - 1)) {
            *p++ = basis_64[((string[i] & 0x3) << 4)];
            *p++ = '=';
        }
        else {
            *p++ = basis_64[((string[i] & 0x3) << 4) |
                            ((int) (string[i + 1] & 0xF0) >> 4)];
            *p++ = basis_64[((string[i + 1] & 0xF) << 2)];
        }
        *p++ = '=';
    }

    *p++ = '\0';
    return p - encoded;
}

JNIEXPORT jstring JNICALL
Java_co_airbitz_fastcrypto_RNFastCryptoModule_scryptJNI(JNIEnv *env, jobject thiz,
                                                        jstring jsPassword, jstring jsSalt, jint N,
                                                        jint r, jint p, jint size) {
    // Create password character buffer
    char *szPassword = (char *) 0;
    szPassword = 0;
    if (jsPassword) {
        szPassword = (char *) env->GetStringUTFChars(jsPassword, 0);
        if (!szPassword) {
            return env->NewStringUTF("Password error!");
        }
    }

    char *szSalt = (char *) 0;
    szSalt = 0;
    if (jsSalt) {
        szSalt = (char *) env->GetStringUTFChars(jsSalt, 0);
        if (!szSalt) {
            return env->NewStringUTF("Salt error!");
        }
    }
    LOGD("passwd=%s, salt=%s, n=%d, r=%d, p=%d, size=%d", szPassword, szSalt, N, r, p, size);

    // Base64 decode string into a buffer
    size_t passwordBufSize = Base64decode_len(szPassword);
    size_t saltBufSize = Base64decode_len(szSalt);

    unsigned char *passwordBuf = (unsigned char *) malloc(sizeof(char) * passwordBufSize);
    unsigned char *saltBuf = (unsigned char *) malloc(sizeof(char) * saltBufSize);

    int passwordBufLen = Base64decode((char *)passwordBuf, szPassword);
    int saltBufLen = Base64decode((char *)saltBuf, szSalt);

    LOGD("passwordBuf len=%d :%d %d %d %d %d %d", passwordBufLen, passwordBuf[0], passwordBuf[1], passwordBuf[2], passwordBuf[3], passwordBuf[4], passwordBuf[5]);
    LOGD("saltBuf len=%d :%d %d %d %d %d %d", saltBufLen, saltBuf[0], saltBuf[1], saltBuf[2], saltBuf[3], saltBuf[4], saltBuf[5]);

    uint8_t *buffer = (uint8_t *) malloc(sizeof(char) * size);

    fast_crypto_scrypt((uint8_t *) passwordBuf, passwordBufLen, (uint8_t *) saltBuf, saltBufLen, N, r, p, buffer, size);

    LOGD("buffer:%d %d %d %d %d %d", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
    int b64encSize = Base64encode_len(size);

    char *szB64Encoded = (char *)malloc(sizeof(char) * b64encSize);
    int b64encLen = Base64encode(szB64Encoded, (const char *) buffer, size);

    LOGD("result szB64Encoded:%s len:%d", szB64Encoded, b64encLen);

    jstring out = env->NewStringUTF(szB64Encoded);
    free(buffer);
    free(szB64Encoded);
    free(passwordBuf);
    free(saltBuf);
    env->ReleaseStringUTFChars(jsPassword, szPassword);
    env->ReleaseStringUTFChars(jsSalt, szSalt);

    return out;
}

JNIEXPORT jstring JNICALL
Java_co_airbitz_fastcrypto_RNFastCryptoModule_secp256k1EcPubkeyCreateJNI(JNIEnv *env, jobject thiz,
                                                        jstring jsPrivateKeyHex, jint jiCompressed) {
    char *szPrivateKeyHex = (char *) 0;
    szPrivateKeyHex = 0;
    if (jsPrivateKeyHex) {
        szPrivateKeyHex = (char *) env->GetStringUTFChars(jsPrivateKeyHex, 0);
        if (!szPrivateKeyHex) {
            return env->NewStringUTF("Invalid private key error!");
        }
    }
    int privateKeyLen = strlen(szPrivateKeyHex);

    char szPublicKeyHex[privateKeyLen * 2];

    fast_crypto_secp256k1_ec_pubkey_create(szPrivateKeyHex, szPublicKeyHex, jiCompressed);
    jstring out = env->NewStringUTF(szPublicKeyHex);
    env->ReleaseStringUTFChars(jsPrivateKeyHex, szPrivateKeyHex);

    return out;
}

JNIEXPORT jstring JNICALL
Java_co_airbitz_fastcrypto_RNFastCryptoModule_secp256k1EcPrivkeyTweakAddJNI(JNIEnv *env, jobject thiz,
                                                                            jstring jsPrivateKeyHex,
                                                                            jstring jsTweakHex) {
    char *szPrivateKeyHexTemp = (char *) 0;
    char *szTweakHex = (char *) 0;

    if (jsPrivateKeyHex) {
        szPrivateKeyHexTemp = (char *) env->GetStringUTFChars(jsPrivateKeyHex, 0);
        if (!szPrivateKeyHexTemp) {
            return env->NewStringUTF("Invalid private key error!");
        }
    }

    int privateKeyLen = strlen(szPrivateKeyHexTemp);
    char szPrivateKeyHex[privateKeyLen + 1];
    strcpy(szPrivateKeyHex, (const char *) szPrivateKeyHexTemp);

    if (jsTweakHex) {
        szTweakHex = (char *) env->GetStringUTFChars(jsTweakHex, 0);
        if (!szTweakHex) {
            env->ReleaseStringUTFChars(jsPrivateKeyHex, szPrivateKeyHexTemp);
            return env->NewStringUTF("Invalid tweak error!");
        }
    }

    fast_crypto_secp256k1_ec_privkey_tweak_add(szPrivateKeyHex, szTweakHex);
    jstring out = env->NewStringUTF(szPrivateKeyHex);
    env->ReleaseStringUTFChars(jsPrivateKeyHex, szPrivateKeyHexTemp);
    env->ReleaseStringUTFChars(jsTweakHex, szTweakHex);
    return out;
}

JNIEXPORT jstring JNICALL
Java_co_airbitz_fastcrypto_RNFastCryptoModule_secp256k1EcPubkeyTweakAddJNI(JNIEnv *env, jobject thiz,
                                                                           jstring jsPublicKeyHex,
                                                                           jstring jsTweakHex,
                                                                           jint jiCompressed) {
    char *szPublicKeyHexTemp = (char *) 0;
    char *szTweakHex = (char *) 0;

    if (jsPublicKeyHex) {
        szPublicKeyHexTemp = (char *) env->GetStringUTFChars(jsPublicKeyHex, 0);
        if (!szPublicKeyHexTemp) {
            return env->NewStringUTF("Invalid private key error!");
        }
    }

    int publicKeyLen = strlen(szPublicKeyHexTemp);
    char szPublicKeyHex[publicKeyLen + 1];
    strcpy(szPublicKeyHex, (const char *) szPublicKeyHexTemp);

    if (jsTweakHex) {
        szTweakHex = (char *) env->GetStringUTFChars(jsTweakHex, 0);
        if (!szTweakHex) {
            env->ReleaseStringUTFChars(jsPublicKeyHex, szPublicKeyHexTemp);
            return env->NewStringUTF("Invalid tweak error!");
        }
    }

    fast_crypto_secp256k1_ec_pubkey_tweak_add(szPublicKeyHex, szTweakHex, jiCompressed);
    jstring out = env->NewStringUTF(szPublicKeyHex);
    env->ReleaseStringUTFChars(jsPublicKeyHex, szPublicKeyHexTemp);
    env->ReleaseStringUTFChars(jsTweakHex, szTweakHex);
    return out;
}

JNIEXPORT jstring JNICALL
Java_co_airbitz_fastcrypto_RNFastCryptoModule_pbkdf2Sha512JNI(JNIEnv *env, jobject thiz,
                                                                           jstring jsPassHex,
                                                                           jstring jsSaltHex,
                                                                           jint iterations,
                                                                           jint outputBytes) {
    char *szPassHex = (char *) 0;
    char *szSaltHex = (char *) 0;

    if (jsPassHex) {
        szPassHex = (char *) env->GetStringUTFChars(jsPassHex, 0);
        if (!szPassHex) {
            return env->NewStringUTF("Invalid private key error!");
        }
    }

    if (jsSaltHex) {
        szSaltHex = (char *) env->GetStringUTFChars(jsSaltHex, 0);
        if (!szSaltHex) {
            env->ReleaseStringUTFChars(jsPassHex, szPassHex);
            return env->NewStringUTF("Invalid tweak error!");
        }
    }

    char szResultHex[(outputBytes * 2) + 1];
    fast_crypto_pbkdf2_sha512(szPassHex, szSaltHex, iterations, outputBytes, szResultHex);
    jstring out = env->NewStringUTF(szResultHex);
    env->ReleaseStringUTFChars(jsPassHex, szPassHex);
    env->ReleaseStringUTFChars(jsSaltHex, szSaltHex);
    return out;
}
JNIEXPORT jstring JNICALL
Java_co_airbitz_fastcrypto_RNFastCryptoModule_moneroCoreJNI(JNIEnv *env, jobject thiz,
                                                                         jstring jsMethod,
                                                                         jstring jsJsonParams) {
    char *szJsonParams = (char *) 0;
    char *szMethod = (char *) 0;

    if (jsMethod) {
        szMethod = (char *) env->GetStringUTFChars(jsMethod, 0);
        if (!szMethod) {
            return env->NewStringUTF("Invalid monero method!");
        }
    }

    if (jsJsonParams) {
        szJsonParams = (char *) env->GetStringUTFChars(jsJsonParams, 0);
        if (!szJsonParams) {
            env->ReleaseStringUTFChars(jsMethod, szMethod);
            return env->NewStringUTF("Invalid monero jsonParams!");
        }
    }

    char *szResultHex = NULL;
    fast_crypto_monero_core(szMethod, szJsonParams, &szResultHex);
    jstring out = env->NewStringUTF(szResultHex);
    free(szResultHex);
    env->ReleaseStringUTFChars(jsJsonParams, szJsonParams);
    env->ReleaseStringUTFChars(jsMethod, szMethod);
    return out;
}

}