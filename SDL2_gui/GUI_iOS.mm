//
//  NSObject+GUI_iOS.m
//  iOS
//
//  Created by Panutat Tejasen on 29/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GUI_iOS.h"

CGFloat AACStatusBarHeight()
{
    CGSize statusBarSize = [[UIApplication sharedApplication] statusBarFrame].size;
    return MIN(statusBarSize.width, statusBarSize.height);
}

extern "C" int getiOSStatusBarHeight() {
    return AACStatusBarHeight();
}

@implementation GUI_iOS

@end
