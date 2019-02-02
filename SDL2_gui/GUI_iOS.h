//
//  NSObject+GUI_iOS.h
//  iOS
//
//  Created by Panutat Tejasen on 29/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface GUI_iOS : NSObject

@end

extern "C" bool isIPAD();
extern "C" int getiOSStatusBarHeight();
extern "C" char *getOrientation();
extern "C" bool hasNotch();
extern "C" int *getContentSaftyMargin();

