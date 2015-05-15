#include "resources.h"

#include "gen/bg.png.h"
#include "gen/spray.png.h"
#include "gen/spray_mask.png.h"

struct resources R;

void resources_load()
{
	pic_load_png_data(&R.bg_pic, bg_png, bg_png_len);
	pic_load_png_data(&R.spray_pic, spray_png, spray_png_len);
	pic_load_png_data(&R.spray_mask_pic, spray_mask_png, spray_mask_png_len);
}

void resources_destroy()
{
	pic_destroy(&R.bg_pic);
	pic_destroy(&R.spray_pic);
	pic_destroy(&R.spray_mask_pic);
}
