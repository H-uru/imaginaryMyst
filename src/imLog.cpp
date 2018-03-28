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

#include "imCommon.h"
#include "imThread.h"

FILE* s_logFile = stderr;

void imLog(const char* message, ...)
{
    va_list va;
    va_start(va, message);
    imLog(message, va);
    va_end(va);
}

void imLog(const char* message, va_list args)
{
    LOCK_FUNCTION

    vfprintf(s_logFile, message, args);
    fprintf(s_logFile, "\n");
}
