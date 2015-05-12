#ifndef LEIKA_H
#define LEIKA_H

struct leika {
	float               x;                      // x coordinate
	float               y;                      // y coordinate
	float               scale;                  // scale
	const char*         name;                   // name for debugging
	long                age;                    // age in ms
};

void leika_create(struct leika* p, const char* name, float x, float y);
void leika_draw(const struct leika* p);
void leika_recalc(struct leika* p, long ms);
void leika_destroy(struct leika* p);

#endif
