#define WIN32_LEAN_AND_MEAN

#include <jni.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GLES/gl.h>
#include <stdio.h>
#include "../common/app.h"

//static HWND frustLabel;
static int sWindowWidth = 400;
static int sWindowHeight = 400;

struct rectf
{
	float l, t, r, b;
};

struct vecf
{
	float x, y, z;
};

static struct rectf frc = { -2, 2, 2, -2 };
static struct vecf tran = { 0, 0, -3 };
static struct vecf rot = { 50, 70, 0 };

void
app_touch(int release, int w, int h)
{
}

void
update_controls()
{
	char text[1000];
	sprintf(text, 
		"\n\n=====\n"
		"frustum: l:%.2f r:%.2f b:%.2f t:%.2f\n"
		"         AZ      SX     DC      FV\n"
		"tran:    x:%.2f, y:%.2f, z:%.2f\n"
		"         QW      ER     TY\n"
		"rot:     x:%.0f, y:%.0f, z:%.0f\n"
		"         GB      HN     JM\n",
		frc.l, frc.r, frc.b, frc.t,
		tran.x, tran.y, tran.z,
		rot.x, rot.y, rot.z);
//	SetWindowText(frustLabel, text);
	printf("%s", text);
}

void app_init(int w, int h)
{
/*	update_controls();

	glClearColor(0.1f, 0.39f, 0.88f, 1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(frc.l, frc.r, frc.b, frc.t, 1, 40);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(tran.x, tran.y, tran.z);
	glRotatef(rot.x, 1, 0, 0);
	glRotatef(rot.y, 0, 1, 0);
	glRotatef(rot.z, 0, 0, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColor3f(1, 0, 0);
	glEnable(GL_COLOR_MATERIAL);*/
}

void app_render2(long ticj, int w, int h)
{
	glViewport(0, 0, w, h);
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glOrtho(-1.25, 1.25, -1.25, 1.25, 1., 20.);
	glFrustum(frc.l, frc.r, frc.b, frc.t, 1, 40);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(tran.x, tran.y, tran.z);
	glRotatef(rot.x, 1, 0, 0);
	glRotatef(rot.y, 0, 1, 0);
	glRotatef(rot.z, 0, 0, 1);
//	gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

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

	update_controls();
}

void app_render(long ticj, int w, int h)
{
	GLfloat i;
	//glClear(GL_COLOR_BUFFER_BIT);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//==
	glViewport(0, 0, w, h);
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glOrtho(-1.25, 1.25, -1.25, 1.25, 1., 20.);
	glFrustum(frc.l, frc.r, frc.b, frc.t, 1, 40);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(tran.x, tran.y, tran.z);
	glRotatef(rot.x, 1, 0, 0);
	glRotatef(rot.y, 0, 1, 0);
	glRotatef(rot.z, 0, 0, 1);
	//==

	// Draw a white grid "floor" for the tetrahedron to sit on.
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	for (i = -2.5; i <= 2.5; i += 0.25)
	{
		glVertex3f(i, 0, 2.5);
		glVertex3f(i, 0, -2.5);
		glVertex3f(2.5, 0, i);
		glVertex3f(-2.5, 0, i);
	}
	glEnd();

	// Draw the tetrahedron.  It is a four sided figure, so when defining it
	// with a triangle strip we have to repeat the last two vertices.
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1, 1, 1);
	glVertex3f(0, 2, 0);
	glColor3f(1, 0, 0);
	glVertex3f(-1, 0, 1);
	glColor3f(0, 1, 0);
	glVertex3f(1, 0, 1);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, -1.4f);
	glColor3f(1, 1, 1);
	glVertex3f(0, 2, 0);
	glColor3f(1, 0, 0);
	glVertex3f(-1, 0, 1);
	glEnd();
	
	update_controls();
}

void app_key(int key)
{
	if (key == 'A')
		frc.l -= 0.1f;
	else if (key == 'Z')
		frc.l += 0.1f;
	else if (key == 'S')
		frc.r -= 0.1f;
	else if (key == 'X')
		frc.r += 0.1f;
	else if (key == 'D')
		frc.b -= 0.1f;
	else if (key == 'C')
		frc.b += 0.1f;
	else if (key == 'F')
		frc.t -= 0.1f;
	else if (key == 'V')
		frc.t += 0.1f;

	else if (key == 'Q')
		tran.x -= 0.1f;
	else if (key == 'W')
		tran.x += 0.1f;
	else if (key == 'E')
		tran.y -= 0.1f;
	else if (key == 'R')
		tran.y += 0.1f;
	else if (key == 'T')
		tran.z -= 0.1f;
	else if (key == 'Y')
		tran.z += 0.1f;

	else if (key == 'G')
		rot.x -= 1;
	else if (key == 'B')
		rot.x += 1;
	else if (key == 'H')
		rot.y -= 1;
	else if (key == 'N')
		rot.y += 1;
	else if (key == 'J')
		rot.z -= 1;
	else if (key == 'M')
		rot.z += 1;

	app_init(sWindowWidth, sWindowHeight);
}

void create_controls()
{
//	frustLabel = CreateWindow(L"Static", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 1300, 300, 300, 100, g_hwnd, NULL, NULL, NULL);
//	SendMessage(frustLabel, WM_SETFONT, (WPARAM)GetStockObject(ANSI_FIXED_FONT), (LPARAM)NULL);
}

void app_deinit()
{
}
