# react-native-fast-crypto

This library implements fast, fully native crypto routines for React Native under iOS and Android. Fully built binaries are committed for both platforms but can also be built from scratch.

## Getting started

This library directly supports React Native v0.60+ with autolinking. Simply add react-native-fast-crypto to your application, and React Native will link the necessary native modules into your app.

`npm install react-native-fast-crypto --save`

## Usage

```javascript
import { scrypt } from 'react-native-fast-crypto';

const data = Uint8Array.from([1, 250, 3, 4, 34, 64, 39, 43, 12])
const salt = Uint8Array.from([45, 124, 45, 29, 172, 238, 35])

const result: Uint8Array = await crypto.scrypt(data, salt, 16384, 8, 1, 32)
console.log(result)
```

## Developing

This library relies on native C++ code from other repos. To integrate this code, you must run the following script before publishing this library to NPM:

```sh
npm run build-native
```

This script does the following tasks:

- Download third-party source code (libsecp256k1).
- Compile shared libraries for Android.
- Compile an iOS universal static library and put it into an XCFramework.

The `build-native` script is also the place to make edits when upgrading any of the third-party dependencies.

For this to work, you need:

- A recent Android SDK, installed at `$ANDROID_HOME`
- Xcode command-line tools
- `cmake`, provided by `brew install cmake`
- `llvm-objcopy`, provided by `brew install llvm`
