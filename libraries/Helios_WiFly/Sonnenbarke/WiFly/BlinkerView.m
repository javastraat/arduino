//
//  BlinkerView.m
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 04-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import "BlinkerView.h"

@interface BlinkerView () {
    NSTimer * loop;
	int state;
    int steps;
}
@end

@implementation BlinkerView

-(id)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    
    return [self completeInit];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    return [self completeInit];
}

-(id)completeInit {
    self.color = [UIColor redColor];
    self.lightColor = [UIColor lightGrayColor];
    self.expectedUpdateCycleInSeconds = 12.5;
    self.backgroundColor = [UIColor clearColor];

    [self off];
    return self;
}

-(void)off {
    state = 0;
    [self setNeedsDisplay];
}

-(void)ping {
    state = 1;
    [self nxt];
}

-(void)nxt {
    if (state <= 0)
        return;
        
    state+=1;
    
    steps = MIN(300,MAX(6,self.expectedUpdateCycleInSeconds*24));
    if (state < steps)
        loop = [NSTimer scheduledTimerWithTimeInterval:(state < 4) ? 0.1 : (self.expectedUpdateCycleInSeconds/steps)
                                                target:self
                                              selector:@selector(nxt)
                                              userInfo:nil
                                               repeats:NO];
    else
        state = 0;

    [self setNeedsDisplay];
}

-(void)dealloc {
    loop = nil;
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    CGContextSetStrokeColorWithColor(context,  (state < 2 || state > 4) ? [self.lightColor CGColor] : [self.color CGColor]);
    CGContextSetFillColorWithColor(context, (state < 2 || state > 4) ? [self.lightColor CGColor] : [self.color CGColor]);

    if (state > 1) {
        if (state < 5) {
            // Initial flash
            CGContextFillEllipseInRect(context, CGRectInset(self.bounds,4-state,4-state));
        } else {
            // Shrinking pie later in the cycle.
            //
            double r = self.bounds.size.width/2.;
            double x = self.bounds.origin.x + r;
            double y = self.bounds.origin.y + r;
            r -= 3;
            
            CGContextBeginPath(context);
            CGContextMoveToPoint(context, x, y);
            CGContextAddArc(context, x, y, r, 3*M_PI/2,  -M_PI/2+2*M_PI*state/steps, 1);
            CGContextClosePath(context);
            
            CGContextFillPath(context);
        }
    }
    else {
        // Neutral display - just a line.
        CGContextStrokeEllipseInRect(context, CGRectInset(self.bounds,4,4));
    }
    // Outer line.
    CGContextStrokeEllipseInRect(context, CGRectInset(self.bounds,1,1));
}

@end
