#include "pic.h"
#include <stdio.h>
#include <stdlib.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "log.h"

#include "../font/char_0030.xbm"
#include "../font/char_0031.xbm"
#include "../font/char_0032.xbm"
#include "../font/char_0033.xbm"
#include "../font/char_0034.xbm"
#include "../font/char_0035.xbm"
#include "../font/char_0036.xbm"
#include "../font/char_0037.xbm"
#include "../font/char_0038.xbm"
#include "../font/char_0039.xbm"

#include "../font/char_0041.xbm"
#include "../font/char_0042.xbm"
#include "../font/char_0043.xbm"
#include "../font/char_0044.xbm"
#include "../font/char_0045.xbm"
#include "../font/char_0046.xbm"
#include "../font/char_0047.xbm"
#include "../font/char_0048.xbm"
#include "../font/char_0049.xbm"
#include "../font/char_004a.xbm"
#include "../font/char_004b.xbm"
#include "../font/char_004c.xbm"
#include "../font/char_004d.xbm"
#include "../font/char_004e.xbm"
#include "../font/char_004f.xbm"
#include "../font/char_0050.xbm"
#include "../font/char_0051.xbm"
#include "../font/char_0052.xbm"
#include "../font/char_0053.xbm"
#include "../font/char_0054.xbm"
#include "../font/char_0055.xbm"
#include "../font/char_0056.xbm"
#include "../font/char_0057.xbm"
#include "../font/char_0058.xbm"
#include "../font/char_0059.xbm"
#include "../font/char_005a.xbm"

#include "gen/font.png.h"

const char* courier_xbms[] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	char_0030_bits,
	char_0031_bits,
	char_0032_bits,
	char_0033_bits,
	char_0034_bits,
	char_0035_bits,
	char_0036_bits,
	char_0037_bits,
	char_0038_bits,
	char_0039_bits,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	char_0041_bits,
	char_0042_bits,
	char_0043_bits,
	char_0044_bits,
	char_0045_bits,
	char_0046_bits,
	char_0047_bits,
	char_0048_bits,
	char_0049_bits,
	char_004a_bits,
	char_004b_bits,
	char_004c_bits,
	char_004d_bits,
	char_004e_bits,
	char_004f_bits,
	char_0050_bits,
	char_0051_bits,
	char_0052_bits,
	char_0053_bits,
	char_0054_bits,
	char_0055_bits,
	char_0056_bits,
	char_0057_bits,
	char_0058_bits,
	char_0059_bits,
	char_005a_bits
};

static void copy_bits(const char *src, int src_size, unsigned char *map)
{
	int i, j, bit, cnt;
	unsigned char c, mask;
	cnt = 0;
	for (i = 0; i < src_size; i++) {
		c = src[i];
		mask = 1;

		for (j = 0; j < 8; j++) {
			bit = (mask & c) > 0;

			if (bit) {
				map[cnt] = 0xFF;
				map[cnt + 1] = 0x80;
				map[cnt + 2] = 0x40;
				map[cnt + 3] = 0xFF;
			} else {
				map[cnt] = map[cnt + 1] = map[cnt + 2] = 0xFF;
				map[cnt + 3] = 0x00;
			}

			mask <<= 1;
			cnt += 4;
		}
	}
}

static unsigned char buff[char_0041_width * char_0041_height * 4];
const int buff_size = char_0041_width * char_0041_height * 4;

struct pic courier_pics[255];

// version 2

const char* char_order[] = {
	"abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	"1234567890.:,; '\" (!?) +-*/=",
};

struct pic font_pic;
FT_Library library;

