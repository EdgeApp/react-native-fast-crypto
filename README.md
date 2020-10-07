
# react-native-fast-crypto

This library implements fast, fully native crypto routines for React Native under iOS and Android. Fully built binaries are committed for both platforms but can also be built from scratch. 

## Getting started

`npm install react-native-fast-crypto --save`

### Mostly automatic installation

`react-native link react-native-fast-crypto`

### Manual installation

#### Install in iOS app

1. In XCode, in the project navigator, right click `Libraries` ➜ `Add Files to [your project's name]`
2. Go to `node_modules` ➜ `react-native-fast-crypto` and add `RNFastCrypto.xcodeproj`
3. In XCode, in the project navigator, select your project. Add `libRNFastCrypto.a` to your project's `Build Phases` ➜ `Link Binary With Libraries`
4. Run your project (`Cmd+R`)<

#### Install in Android app

1. Open up `android/app/src/main/java/[...]/MainActivity.java`
  - Add `import com.reactlibrary.RNFastCryptoPackage;` to the imports at the top of the file
  - Add `new RNFastCryptoPackage()` to the list returned by the `getPackages()` method
2. Append the following lines to `android/settings.gradle`:
  	```
  	include ':react-native-fast-crypto'
  	project(':react-native-fast-crypto').projectDir = new File(rootProject.projectDir, 	'../node_modules/react-native-fast-crypto/android')
  	```
3. Insert the following lines inside the dependencies block in `android/app/build.gradle`:
  	```
      compile project(':react-native-fast-crypto')
  	```

## Usage
```javascript
import { scrypt } from 'react-native-fast-crypto';

const data = new Uint8Array([1, 250, 3, 4, 34, 64, 39, 43, 12])
const salt = new Uint8Array([45, 124, 45, 29, 172, 238, 35])

const result: Uint8Array = await crypto.scrypt(data, salt, 16384, 8, 1, 32)
console.log(result)
```

## Build the C/C++ binaries from scratch (optional)

The build process requires several pieces of software to be installed on the
host system:

* autoconf
* automake
* cmake
* git
* libtool
* pkgconfig
* protobuf
* astyle

To install these on the Mac, please use [Homebrew](http://brew.sh/):

    brew install autoconf automake cmake git libtool pkgconfig protobuf astyle

The 'wget' and 'cmake' that come from MacPorts are known to be broken.
If you are building for iOS or Mac native, you also need a working installation
of the XCode command-line tools.

### Xcode 10

Xcode 10 removes the /usr/include files so they must be re-installed by using the package at

`/Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_10.14.pkg`

To build `npm run build`. This can take quite a long time as it builds binaries for multiple different architectures.
