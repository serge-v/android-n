#include "spray.h"
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include "compat.h"
#include "log.h"
#include "pic.h"
#include "resources.h"

void spray_create(struct spray* p, const char* name, float x, float y)
{
	memset(p, 0, sizeof(struct spray));

	p->name = strdup(name);
	p->scale = 1;
	p->x = x;
	p->y = y;
}

void spray_draw(const struct spray* p)
{
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
