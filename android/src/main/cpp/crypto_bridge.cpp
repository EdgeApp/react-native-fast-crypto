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

#include <string.h>

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