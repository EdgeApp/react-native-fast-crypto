
#import "RNFastCrypto.h"
#import "native-crypto.h"
#import <Foundation/Foundation.h>

#include <stdbool.h>
#include <stdint.h>

@implementation RNFastCrypto

- (dispatch_queue_t)methodQueue
{
    return dispatch_get_main_queue();
}

RCT_EXPORT_MODULE()

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
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void) {
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
    });
}

RCT_REMAP_METHOD(secp256k1EcPubkeyCreate,
                 secp256k1EcPubkeyCreate:(NSString *)privateKeyHex
                 compressed:(NSInteger)compressed
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void) {
        char *szPublicKeyHex = malloc(sizeof(char) * [privateKeyHex length] * 2);
        fast_crypto_secp256k1_ec_pubkey_create([privateKeyHex UTF8String], szPublicKeyHex, compressed);
        NSString *publicKeyHex = [NSString stringWithUTF8String:szPublicKeyHex];
        free(szPublicKeyHex);
        resolve(publicKeyHex);
    });
}

RCT_REMAP_METHOD(secp256k1EcPrivkeyTweakAdd,
                 secp256k1EcPrivkeyTweakAdd:(NSString *)privateKeyHex
                 tweak:(NSString *)tweakHex
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void) {
        int privateKeyHexLen = [privateKeyHex length] + 1;
        char szPrivateKeyHex[privateKeyHexLen];
        const char *szPrivateKeyHexConst = [privateKeyHex UTF8String];
        
        strcpy(szPrivateKeyHex, szPrivateKeyHexConst);
        fast_crypto_secp256k1_ec_privkey_tweak_add(szPrivateKeyHex, [tweakHex UTF8String]);
        NSString *privateKeyTweakedHex = [NSString stringWithUTF8String:szPrivateKeyHex];
        resolve(privateKeyTweakedHex);
    });
}

RCT_REMAP_METHOD(secp256k1EcPubkeyTweakAdd,
                 secp256k1EcPubkeyTweakAdd:(NSString *)publicKeyHex
                 tweak:(NSString *)tweakHex
                 compressed:(NSInteger) compressed
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void) {
        int publicKeyHexLen = [publicKeyHex length] + 1;
        char szPublicKeyHex[publicKeyHexLen];
        const char *szPublicKeyHexConst = [publicKeyHex UTF8String];
        
        strcpy(szPublicKeyHex, szPublicKeyHexConst);
        fast_crypto_secp256k1_ec_pubkey_tweak_add(szPublicKeyHex, [tweakHex UTF8String], compressed);
        NSString *publicKeyTweakedHex = [NSString stringWithUTF8String:szPublicKeyHex];
        resolve(publicKeyTweakedHex);
    });
}

RCT_REMAP_METHOD(pbkdf2Sha512,
                 pbkdf2Sha512:(NSString *)pass
                 salt:(NSString *)salt
                 iterations:(NSInteger) iterations
                 outputBytes:(NSInteger) outputBytes
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void) {
        char szResultHex[(outputBytes * 2) + 1];
        
        fast_crypto_pbkdf2_sha512([pass UTF8String], [salt UTF8String], (int) iterations, (int) outputBytes, szResultHex);
        
        NSString *resultHex = [NSString stringWithUTF8String:szResultHex];
        
        resolve(resultHex);
    });
}

RCT_REMAP_METHOD(decryptJsonBoxBinary, decryptJsonBoxBinary:(NSString *)jsonBox
                 base16Key:(NSString *)base16Key
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
    char *pszResult = NULL;
    int size;
    
    fast_crypto_decrypt_jsonbox([jsonBox UTF8String], [base16Key UTF8String], &pszResult, &size);
    NSData *data = [NSData dataWithBytes:pszResult length:size];
    NSString *str = [data base64EncodedStringWithOptions:0];
    free(pszResult);
    resolve(str);
}

RCT_REMAP_METHOD(decryptJsonBoxText, decryptJsonBoxText:(NSString *)jsonBox
                 base16Key:(NSString *)base16Key
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
    char *pszResult = NULL;
    int size;
    
    fast_crypto_decrypt_jsonbox([jsonBox UTF8String], [base16Key UTF8String], &pszResult, &size);
    NSString *str = [NSString stringWithUTF8String:pszResult];
    free(pszResult);
    resolve(str);
}

RCT_REMAP_METHOD(repoFileGetData, repoFileGetData:(NSString *)path
                 pathFallback:(NSString *)pathFallback
                 pathWhiteout:(NSString *)pathWhiteout
                 base16Key:(NSString *)base16Key
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void) {
        char *pszResult = NULL;
        int size;
        
        fast_crypto_repo_get_data([path UTF8String],
                                  [pathFallback UTF8String],
                                  [pathWhiteout UTF8String],
                                  [base16Key UTF8String],
                                  &pszResult, &size);
        if (pszResult) {
            NSData *data = [NSData dataWithBytes:pszResult length:size];
            NSString *str = [data base64EncodedStringWithOptions:0];
            free(pszResult);
            resolve(str);
        } else {
            NSError *error =  [NSError errorWithDomain:@"RNFastCrypto"
                                                  code:1
                                              userInfo:NULL];
            reject(@"repoFileGetData ERROR", @"repoFileGetData ERROR", error);
        }
    });
}

RCT_REMAP_METHOD(repoFileGetText, repoFileGetText:(NSString *)path
                 pathFallback:(NSString *)pathFallback
                 pathWhiteout:(NSString *)pathWhiteout
                 base16Key:(NSString *)base16Key
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void) {
        char *pszResult = NULL;
        int size;
        
        fast_crypto_repo_get_text([path UTF8String],
                                  [pathFallback UTF8String],
                                  [pathWhiteout UTF8String],
                                  [base16Key UTF8String],
                                  &pszResult);
        if (pszResult) {
            NSString *str = [NSString stringWithUTF8String:pszResult];
            free(pszResult);
            resolve(str);
        } else {
            NSError *error =  [NSError errorWithDomain:@"RNFastCrypto"
                                                  code:1
                                              userInfo:NULL];
            reject(@"repoFileGetText ERROR", @"repoFileGetText ERROR", error);
        }
    });
}

@end

