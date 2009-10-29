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
