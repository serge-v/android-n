#include "worm.h"
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include "compat.h"
#include "log.h"
#include "pic.h"
#include "resources.h"

void worm_create(struct worm* p, const char* name, float x, float y)
{
    memset(p, 0, sizeof(struct worm));

    p->name = strdup(name);
    p->scale = 1;
    p->x = x;
    p->y = y;
}

void worm_draw(const struct worm* p)
{
    struct pic *pic, *mask;

    if (p->crawling == 1)
    {
        mask = &R.worm1_mask_pic;
        pic = &R.worm1_pic;
    }
    else if (p->crawling == 2)
    {
        mask = &R.worm2_mask_pic;
        pic = &R.worm2_pic;
    }
    else
    {
        mask = &R.worm3_mask_pic;
        pic = &R.worm3_pic;
    }

    pic_draw2(pic, mask);
}

void worm_recalc(struct worm* p, long ms)
{
    p->age += ms;
    if (p->age > 300)
    {
        p->crawling = (p->crawling == 3) ? 2 : 3;
        if (p->crawling == 2)
            p->x += 0.6f;
        p->age = 0;
    }
    
    if (p->x > 10)
    {
       p->x = -1;
       p->y += 1;
       if (p->y > 3)
       {
          p->y = 0.5f;
       }
    }
}

void worm_destroy(struct worm* p)
{
    if (p->name)
        free((char*)p->name);

    memset(p, 0, sizeof(struct worm));
}
