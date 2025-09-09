#!/bin/sh
# This is meant to run from the parent folder, like `./scripts/update-tests.sh`

dest=FastCryptoTest/node_modules/react-native-fast-crypto

rm -rf $dest
mkdir -p $dest/android
mkdir -p $dest/lib
mkdir -p $dest/src

cp -rv android/build.gradle $dest/android
cp -rv android/jni $dest/android
cp -rv android/src $dest/android
cp -rv ios $dest
cp -rv lib/index.js $dest/lib
cp -rv package.json $dest
cp -rv react-native-fast-crypto.podspec $dest
