#include "pic.h"

struct sound_res {
	unsigned char* data;
	int size;
};

struct resources {
	struct pic pumpkin_mask_pic;
	struct pic pumpkin_green_pic;
	struct pic pumpkin_yellow_pic;
	struct pic pumpkin_red_pic;
	struct pic pumpkin_spoiled_pic;
	struct pic bg_pic;
	struct pic cart_pic;
	struct pic cart_mask_pic;
	struct pic worm1_pic;
	struct pic worm1_mask_pic;
	struct pic worm2_pic;
	struct pic worm2_mask_pic;
	struct pic worm3_pic;
	struct pic worm3_mask_pic;
	struct pic leika_pic;
	struct pic leika_mask_pic;
	struct pic spray_pic;
	struct pic spray_mask_pic;
};

extern struct resources R;

void resources_load();
void resources_destroy();
