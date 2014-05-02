#include "resources.h"

#include "gen/bg.png.h"
#include "gen/pumpkin_green.png.h"
#include "gen/pumpkin_yellow.png.h"
#include "gen/pumpkin_red.png.h"
#include "gen/pumpkin_spoiled.png.h"
#include "gen/pumpkin_mask.png.h"
#include "gen/cart.png.h"
#include "gen/cart_mask.png.h"
#include "gen/worm1.png.h"
#include "gen/worm1_mask.png.h"
#include "gen/worm2.png.h"
#include "gen/worm2_mask.png.h"
#include "gen/worm3.png.h"
#include "gen/worm3_mask.png.h"
#include "gen/leika.png.h"
#include "gen/leika_mask.png.h"
#include "gen/spray.png.h"
#include "gen/spray_mask.png.h"

struct resources R;

void resources_load()
{
    pic_load_png_data(&R.bg_pic, bg_png, bg_png_len);
    pic_load_png_data(&R.pumpkin_green_pic, pumpkin_green_png, pumpkin_green_png_len);
    pic_load_png_data(&R.pumpkin_yellow_pic, pumpkin_yellow_png, pumpkin_yellow_png_len);
    pic_load_png_data(&R.pumpkin_red_pic, pumpkin_red_png, pumpkin_red_png_len);
    pic_load_png_data(&R.pumpkin_mask_pic, pumpkin_mask_png, pumpkin_mask_png_len);
    pic_load_png_data(&R.pumpkin_spoiled_pic, pumpkin_spoiled_png, pumpkin_spoiled_png_len);
    pic_load_png_data(&R.cart_pic, cart_png, cart_png_len);
    pic_load_png_data(&R.cart_mask_pic, cart_mask_png, cart_mask_png_len);
    pic_load_png_data(&R.worm1_pic, worm1_png, worm1_png_len);
    pic_load_png_data(&R.worm1_mask_pic, worm1_mask_png, worm1_mask_png_len);
    pic_load_png_data(&R.worm2_pic, worm2_png, worm2_png_len);
    pic_load_png_data(&R.worm2_mask_pic, worm2_mask_png, worm2_mask_png_len);
    pic_load_png_data(&R.worm3_pic, worm3_png, worm3_png_len);
    pic_load_png_data(&R.worm3_mask_pic, worm3_mask_png, worm3_mask_png_len);
    pic_load_png_data(&R.leika_pic, leika_png, leika_png_len);
    pic_load_png_data(&R.leika_mask_pic, leika_mask_png, leika_mask_png_len);
    pic_load_png_data(&R.spray_pic, spray_png, spray_png_len);
    pic_load_png_data(&R.spray_mask_pic, spray_mask_png, spray_mask_png_len);
}

void resources_destroy()
{
    pic_destroy(&R.bg_pic);
    pic_destroy(&R.pumpkin_green_pic);
    pic_destroy(&R.pumpkin_yellow_pic);
    pic_destroy(&R.pumpkin_red_pic);
    pic_destroy(&R.pumpkin_spoiled_pic);
    pic_destroy(&R.pumpkin_mask_pic);
    pic_destroy(&R.cart_pic);
    pic_destroy(&R.cart_mask_pic);
    pic_destroy(&R.worm1_pic);
    pic_destroy(&R.worm1_mask_pic);
    pic_destroy(&R.worm2_pic);
    pic_destroy(&R.worm2_mask_pic);
    pic_destroy(&R.worm3_pic);
    pic_destroy(&R.worm3_mask_pic);
    pic_destroy(&R.leika_pic);
    pic_destroy(&R.leika_mask_pic);
    pic_destroy(&R.spray_pic);
    pic_destroy(&R.spray_mask_pic);
}
