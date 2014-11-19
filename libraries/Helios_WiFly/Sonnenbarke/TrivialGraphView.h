//
//  TrivialGraphView.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 06-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TrivialGraphView : UIView
@property (strong,nonatomic) NSString * valFormat;
-(void)addPoint:(double)val;
@end
