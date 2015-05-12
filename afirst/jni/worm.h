#ifndef WORM_H
#define WORM_H

struct worm {
	float               x;                      // x coordinate
	float               y;                      // y coordinate
	float               scale;                  // scale
	const char*         name;                   // name for debugging
	long                age;                    // age in ms
	int                 crawling;               // crawling stage
};

void worm_create(struct worm* p, const char* name, float x, float y);
void worm_draw(const struct worm* p);
void worm_recalc(struct worm* p, long ms);
void worm_destroy(struct worm* p);

#endif
