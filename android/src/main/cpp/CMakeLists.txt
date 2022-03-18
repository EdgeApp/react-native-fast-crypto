cmake_minimum_required(VERSION 3.4.1)

add_library(
  crypto_bridge
  SHARED
  crypto_bridge.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/../../../jni/libs/${ANDROID_ABI})

add_library(
  nativecrypto
  SHARED
  IMPORTED
)

set_target_properties(
  nativecrypto
  PROPERTIES
  IMPORTED_LOCATION
  ${CMAKE_SOURCE_DIR}/../../../jni/libs/${ANDROID_ABI}/libnativecrypto.so
)

# Include libraries needed for crypto_bridge lib
target_link_libraries(crypto_bridge nativecrypto android log)

# copy out the lib binary... need to leave the static lib around to pass gradle check
set(distribution_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(
  crypto_bridge
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY
  "${distribution_DIR}/lib/${ANDROID_ABI}"
)
