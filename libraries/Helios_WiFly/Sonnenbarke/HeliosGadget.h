//
//  HeliosGadget.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 03-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NetDevice.h"

@interface HeliosGadget : NetDevice;

@property (assign,nonatomic) double lux, cct;
@property (assign,nonatomic) double red,green,blue;
@property (assign,nonatomic) double temp;
@property (assign,nonatomic) BOOL btn;
@property (assign,nonatomic) uint32_t gpio;

-(UIColor *)color;
@end
