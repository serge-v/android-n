#include "hidrogen.h"
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include "pic.h"
#include "resources.h"

void hidrogen_create(struct hidrogen* p, const char* name, float x, float y)
{
	memset(p, 0, sizeof(struct hidrogen));

	p->name = strdup(name);
	p->scale = 1;
	p->x = x;
	p->y = y;
}

void spray_draw(const struct spray* p)
{
	pic_draw2(&R.spray_pic, &R.spray_mask_pic);
}

void spray_recalc(struct spray* p, long ms)
{
	p->age += ms;
}

void spray_destroy(struct spray* p)
{
	if (p->name)
		free((char*)p->name);

	memset(p, 0, sizeof(struct spray));
}
