#include "leika.h"
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include "compat.h"
#include "log.h"
#include "pic.h"
#include "resources.h"

void leika_create(struct leika* p, const char* name, float x, float y)
{
	memset(p, 0, sizeof(struct leika));

	p->name = strdup(name);
	p->scale = 1;
	p->x = x;
	p->y = y;
}

void leika_draw(const struct leika* p)
{
	pic_draw2(&R.leika_pic, &R.leika_mask_pic);
}

void leika_recalc(struct leika* p, long ms)
{
	p->age += ms;
}

void leika_destroy(struct leika* p)
{
	if (p->name)
		free((char*)p->name);

	memset(p, 0, sizeof(struct leika));
}
