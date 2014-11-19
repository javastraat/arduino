//
//  WiFlyTableViewCell.m
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 01/06/2013.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import "WiFlyTableViewCell.h"

@implementation WiFlyTableViewCell
CGFloat height = 0;

+ (UINib*)nib
{
    static dispatch_once_t pred = 0;
    __strong static UINib* _sharedNibObject = nil;

    dispatch_once(&pred, ^{
        _sharedNibObject = [UINib nibWithNibName:NSStringFromClass([self class]) bundle:nil];
    });
    UITableViewCell * tmp = [[_sharedNibObject instantiateWithOwner:self options:nil] objectAtIndex:0];
    height = tmp.frame.size.height;

    return _sharedNibObject;
}

- (NSString *)reuseIdentifier
{
    return [[self class] reuseIdentifier];
}

+ (NSString *)reuseIdentifier
{
    return NSStringFromClass([self class]);
}

+ (CGFloat)height {
    return height;
}

-(id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {

#if 0
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
#else
    self = [[[[self class] nib] instantiateWithOwner:self options:nil] objectAtIndex:0];
#endif
    if (!self)
        return nil;
    
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        self.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    }
    
    return self;
}

-(void)setDev:(WiFly *)object {
    self.mainTextLabel.text = [object device];
    self.topLabel.text = [NSString stringWithFormat:@"%@ / %@:%d",
                                 [object macAddressAsString],
                            [object ipAddressAsString], [object port]];
    
    self.extraLabel.text = [NSString stringWithFormat:@"%@",
                            [object firmware]];

    [self.battView setImage:[UIImage imageNamed:[NSString stringWithFormat:@"bat%d",
                                                (int)(object.batteryCharge * 3+0.1)]]];
    
    [self.signalView setImage:[UIImage imageNamed:[NSString stringWithFormat:@"signal%d",
                                                 (int)(object.signal * 9 +0.1)]]];
    
    [self.blinkerView ping];
}

@end
