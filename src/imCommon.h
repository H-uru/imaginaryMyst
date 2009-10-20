#ifndef _IM_COMMON_H
#define _IM_COMMON_H

#include <SDL.h>
#include <SDL_opengl.h>
#include "vfs/imVfs.h"

void imLog(const char* message, ...);
void imLog(const char* message, va_list args);

extern imString s_rootPath;
extern imVfs s_vfs;
extern FILE* s_logFile;
extern SDL_Surface* s_display;

/* OpenGL Extensions */
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2D_EXT;

#endif
