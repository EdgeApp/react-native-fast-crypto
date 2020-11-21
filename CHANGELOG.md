# react-native-fast-crypto

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