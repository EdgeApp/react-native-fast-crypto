
package co.airbitz.fastcrypto;

import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.Callback;


public class RNFastCryptoModule extends ReactContextBaseJavaModule {

    //this loads the libraries when the class is loaded
    static {
        System.loadLibrary("stdc++");
        System.loadLibrary("nativecrypto");
        System.loadLibrary("crypto_bridge");
    }
    public native String cryptoBridgeJNI(String passwd, String salt, int N, int r, int p, int size);

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
            String reply = cryptoBridgeJNI(passwd, salt, N, r, p, size); // test response from JNI
            promise.resolve(reply);
        } catch (Exception e) {
            promise.reject("Err", e);
        }
    }
}
