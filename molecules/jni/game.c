#include "audio.h"
#include "game.h"
#include "pic.h"
#include "log.h"
#include <errno.h>
#include <stdlib.h>
#include <memory.h>
#include "sound.h"

int game_create(struct game* g)
{
	memset(g, 0, sizeof(struct game));

	g->state = GAME_PLAY;
	g->max_x = 8;
	g->max_y = 3;

	molecule_create(&g->mol, "mol", 1.0f, 1.0f);

	sound_play(sound_bg);

	return 0;
}


int game_destroy(struct game* g)
{
	memset(g, 0, sizeof(struct game));
	return 0;
}

void game_clear(struct game* g)
{
	g->elapsed = 0;
	g->state = GAME_PLAY;
	LOGT("game_clear");
}

void game_touch(struct game* g, int touch_x, int touch_y)
{
}

void game_elapse(struct game* g, long ms)
{
	if (g->state == GAME_OVER)
		return;

	g->elapsed += ms;
}
