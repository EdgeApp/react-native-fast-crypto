
#import "RNFastCrypto.h"
#import "native-crypto.h"
#import <Foundation/Foundation.h>

#include <stdbool.h>
#include <stdint.h>

@implementation RNFastCrypto

- (dispatch_queue_t)methodQueue
{
    dispatch_queue_attr_t qosAttribute = dispatch_queue_attr_make_with_qos_class(DISPATCH_QUEUE_CONCURRENT, QOS_CLASS_BACKGROUND, 0);
    return dispatch_queue_create("io.exodus.RNFastCrypto.MainQueue", qosAttribute);
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

