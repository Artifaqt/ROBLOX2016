//
//  NAMEHEREXSRFRequest.h
//  NAMEHEREMobile
//
//  Created by Ariel Lichtin on 6/25/14.
//  Copyright (c) 2014 NAMEHERE. All rights reserved.
//

#import "AFHTTPRequestOperationManager.h"

@interface NAMEHEREXSRFRequest : AFHTTPRequestOperationManager

/* XSRF Protection: Post a message that modifies the state of the server using the safety NAMEHERE token */
- (AFHTTPRequestOperation *)XSRFPOST:(NSString *)URLString
                          parameters:(id)parameters
                            success:(void (^)(AFHTTPRequestOperation *operation, id responseObject))success
                            failure:(void (^)(AFHTTPRequestOperation *operation, NSError *error))failure;

@end
