//
//  WiFlyDiscoverer.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 31/05/2013.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GCDAsyncUdpSocket.h"
#import "DDLog.h"

// Messages sent when devices are added/removed. The class will
// be the instance of WiFlyDiscoverer that noticed the device,
// the user argument is the device.
//
extern NSString * kWiFlyDiscovererDeviceAdded;
extern NSString * kWiFlyDiscovererDeviceUpdated;
extern NSString * kWiFlyDiscovererDeviceRemoved;

@interface WiFlyDiscoverer : NSObject {
}
-(NSArray *)devices;
@end
