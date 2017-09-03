//crypto_bridge.cpp
#include <android/log.h>
#include <jni.h>
#include "fast_crypto.h"

#define LOG_TAG "crypto_bridge-JNI"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern "C" {

JNIEXPORT jstring JNICALL
Java_co_airbitz_fastcrypto_RNFastCryptoModule_cryptoBridgeJNI(JNIEnv *env, jobject thiz,
                                                              jstring passwd, jstring salt, jint n,
                                                              jint r, jint p, jint size) {
    LOGD("passwd=%s, salt=%s, n=%d, r=%d, p=%d, size=%d", passwd, salt, n, r, p, size);

    // Create password character buffer
    uint8_t *password = (uint8_t *) 0;
    password = 0;
    if (passwd) {
        password = (uint8_t *) env->GetStringUTFChars(passwd, 0);
        if (!password) {
            return env->NewStringUTF("Password error!");
        }
    }
    size_t passwdLen = (size_t) env->GetStringUTFLength(passwd);

    uint8_t *salty = (uint8_t *) 0;
    salty = 0;
    if (salt) {
        salty = (uint8_t *) env->GetStringUTFChars(salt, 0);
        if (!salty) {
            return env->NewStringUTF("Password error!");
        }
    }
    size_t saltLen = (size_t) env->GetStringUTFLength(salt);

    int64_t n64 = n;
    int64_t r64 = r;
    int64_t p64 = p;
    int64_t size64 = size;

    uint8_t *buf;

//    fast_crypto_scrypt(password, passwdLen, salty, saltLen, n64, r64, p64,
//        buf, size64);
//    jstring jresult = env->NewStringUTF((char *) buf);
//    return jresult;

    return env->NewStringUTF("sweet crypto_bridge");
}

}