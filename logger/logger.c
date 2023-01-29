#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void PrintTime()
{
   time_t timer;
   char buffer[26];
   struct tm* tm_info;
   timer = time(NULL);
   tm_info = localtime(&timer);

   strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
   fprintf(stdout, "%s: ", buffer);
}

void LogInfo(const char* fmt, ...)
{
   PrintTime();
   va_list args;
   va_start(args, fmt);
   vfprintf(stdout, fmt, args);
   fprintf(stdout, "\n");
   va_end(args);
}

void LogDebug(const char* fmt, ...)
{
#ifdef DEBUG
   PrintTime();
   va_list args;
   va_start(args, fmt);
   vfprintf(stdout, fmt, args);
   fprintf(stdout, "\n");
   va_end(args);
#endif
}
