//
//  MasterViewController.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 31/05/2013.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LicenseViewController.h"

@class DetailViewController;

@interface MasterViewController : UITableViewController <LicenseViewControllerDelegate>

@property (strong, nonatomic) DetailViewController *detailViewController;
-(IBAction)infoButtonTapped:(id)sender;
@end
