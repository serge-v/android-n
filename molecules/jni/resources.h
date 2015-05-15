#include "pic.h"

struct sound_res {
	unsigned char* data;
	int size;
};

struct resources {
	struct pic bg_pic;
	struct pic spray_pic;
	struct pic spray_mask_pic;
};

extern struct resources R;

void resources_load();
void resources_destroy();
