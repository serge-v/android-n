#ifndef PUMPKIN_H
#define PUMPKIN_H

#include "pic.h"

enum life_stages
{
    PUMPKIN_NONE,
    PUMPKIN_CREATED,
    PUMPKIN_GREEN,
    PUMPKIN_YELLOW,
    PUMPKIN_RED,
    PUMPKIN_PICKING,
    PUMPKIN_PICKED,
    PUMPKIN_SPOILING,
    PUMPKIN_SPOILED
};

struct particles
{
    int                 count;                  // particles count
    const struct point* tex_vertices;           // particles texture offsets
    const struct point* pos_vertices;           // single particle vertices
};

struct pumpkin
{
    enum life_stages    stage;                  // live stages
    float               x;                      // x coordinate
    float               y;                      // y coordinate
    float               scale;                  // scale depending on age
    float               rot_angle;              // current rotation angle [0..359]
    float               rot_speed;              // rotation speed deg/s
    const char*         name;                   // name for debugging
    struct particles*   particles;              // particles for crashing stage
    long                age;                    // age in ms
};

void pumpkin_create(struct pumpkin* p, const char* name, float x, float y);
void pumpkin_draw(const struct pumpkin* p);
void pumpkin_recalc(struct pumpkin* p, long ms);
void pumpkin_destroy(struct pumpkin* p);
int pumpkin_is_eaten(struct pumpkin* p, float worm_x, float worm_y);

#endif
