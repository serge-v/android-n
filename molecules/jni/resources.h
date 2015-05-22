#include "pic.h"

struct sound_res {
	unsigned char* data;
	int size;
};

struct resources {
	struct pic bg_pic;
	struct pic hidrogen_pic;
	struct pic hidrogen_mask_pic;
	struct pic carbon_pic;
	struct pic bond1_pic;
	struct pic bond2_pic;
	struct pic oxygen_pic;
	struct pic oxygen_mask_pic;
};

extern struct resources R;

void resources_load();
void resources_destroy();
