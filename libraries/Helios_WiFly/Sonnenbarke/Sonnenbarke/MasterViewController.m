//
//  MasterViewController.m
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 31/05/2013.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import "MasterViewController.h"
#import "WiFlyDiscoverer.h"
#import "WiFly.h"
#import "AppDelegate.h"

#import "DetailViewController.h"
#import "WiFlyTableViewCell.h"

@interface MasterViewController () {
    NSMutableArray *_objects;
}
@end

@implementation MasterViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.title = NSLocalizedString(@"Master", @"Master");
        if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
            self.clearsSelectionOnViewWillAppear = NO;
            self.contentSizeForViewInPopover = CGSizeMake(320.0, 600.0);
        }
    }
    return self;
}
							
- (void)viewDidLoad
{
    [super viewDidLoad];

    _objects = [[NSMutableArray alloc] initWithCapacity:10];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceFound:)
                                                 name:kWiFlyDiscovererDeviceAdded
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceRemoved:)
                                                 name:kWiFlyDiscovererDeviceRemoved
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceUpdated:)
                                                 name:kWiFlyDiscovererDeviceUpdated
                                               object:nil];
}

-(void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:nil name:kWiFlyDiscovererDeviceAdded object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:nil name:kWiFlyDiscovererDeviceRemoved object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:nil name:kWiFlyDiscovererDeviceUpdated object:nil];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)deviceFound:(NSNotification *)notif {
    WiFly * dev  = (WiFly *)notif.object;

    // Remove it first - just in case. And append it at the end. I.e.
    // as to distingish it from a normal update.
    //
    [self deviceRemoved:notif];

    [_objects addObject:dev];
    
    
    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:[_objects count]-1 inSection:0];
    [self.tableView insertRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];

    // Auto show the very first item.
    //
    if ([_objects count] == 1) {
        NSIndexPath * firstItem = [NSIndexPath indexPathForRow:0 inSection:0];
        [self tableView:self.tableView didSelectRowAtIndexPath:firstItem];
    }
}


-(void)deviceRemoved:(NSNotification *)notif {
    WiFly * dev  = (WiFly *)notif.object;
    
    NSInteger i = [_objects indexOfObject:dev];
    
    if (i == NSNotFound || !_objects)
        return;

    [_objects removeObject:dev];
    
    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:i inSection:0];
    [self.tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
}

-(void)deviceUpdated:(NSNotification *)notif {
    WiFly * dev  = (WiFly *)notif.object;
    NSInteger i = [_objects indexOfObject:dev];
    
    if (i == NSNotFound || !_objects)
        return;
    
    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:i inSection:0];


    [self.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
}

- (void)insertNewObject:(id)sender
{
    if (!_objects) {
        _objects = [[NSMutableArray alloc] init];
    }
    [_objects insertObject:[NSDate date] atIndex:0];
    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:0 inSection:0];
    [self.tableView insertRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
}

#pragma mark - Table View

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _objects.count;
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"WiFlyTableViewCell";
    
    WiFlyTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[WiFlyTableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle
                                      reuseIdentifier:CellIdentifier];
        if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
            cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        }
    }

    [cell setDev:_objects[indexPath.row]];
    
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NetDevice *object = _objects[indexPath.row];
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
	    if (!self.detailViewController) {
	        self.detailViewController = [[DetailViewController alloc] initWithNibName:@"DetailViewController_iPhone" bundle:nil];
	    }
        AppDelegate * appDelegate = [[UIApplication sharedApplication] delegate];
        
        self.detailViewController.detailItem = [appDelegate matchingDevice:object];
        if (self.detailViewController.detailItem )
            [self.navigationController pushViewController:self.detailViewController animated:YES];
        
    } else {
        self.detailViewController.detailItem = object;
    }
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath*)indexPath; {
    return [WiFlyTableViewCell height];
}

-(void)infoButtonTapped:(id)sender {
    LicenseViewController * vc = [[LicenseViewController alloc] initWithDelegate:self];
    [self.navigationController pushViewController:vc animated:YES];
}

-(void)thanksButtonPressed:(id)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return YES;
}
@end
