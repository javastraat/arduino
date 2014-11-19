//
//  LicenseViewController.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 03-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol LicenseViewControllerDelegate <NSObject>
-(void)thanksButtonPressed:(id)sender;
@end

@interface LicenseViewController : UIViewController

@property (assign, nonatomic) id delegate;
@property (strong, nonatomic) IBOutlet UIWebView * rtfTextField;
@property (strong, nonatomic) IBOutlet UIImageView * logoImage;

-(id)initWithDelegate:(id)aDelegate;
-(IBAction)thanksButton:(id)sender;
@end
