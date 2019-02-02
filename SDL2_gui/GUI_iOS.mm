//
//  NSObject+GUI_iOS.m
//  iOS
//
//  Created by Panutat Tejasen on 29/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GUI_iOS.h"
#include <string>

CGFloat AACStatusBarHeight()
{
    CGSize statusBarSize = [[UIApplication sharedApplication] statusBarFrame].size;
    return MIN(statusBarSize.width, statusBarSize.height);
}

extern "C" int getiOSStatusBarHeight() {
    int h = AACStatusBarHeight();
    return h;
}

extern "C" bool isIPAD() {
    if ( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad )
    {
        return YES; /* Device is iPad */
    }
    return NO;
}

extern "C" bool hasNotch() {
    if( [[[UIApplication sharedApplication] keyWindow] respondsToSelector:@selector(safeAreaInsets)] ) {
        if( [[[UIApplication sharedApplication] keyWindow] safeAreaInsets].bottom != 0 ) {
            return true;
        }
    }
    return false;
}

extern "C" int *getContentSaftyMargin() {
    static int margin[4];
    if( [[[UIApplication sharedApplication] keyWindow] respondsToSelector:@selector(safeAreaInsets)] ) {
        margin[0] = [[[UIApplication sharedApplication] keyWindow] safeAreaInsets].top;
        margin[1] = [[[UIApplication sharedApplication] keyWindow] safeAreaInsets].right;
        margin[2] = [[[UIApplication sharedApplication] keyWindow] safeAreaInsets].bottom;
        margin[3] = [[[UIApplication sharedApplication] keyWindow] safeAreaInsets].left;
    }
    else {
        margin[0] = margin[1] = margin[2] = margin[3] = 0;
    }
    return margin;
}

extern "C" char * getDeviceModel() {
    static std::string str;
    
    
    if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        switch ((int)[[UIScreen mainScreen] nativeBounds].size.height) {
            case 1136:
                str = "iPhone 5 or 5S or 5C";
                break;
                
            case 1334:
                str = "iPhone 6/6S/7/8";
                break;
                
            case 1920:
            case 2208:
                str = "iPhone 6+/6S+/7+/8+";
                break;
                
            case 2436:
                str = "iPhone X, XS";
                break;
                
            case 2688:
                str = "iPhone XS Max";
                break;
                
            case 1792:
                str = "iPhone XR";
                break;
                
            default:
                str = "iPhone";
                break;
        }
    }
    else {
        str = "iPad";
    }
    return (char *)str.c_str();
}

extern "C" char *getOrientation() {
    static std::string str;
    
    // Initial device orientation
    UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
    
    
    if(orientation == UIInterfaceOrientationUnknown){
        // code for Unknown
        str = "Unknown orientation";
    }
    else if(orientation == UIInterfaceOrientationPortrait){
        // code for Portrait
        str = "Portrait";
    }
    else if(orientation == UIInterfaceOrientationPortraitUpsideDown){
        // code for Portrait
        str = "Portrait Upside Down";
    }
    else if(orientation == UIInterfaceOrientationLandscapeLeft){
        // code for Landscape
        str = "Landsace Left";
    }
    else if(orientation == UIInterfaceOrientationLandscapeRight){
        // ode for Landscape
        str = "Landscape Right";
    }
    
    return (char *)str.c_str();
}

@implementation GUI_iOS

@end