void font_create()
{
	int i, size;
	char name[10];

	memset(courier_pics, 0, sizeof(courier_pics));

	FT_Init_FreeType(&library);

	size = sizeof(courier_xbms) / sizeof(courier_xbms[0]);

	for (i = 0; i < size; i++) {
		struct pic* p = &courier_pics[i];
		if (!courier_xbms[i])
			continue;

		sprintf(name, "%c", i);
		copy_bits(courier_xbms[i], sizeof(char_0041_bits), buff);
		pic_create(p, name, buff, char_0041_width, char_0041_height);
		p->scale = 0.1f;
	}

	// version 2
	pic_load_png_data(&font_pic, font_png, font_png_len);

	font_pic.vertices[0].x = 0;
	font_pic.vertices[0].y = 0;

	font_pic.vertices[1].x = 0;
	font_pic.vertices[1].y = (float)font_pic.height;

	font_pic.vertices[2].x = (float)font_pic.width;
	font_pic.vertices[2].y = 0;

	font_pic.vertices[3].x = (float)font_pic.width;
	font_pic.vertices[3].y = (float)font_pic.height;

	font_pic.scale = 1;
}

void font_destroy()
{
	int i, size;

	size = sizeof(courier_xbms) / sizeof(courier_xbms[0]);

	for (i = 0; i < size; i++)
		pic_destroy(&courier_pics[i]);

	memset(courier_pics, 0, sizeof(courier_pics));
}

static const struct point chars_tex[] = {
	{ 0, 0 }, { 0, 1 },
	{ 1, 0 }, { 1, 1 },
};

static void draw_char(const struct pic* p)
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, p->tex);
	LOGGLE(">>bound");

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, sizeof(struct point), p->vertices);
	LOGGLE(">>glVertexPointer");

	glTexCoordPointer(2, GL_FLOAT, sizeof(struct point), chars_tex);
	LOGGLE("glTexCoordPointer");

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	LOGGLE("draw_square draw arrays");

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);
}

void draw_str(const char* s, GLfloat x, GLfloat y)
{
	glTranslatef(x, y, 0);
	while (*s) {
		struct pic* p = &courier_pics[(size_t) * s];
		if (p->name)
			draw_char(p);
		glTranslatef(1, 0, 0);
		s++;
	}
}

int find_char(int c, int* row, int* col)
{
	int i, size;
	const char* ptr;

	*row = 0;
	*col = 0;

	size = sizeof(char_order) / sizeof(char_order[0]);
	for (i = 0; i < size; i++) {
		ptr = char_order[i];
		while (*ptr) {
			if (*ptr == c)
				return 1;
			ptr++;
			(*col)++;
		}
		(*row)++;
	}

	return 0;
}

void draw_char2(int c)
{
	int row, col;
	struct point tex_points[4];
	struct point ver_points[4];
	float x0, x1, y0, y1;

	struct pic* p = &font_pic;

	if (!find_char(c, &row, &col))
		find_char('?', &row, &col);

	x0 = (float)col / 53.2f + 0.001f;
	x1 = (float)(col + 1) / 53.0f + 0.001f;
	y0 = (float)(1 - row) / 2.0f;
	y1 = (float)(2 - row) / 2.0f;

	if (row == 1) {
		y0 += 0.04f;
		y1 += 0.04f;
	}

	tex_points[0].x = x0;
	tex_points[0].y = y0;

	tex_points[1].x = x0;
	tex_points[1].y = y1;

	tex_points[2].x = x1;
	tex_points[2].y = y0;

	tex_points[3].x = x1;
	tex_points[3].y = y1;

	ver_points[0].x = 0;
	ver_points[0].y = 0;

	ver_points[1].x = 0;
	ver_points[1].y = 24;

	ver_points[2].x = 12;
	ver_points[2].y = 0;

	ver_points[3].x = 12;
	ver_points[3].y = 24;

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, p->tex);
	LOGGLE(">>bound");

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, sizeof(struct point), ver_points);
	LOGGLE(">>glVertexPointer");

	glTexCoordPointer(2, GL_FLOAT, sizeof(struct point), tex_points);
	LOGGLE("glTexCoordPointer");

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	LOGGLE("draw_square draw arrays");

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);
}
