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

## Build libsecp256k1 from scratch (optional)

The build process requires several pieces of software to be installed on the
host system:

* autoconf
* automake
* git
* libtool

To install these on the Mac, please use [Homebrew](http://brew.sh/):

    brew install autoconf automake git libtool

If you are building for iOS, you also need a working installation
of the XCode command-line tools. In addition, Macs with Apple Silicon
will also need to do `softwareupdate --install-rosetta`.

Finally, run `./build-secp256k1.sh`
