//
//  AppDelegate.h
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 31/05/2013.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "WiFlyDiscoverer.h"
#import "WiFly.h"
#import "HeliosListener.h"
#import "NetListener.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate> {
    NSMutableArray * listeners;
}

@property (strong, nonatomic) WiFlyDiscoverer * discoverer;
@property (strong, nonatomic) HeliosListener * helios;

@property (strong, nonatomic) UIWindow *window;

@property (strong, nonatomic) UINavigationController *navigationController;

@property (strong, nonatomic) UISplitViewController *splitViewController;

-(id)matchingDevice:(NetDevice *)dev;
@end
