//
//  DetailViewController.m
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 31/05/2013.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import "DetailViewController.h"
#import "LicenseViewController.h"
#import "HeliosListener.h"

@interface DetailViewController ()
@property (strong, nonatomic) UIPopoverController *masterPopoverController;
@property (strong, nonatomic) UIPopoverController *infoPopoverController;
- (void)configureView;
@end

@implementation DetailViewController

#pragma mark - Managing the detail item

- (void)setDetailItem:(id)newDetailItem
{
    if (_detailItem != newDetailItem) {
        [[NSNotificationCenter defaultCenter] removeObserver:nil
                                                        name:kHeliosDeviceUpdated
                                                      object:_detailItem];

        _detailItem = newDetailItem;

        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceUpdated:)
                                                     name:kHeliosDeviceUpdated
                                                   object:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceLost:)
                                                     name:kHeliosDeviceLost
                                                   object:nil];

        // Update the view.
        [self configureView];
    }

    if (self.masterPopoverController != nil) {
        [self.masterPopoverController dismissPopoverAnimated:YES];
    }        
}

- (void)configureView
{
    self.greenLabel.text = self.redLabel.text = self.blueLabel.text = @"...";
    self.detailDescriptionLabel.text = @"No device selected";
    self.tempGraphView.valFormat = @"%.1f˚C";
    
    if (self.detailItem) {
        self.detailDescriptionLabel.text = [self.detailItem description];
    }

}

-(void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:nil name:kHeliosDeviceUpdated object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:nil name:kHeliosDeviceLost object:nil];
}

-(void)deviceLost:(NSNotification *)notif {
    self.detailDescriptionLabel.text = @"Lost";
}

-(void)deviceUpdated:(NSNotification *)notif {
    HeliosGadget * dev  = (HeliosGadget *)notif.object;
    
    self.colourShowArea.backgroundColor = [dev color];
    self.redLabel.text = [NSString stringWithFormat:@"%01.02f", dev.red];
    self.greenLabel.text = [NSString stringWithFormat:@"%01.02f", dev.green];
    self.blueLabel.text = [NSString stringWithFormat:@"%01.02f", dev.blue];
    
    [self.tempGraphView addPoint:dev.temp];
    [self.luxGraphView addPoint:dev.lux];
    
    [self.btnView setHighlighted:dev.btn];
    
    NSMutableString *str = [NSMutableString stringWithFormat:@"GPIO 0..31 = "];
    for(int i = 0; i < 32; i++) {
        [str appendFormat:@"%c", (dev.gpio & (1<<i)) ? '1' : '0'];
        if (i && !(i % 4))
            [str appendString:@" "];
        if (i && !(i % 8))
            [str appendString:@"- "];
    }
    self.bitsLabel.text = str;
    
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    [self configureView];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.title = NSLocalizedString(@"Detail", @"Detail");
    }
    return self;
}
							
#pragma mark - Split view

- (void)splitViewController:(UISplitViewController *)splitController willHideViewController:(UIViewController *)viewController withBarButtonItem:(UIBarButtonItem *)barButtonItem forPopoverController:(UIPopoverController *)popoverController
{
    barButtonItem.title = NSLocalizedString(@"Master", @"Master");
    [self.navigationItem setLeftBarButtonItem:barButtonItem animated:YES];
    self.masterPopoverController = popoverController;
}

- (void)splitViewController:(UISplitViewController *)splitController willShowViewController:(UIViewController *)viewController invalidatingBarButtonItem:(UIBarButtonItem *)barButtonItem
{
    // Called when the view is shown again in the split view, invalidating the button and popover controller.
    [self.navigationItem setLeftBarButtonItem:nil animated:YES];
    self.masterPopoverController = nil;
}

#pragma  UI interaction

-(IBAction)infoButtonTapped:(id)sender {
    // currently only shown on the iPad - so safe to do the
    // popup without much ado.
    //
    LicenseViewController * vc = [[LicenseViewController alloc] initWithDelegate:self];
    CGRect screen = CGRectInset([[UIScreen mainScreen] bounds], 32,32);
    
    self.infoPopoverController = [[UIPopoverController alloc] initWithContentViewController:vc];
    [self.infoPopoverController setPopoverContentSize:screen.size];
    
    [self.infoPopoverController presentPopoverFromRect:((UIButton *)sender).frame
                                                inView:self.view
                              permittedArrowDirections:UIPopoverArrowDirectionDown
                                              animated:YES];
}

-(void)thanksButtonPressed:(id)sender {
    [self.infoPopoverController dismissPopoverAnimated:YES];
}

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return YES;
}
@end
