# react-native-fast-crypto

## Unreleased

## 2.2.1 (2025-07-11)

- fixed: Malloc proper size for secp256k1 keys.

## 2.2.0 (2022-03-18)

- changed: Compile secp256k1 as an XCFramework, making it compatible with the iOS simulator on M1 Macs.

## 2.1.1 (2022-03-16)

- fixed: Return the correct number of bytes from PBKDF2 on Android.
- fixed: Include the correct libraries in the APK when using Android Gradle plugin v3 or below.
- fixed: Stop including extra files in the NPM bundle that should not have been there.
- fixed: Remove the iOS dependency on OpenSSL.
- added: Unit tests for secp256k1 functions.

## 2.1.0 (2022-03-07)

- Remove the OpenSSL-Universal package
- Add a unit-test application
- Upgrade to the Android gradle plugin v3.6.0
- Add code-formatting tools

## 2.0.0 (2020-11-20)

- Remove all Monero features. These live in [react-native-mymonero-core](https://github.com/EdgeApp/react-native-mymonero-core) now.
- Use the OpenSSL-Universal package to provide OpenSSL libraries on iOS. This fixes react-native 0.63 compatibility.
- Remove the deprecated `default` export. This library uses named ES6 exports.

## 1.8.3 (2020-11-19)

- android: Use `implementation` gradle command instead of deprecated `compile`.
- android: Fix some documentation errors.

## 1.8.2 (2020-10-19)

- Upgrade mymonero-core-cpp and mymonero-core-custom for v17 hard fork

## 1.8.1 (2019-04-22)

- Fix compatibility with React Native v0.59.

## 1.8.0 (2019-04-03)

- Add a podspec file.
- Add 64-bit builds for Android.

## 1.7.0 (2019-03-07)

- Upgrade `mymonero-core-cpp` for the v0.14.0 hard fork.
- Remove inappropriate logging in Android.
- Clarify the `peerDependencies` version range for react-native.

## 1.6.0

* Add support for monero core libaries
*
