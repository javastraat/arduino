//
//  NetDevice.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 06-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NetDevice : NSObject;

-(id)initWithAddress:(NSData *)address;

@property (strong, nonatomic) NSData * address;
@property (strong,nonatomic) NSDate * lastSeen;
@end
