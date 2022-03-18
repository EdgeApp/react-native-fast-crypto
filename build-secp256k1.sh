#!/bin/bash
set -u
set -e

echo Build Started
date

# Pick up the current directory:
CURRENT_DIR=$(pwd)
export BUILD_DIR=$CURRENT_DIR/native-libs/deps/build
OUT_DIR=$BUILD_DIR/prefix

# Do the build:
(
  cd native-libs/deps
  make \
    libsecp256k1.build-android-arm \
    libsecp256k1.build-android-arm64 \
    libsecp256k1.build-android-x86_64 \
    libsecp256k1.build-android-x86 \
    libsecp256k1.build-iphoneos-arm64 \
    libsecp256k1.build-iphoneos-armv7 \
    libsecp256k1.build-iphoneos-armv7s \
    libsecp256k1.build-iphonesimulator-arm64 \
    libsecp256k1.build-iphonesimulator-i386 \
    libsecp256k1.build-iphonesimulator-x86_64 \
)

# Copy the library to Android:
JNI_DIR=android/jni
mkdir -p $JNI_DIR/libs/armeabi-v7a
mkdir -p $JNI_DIR/libs/arm64-v8a
mkdir -p $JNI_DIR/libs/x86
mkdir -p $JNI_DIR/libs/x86_64
mkdir -p $JNI_DIR/include
cp $OUT_DIR/android/arm/lib/libsecp256k1.so $JNI_DIR/libs/armeabi-v7a/
cp $OUT_DIR/android/arm64/lib/libsecp256k1.so $JNI_DIR/libs/arm64-v8a/
cp $OUT_DIR/android/x86_64/lib/libsecp256k1.so $JNI_DIR/libs/x86_64/
cp $OUT_DIR/android/x86/lib/libsecp256k1.so $JNI_DIR/libs/x86/
cp $OUT_DIR/android/arm/include/secp256k1.h $JNI_DIR/include/

# Use lipo to squash together iOS libraries:
mkdir -p $BUILD_DIR/lipo-iphoneos
mkdir -p $BUILD_DIR/lipo-iphonesimulator
lipo \
  -create \
  $OUT_DIR/iPhoneOS/*/lib/libsecp256k1.a \
  -output \
  $BUILD_DIR/lipo-iphoneos/libsecp256k1.a
lipo \
  -create \
  $OUT_DIR/iPhoneSimulator/*/lib/libsecp256k1.a \
  -output \
  $BUILD_DIR/lipo-iphonesimulator/libsecp256k1.a

# Build an XCFramework out of those:
rm -fr ios/secp256k1.xcframework
xcodebuild -create-xcframework \
  -library $BUILD_DIR/lipo-iphoneos/libsecp256k1.a \
  -library $BUILD_DIR/lipo-iphonesimulator/libsecp256k1.a \
  -output ios/secp256k1.xcframework

echo Build Finished
date
