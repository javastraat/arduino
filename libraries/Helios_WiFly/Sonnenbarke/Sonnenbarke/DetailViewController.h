//
//  DetailViewController.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 31/05/2013.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TrivialGraphView.h"

@interface DetailViewController : UIViewController <UISplitViewControllerDelegate>

@property (strong, nonatomic) id detailItem;

@property (weak, nonatomic) IBOutlet UILabel *detailDescriptionLabel;
@property (weak, nonatomic) IBOutlet UIView *colourShowArea;

@property (weak, nonatomic) IBOutlet TrivialGraphView *tempGraphView;
@property (weak, nonatomic) IBOutlet TrivialGraphView *luxGraphView;

@property (weak, nonatomic) IBOutlet UILabel *redLabel;
@property (weak, nonatomic) IBOutlet UILabel *greenLabel;
@property (weak, nonatomic) IBOutlet UILabel *blueLabel;

@property (weak, nonatomic) IBOutlet UIImageView *btnView;
@property (weak, nonatomic) IBOutlet UILabel *bitsLabel;

-(IBAction)infoButtonTapped:(id)sender;
@end
