//
//  TrivialGraphView.m
//  Sonnenbarke
//
//  Created by Dirk-Willem van Gulik on 06-06-13.
//  Copyright (c) 2013 Dirk-Willem van Gulik. All rights reserved.
//

#import "TrivialGraphView.h"

#define N (150)

@interface TrivialGraphView () {
    UILabel * textLabel;
    NSMutableArray * values;
    double min,max;
}
@property (strong, nonatomic) UIColor * color;
@end

@implementation TrivialGraphView

-(id)initWithCoder:(NSCoder *)aDecoder {
    
    self = [super initWithCoder:aDecoder];
    if (self) {
        [self completeInit];
    }
    return self;
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self completeInit];
    }
    return self;
}

-(void)completeInit {
    values = [[NSMutableArray alloc] initWithCapacity:N];
    self.color = [UIColor darkGrayColor];
    textLabel = [[UILabel alloc] initWithFrame:self.bounds];
    textLabel.textAlignment = NSTextAlignmentCenter;
    textLabel.textColor = [UIColor whiteColor];
    textLabel.backgroundColor = [UIColor clearColor];
    textLabel.font = [UIFont systemFontOfSize:(self.bounds.size.height - 4) * 0.4];
    textLabel.text = @"no data";
    self.valFormat = @"%.02f";
    
    [self addSubview:textLabel];
}

-(void)addPoint:(double)val {
    while([values count] + 1 >= N)
        [values removeObjectAtIndex:0];
    [values addObject:[NSNumber numberWithDouble:val]];
    
    max = [[values objectAtIndex:0] doubleValue];
    min = max;
    for(NSNumber * n in values) {
        if (max < [n doubleValue])
            max = [n doubleValue];
        if (min > [n doubleValue])
            min = [n doubleValue];
    };

    double d = MAX(10.,max - min);
    min -= d/2.;
    max += d/2.;
    
    textLabel.text = [NSString stringWithFormat:self.valFormat, val];
    [self setNeedsDisplay];
    
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    CGContextSetStrokeColorWithColor(context, [self.color CGColor]);
    CGContextSetLineWidth(context, 2);
    
    int n = [values count];
    int sx = (self.bounds.size.width - 8) / (n-1);
    int sy = (self.bounds.size.height - 8) / (max - min);

    CGContextBeginPath(context);
    for(int i = 0; i < n; i++) {
        double x = 4 + i * sx;
        double y = 4 + ([[values objectAtIndex:i] doubleValue] - min) * sy;
        if (i)
            CGContextAddLineToPoint(context, x,y);
        else
            CGContextMoveToPoint(context, x, y);
    }
    CGContextStrokePath(context);
}

@end
