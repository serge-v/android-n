enum game_sound {
	sound_test,
	sound_bg,
	sound_new_game,
	sound_new_pumpkin,
	sound_end_game,
};

void sound_create();
void sound_destroy();
void sound_play(enum game_sound sound);
