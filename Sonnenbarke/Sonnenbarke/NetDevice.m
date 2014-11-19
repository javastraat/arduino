//
//  NetDevice.m
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 06-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import "NetDevice.h"

@implementation NetDevice

-(id)initWithAddress:(NSData *)anAddress {
    self = [super init];
    self.address = anAddress;
    return self;
}

-(BOOL)isEqual:(id)object {
    if (object == self)
        return YES;
    if (![object isKindOfClass:[self class]])
        return NO;
    
    return [self.address isEqual:((NetDevice *)object).address];
}

-(NSUInteger)hash {
    return [self.address hash];
}
@end
