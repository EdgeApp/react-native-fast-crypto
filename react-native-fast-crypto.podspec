require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))

Pod::Spec.new do |s|
  s.name         = package["name"]
  s.version      = package["version"]
  s.summary      = package["description"]
  s.homepage     = package["homepage"]
  s.license      = package["license"]
  s.authors      = package["author"]

  s.platform = :ios, "8.0"
  s.requires_arc = true
  s.source = {
    :git => "https://github.com/EdgeApp/react-native-fast-crypto.git",
    :tag => "v#{s.version}"
  }
  s.source_files =
    "android/jni/include/secp256k1.h",
    "android/src/main/cpp/native-crypto.cpp",
    "android/src/main/cpp/native-crypto.h",
    "android/src/main/cpp/scrypt/crypto_scrypt.c",
    "android/src/main/cpp/scrypt/crypto_scrypt.h",
    "android/src/main/cpp/scrypt/sha256.c",
    "android/src/main/cpp/scrypt/sha256.h",
    "android/src/main/cpp/scrypt/sysendian.h",
    "ios/RNFastCrypto.h",
    "ios/RNFastCrypto.m"
  s.vendored_frameworks =
    "ios/secp256k1.xcframework"

  s.dependency "React"
end
