#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>

FILE* g_log = NULL;
int g_app_alive = 1;

static const char* levels[] =
{
	"LEVEL0",
	"LEVEL1",
	"TRACE",
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL"
};

static int
print_level(int level)
{
	time_t now = time(NULL);
	struct tm* t = gmtime(&now);
	return fprintf(g_log, "%04d-%02d-%02d %02d:%02d:%02d %s ",
	               t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, levels[level]);
}

int
__android_log_print(int prio, const char *tag,  const char *fmt, ...)
{
	va_list marker;
	int n;

	va_start(marker, fmt);
	n = print_level(prio);
	n += vfprintf(g_log, fmt, marker);
	n += fprintf(g_log, "\n");
	fflush(g_log);
	return n;
}
