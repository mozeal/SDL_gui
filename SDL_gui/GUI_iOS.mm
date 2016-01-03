//
//  GUI_iOS.m
//  iOS
//
//  Created by Jimmy on 1/4/2559 BE.
//  Copyright © 2559 Jimmy Software Co.,Ltd. All rights reserved.
//

#import "GUI_iOS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Jimmy
char __iOS_DOCUMENTS_FOLDER[512];

// Jimmy
NSString *dir = [NSString stringWithFormat:@"%@/Documents", NSHomeDirectory()];
//NSLog( @"Dir: %@", dir );

extern "C" void GUI_iOS_Init() {
    strcpy( __iOS_DOCUMENTS_FOLDER, [dir UTF8String] );
}

@implementation GUI_iOS

@end
