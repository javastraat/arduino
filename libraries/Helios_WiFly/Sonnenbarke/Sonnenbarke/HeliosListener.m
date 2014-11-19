//
//  HeliosListener.m
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 03-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import "HeliosListener.h"
#import "GCDAsyncUdpSocket.h"

#define HELIOSPORT (12345)

typedef struct {
    uint16_t version;
    uint16_t lux;
    uint16_t cct;
    uint16_t red, green, blue;
    uint16_t temp;
    uint32_t gpio;
}  __attribute__((packed)) heliosPacket;

NSString * kHeliosDeviceUpdated = @"HeliosDeviceUpdated";
NSString * kHeliosDeviceLost = @"HeliosDeviceLost";

@interface HeliosListener () {
    GCDAsyncUdpSocket *udpSocket;
    NSMutableSet * devices;
    NSTimer * deviceLoop;
}
@end

@implementation HeliosListener

-(id)init {
    return [self initWithPort:HELIOSPORT];
}

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

-(void)checkLostDevices:(NSTimer *)timer {
    NSMutableSet * toRemove = [[NSMutableSet alloc] initWithCapacity:5];
    for(HeliosGadget * dev in devices) {
        
        if (-[dev.lastSeen timeIntervalSinceNow] > 300)
            [toRemove addObject:dev];
    }
    
    for(HeliosGadget * dev in toRemove) {
        [devices removeObject:dev];
        
        [[NSNotificationCenter defaultCenter] postNotificationName:kHeliosDeviceLost
                                                            object:dev];
    }
}

-(void)dealloc {
    deviceLoop = nil;
}

- (void)udpSocket:(GCDAsyncUdpSocket *)sock
   didReceiveData:(NSData *)data
      fromAddress:(NSData *)address
withFilterContext:(id)filterContext
{
    NSString *host = nil;
    uint16_t port = 0;
    [GCDAsyncUdpSocket getHost:&host port:&port fromAddress:address];
    
    if ([data length] != sizeof(heliosPacket)) {
        
        NSLog(@"%s RECV: Unknown message from: %@:%hu (length %d) - ignored", __PRETTY_FUNCTION__, host, port, [data length]
              );
        return;
    }
    
    heliosPacket * packet = (heliosPacket*)[data bytes];
    
    if (((packet->version) >> 8) != 1) {
        NSLog(@"%s RECV: Packet with unknown version %d.%02d (length %d) from: %@:%hu - ignored", __PRETTY_FUNCTION__,
              (packet->version >> 8), (packet->version) & 0xFF, [data length],
              host, port);
        return;
    }
    
    HeliosGadget * tmp =[[HeliosGadget alloc] initWithAddress:address];
    
    HeliosGadget * dev = [devices member:tmp];
    if (!dev) {
        dev = tmp; // new device.

        [devices addObject:dev];
//        NSLog(@"New device %@",[GCDAsyncUdpSocket hostFromAddress:address]);
    } else {
//        NSLog(@"Update device %@",[GCDAsyncUdpSocket hostFromAddress:address]);
    }
    
    dev.lux = 1.0 * ntohs(packet->lux);
    dev.cct = 1.0 * ntohs(packet->cct);
    
    dev.red = 1.0 * ntohs(packet->red);
    dev.green = 1.0 * ntohs(packet->green);
    dev.blue = 1.0 * ntohs(packet->blue);

    // simple normalize
    double nc = MAX(dev.red, MAX(dev.blue, dev.green));
    dev.red /= nc;
    dev.blue /= nc;
    dev.green /= nc;
    
    dev.temp = 1.0 * ntohs(packet->temp);

    dev.lastSeen = [NSDate date];
    dev.gpio = (ntohl(packet->gpio));
    dev.btn = (dev.gpio & (1<<30)) ? TRUE : FALSE;
    
    [[NSNotificationCenter defaultCenter] postNotificationName:kHeliosDeviceUpdated
                                                        object:dev];
}


@end
