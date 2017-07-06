
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
    const uint8_t *szPasswd = (uint8_t *) [passwd UTF8String];
    const uint8_t *szSalt = (uint8_t *) [salt UTF8String];
    size_t passwdlen = [passwd length];
    size_t saltlen = [salt length];
    uint8_t *buffer = malloc(sizeof(char) * size);
    fast_crypto_scrypt(szPasswd, passwdlen, szSalt, saltlen, N, r, p, buffer, size);
    
    NSData *data = [NSData dataWithBytes:buffer length:size];
    NSString *str = [data base64EncodedStringWithOptions:0];
    free(buffer);
    
    // Already initialized
    resolve(str);
    //    callback(@[[NSNull null], str]);
}

@end

