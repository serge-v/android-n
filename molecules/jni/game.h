#include "molecule.h"

enum game_state {
	GAME_PLAY,
	GAME_OVER
};

struct game {
	enum game_state state;
	int max_x;
	int max_y;
	long elapsed;                   // milliseconds since start

	struct molecule mol;            // first molecule
};

int game_create(struct game* g);
int game_destroy(struct game* g);
void game_clear(struct game* g);
void game_elapse(struct game* g, long ms);
void game_touch(struct game* g, int touch_x, int touch_y);
