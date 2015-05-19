struct atom {
	char symbol;           // chemical symbol
	int bond;              // number of chemical bonds to the parent
	float angle;           // angle from north clockwize relative to the parent
	struct atom *child;    // first child
	struct atom *next;     // next sibling
};

struct molecule {
	float               x;                      // x coordinate
	float               y;                      // y coordinate
	float               scale;                  // scale
	const char          *name;                  // name
	struct atom         *a;                     // central atom
	struct molecule     *next;                  // for lists
};

void molecule_create(struct molecule* p, const char* name, float x, float y);
void molecule_draw(const struct molecule* p);
void molecule_recalc(struct molecule* p, long ms);
void molecule_destroy(struct molecule* p);
