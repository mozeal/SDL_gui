/*
 *  GUI_utils.c
 *  adapted from source code written by Holmes Futrell
 *  use however you want
 */
#include <iostream>
#include <string>
#include "SDL.h"
#include <stdlib.h>
#ifdef __ANDROID__
#include <android/log.h>
#endif
#include "GUI_utils.h"

int SCREEN_WIDTH = 320;
int SCREEN_HEIGHT = 480;

static const char* LOGNAME = "__GUI__";

/*
    Produces a random int x, min <= x <= max
    following a uniform distribution
*/
int
randomInt(int min, int max)
{
    return min + rand() % (max - min + 1);
}

/*
    Produces a random float x, min <= x <= max
    following a uniform distribution
 */
float
randomFloat(float min, float max)
{
    return rand() / (float) RAND_MAX *(max - min) + min;
}

void
fatalError(const char *string)
{
    printf("%s: %s\n", string, SDL_GetError());
    exit(1);
}

void
GUI_log( const char * format, ... )
{
#ifdef __ANDROID__
    char buffer[512];
    va_list args;
    va_start (args, format);
    vsprintf (buffer,format, args);
    va_end (args);

    __android_log_print(ANDROID_LOG_VERBOSE, LOGNAME, buffer, 1 );
#else
    va_list args;
    va_start (args, format);
    vprintf (format, args);
    va_end (args);
#endif
}

/*
 * Get the resource path for resources located in res/subDir
 * It's assumed the project directory is structured like:
 * bin/
 *  the executable
 * res/
 *  Lesson1/
 *  Lesson2/
 *
 * Paths returned will be Project_Root/res/subDir
 */
std::string GUI_getResourcePath(const std::string &subDir)
{
#ifdef _WIN32
	const char PATH_SEP = '\\';
#else
	const char PATH_SEP = '/';
#endif
	static std::string baseRes;
    
    baseRes = "";
#ifndef __ANDROID__
	if (baseRes.empty()){
		char *basePath = SDL_GetBasePath();
		if (basePath){
			baseRes = basePath;
			SDL_free(basePath);
		}
		else {
			std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
			return "";
		}
		//We replace the last bin/ with res/ to get the the resource path
		size_t pos = baseRes.rfind("bin");
		baseRes = baseRes.substr(0, pos) + "data" + PATH_SEP;
	}
#else
    baseRes = baseRes + "data" + PATH_SEP;
#endif
	return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}