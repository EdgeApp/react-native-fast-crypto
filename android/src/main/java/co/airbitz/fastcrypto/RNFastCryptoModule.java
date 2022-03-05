package co.airbitz.fastcrypto;

import android.util.Base64;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;

public class RNFastCryptoModule extends ReactContextBaseJavaModule {

  // this loads the library when the class is loaded
  static {
    System.loadLibrary("nativecrypto");
    System.loadLibrary("crypto_bridge"); // this loads the library when the class is loaded
  }

  public native String scryptJNI(String passwd, String salt, int N, int r, int p, int size);

  public native String secp256k1EcPubkeyCreateJNI(String privateKeyHex, int compressed);

  public native String secp256k1EcPrivkeyTweakAddJNI(String privateKeyHex, String tweakHex);

  public native String secp256k1EcPubkeyTweakAddJNI(
      String publicKeyHex, String tweakHex, int compressed);

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
  public void pbkdf2Sha512(
      String data64, String salt64, int iterations, int keyLength, Promise promise) {
    try {
      byte[] data = Base64.decode(data64, Base64.DEFAULT);
      byte[] salt = Base64.decode(salt64, Base64.DEFAULT);

      // Pack our arguments into an object:
      char[] dataChars = new String(data, "UTF-8").toCharArray();
      PBEKeySpec keySpec = new PBEKeySpec(dataChars, salt, iterations, keyLength);

      SecretKeyFactory secretKeyFactory = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA512");
      byte[] out = secretKeyFactory.generateSecret(keySpec).getEncoded();
      promise.resolve(Base64.encodeToString(out, Base64.NO_WRAP));
    } catch (Exception e) {
      promise.reject(e);
    }
  }

  @ReactMethod
  public void scrypt(
      String passwd, String salt, Integer N, Integer r, Integer p, Integer size, Promise promise) {
    try {
      String reply = scryptJNI(passwd, salt, N, r, p, size); // test response from JNI
      promise.resolve(reply);
    } catch (Exception e) {
      promise.reject("Err", e);
    }
  }

  @ReactMethod
  public void secp256k1EcPubkeyCreate(String privateKeyHex, Boolean compressed, Promise promise) {
    int iCompressed = compressed ? 1 : 0;
    try {
      String reply =
          secp256k1EcPubkeyCreateJNI(privateKeyHex, iCompressed); // test response from JNI
      promise.resolve(reply);
    } catch (Exception e) {
      promise.reject("Err", e);
    }
  }

  @ReactMethod
  public void secp256k1EcPrivkeyTweakAdd(String privateKeyHex, String tweakHex, Promise promise) {
    try {
      String reply =
          secp256k1EcPrivkeyTweakAddJNI(privateKeyHex, tweakHex); // test response from JNI
      promise.resolve(reply);
    } catch (Exception e) {
      promise.reject("Err", e);
    }
  }

  @ReactMethod
  public void secp256k1EcPubkeyTweakAdd(
      String publicKeyHex, String tweakHex, Boolean compressed, Promise promise) {
    int iCompressed = compressed ? 1 : 0;
    try {
      String reply =
          secp256k1EcPubkeyTweakAddJNI(
              publicKeyHex, tweakHex, iCompressed); // test response from JNI
      promise.resolve(reply);
    } catch (Exception e) {
      promise.reject("Err", e);
    }
  }
}
