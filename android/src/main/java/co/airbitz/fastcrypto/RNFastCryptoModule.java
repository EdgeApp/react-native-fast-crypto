
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
    }
    public native String scryptJNI();

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
//            String reply = scryptJNI(); // test response from JNI
            promise.resolve("scrypt returned this");
        } catch (Exception e) {
            promise.reject("Err", e);
        }
    }
}
