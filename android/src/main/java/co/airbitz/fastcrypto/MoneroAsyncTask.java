package co.airbitz.fastcrypto;

import android.os.Build;

import com.facebook.react.BuildConfig;
import com.facebook.react.bridge.Promise;

import org.json.JSONObject;

import java.io.DataInputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.ByteBuffer;

public class MoneroAsyncTask extends android.os.AsyncTask<Void, Void, Void> {
    private static final String USER_AGENT;

    static {

        //this loads the library when the class is loaded
        System.loadLibrary("nativecrypto");
        System.loadLibrary("crypto_bridge"); //this loads the library when the class is loaded

        String applicationName = BuildConfig.FLAVOR;
        String version = BuildConfig.VERSION_NAME;
        // modeled after User-Agent on ios: exodus/1.13.0 CFNetwork/978.0.7 Darwin/18.6.0
        USER_AGENT = (applicationName + "/" + version + " Android/" + Build.VERSION.SDK_INT)
                // strip off all non-ASCII characters (just in case)
                .replaceAll("[^\\x00-\\x7F]", "");
    }


    private final Promise promise;
    private final String method;
    private final String jsonParams;
    public MoneroAsyncTask(String method, String jsonParams, Promise promise) {
        this.method = method;
        this.jsonParams = jsonParams;
        this.promise = promise;
    }

    public native String moneroCoreJNI(String method, String jsonParams);
    public native int moneroCoreCreateRequest(ByteBuffer requestBuffer, int height);
    public native String extractUtxosFromBlocksResponse(ByteBuffer buffer, String jsonParams);

    @Override
    protected Void doInBackground(Void... voids) {
        if (method.equals("download_and_process")) {
            try {
                JSONObject params = new JSONObject(jsonParams);
                String addr = params.getString("url");
                int startHeight = params.getInt("start_height");
                ByteBuffer requestBuffer = ByteBuffer.allocateDirect(1000);
                int requestLength = moneroCoreCreateRequest(requestBuffer, startHeight);
                URL url = new URL(addr);
                HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                connection.setRequestMethod("POST");
                connection.setRequestProperty("Content-Type", "application/octet-stream");
                connection.setRequestProperty("User-Agent", USER_AGENT);
                connection.setConnectTimeout(10000);
                connection.setReadTimeout(4 * 60 * 1000);
                connection.setDoOutput(true);
                try (OutputStream outputStream = connection.getOutputStream()) {
                    for (int i = 0; i < requestLength; i++) {
                        outputStream.write(requestBuffer.get(i));
                    }
                }
                connection.connect();
                String contentLength = connection.getHeaderField("Content-Length");
                int responseLength = Integer.parseInt(contentLength);
                try (DataInputStream dataInputStream = new DataInputStream(connection.getInputStream())) {
                    byte[] bytes = new byte[responseLength];
                    dataInputStream.readFully(bytes);
                    ByteBuffer responseBuffer = ByteBuffer.allocateDirect(responseLength);
                    responseBuffer.put(bytes, 0, responseLength);
                    String out = extractUtxosFromBlocksResponse(responseBuffer, jsonParams);
                    promise.resolve(out);
                }
            } catch (Exception e) {
                promise.reject("Err", e);
            }
            return null;
        }
        try {
            String reply = moneroCoreJNI(method, jsonParams); // test response from JNI
            promise.resolve(reply);
        } catch (Exception e) {
            promise.reject("Err", e);
        }
        return null;
    }
};

