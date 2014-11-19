//
//  BlinkerView.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 04-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//
// Simple hourglass style blinker. Will light up in color (default red);
// and then revert to a shringking pie of lightColor (default lightGrey)
// for roughly the expectedUpdateCycleInSeconds (default 12.5 seconds),
// before reverting back to a neutral lightColor depiction.
//
// A call to the ping method will reset things again; whereas a call
// to 'off' will revert the thing to the neutral depiction.
//
#import <UIKit/UIKit.h>

@interface BlinkerView : UIView

@property (strong,nonatomic) UIColor *color, *lightColor;
@property (assign,nonatomic) NSTimeInterval expectedUpdateCycleInSeconds;

-(void)ping;
-(void)off;

@end
