//
//  NetListener.m
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 06-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import "NetListener.h"
#import "GCDAsyncUdpSocket.h"

@interface NetListener () {
    GCDAsyncUdpSocket *udpSocket;
    NSMutableSet * devices;
    NSTimer * deviceLoop;
}
@end

@implementation NetListener

-(id)initWithPort:(NSInteger)port {
    
    self = [super init];
    if (!self) return nil;
    
    udpSocket = [[GCDAsyncUdpSocket alloc] initWithDelegate:self
                                              delegateQueue:dispatch_get_main_queue()];
    [udpSocket setIPv6Enabled:NO];
    
    NSError *error = nil;
    
    if (![udpSocket bindToPort:port error:&error])
    {
        NSLog(@"Error binding: %@", error);
        return nil;
    }
    if (![udpSocket beginReceiving:&error])
    {
        NSLog(@"Error beginReceiving: %@", error);
        return nil;
    }
    
    devices = [[NSMutableSet alloc] initWithCapacity:20];
    
    deviceLoop = [NSTimer scheduledTimerWithTimeInterval:300
                                                  target:self
                                                selector:@selector(checkLostDevices:)
                                                userInfo:nil
                                                 repeats:YES];
    
    return self;
}

-(NetDevice *)match:(NetDevice *)dev {
    for(NetDevice * d in devices) {
        if ([d.address isEqual:dev.address])
            return d;
    }
    return nil;
}

-(void)dealloc {
    deviceLoop = nil;
}

@end
