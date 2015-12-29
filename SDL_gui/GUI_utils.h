/*
 *  GUI_utils.h
 *
 *  adapted from source code written by Holmes Futrell
 *  use however you want
 */

#ifndef __JS_GUI_UTILS_H__
#define __JS_GUI_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif
extern int randomInt(int min, int max);
extern float randomFloat(float min, float max);
extern void fatalError(const char *string);
#ifdef __cplusplus
}
#endif

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

#define MILLESECONDS_PER_FRAME 1000.0/60.0       /* about 60 frames per second */

void GUI_log( const char * format, ... );
std::string GUI_getResourcePath(const std::string &subDir = "");

#endif

