#include "molecule.h"
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include "pic.h"
#include "resources.h"

void molecule_create(struct molecule* p, const char* name, float x, float y)
{
	memset(p, 0, sizeof(struct molecule));

	p->name = strdup(name);
	p->scale = 1;
	p->x = x;
	p->y = y;
}

void molecule_draw(const struct molecule* p)
{
	pic_draw2(&R.hidrogen_pic, &R.hidrogen_mask_pic);
}

void molecule_recalc(struct molecule* p, long ms)
{
}

void molecule_destroy(struct molecule* p)
{
	if (p->name)
		free((char*)p->name);

	memset(p, 0, sizeof(struct molecule));
}
