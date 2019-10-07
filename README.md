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

## Build the C/C++ binaries from scratch (optional)

### Prerequisites

- brew
- Xcode 10.3

### Setup

1. Setup Xcode

```bash
sudo xcode-select --switch /Applications/Xcode.app
sudo xcodebuild -license
```

2. Install build tools

```bash
brew install autoconf automake cmake git libtool pkgconfig protobuf astyle
```

2. Install SDK headers

```bash
open /Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_10.14.pkg
```

3. Clone the repository

```bash
git clone git@github.com:ExodusMovement/react-native-fast-crypto.git
```

### Build

1. Build binaries

```bash
yarn build
```

## tl;dr

Setup (once)

```bash
# macOS 10.14.6 - Xcode 11.0
sudo xcode-select --switch /Applications/Xcode.app
sudo xcodebuild -license
brew install autoconf automake cmake git libtool pkgconfig protobuf astyle
open /Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_10.14.pkg
```

Build

```bash
rm -r react-native-fast-crypto
git clone git@github.com:ExodusMovement/react-native-fast-crypto.git
cd react-native-fast-crypto && yarn build
```
