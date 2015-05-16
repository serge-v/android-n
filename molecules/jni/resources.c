#include "resources.h"

#include "gen/bg.png.h"
#include "gen/hidrogen.png.h"
#include "gen/hidrogen_mask.png.h"

struct resources R;

void resources_load()
{
	pic_load_png_data(&R.bg_pic, bg_png, bg_png_len);
	pic_load_png_data(&R.hidrogen_pic, hidrogen_png, hidrogen_png_len);
	pic_load_png_data(&R.hidrogen_mask_pic, hidrogen_mask_png, hidrogen_mask_png_len);
}

void resources_destroy()
{
	pic_destroy(&R.bg_pic);
	pic_destroy(&R.hidrogen_pic);
	pic_destroy(&R.hidrogen_mask_pic);
}
