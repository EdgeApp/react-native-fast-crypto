
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

+ (void) handleDownloadAndProcess:(NSString*) method
                                 :(NSString*) params
                                 :(RCTPromiseResolveBlock) resolve
                                 :(RCTPromiseRejectBlock) reject {

    NSData *paramsData = [params dataUsingEncoding:NSUTF8StringEncoding];
    NSError *jsonError;
    NSDictionary *jsonParams = [NSJSONSerialization JSONObjectWithData:paramsData options:kNilOptions error:&jsonError];

    NSString *addr = jsonParams[@"url"];
    NSString *startHeight = jsonParams[@"start_height"];


    size_t length = 0;
    const char *m_body = create_blocks_request([startHeight intValue], &length);

    NSURL *url = [NSURL URLWithString:addr];
    NSData *binaryData = [NSData dataWithBytes:m_body length:length];
    free((void *)m_body);

    NSMutableURLRequest *urlRequest = [[NSMutableURLRequest alloc] initWithURL:url];
    [urlRequest setHTTPMethod:@"POST"];
    [urlRequest setHTTPBody:binaryData];
    [urlRequest setTimeoutInterval: 4 * 60];

    NSURLSession *session = [NSURLSession sharedSession];

    NSURLSessionDataTask *task = [session dataTaskWithRequest:urlRequest completionHandler: ^(NSData *data, NSURLResponse *response, NSError *error) {
        if (error) {
            resolve(@"{\"err_msg\":\"Network request failed\"}");
            return;
        }

        char *pszResult = NULL;

        extract_utxos_from_blocks_response(data.bytes, data.length, [params UTF8String], &pszResult);

        NSString *jsonResult = [NSString stringWithUTF8String:pszResult];
        free(pszResult);
        resolve(jsonResult);
    }];
    [task resume];
}

+ (void) handleDefault:(NSString*) method
                      :(NSString*) params
                      :(RCTPromiseResolveBlock) resolve
                      :(RCTPromiseRejectBlock) reject {
    char *pszResult = NULL;

    fast_crypto_monero_core([method UTF8String], [params UTF8String], &pszResult);

    if (pszResult == NULL) {
        resolve(NULL);
        return;
    }

    NSString *jsonResult = [NSString stringWithUTF8String:pszResult];
    free(pszResult);
    resolve(jsonResult);
}

RCT_EXPORT_MODULE()

RCT_REMAP_METHOD(moneroCore, :(NSString*) method
                 :(NSString*) params
                 :(RCTPromiseResolveBlock) resolve
                 :(RCTPromiseRejectBlock) reject)
{
    if ([method isEqualToString:@"download_and_process"]) {
        [RNFastCrypto handleDownloadAndProcess:method :params :resolve :reject];
    } else {
        [RNFastCrypto handleDefault:method :params :resolve :reject];
    }
}
@end
