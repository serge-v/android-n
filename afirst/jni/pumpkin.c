#include "pumpkin.h"
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include "compat.h"
#include "log.h"
#include "resources.h"

static const struct point particle_vertices[] = {
	{ 0, 0.5f }, { 0, 0 },
	{ 0.5f, 0.5f }, { 0.5f, 0 }
};

void pumpkin_create(struct pumpkin* p, const char* name, float x, float y)
{
	memset(p, 0, sizeof(struct pumpkin));

	p->stage = PUMPKIN_CREATED;
	p->name = strdup(name);
	p->x = x;
	p->y = y;
	p->scale = 0.4f;
	p->rot_angle = 0;
	p->rot_speed = 180;
	p->particles = malloc(sizeof(struct particles));
	p->particles->count = 4;
	p->particles->tex_vertices = malloc(sizeof(struct point) * p->particles->count);
	p->particles->pos_vertices = particle_vertices;
}

static const struct point pic_tex[] = {
	{ 0, 1 }, { 0, 0 },
	{ 1, 1 }, { 1, 0 }
};

void pumpkin_draw(const struct pumpkin* p)
{
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glScalef(p->scale, p->scale, 0);

	// draw mask
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
	glBindTexture(GL_TEXTURE_2D, R.pumpkin_mask_pic.tex);
	glVertexPointer(2, GL_FLOAT, sizeof(struct point), R.pumpkin_mask_pic.vertices);
	LOGGLE(">>glVertexPointer");
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct point), pic_tex);
	LOGGLE("glTexCoordPointer");

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// draw picture
	if (p->stage == PUMPKIN_SPOILED)
		glBindTexture(GL_TEXTURE_2D, R.pumpkin_spoiled_pic.tex);
	else if (p->stage == PUMPKIN_RED || p->stage == PUMPKIN_PICKING)
		glBindTexture(GL_TEXTURE_2D, R.pumpkin_red_pic.tex);
	else if (p->stage == PUMPKIN_YELLOW)
		glBindTexture(GL_TEXTURE_2D, R.pumpkin_yellow_pic.tex);
	else
		glBindTexture(GL_TEXTURE_2D, R.pumpkin_green_pic.tex);

	glVertexPointer(2, GL_FLOAT, sizeof(struct point), R.pumpkin_green_pic.vertices);
	LOGGLE(">>glVertexPointer");

	glTexCoordPointer(2, GL_FLOAT, sizeof(struct point), pic_tex);
	LOGGLE("glTexCoordPointer");

	glBlendFunc(GL_ONE, GL_ONE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	LOGGLE("draw_square draw arrays");

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glPopMatrix();
}

void pumpkin_recalc(struct pumpkin* p, long ms)
{
	if (p->stage == PUMPKIN_CREATED) {
		p->age += ms;
		p->scale = p->age * 0.002f;
		if (p->age > 5000 || p->scale > 0.4f) {
			p->stage = PUMPKIN_GREEN;
			p->age = 0;
		}
	} else if (p->stage == PUMPKIN_PICKING) {
		p->age += ms;
		p->scale = 0.4f - p->age / 50 * 0.1f;
		if (p->age > 1000 || p->scale < 0.05f)
			p->stage = PUMPKIN_PICKED;
	} else if (p->stage == PUMPKIN_GREEN) {
		p->age += ms;
		p->scale = 0.4f + p->age / 2000 * 0.05f;
		if (p->age > 5000) {
			p->stage = PUMPKIN_YELLOW;
			p->age = 0;
		}
	} else if (p->stage == PUMPKIN_YELLOW) {
		p->age += ms;
		p->scale = 0.6f + p->age / 2000 * 0.05f;
		if (p->age > 5000) {
			p->stage = PUMPKIN_RED;
			p->age = 0;
		}
	} else if (p->stage == PUMPKIN_RED) {
		p->age += ms;
		p->scale = 0.8f + p->age / 2000 * 0.05f;
		if (p->age > 5000) {
			p->stage = PUMPKIN_SPOILING;
			p->age = 0;
		}
	}
}

void pumpkin_destroy(struct pumpkin* p)
{
	if (p->name)
		free((char*)p->name);

	if (p->particles->tex_vertices)
		free((struct point*)p->particles->tex_vertices);

	if (p->particles)
		free(p->particles);

	memset(p, 0, sizeof(struct pumpkin));
}

int pumpkin_is_eaten(struct pumpkin* p, float worm_x, float worm_y)
{
	if (p->x - worm_x < 0 && p->y - worm_y < 0 && p->x - worm_x > -2 && p->y - worm_y > -1)
		return 1;

	return 0;
}
