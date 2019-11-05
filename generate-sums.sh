#!/usr/bin/env sh

FILE=shasums.txt

rm -rf $FILE
shasum -a 256 ios/Libraries/* >> $FILE 2>/dev/null
shasum -a 256 android/jni/libs/*/* >> $FILE 2>/dev/null
