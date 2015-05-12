#include "pumpkin.h"
#include "worm.h"
#include "cart.h"
#include "leika.h"
#include "spray.h"

enum game_state {
	GAME_PLAY,
	GAME_OVER
};

struct game {
	enum game_state state;
	int max_x;
	int max_y;
	int pumpkin_count;              // total pumpkin count. Max is 8*3
	int spoiled_count;
	int pickup_count;
	long since_last_pumpkin;        // milliseconds since last pumpkin was created
	long elapsed;                   // milliseconds since start

	struct pumpkin field[8][3];     // pumpkin field
	struct worm worm;               // worm who eats pumpkins
	struct cart cart;               // cart for pumpkin transportation
	struct leika leika;             // water can for pumpkin watering
	struct spray spray;             // spray agains the worm
};

int game_create(struct game* g);
int game_destroy(struct game* g);
void game_clear(struct game* g);
void game_elapse(struct game* g, long ms);
void game_touch(struct game* g, int touch_x, int touch_y);
