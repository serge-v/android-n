#include "pic.h"
#include "../common/compat.h"
#include "../common/log.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <png.h>

void pic_create(struct pic* p, const char* name, const unsigned char* d, int w, int h)
{
	float a;
	memset(p, 0, sizeof(struct pic));
	p->name = strdup(name);
	glGenTextures(1, &p->tex);
	LOGGLE("glGenTextures");
	glBindTexture(GL_TEXTURE_2D, p->tex);
	LOGGLE("glBindTexture");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	LOGGLE("glTexParameteri 1");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	LOGGLE("glTexParameteri 2");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, d);
	LOGGLE("texture loading: %s", name);
	LOGI("texture loaded: %s, %dx%d", name, w, h);

	a = (float)w / (float)h;

	p->vertices[0].x = -0.5f;
	p->vertices[0].y = 0.5f / a;

	p->vertices[1].x = -0.5f;
	p->vertices[1].y = -0.5f / a;

	p->vertices[2].x = 0.5f;
	p->vertices[2].y = 0.5f / a;

	p->vertices[3].x = 0.5f;
	p->vertices[3].y = -0.5f / a;

	p->scale = 1;
	p->width = w;
	p->height = h;
}

struct read_params {
	FILE* f;
	const unsigned char* data;
	size_t len;
	size_t pos;
};

void read_file_fn(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_voidp a = png_get_io_ptr(png_ptr);
	struct read_params* p = (struct read_params*)a;
	size_t was_read = fread(data, 1, length, p->f);
	if (was_read == 0) {
		if (ferror(p->f) != 0)
			png_error(png_ptr, "cannot read file");
	}
}

void read_data_fn(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_voidp a = png_get_io_ptr(png_ptr);
	struct read_params* p = (struct read_params*)a;
	size_t to_read = (p->pos + length > p->len) ? p->len - p->pos : length;
	memcpy(data, p->data + p->pos, to_read);
	p->pos += length;
}

int pic_load_png(struct pic* p, const char* fname)
{
	struct read_params params;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;

	int bit_depth, color_type, rowbytes, channels = 1, rc = -1;
	png_uint_32 twidth, theight, i;
	png_byte* image_data = NULL;
	png_bytep* row_pointers = NULL;

	memset(&params, 0, sizeof(struct read_params));
	params.f = fopen(fname, "rb");
	if (params.f == NULL)
		goto out;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		goto out;

	png_set_read_fn(png_ptr, &params, read_file_fn);

	info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr)
		goto out;

	png_read_info(png_ptr, info_ptr);

	if (png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL) != 1)
		goto out;

	if (color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
		channels = 3;
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {

		png_set_tRNS_to_alpha(png_ptr);
		channels += 1;
	}

	png_read_update_info(png_ptr, info_ptr);
	rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	image_data = (png_byte*)malloc(sizeof(png_byte) * rowbytes * theight);
	if (image_data == NULL)
		goto out;

	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * theight);
	if (row_pointers == NULL)
		goto out;

	for (i = 0; i < theight; ++i)
		row_pointers[theight - 1 - i] = image_data + i * rowbytes;

	png_read_image(png_ptr, row_pointers);
	pic_create(p, fname, image_data, twidth, theight);
	rc = 0;

out:

	if (png_ptr != NULL && info_ptr != NULL)
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	else if (png_ptr != NULL)
		png_destroy_read_struct(&png_ptr, 0, 0);

	if (image_data != NULL)
		free(image_data);

	if (row_pointers != NULL)
		free(row_pointers);

	if (params.f != NULL)
		fclose(params.f);

	return rc;
}

int pic_load_png_data(struct pic* p, const unsigned char* data, int len)
{
	struct read_params params;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;

	int bit_depth, color_type, rowbytes, channels = 1, rc = -1;
	png_uint_32 twidth, theight, i;
	png_byte* image_data = NULL;
	png_bytep* row_pointers = NULL;

	memset(&params, 0, sizeof(struct read_params));
	params.data = data;
	params.len = len;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		goto out;

	png_set_read_fn(png_ptr, &params, read_data_fn);

	info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr)
		goto out;

	png_read_info(png_ptr, info_ptr);

	if (png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL) != 1)
		goto out;

	if (color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
		png_set_expand(png_ptr);
		png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
		channels = 3;
	}

	if (color_type == PNG_COLOR_TYPE_GRAY) {
		png_set_gray_to_rgb(png_ptr);
		png_set_expand(png_ptr);
		png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
		channels = 3;
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
		channels += 1;
	}

	png_read_update_info(png_ptr, info_ptr);
	rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	image_data = (png_byte*)malloc(sizeof(png_byte) * rowbytes * theight);
	if (image_data == NULL)
		goto out;

	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * theight);
	if (row_pointers == NULL)
		goto out;

	for (i = 0; i < theight; ++i)
		row_pointers[theight - 1 - i] = image_data + i * rowbytes;

	png_read_image(png_ptr, row_pointers);
	pic_create(p, "fromdata", image_data, twidth, theight);
	rc = 0;

out:

	if (png_ptr != NULL && info_ptr != NULL)
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	else if (png_ptr != NULL)
		png_destroy_read_struct(&png_ptr, 0, 0);

	if (image_data != NULL)
		free(image_data);

	if (row_pointers != NULL)
		free(row_pointers);

	if (params.f != NULL)
		fclose(params.f);

	return rc;
}

void pic_destroy(struct pic* p)
{
	if (p->tex)
		glDeleteTextures(1, &p->tex);

	if (p->name)
		free((char*)p->name);

	memset(p, 0, sizeof(struct pic));
}

static const struct point pic_tex[] = {
	{ 0, 1 }, { 0, 0 },
	{ 1, 1 }, { 1, 0 }
};

void pic_draw3(const struct pic* p)
{
	glEnable(GL_TEXTURE_2D);

//	glEnable(GL_BLEND);
//    glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	glBindTexture(GL_TEXTURE_2D, p->tex);
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	LOGGLE(">>bound");

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, sizeof(struct point), p->vertices);
	LOGGLE(">>glVertexPointer");

	glTexCoordPointer(2, GL_FLOAT, sizeof(struct point), pic_tex);
	LOGGLE("glTexCoordPointer");

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	LOGGLE("draw_square draw arrays");

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);

	if (p->selected) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(struct point), p->vertices);
		glDrawArrays(GL_LINES, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void pic_draw(const struct pic* p)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, p->tex);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, sizeof(struct point), p->vertices);
	LOGGLE(">>glVertexPointer");

	glTexCoordPointer(2, GL_FLOAT, sizeof(struct point), pic_tex);
	LOGGLE("glTexCoordPointer");

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	LOGGLE("draw_square draw arrays");

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void pic_draw2(const struct pic* p, const struct pic* mask)
{
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glScalef(p->scale, p->scale, 0);

	// draw mask
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
	glBindTexture(GL_TEXTURE_2D, mask->tex);
	glVertexPointer(2, GL_FLOAT, sizeof(struct point), mask->vertices);
	LOGGLE(">>glVertexPointer");
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct point), pic_tex);
	LOGGLE("glTexCoordPointer");

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindTexture(GL_TEXTURE_2D, p->tex);

	glVertexPointer(2, GL_FLOAT, sizeof(struct point), p->vertices);
	LOGGLE(">>glVertexPointer");

	glTexCoordPointer(2, GL_FLOAT, sizeof(struct point), pic_tex);
	LOGGLE("glTexCoordPointer");

	glBlendFunc(GL_ONE, GL_ONE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glPopMatrix();
}
