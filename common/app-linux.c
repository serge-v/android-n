#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "log.h"
#include "app.h"

Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
Pixmap                  pixmap;
int                     pixmap_width = 128, pixmap_height = 128;
GC                      gc;
XImage                  *xim;
GLuint                  texture_id;

const char* g_appname = "";
int g_log_level = 0;

int main(int argc, char *argv[])
{
	g_log = fopen("1.log", "wt");
	LOGI("log opened");

	XEvent xev;

	dpy = XOpenDisplay(NULL);

	if (dpy == NULL) {
		printf("\n\tcannot open display\n\n");
		exit(0);
	}

	root = DefaultRootWindow(dpy);

	vi = glXChooseVisual(dpy, 0, att);

	if (vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(0);
	}

	swa.event_mask = ExposureMask | KeyPressMask;
	swa.colormap   = XCreateColormap(dpy, root, vi->visual, AllocNone);

	win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWEventMask  | CWColormap, &swa);
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "PIXMAP TO TEXTURE");

	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);

	if (glc == NULL) {
		printf("\n\tcannot create gl context\n\n");
		exit(0);
	}

	glXMakeCurrent(dpy, win, glc);
	glEnable(GL_DEPTH_TEST);

	pixmap = XCreatePixmap(dpy, root, pixmap_width, pixmap_height, vi->depth);
	gc = DefaultGC(dpy, 0);

	XSetForeground(dpy, gc, 0x00c0c0);
	XFillRectangle(dpy, pixmap, gc, 0, 0, pixmap_width, pixmap_height);

	XSetForeground(dpy, gc, 0x000000);
	XFillArc(dpy, pixmap, gc, 15, 25, 50, 50, 0, 360 * 64);

	XSetForeground(dpy, gc, 0x0000ff);
	XDrawString(dpy, pixmap, gc, 10, 15, "PIXMAP TO TEXTURE", strlen("PIXMAP TO TEXTURE"));

	XSetForeground(dpy, gc, 0xff0000);
	XFillRectangle(dpy, pixmap, gc, 75, 75, 45, 35);

	XFlush(dpy);
	xim = XGetImage(dpy, pixmap, 0, 0, pixmap_width, pixmap_height, AllPlanes, ZPixmap);

	if (xim == NULL)
		printf("\n\tximage could not be created.\n\n");

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pixmap_height, pixmap_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)(&(xim->data[0])));

	XDestroyImage(xim);

	XWindowAttributes gwa;
	XGetWindowAttributes(dpy, win, &gwa);
	app_init(gwa.width, gwa.height);

	while (g_app_alive) {
		XNextEvent(dpy, &xev);

		if (xev.type == KeyPress) {
			int keysyms_per_keycode_return;
			KeySym *keysym = XGetKeyboardMapping(dpy, xev.xkey.keycode, 1, &keysyms_per_keycode_return);
			if (*keysym == XK_Escape)
				g_app_alive = 0;
			else {
				LOGI("key: %c(%x)", (int)*keysym, (int)*keysym);
				app_key(toupper(*keysym));
			}
			XFree(keysym);
		}

		if (g_app_alive) {
			struct timeval timeNow;
			gettimeofday(&timeNow, NULL);
			uint64_t ms = timeNow.tv_sec * 1000 + timeNow.tv_usec / 1000;
			XWindowAttributes gwa;
			XGetWindowAttributes(dpy, win, &gwa);
			app_render(ms, gwa.width, gwa.height);
//			app_render(ms, 400, 400);
			glXSwapBuffers(dpy, win);
		}
	}

	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}
