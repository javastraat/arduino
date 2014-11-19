//
//  WiFlyTableViewCell.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 01/06/2013.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "WiFly.h"
#import "BlinkerView.h"

@interface WiFlyTableViewCell : UITableViewCell

@property (strong,nonatomic) WiFly * dev;

@property (strong,nonatomic) IBOutlet UIImageView * battView, * signalView;
@property (strong,nonatomic) IBOutlet BlinkerView * blinkerView;
@property (strong,nonatomic) IBOutlet UILabel * mainTextLabel;
@property (strong,nonatomic) IBOutlet UILabel * extraLabel;
@property (strong,nonatomic) IBOutlet UILabel * topLabel;
+(CGFloat)height;
@end
