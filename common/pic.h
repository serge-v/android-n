#ifndef PIC_H_
#define PIC_H_

#include <jni.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

struct point
{
    GLfloat x, y;
};

struct pic
{
    const char*     name;
    GLuint          tex;
    GLfloat         scale;
    struct point    vertices[4];
    struct point    pos;
    int             mouseover;
    int             selected;
    int             width;
    int             height;
};

int pic_load_png(struct pic* p, const char* fname);
int pic_load_png_data(struct pic* p, const unsigned char* data, int len);
void pic_create(struct pic* p, const char* name, const unsigned char* d, int w, int h);
void pic_destroy(struct pic* p);
void pic_draw(const struct pic* p);
void pic_draw2(const struct pic* p, const struct pic* mask);

#endif // PIC_H_
