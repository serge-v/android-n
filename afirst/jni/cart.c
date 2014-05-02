#include "cart.h"
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include "compat.h"
#include "log.h"
#include "pic.h"
#include "resources.h"

void cart_create(struct cart* p, const char* name, float x, float y)
{
    memset(p, 0, sizeof(struct cart));

    p->name = strdup(name);
    p->scale = 3;
    p->speed = 0;
    p->x = x;
    p->y = y;
    p->state = CART_LOADING;
    p->pumpkin_count = 0;
    p->trips = 0;
}

void cart_draw(const struct cart* p)
{
    int i;

    glLoadIdentity();
    glTranslatef(p->x - 1.2f, p->y + 0.8f, 0);

    for (i = 0; i < p->pumpkin_count; i++)
    {
        glTranslatef(0.4f, 0, 0);
        pic_draw2(&R.pumpkin_red_pic, &R.pumpkin_mask_pic);
    }

    glLoadIdentity();
    glTranslatef(p->x, p->y, 0);
    glScalef(p->scale, p->scale, 0);
    pic_draw2(&R.cart_pic, &R.cart_mask_pic);

    glLoadIdentity();
    glTranslatef(p->x - 1.25f, p->y + 0.16f, 0);
    glScalef(0.3f, 0.3f, 0);

    for (i = 0; i < p->trips; i++)
    {
        glTranslatef(1.1f, 0, 0);
        pic_draw2(&R.pumpkin_red_pic, &R.pumpkin_mask_pic);
    }
}

void cart_recalc(struct cart* p, long ms)
{
    p->age += ms;

    if (p->pumpkin_count >= 5 && p->state == CART_LOADING)
    {
        p->state = CART_MOVEOUT;
        p->speed = 3;
        p->age = 0;
        return;
    }

    if (p->state == CART_MOVEOUT)
    {
        p->x += p->speed * (float)ms / 1000.0f;
        if (p->x > 13)
        {
            p->state = CART_MOVEIN;
            p->speed = -3;
            p->age = 0;
            p->pumpkin_count = 0;
        }
        return;
    }
    
    
    if (p->state == CART_MOVEIN)
    {
        p->x += p->speed * (float)ms / 1000.0f;
        if (p->x < 1)
        {
            p->state = CART_LOADING;
            p->speed = 0;
            p->age = 0;
            p->trips++;
        }
        return;
    }
}

void cart_destroy(struct cart* p)
{
    if (p->name)
        free((char*)p->name);

    memset(p, 0, sizeof(struct cart));
}
