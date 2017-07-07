
package co.airbitz.fastcrypto;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.Callback;


public class RNFastCryptoModule extends ReactContextBaseJavaModule {

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
  public void scrypt(String passwd,
                     String salt,
                     Integer N,
                     Integer r,
                     Integer p,
                     Integer size) {

  }

}
