# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.4.1)

add_library(native_crypto SHARED
            native_crypto.cpp)

# Include libraries needed for native_crypto lib
target_link_libraries(native_crypto
                      android
                      log)

