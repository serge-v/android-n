#include "pic.h"

extern struct pic courier_pics[];

void font_create();
void font_destroy();
void draw_str(const char* s, GLfloat x, GLfloat y);
void draw_char2(int c);
