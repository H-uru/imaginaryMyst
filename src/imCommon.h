/* This file is part of imaginaryMyst.
 *
 * imaginaryMyst is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * imaginaryMyst is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with imaginaryMyst.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _IM_COMMON_H
#define _IM_COMMON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "vfs/imVfs.h"

void imLog(const char* message, ...);
void imLog(const char* message, va_list args);

extern imString s_rootPath;
extern imVfs s_vfs;
extern FILE* s_logFile;
extern SDL_Window* s_display;

/* OpenGL Extensions */
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC GLX_CompressedTexImage2D;

#endif
