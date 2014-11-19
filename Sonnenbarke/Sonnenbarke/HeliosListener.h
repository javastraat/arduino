//
//  HeliosListener.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 03-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GCDAsyncUdpSocket.h"
#import "HeliosGadget.h"

extern NSString * kHeliosDeviceUpdated;
extern NSString * kHeliosDeviceLost;

@interface HeliosListener : NSObject <GCDAsyncUdpSocketDelegate>

@end
