
#import "RNFastCrypto.h"
#import "native-crypto.h"
#import <Foundation/Foundation.h>

#include <stdbool.h>
#include <stdint.h>

@implementation RNFastCrypto

- (dispatch_queue_t)methodQueue
{
    return dispatch_queue_create("io.exodus.RNFastCrypto.MainQueue", DISPATCH_QUEUE_PRIORITY_DEFAULT);
}

RCT_EXPORT_MODULE()

RCT_REMAP_METHOD(moneroCore,
                 moneroCore:(NSString* ) methodEnum
                 jsonParams:(NSString *)jsonParams
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
    char *pszResult = NULL;
    fast_crypto_monero_core([methodEnum UTF8String], [jsonParams UTF8String], &pszResult);
    if (pszResult == NULL) {
        resolve(NULL);
    }
    NSString *jsonResult = [NSString stringWithUTF8String:pszResult];
    free(pszResult);
    resolve(jsonResult);
}
@end

