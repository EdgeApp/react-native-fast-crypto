
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

@end

