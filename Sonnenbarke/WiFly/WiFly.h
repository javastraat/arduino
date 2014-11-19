//
//  WiFly.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 01/06/2013.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NetDevice.h"

@interface WiFly : NetDevice;

@property (assign, nonatomic) unsigned long long macAddress;
@property (assign, nonatomic) NSInteger channel, rssi, timeoutInSeconds, port;
@property (assign, nonatomic) double batteryVoltage, batteryCharge, signal;
@property (strong, nonatomic) NSString * firmware, * device, * timeStr;
@property (strong, nonatomic) NSArray * sendors, * gpio;
@property (strong, nonatomic) NSDate * rtc, *bootTime;
@property (strong, nonatomic) NSString * ipAddressAsString;

+(id)unitWithMac:(unsigned char[6])mac;
-(NSString *)macAddressAsString;
@end
