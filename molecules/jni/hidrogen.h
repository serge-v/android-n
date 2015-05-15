struct hidrogen {
	float               x;                      // x coordinate
	float               y;                      // y coordinate
	float               scale;                  // scale
	const char*         name;                   // name for debugging
};

void hidrogen_create(struct spray* p, const char* name, float x, float y);
void hidrogen_draw(const struct spray* p);
void hidrogen_recalc(struct spray* p, long ms);
void hidrogen_destroy(struct spray* p);
