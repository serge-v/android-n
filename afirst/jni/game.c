#include "audio.h"
#include "game.h"
#include "pic.h"
#include "log.h"
#include <errno.h>
#include <stdlib.h>
#include <memory.h>
#include "sound.h"
#include "worm.h"
#include "cart.h"
#include "spray.h"

int game_create(struct game* g)
{
    memset(g, 0, sizeof(struct game));

    g->state = GAME_PLAY;
    g->max_x = 8;
    g->max_y = 3;

    worm_create(&g->worm, "worm", 0, 1.5f);
    cart_create(&g->cart, "cart", 2, 3.8f);
    leika_create(&g->leika, "leika", 9.2f, 0.5f);
    spray_create(&g->spray, "spray", 9.2f, 1.5f);

    sound_play(sound_bg);

    return 0;
}


int game_destroy(struct game* g)
{
    memset(g, 0, sizeof(struct game));
    return 0;
}

void game_clear(struct game* g)
{
    int x, y;
    struct pumpkin* p;

    g->pumpkin_count = 0;
    g->spoiled_count = 0;
    g->pickup_count = 0;
    g->elapsed = 0;

    for (x = 0; x < g->max_x; x++)
    {
        for (y = 0; y < g->max_y; y++)
        {
            p = &g->field[x][y];
            if (p->stage > PUMPKIN_NONE)
                pumpkin_destroy(p);
        }
    }
    g->state = GAME_PLAY;
    LOGT("game_clear");
}

static void create_new_pumpkin(struct game* g, int x, int y)
{
    if (g->pumpkin_count > g->max_x * g->max_y - 2)
        return;

    pumpkin_create(&g->field[x][y], "b1", (float)x, (float)y);
    g->field[x][y].stage = PUMPKIN_CREATED;
    g->pumpkin_count++;
    LOGT("create_new_pumpkin. x: %d, y: %d, count: %d", x, y, g->pumpkin_count);
    sound_play(sound_new_pumpkin);
}

void game_touch(struct game* g, int touch_x, int touch_y)
{
    struct pumpkin* p = &g->field[touch_x][touch_y];
    
    LOGT("game_touch. x: %d, y: %d, count: %d", touch_x, touch_y, g->pumpkin_count);
    
    if (p->stage == PUMPKIN_NONE)
    {
        create_new_pumpkin(g, touch_x, touch_y);
    }
    else if (p->stage == PUMPKIN_RED && g->cart.state == CART_LOADING)
    {
        g->pumpkin_count--;
        g->pickup_count++;
        p->age = 0;
        p->stage = PUMPKIN_PICKING;
        g->cart.pumpkin_count++;
        LOGT("pickup pumpkin. picked count: %d. age: %.2ld", g->pickup_count, p->age);
    }
}

void game_elapse(struct game* g, long ms)
{
    int x, y;
      struct pumpkin* p;

    if (g->state == GAME_OVER)
        return;

    g->since_last_pumpkin += ms;
    g->elapsed += ms;
    
    worm_recalc(&g->worm, ms);
    cart_recalc(&g->cart, ms);
    leika_recalc(&g->leika, ms);
    spray_recalc(&g->spray, ms);

    for (x = 0; x < g->max_x; x++)
    {
        for (y = 0; y < g->max_y; y++)
        {
            p = &g->field[x][y];
            if (p->stage == PUMPKIN_NONE)
                continue;

            pumpkin_recalc(p, ms);
            
            if (p->stage != PUMPKIN_PICKED && pumpkin_is_eaten(p, g->worm.x, g->worm.y))
            {
                p->stage = PUMPKIN_PICKING;
            }
            if (p->stage == PUMPKIN_SPOILING)
            {
                g->spoiled_count++;
                p->stage = PUMPKIN_SPOILED;
            }
            else if (p->stage == PUMPKIN_PICKED)
            {
                pumpkin_destroy(p);
            }
        }
    }

    if (g->spoiled_count >= 10)
    {
        g->state = GAME_OVER;
        return;
    }
}
