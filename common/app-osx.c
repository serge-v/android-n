#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <GLUT/glut.h>
#include "log.h"
#include "app.h"

GLuint texture_id;
int pixmap_width = 128, pixmap_height = 128;
int window_width = 600, window_height = 400;

const char* g_appname = "";
FILE* g_log = NULL;
int g_log_level = 0;

static void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	struct timeval timeNow;
	gettimeofday(&timeNow, NULL);
	uint64_t ms = timeNow.tv_sec * 1000 + timeNow.tv_usec / 1000;
	app_render(ms, window_width, window_height);

	glLoadIdentity();
	glutSwapBuffers();
	glutPostRedisplay();
}

static void
mouse(int button, int state, int x, int y)
{
	LOGI("mouse: b,s: %d,%d, x,y: %d,%d", button, state, x, y);
	app_touch(state + 1, x, window_height - y);
}

int main(int argc, char *argv[])
{
	g_log = fopen("1.log", "wt");
	LOGI("log opened");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(window_width, window_height);
	glutCreateWindow("OpenGL window");

	glutDisplayFunc(display);
	glutMouseFunc(mouse);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pixmap_height, pixmap_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)(&(xim->data[0])));

	app_init(window_width, window_height);

	glutMainLoop();
}

static const char* levels[] = {
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
