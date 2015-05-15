#include "sound.h"
#include "log.h"
#include "audio.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <errno.h>
#include <stdlib.h>

#include "gen/bg.wav.h"

static struct aengine aeng;
static struct player bg_player;
static struct player eff_player;

static void create_players()
{
}

void sound_create()
{
	create_players();
}

void sound_destroy()
{
}

void sound_play(enum game_sound sound) {
}
