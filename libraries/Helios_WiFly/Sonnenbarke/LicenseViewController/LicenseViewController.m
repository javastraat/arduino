//
//  LicenseViewController.m
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 03-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import "LicenseViewController.h"

@interface LicenseViewController ()
@end

@implementation LicenseViewController

-(id)initWithDelegate:(id)aDelegate {
    self = [self init];
    self.delegate = aDelegate;
    return self;
}

-(id)init {
    return [self initWithNibName:[[self class] description] bundle:[NSBundle mainBundle]];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    NSURL *rtfUrl = [[NSBundle mainBundle] URLForResource:@"credits" withExtension:@"rtf"];

    [self.rtfTextField loadRequest:[NSURLRequest requestWithURL:rtfUrl]];
}

-(void)viewWillAppear:(BOOL)animated {
    self.logoImage.alpha = 1.0;
}

-(void)viewDidAppear:(BOOL)animated {
    [NSTimer scheduledTimerWithTimeInterval:0.25 target:self selector:@selector(fadeOutLogo) userInfo:nil repeats:NO];
}

-(void)fadeOutLogo {
    [UIView animateWithDuration:1.3
                          delay:0
                        options:UIViewAnimationOptionBeginFromCurrentState
                     animations:^{
                         self.logoImage.alpha = 0;
                     }
                     completion:^(BOOL finished){
                     }];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(IBAction)thanksButton:(id)sender {
    if (![self.delegate respondsToSelector:@selector(thanksButtonPressed:)])
        return;
    
    [self.delegate performSelector:@selector(thanksButtonPressed:)
                          onThread:[NSThread mainThread]
                        withObject:sender
                     waitUntilDone:NO];
}
@end
