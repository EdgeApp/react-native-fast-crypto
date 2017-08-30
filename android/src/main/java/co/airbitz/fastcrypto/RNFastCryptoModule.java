
package co.airbitz.fastcrypto;

import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.Callback;


public class RNFastCryptoModule extends ReactContextBaseJavaModule {

    //this loads the library when the class is loaded
    static {
        System.loadLibrary("stdc++");
        System.loadLibrary("nativecrypto");
        System.loadLibrary("fast_crypto_jni"); //this loads the library when the class is loaded
    }
    public native String fastCryptoJNI();

    private final ReactApplicationContext reactContext;

    public RNFastCryptoModule(ReactApplicationContext reactContext) {
        super(reactContext);
        this.reactContext = reactContext;
    }

    @Override
    public String getName() {
        return "RNFastCrypto";
    }

    @ReactMethod
    public void scrypt(
            String passwd,
            String salt,
            Integer N,
            Integer r,
            Integer p,
            Integer size,
            Promise promise) {
        try {
           String reply = fastCryptoJNI(); // test response from JNI
// void fast_crypto_scrypt (const uint8_t *passwd, size_t passwdlen, const uint8_t *salt, size_t saltlen, uint64_t N,
// uint32_t r, uint32_t p, uint8_t *buf, size_t buflen)
            promise.resolve(reply);
        } catch (Exception e) {
            promise.reject("Err", e);
        }
    }
}
