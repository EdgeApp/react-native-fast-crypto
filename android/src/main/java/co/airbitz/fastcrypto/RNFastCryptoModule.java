
package co.airbitz.fastcrypto;

import android.os.AsyncTask;
import android.util.Log;

import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.Callback;


public class RNFastCryptoModule extends ReactContextBaseJavaModule {

    //this loads the library when the class is loaded
    static {
        System.loadLibrary("nativecrypto");
        System.loadLibrary("crypto_bridge"); //this loads the library when the class is loaded
    }
    public native String moneroCoreJNI(String method, String jsonParams);

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
    public void moneroCore(
            final String method,
            final String jsonParams,
            final Promise promise) {
        AsyncTask.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    String reply = moneroCoreJNI(method, jsonParams); // test response from JNI
                    promise.resolve(reply);
                } catch (Exception e) {
                    promise.reject("Err", e);
                }
            }
        });
    }
}
