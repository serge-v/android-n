#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

void app_init(int width, int height);
void app_deinit();
void app_render(long tick, int width, int height);
void app_touch(int release, int x, int y);
void app_key(int key);
void inc_blend_mode();
void create_controls();
void check_glerrors();

extern int g_app_alive;

#endif // !APP_H_INCLUDED
