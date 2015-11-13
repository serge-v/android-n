#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "app.h"

#include "resources.h"
#include "font.h"
#include "game.h"
#include "sound.h"

#include "gen/button_test.png.h"
#include "gen/button_quit.png.h"

static long last_tick = 0;
static float aspect = 1;    // aspect ratio
static float ux, uy;        // user to scene divider

static int mouse_x = -1, mouse_y = -1;
static float left = -0.4f, bottom = -1, size_x = 16, size_y = 16;

static int lb_down = 0;

static struct game g;
static struct pic button_test;
static struct pic button_quit;

static void
check_button(struct pic* p, float glx, float gly)
{
	if (glx > p->pos.x + p->vertices[0].x && glx < p->pos.x + p->vertices[3].x &&
	    gly > p->pos.y + p->vertices[1].x && gly < p->pos.y + p->vertices[2].y)
		p->selected = 1;
	else
		p->selected = 0;
}

void
app_touch(int release, int x, int y)
{
	float glx, gly;

	glx = x / ux + left;
	gly = y / uy + bottom;
	check_button(&button_test, glx, gly);
	check_button(&button_quit, glx, gly);

	if (release == 1) {
		if (button_quit.selected)
			g_app_alive = 0;

		mouse_x = (int)glx;
		mouse_y = (int)gly;
		LOGT("gl x,y: %.2f, %.2f cell x,y : %d, %d", glx, gly, mouse_x, mouse_y);

		lb_down = 1;

		if (g.state == GAME_OVER)
			game_clear(&g);

		if (mouse_x >= 0 && mouse_y >= 0 && mouse_x < g.max_x && mouse_y < g.max_y)
			game_touch(&g, mouse_x, mouse_y);
	} else if (release == 2)
		lb_down = 0;
}

static void
gl_init()
{
//	glEnable(GL_ALPHA_TEST);
//	glAlphaFunc(GL_GREATER, 0.5f);
//	glDisable(GL_DITHER);
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
//	glEnable(GL_DEPTH_TEST);
//	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

static void
gl_init_viewport(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(left, (left + size_x) * aspect, bottom, bottom + size_y, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 1.0);
//	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
}

void
app_init(int width, int height)
{
	aspect = (float)width / (float)height;
	ux = (float)width / size_x / aspect;
	uy = (float)height / size_y;

	resources_load();
	gl_init();
	pic_load_png_data(&button_test, button_test_png, button_test_png_len);
	pic_load_png_data(&button_quit, button_quit_png, button_quit_png_len);

	button_test.pos.x = 2.1f;
	button_test.pos.y = -0.7f;
	button_quit.pos.x = 1;
	button_quit.pos.y = -0.7f;

	font_create();
	sound_create();
	game_create(&g);
	gl_init_viewport(width, height);

	LOGI("app_init. w: %d, h: %d", width, height);
}

void
app_recalc(long tick)
{
	long ms;

	if (last_tick == 0) {
		last_tick = tick;
		return;
	}

	ms = tick - last_tick;

	game_elapse(&g, ms);

	last_tick = tick;
}

static void
draw_debug()
{
	char str[100];

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(3, -1, 0);
	glScalef(0.2f, 0.2f, 0);

	sprintf(str, "X %03d   Y %03d", mouse_x, mouse_y);
	draw_str(str, 1, 1);

	glPopMatrix();
}

static void
draw_controls()
{
	glLoadIdentity();
	glTranslatef(button_test.pos.x, button_test.pos.y, 0);
	pic_draw(&button_test);

	glLoadIdentity();
	glTranslatef(button_quit.pos.x, button_quit.pos.y, 0);
	pic_draw(&button_quit);
}

static void
draw_bg()
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(5.6f + 0.01f, 3 + 0.01f, 0);
	glScalef(48, 48, 0);
	pic_draw(&R.bg_pic);
	glPopMatrix();
}

static void
draw_objects()
{
	glPushMatrix();
	glLoadIdentity();

	struct molecule *m;

	for (m = g.strip.head; m != NULL; m = m->next)
		molecule_draw(m);

	glPopMatrix();
}

#define D 0.05f

struct point axises_vertices[] = {
	{ 0, 0 }, { 20, 0 },
	{ 0, 0 }, { 0, 20 },

	{ 5, 0 }, { 5, -0.2f },
	{ 0, 5 }, { -0.2f, 5 },
	{ 10, 0 }, { 10, -0.2f },
	{ 0, 10 }, { -0.2f, 10 },
	{ 15, 0 }, { 15, -0.2f },
	{ 0, 15 }, { -0.2f, 15 },

	{ 1, 0 }, { 1, D },
	{ 0, 1 }, { D, 1 },
	{ 2, 0 }, { 2, D },
	{ 0, 2 }, { D, 2 },
	{ 3, 0 }, { 3, D },
	{ 0, 3 }, { D, 3 },
	{ 4, 0 }, { 4, D },
	{ 0, 4 }, { D, 4 },

	{ 6, 0 }, { 6, D },
	{ 0, 6 }, { D, 6 },
	{ 7, 0 }, { 7, D },
	{ 0, 7 }, { D, 7 },
	{ 8, 0 }, { 8, D },
	{ 0, 8 }, { D, 8 },
	{ 9, 0 }, { 9, D },
	{ 0, 9 }, { D, 9 },

	{ 11, 0 }, { 11, D },
	{ 0, 11 }, { D, 11 },
	{ 12, 0 }, { 12, D },
	{ 0, 12 }, { D, 12 },
	{ 13, 0 }, { 13, D },
	{ 0, 13 }, { D, 13 },
	{ 14, 0 }, { 14, D },
	{ 0, 14 }, { D, 14 },

};

static void
draw_axises()
{
	glLoadIdentity();
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_FLOAT, sizeof(struct point), axises_vertices);
	glDrawArrays(GL_LINES, 0, sizeof(axises_vertices) / sizeof(axises_vertices[0]));

	glDisableClientState(GL_VERTEX_ARRAY);
}

static void
draw_title(enum game_state state) {
	glLoadIdentity();

	if (state == GAME_OVER)
	{
		glTranslatef(2, 2, 0);
		glScalef(0.5f, 0.5f, 0);
		draw_str("GAME OVER", 0, 0);
	}
}

void
app_render(long tick, int width, int height)
{
	app_recalc(tick);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);

	if (g.state == GAME_OVER)
		draw_title(g.state);

	draw_bg();
	draw_controls();
	draw_debug();
	draw_objects();
	draw_axises();
}

void
app_key(int key)
{
}

void
app_deinit()
{
	game_destroy(&g);
	font_destroy();
	sound_destroy();
	LOGI("app_deinit");
}

void
create_controls()
{
}
