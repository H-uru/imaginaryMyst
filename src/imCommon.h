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
#include <string_theory/stdio>
#include "vfs/imVfs.h"

template <typename... args_T>
void imLog(const char *message, args_T ...args)
{
    extern FILE *s_logFile;
    ST::printf(s_logFile, message, std::forward<args_T>(args)...);
    fputs("\n", s_logFile);
}

extern ST::string s_rootPath;
extern imVfs s_vfs;
extern SDL_Window* s_display;

/* OpenGL Extensions */
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC GLX_CompressedTexImage2D;

#endif
