
package co.airbitz.fastcrypto;

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
    public native String scryptJNI(String passwd, String salt, int N, int r, int p, int size);
    public native String secp256k1EcPubkeyCreateJNI(String privateKeyHex, int compressed);
    public native String secp256k1EcPrivkeyTweakAddJNI(String privateKeyHex, String tweakHex);
    public native String secp256k1EcPubkeyTweakAddJNI(String publicKeyHex, String tweakHex, int compressed);
    public native String pbkdf2Sha512JNI(String passHex, String saltHex, int iterations, int outputBytes);

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
            String reply = scryptJNI(passwd, salt, N, r, p, size); // test response from JNI
            Log.d("scryptJNI", String.format("reply = %s", reply));
            promise.resolve(reply);
        } catch (Exception e) {
            promise.reject("Err", e);
        }
    }

    @ReactMethod
    public void secp256k1EcPubkeyCreate(
            String privateKeyHex,
            Boolean compressed,
            Promise promise) {
        int iCompressed = compressed ? 1 : 0;
        try {
            String reply = secp256k1EcPubkeyCreateJNI(privateKeyHex, iCompressed); // test response from JNI
            Log.d("EcPubkeyCreateJNI", String.format("reply = %s", reply));
            promise.resolve(reply);
        } catch (Exception e) {
            promise.reject("Err", e);
        }
    }

    @ReactMethod
    public void secp256k1EcPrivkeyTweakAdd(
            String privateKeyHex,
            String tweakHex,
            Promise promise) {
        try {
            String reply = secp256k1EcPrivkeyTweakAddJNI(privateKeyHex, tweakHex); // test response from JNI
            Log.d("EcPrivkeyTweakAddJNI", String.format("reply = %s", reply));
            promise.resolve(reply);
        } catch (Exception e) {
            promise.reject("Err", e);
        }
    }

    @ReactMethod
    public void secp256k1EcPubkeyTweakAdd(
            String publicKeyHex,
            String tweakHex,
            Boolean compressed,
            Promise promise) {
        int iCompressed = compressed ? 1 : 0;
        try {
            String reply = secp256k1EcPubkeyTweakAddJNI(publicKeyHex, tweakHex, iCompressed); // test response from JNI
            Log.d("EcPubkeyTweakAddJNI", String.format("reply = %s", reply));
            promise.resolve(reply);
        } catch (Exception e) {
            promise.reject("Err", e);
        }
    }

    @ReactMethod
    public void pbkdf2Sha512(
            String passHex,
            String saltHex,
            int iterations,
            int outputBytes,
            Promise promise) {
        try {
            String reply = pbkdf2Sha512JNI(passHex, saltHex, iterations, outputBytes); // test response from JNI
            Log.d("pbkdf2Sha512JNI", String.format("reply = %s", reply));
            promise.resolve(reply);
        } catch (Exception e) {
            promise.reject("Err", e);
        }
    }
}
