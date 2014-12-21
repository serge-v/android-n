#include <jni.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GLES/gl.h>
#include <stdio.h>
#include "../common/app.h"
#include "../common/pic.h"
#include "cart.png.h"

static int sWindowWidth = 400;
static int sWindowHeight = 400;

void
app_touch(int release, int w, int h)
{
}

struct pic cart_pic;

void
app_init(int w, int h)
{
	pic_load_png_data(&cart_pic, cart_png, cart_png_len);
}

void
app_render(long ticj, int w, int h)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, w, h);
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	pic_draw(&cart_pic);

}

void
app_key(int key)
{
}

void
create_controls()
{
}

void
app_deinit()
{
}
