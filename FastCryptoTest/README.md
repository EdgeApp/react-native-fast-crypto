# FastCryptoTest

Simple test app for `react-native-fast-crypto` library.

## Running the tests

```bash
# Install dependencies
yarn install

# Integrate latest code from the parent folder
yarn update-tests
yarn pods

# Start Metro
yarn start

# Run on Android
yarn android

# Run on iOS
yarn ios
```

The app will automatically run crypto tests including:

- pbkdf2 (ASCII and UTF-8 inputs)
- secp256k1 operations
- scrypt key derivation

Test results are displayed on screen.
