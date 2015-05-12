#ifndef CART_H
#define CART_H

enum cart_state {
	CART_LOADING,
	CART_MOVEOUT,
	CART_MOVEIN
};

struct cart {
	enum cart_state     state;
	float               x;                      // x coordinate
	float               x0;                     // base x coordinate
	float               y;                      // y coordinate
	float               scale;                  // scale
	const char*         name;                   // name for debugging
	long                age;                    // age in ms
	float               speed;                  // cart speed (positive - out, negative - in)
	int                 pumpkin_count;          // pumpkins loaded into cart
	int                 trips;                  // count of successfull loaded trips
};

void cart_create(struct cart* p, const char* name, float x, float y);
void cart_draw(const struct cart* p);
void cart_recalc(struct cart* p, long ms);
void cart_destroy(struct cart* p);

#endif
