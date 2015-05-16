struct molecule {
	float               x;                      // x coordinate
	float               y;                      // y coordinate
	float               scale;                  // scale
	const char*         name;                   // name for debugging
};

void molecule_create(struct molecule* p, const char* name, float x, float y);
void molecule_draw(const struct molecule* p);
void molecule_recalc(struct molecule* p, long ms);
void molecule_destroy(struct molecule* p);
