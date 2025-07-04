
#import "RNFastCrypto.h"
#import "native-crypto.h"

#import <CommonCrypto/CommonKeyDerivation.h>
#import <Foundation/Foundation.h>
#include <stdbool.h>
#include <stdint.h>

@implementation RNFastCrypto

- (dispatch_queue_t)methodQueue
{
  return dispatch_get_main_queue();
}

RCT_EXPORT_MODULE()

RCT_REMAP_METHOD(
  pbkdf2Sha512,
  pbkdf2Sha512:(NSString *)data64
  salt:(NSString *)salt64
  iterations:(NSInteger)iterations
  size:(NSInteger)size
  resolver:(RCTPromiseResolveBlock)resolve
  rejecter:(RCTPromiseRejectBlock)reject
) {
  NSData *data = [[NSData alloc] initWithBase64EncodedString:data64 options:0];
  NSData *salt = [[NSData alloc] initWithBase64EncodedString:salt64 options:0];
  NSMutableData *out = [NSMutableData dataWithLength:size];

  CCKeyDerivationPBKDF(
    kCCPBKDF2,
    data.bytes,
    data.length,
    salt.bytes,
    salt.length,
    kCCPRFHmacAlgSHA512,
    iterations,
    out.mutableBytes,
    size
  );

  resolve([out base64EncodedStringWithOptions:0]);
}

RCT_REMAP_METHOD(scrypt, scrypt:(NSString *)passwd
                 salt:(NSString *)salt
                 N:(NSUInteger)N
                 r:(NSUInteger)r
                 p:(NSUInteger)p
                 size:(NSUInteger)size
                 //                 callback:(RCTResponseSenderBlock)callback)
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
  NSData *passwdData = [[NSData alloc] initWithBase64EncodedString:passwd options:0];
  NSData *saltData = [[NSData alloc] initWithBase64EncodedString:salt options:0];
  char *rawPasswd = (char *)[passwdData bytes];
  char *rawSalt = (char *)[saltData bytes];
  size_t passwdlen = [passwdData length];
  size_t saltlen = [saltData length];

  uint8_t *buffer = malloc(sizeof(char) * size);
  fast_crypto_scrypt(rawPasswd, passwdlen, rawSalt, saltlen, N, r, p, buffer, size);

  NSData *data = [NSData dataWithBytes:buffer length:size];
  NSString *str = [data base64EncodedStringWithOptions:0];
  free(buffer);

  // Already initialized
  resolve(str);
  //    callback(@[[NSNull null], str]);
}

RCT_REMAP_METHOD(secp256k1EcPubkeyCreate,
                 secp256k1EcPubkeyCreate:(NSString *)privateKeyHex
                 compressed:(NSInteger)compressed
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
  // Buffer size: privateKeyLen * 2 + 3 bytes
  // For 64-char private key: uncompressed public key = 130 hex chars (string) + 1 null terminator = 131 bytes
  char *szPublicKeyHex = malloc(sizeof(char) * ([privateKeyHex length] * 2 + 3)); 
  fast_crypto_secp256k1_ec_pubkey_create([privateKeyHex UTF8String], szPublicKeyHex, compressed);
  NSString *publicKeyHex = [NSString stringWithUTF8String:szPublicKeyHex];
  free(szPublicKeyHex);
  resolve(publicKeyHex);
}

RCT_REMAP_METHOD(secp256k1EcPrivkeyTweakAdd,
                 secp256k1EcPrivkeyTweakAdd:(NSString *)privateKeyHex
                 tweak:(NSString *)tweakHex
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
  int privateKeyHexLen = [privateKeyHex length] + 1;
  char szPrivateKeyHex[privateKeyHexLen];
  const char *szPrivateKeyHexConst = [privateKeyHex UTF8String];

  strcpy(szPrivateKeyHex, szPrivateKeyHexConst);
  fast_crypto_secp256k1_ec_privkey_tweak_add(szPrivateKeyHex, [tweakHex UTF8String]);
  NSString *privateKeyTweakedHex = [NSString stringWithUTF8String:szPrivateKeyHex];
  resolve(privateKeyTweakedHex);
}

RCT_REMAP_METHOD(secp256k1EcPubkeyTweakAdd,
                 secp256k1EcPubkeyTweakAdd:(NSString *)publicKeyHex
                 tweak:(NSString *)tweakHex
                 compressed:(NSInteger) compressed
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
  int publicKeyHexLen = [publicKeyHex length] + 1;
  char szPublicKeyHex[publicKeyHexLen];
  const char *szPublicKeyHexConst = [publicKeyHex UTF8String];

  strcpy(szPublicKeyHex, szPublicKeyHexConst);
  fast_crypto_secp256k1_ec_pubkey_tweak_add(szPublicKeyHex, [tweakHex UTF8String], compressed);
  NSString *publicKeyTweakedHex = [NSString stringWithUTF8String:szPublicKeyHex];
  resolve(publicKeyTweakedHex);
}

@end

