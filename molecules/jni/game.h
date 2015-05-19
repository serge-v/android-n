#include "molecule.h"

enum game_state {
	GAME_PLAY,
	GAME_OVER
};

struct strip {
	float speed;
	struct molecule *head;    // first molecule on the running strip
	int count;                // number of moleculrs on the strip
	long last_age;            // age of last molecule
};

struct game {
	enum game_state state;
	int max_x;
	int max_y;
	long elapsed;                   // milliseconds since start
	struct strip strip;             // running strip of molecules

	struct molecule mol;            // first test molecule
};

int game_create(struct game* g);
int game_destroy(struct game* g);
void game_clear(struct game* g);
void game_elapse(struct game* g, long ms);
void game_touch(struct game* g, int touch_x, int touch_y);
