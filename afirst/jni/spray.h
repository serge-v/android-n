#ifndef SPRAY_H
#define SPRAY_H

struct spray
{
    float               x;                      // x coordinate
    float               y;                      // y coordinate
    float               scale;                  // scale
    const char*         name;                   // name for debugging
    long                age;                    // age in ms
};

void spray_create(struct spray* p, const char* name, float x, float y);
void spray_draw(const struct spray* p);
void spray_recalc(struct spray* p, long ms);
void spray_destroy(struct spray* p);

#endif
