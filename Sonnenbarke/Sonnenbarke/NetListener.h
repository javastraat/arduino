//
//  NetListener.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 06-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NetDevice.h"

@interface NetListener : NSObject
-(NetDevice *)match:(NetDevice *)dev;
@end
