#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

void app_render(long ticj, int w, int h)
{
	glViewport(0, 0, w, h);
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.25, 1.25, -1.25, 1.25, 1., 20.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0,  1.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0,  1.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glEnd();
}

void app_init(int w, int h)
{
}

void app_deinit()
{
}
