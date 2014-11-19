//
//  WiFlyDiscoverer.m
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 31/05/2013.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import "WiFlyDiscoverer.h"
#import "WiFly.h"

NSString * kWiFlyDiscovererDeviceAdded = @"WiFlyDiscovererDeviceAdded";
NSString  * kWiFlyDiscovererDeviceUpdated = @"WiFlyDiscovererDeviceUpdated";
NSString * kWiFlyDiscovererDeviceRemoved = @"WiFlyDiscovererDeviceRemoved";

@interface NSString (embeddedExtensions)
-(id)initWithCString:(const char *)bytes length:(NSUInteger)length encoding:(NSStringEncoding)encoding;
@end

@implementation NSString (embeddedExtensions)
-(id)initWithCString:(const char *)bytes length:(NSUInteger)length encoding:(NSStringEncoding)encoding {
    int l = 0;
    while(l < length && bytes[l])
        l++;
    return [self initWithBytes:bytes length:l encoding:encoding];
}
@end

static const int ddLogLevel = LOG_LEVEL_VERBOSE;

#define PORT (55555)

// very simple linear model of voltage measured to battery charge (0...1)
//
const double kMINVOLT = 2.71f; // 2.68f on AP unit
const double kFULLVOLT = 3.10f; // 3.09 perhaps.

// RN-WIFLYCR-UGUser Manual and Command Reference
// 1.2r 4/30/13, Page 73, Roving Networks
//
typedef struct {
    uint8_t  mac[6];
    uint8_t  channel;
    uint8_t  rssi;
    uint16_t port;
    uint32_t rts; //  order MSB to LSB
    uint16_t vBatt; // millivolt
    uint16_t gpio;
    char  timeStr[13];
    char  firmware[26];
    char  deviceId[32];
    uint16_t bootTime; // milliseconds
    uint16_t advals[8];
} __attribute__((packed)) infopacket_2013;

// WiFlyGSX-um User Manual and Command Reference
// 7/16/2010, Page 35, Roving Networks
//
typedef struct {
    uint8_t  mac[6];
    uint8_t  channel;
    uint8_t  rssi;
    uint16_t port;
    uint32_t rts; //  order MSB to LSB
    uint16_t vBatt; // millivolt
    uint16_t gpio;
    char  timeStr[14];  
    char  firmware[28];
    char  deviceId[31];
    uint16_t bootTime; // milliseconds
    uint16_t advals[8];
} __attribute__((packed)) infopacket_2012;

@interface WiFlyDiscoverer () {
    GCDAsyncUdpSocket *udpSocket;
    NSMutableSet * _devices;
}
@property (strong) NSTimer * deviceLoop;
@end

#define LOOP_CYCLE (10) // seconds

@implementation WiFlyDiscoverer

-(id)devices {
    return _devices;
}

#ifdef DEBUG
-(void)initialize {
    assert(sizeof(infopacket_2012)==110);
    assert(sizeof(infopacket_2013)==110);
}
#endif

-(id)init {
    return [self initWithPort:PORT];
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

    NSLog(@"Listening to port %d",port);
    
    _devices = [[NSMutableSet alloc] initWithCapacity:20];
    self.deviceLoop = [NSTimer scheduledTimerWithTimeInterval:LOOP_CYCLE
                                                       target:self
                                                     selector:@selector(checkLostDevices:)
                                                     userInfo:nil
                                                      repeats:YES];
    
    return self;
}

-(void)checkLostDevices:(NSTimer *)timer {
    NSMutableSet * toRemove = [[NSMutableSet alloc] initWithCapacity:5];
    for(WiFly * dev in _devices) {
        if (-[dev.lastSeen timeIntervalSinceNow] > dev.timeoutInSeconds)
            [toRemove addObject:dev];
    }
    
    for(WiFly * dev in toRemove) {
        [_devices removeObject:dev];

        [[NSNotificationCenter defaultCenter] postNotificationName:kWiFlyDiscovererDeviceRemoved
                                                            object:dev];
    }
}

- (void)udpSocket:(GCDAsyncUdpSocket *)sock
   didReceiveData:(NSData *)data
      fromAddress:(NSData *)address
withFilterContext:(id)filterContext
{
    if ([data length] != 110) {
        NSString *host = nil;
        uint16_t port = 0;
        [GCDAsyncUdpSocket getHost:&host port:&port fromAddress:address];
        
        NSLog(@"RECV: Unknown message from: %@:%hu - ignored", host, port);
        return;
    }
    
    infopacket_2012 * packet = (infopacket_2012*)[data bytes];

#if 0
    for(int i = 0, k = 0; i < [data length]; i++) {
        unsigned char c = ((unsigned char *)[data bytes])[i];
        if (i % 16 == 0)
            printf("%08d : ",i);
        
        printf("%02x ", c);
        
        if ((i % 16 == 15) || (i == [data length]-1)) {
            printf(" : ");
            for(int j = k; j <= i; j++) {
                unsigned char cc = ((unsigned char *)[data bytes])[j];
                printf("%c", cc >= 32 && cc < 128 ? cc : '.');
            }
            printf("\n");
            k = i+1;
        }
    }
#endif
    
    WiFly * mac = [WiFly unitWithMac:packet->mac];
    WiFly * dev = [_devices member:mac];
    
    if (!dev) {
        dev = mac;
        dev.firmware =  [[NSString alloc] initWithCString:packet->firmware
                                                 length:sizeof(packet->firmware)
                                               encoding:NSASCIIStringEncoding];
        
        dev.device = [[NSString alloc] initWithCString:packet->deviceId
                                              length:sizeof(packet->deviceId)
                                            encoding:NSASCIIStringEncoding];
        
        dev.lastSeen = [NSDate date];
        dev.timeoutInSeconds = LOOP_CYCLE * 2.5;
        [_devices addObject:dev];
        
        [[NSNotificationCenter defaultCenter] postNotificationName:kWiFlyDiscovererDeviceAdded
                                                            object:dev];
    }

    
    NSInteger lastDelta = - [dev.lastSeen timeIntervalSinceNow];
    if (lastDelta > 1)
        dev.timeoutInSeconds = MAX(LOOP_CYCLE * 2,(dev.timeoutInSeconds * 5 + lastDelta)/6);
    
    dev.lastSeen = [NSDate date];
    dev.address = address;
    
    dev.rssi = packet->rssi;
    dev.port = htons(packet->port);
    dev.ipAddressAsString = [GCDAsyncUdpSocket hostFromAddress:address];
                             
    dev.timeStr = [[NSString alloc] initWithCString:packet->timeStr
                                           length:sizeof(packet->timeStr)
                                         encoding:NSASCIIStringEncoding];
    dev.rtc = [NSDate dateWithTimeIntervalSince1970:ntohl(packet->rts)];
    // dev.bootTime = [NSDate dateWithTimeIntervalSince1970:ntohl(packet->bootTime)];
    
    dev.batteryVoltage = ntohs(packet->vBatt) / 1000.0;
    
    dev.batteryCharge = MAX(0,(MIN(dev.batteryVoltage, kFULLVOLT) - kMINVOLT)) / (kFULLVOLT-kMINVOLT);
    
    // Assume they are dBm; resonable range is 100 (weakest) to 40 (strongest)
    // which is mapped to a 0..1 float (0 weakest-virtually no connection, 1
    // strongest, next to AP).
    //
    dev.signal = MIN(MAX(100-dev.rssi,10)-10,40)/60.;
    
    [[NSNotificationCenter defaultCenter] postNotificationName:kWiFlyDiscovererDeviceUpdated
                                                        object:dev];
}

@end
