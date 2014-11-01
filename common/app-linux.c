#include <stdio.h>
#include <time.h>
#include <jni.h>
#include <GLES/gl.h>
#include <GL/glut.h>
#include "app.h"
#include "log.h"

int g_app_alive = 1;
static FILE* g_log = NULL;

static const wchar_t sAppName[] = L"test";

static int W = 1280;
static int H = 800;

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

int print_level(int level)
{
	time_t now = time(NULL);
	struct tm* t = gmtime(&now);
	return fprintf(g_log, "%04d-%02d-%02d %02d:%02d:%02d %s ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, levels[level]);
}

void check_glerrors()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		wchar_t errorString[32];
		wprintf(L"0x%04x", error);
	}
}

int __android_log_print(int prio, const char *tag,  const char *fmt, ...)
{
	va_list marker;
	int n;
	tag;

	va_start(marker, fmt);
	n = print_level(prio);
	n += vfprintf(g_log, fmt, marker);
	n += fprintf(g_log, "\n");
	fflush(g_log);
	return n;
}

static long get_ms()
{
    struct timeval  now;
    gettimeofday(&now, NULL);
    return (long)(now.tv_sec * 1000 + now.tv_usec / 1000);
}

void displayMe(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	glVertex3f(0.0, 0.5, 0.0);
	glEnd();
	app_render(get_ms(), W, H);
	glFlush();
}

int main(int argc, char** argv)
{
	g_log = fopen("1.log", "wt");
	LOGI("log opened");

	app_init(W, H);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Hello world :D");
	glutDisplayFunc(displayMe);
	glutMainLoop();

	return 0;
}
