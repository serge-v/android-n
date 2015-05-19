#include "audio.h"
#include "game.h"
#include "pic.h"
#include "log.h"
#include <errno.h>
#include <stdlib.h>
#include <memory.h>
#include "sound.h"

int
game_create(struct game* g)
{
	memset(g, 0, sizeof(struct game));

	g->state = GAME_PLAY;
	g->max_x = 24;
	g->max_y = 12;

	molecule_create(&g->mol, "ethylene", 1, 2);

	g->strip.head = calloc(1, sizeof(struct molecule));
	molecule_create(g->strip.head, "ethylene", 1, 2);
	g->strip.count = 1;
	g->strip.speed = 1;

	sound_play(sound_bg);

	return 0;
}

static void
strip_destroy(struct strip* sr)
{
	struct molecule *m, *tmp;
	for (m = sr->head; m != NULL;) {
		tmp = m;
		m = m->next;
		molecule_destroy(tmp);
		free(tmp);
	}
	memset(sr, 0, sizeof(struct strip));
}

static void
strip_recalc(struct strip* sr, long ms)
{
	sr->last_age += ms;

	struct molecule *m, *last_valid = sr->head, *tmp;
	float dx = sr->speed * (float)ms / 1000.0f;

	for (m = sr->head; m != NULL; m = m->next) {
		m->x += dx;
		if (m->x < 28)
			last_valid = m;
	}

	if (last_valid != NULL && last_valid->next != NULL) {
		for (m = last_valid->next; m != NULL;) {
			tmp = m;
			m = m->next;
			free(tmp);
			sr->count--;
		}
		last_valid->next = NULL;
	}

	if (sr->head->x > 1) {
		m = calloc(1, sizeof(struct molecule));
		molecule_create(m, "ethylene", -6, 2);
		m->next = sr->head;
		sr->head = m;
		sr->count++;
	}
}

int
game_destroy(struct game* g)
{
	strip_destroy(&g->strip);
	memset(g, 0, sizeof(struct game));
	return 0;
}

void
game_clear(struct game* g)
{
	g->elapsed = 0;
	g->state = GAME_PLAY;
	LOGT("game_clear");
}

void
game_touch(struct game* g, int touch_x, int touch_y)
{
	for (struct molecule *m = g->strip.head; m != NULL; m = m->next) {
		if ((touch_x-4) < m->x && (touch_x+2) > m->x)
			m->y += 1;
	}
}

void
game_elapse(struct game* g, long ms)
{
	if (g->state == GAME_OVER)
		return;

	g->elapsed += ms;
	strip_recalc(&g->strip, ms);
}
