//
//  WiFly.m
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 01/06/2013.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import "WiFly.h"

@implementation WiFly

- (BOOL)isEqual:(id)object {
    if (![object isKindOfClass:[self class]])
        return false;

    return self.macAddress == ((WiFly *)object).macAddress;
}

- (NSUInteger)hash {
    return (NSUInteger)self.macAddress;
}

+(id)unitWithMac:(unsigned char[6])mac {
    WiFly *dev = [[self alloc] init];
    unsigned long long tmp = 0;
    assert(sizeof(tmp)>6);
    
    memcpy(&tmp, mac, 6);
    dev.macAddress = tmp;
    
    return dev;
}

-(NSString *)macAddressAsString {
    unsigned long long tmp = self.macAddress;
    return [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X",
            ((unsigned char *)&tmp)[0],
            ((unsigned char *)&tmp)[1],
            ((unsigned char *)&tmp)[2],
            ((unsigned char *)&tmp)[3],
            ((unsigned char *)&tmp)[4],
            ((unsigned char *)&tmp)[5]
            ];
}

-(NSString *)description {
    return [NSString stringWithFormat:@"<%@ %p: %@%@%@>",
            [[self class] description], self,
            [self device], [self.device length] ? @"/" : @"",
            [self macAddressAsString]
            ];
}
@end


