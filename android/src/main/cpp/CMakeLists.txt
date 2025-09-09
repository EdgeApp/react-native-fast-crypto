cmake_minimum_required(VERSION 3.4.1)

# Add support for Android 16KB page sizes
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON")

# Add comprehensive 16KB alignment flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

# Add linker flags for 16KB page alignment
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,max-page-size=16384")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,common-page-size=16384")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")

# Force 16KB alignment for all sections
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,relro,-z,now")

add_compile_options(-fvisibility=hidden -w)
include_directories(${CMAKE_SOURCE_DIR}/scrypt)
include_directories(${CMAKE_SOURCE_DIR}/../../../jni/include)

add_library(
  crypto_bridge
  SHARED
  crypto_bridge.cpp
  native-crypto.cpp
  scrypt/crypto_scrypt.c
  scrypt/sha256.c
)

add_library(
  secp256k1
  SHARED
  IMPORTED
)

set_target_properties(
  secp256k1
  PROPERTIES
  IMPORTED_LOCATION
  ${CMAKE_SOURCE_DIR}/../../../jni/libs/${ANDROID_ABI}/libsecp256k1.so
)

# Include libraries needed for crypto_bridge lib
target_link_libraries(crypto_bridge secp256k1 android log)
