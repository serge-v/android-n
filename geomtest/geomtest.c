#define _USE_MATH_DEFINES

#include <jni.h>
#include <GLES/gl.h>
#include <stdio.h>
#include <math.h>
#include "../common/app.h"

HWND hWnd;
HDC hDC;
HGLRC hRC;
static int sWindowWidth = 400;
static int sWindowHeight = 300;
int mouse_x = 0, mouse_y = 0;
int mouse_px = -1, mouse_py = -1;
int mouse_lb = 0;

const char* g_appname = "frustum";
int g_log_level = 0;

void app_init(int w, int h)
{
//     glViewport(0, 0, w, h);
//     glEnable(GL_DEPTH);
}

RECT rc = { 20, 20, 40, 40 };

void app_render_ortho(long tick, int w, int h)
{
	POINT pt = { mouse_x, mouse_y };

	check_glerrors();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, w, 0.0, h, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	check_glerrors();
	glLoadIdentity();
	check_glerrors();

	if (PtInRect(&rc, pt)) {
		if (mouse_lb)
			glColor3b(50, 90, 50);
		else
			glColor3b(90, 50, 50);
	} else
		glColor3b(50, 50, 50);

	glBegin(GL_LINES);

	glVertex3i(rc.left, rc.top, 10);
	glVertex3i(rc.right, rc.top, 10);

	glVertex3i(rc.left, rc.bottom, 0);
	glVertex3i(rc.right, rc.bottom, 0);

	glVertex3i(rc.left, rc.top, 0);
	glVertex3i(rc.left, rc.bottom, 0);

	glVertex3i(rc.right, rc.top, 0);
	glVertex3i(rc.right, rc.bottom, 0);

	glEnd();
}

struct rectf {
	float l, t, r, b;
};



float fl = -2.0f, fr = 2.0f, fb = -2.0f, ft = 2.0f;
float ra = 0.0f;

void perspective(double fovy, double aspect, double zNear, double zFar)
{
	double xmin, xmax, ymin, ymax;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	ymax = zNear * tan(fovy * M_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;


	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glDepthMask(GL_TRUE);
}

void app_render(long tick, int w, int h)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	check_glerrors();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(fl, fr, fb, ft, 1.0f, 10.0f);
	glTranslatef(0, 0, -2);
	check_glerrors();

	glMatrixMode(GL_MODELVIEW);
	check_glerrors();
	glLoadIdentity();
	check_glerrors();

	glRotated(45, 0, 0, 1);
//    glRotated(45, 0, 1, 0);
	glRotated(45, 0, 0, 1);
	glBegin(GL_LINES);
	glColor3b(80, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glColor3b(0, 80, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glColor3b(0, 0, 80);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);

	glEnd();

	glPopMatrix();
}

void app_touch(int release, int x, int y)
{
	mouse_x = x;
	mouse_y = y;

	if (mouse_x < 0 || mouse_y < 0 || mouse_x >= sWindowWidth || mouse_y >= sWindowHeight)
		return;
	else if (mouse_lb == 1 && mouse_px == -1) {
		mouse_px = x;
		mouse_py = y;
	} else if (mouse_lb == 0 && mouse_px > 0) {
		mouse_px = -1;
		mouse_py = -1;
	} else if (mouse_lb == 1 && mouse_px != -1) {
		OffsetRect(&rc, mouse_x - mouse_px, mouse_y - mouse_py);
		mouse_px = mouse_x;
		mouse_py = mouse_y;
	}
}

void app_key(int key)
{
	if (key == 'A')
		fl -= 0.1f;
	else if (key == 'S')
		fl += 0.1f;
	else if (key == 'D')
		ft -= 0.1f;
	else if (key == 'F')
		ft += 0.1f;
	else if (key == 'G')
		fb -= 0.1f;
	else if (key == 'H')
		fb += 0.1f;
	else if (key == 'J')
		fr -= 0.1f;
	else if (key == 'K')
		fr += 0.1f;
	else if (key == 'Z')
		ra -= 1.0f;
	else if (key == 'X')
		ra += 1.0f;
}

void create_controls()
{
}

void app_deinit()
{
}
