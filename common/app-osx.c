#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <GLUT/glut.h>
#include "log.h"
#include "app.h"

//GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
GLuint texture_id;
int pixmap_width = 128, pixmap_height = 128;
int window_width = 600, window_height = 400;

const char* g_appname = "";
int g_log_level = 0;
/*
static void
demo(void)
{
	int i; double angle;
	glClear(GL_COLOR_BUFFER_BIT);
	for (i=0; i<360; i+=4) {
		glColor3f((float)i/360.0,1.0,1.0);
		glBegin(GL_LINES);
		glVertex2d(cos(i/57.25779),sin(i/57.25779));
		glVertex2d(cos((i+90)/57.25779),sin((i+90)/57.25779));
		glEnd();

		glColor3f(1.0, (float)i/360.0,1.0);
		glBegin(GL_LINES);
		glVertex2d(cos(i/57.25779),sin(i/57.25779));
		glVertex2d(cos((i*2)/57.25779),sin((i+90)/57.25779));
		glEnd();
	}
	glLoadIdentity();
	glutSwapBuffers();
}
*/
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
}

int main(int argc, char *argv[])
{
	g_log = fopen("1.log", "wt");
	LOGI("log opened");

	app_init(window_width, window_height);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(window_width, window_height);
	glutCreateWindow("OpenGL window");

	glutDisplayFunc(display);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pixmap_height, pixmap_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)(&(xim->data[0])));

	glutMainLoop();
}
