//fastCrypto.c
#include <jni.h>

JNIEXPORT jstring JNICALL
Java_co_airbitz_fastcrypto_RNFastCryptoModule_nativeCryptoJNI(JNIEnv* env, jobject thiz) {
  return (*env)->NewStringUTF(env, "Hello FastCrypto!");
}