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
	SLresult rc;

	if (aeng.object) {
		LOGE("aengine already created");
		return;
	}

	rc = create_engine(&aeng);

	if (rc == -1)
		LOGE("create_engine: rc: %d, errno: %d, slerror: %d", rc, errno, get_sl_error());
	else
		LOGI("aengine created");

	rc = create_queued_player(&bg_player, &aeng);

	if (rc == -1)
		LOGE("create bg_player: rc: %d, errno: %d, slerror: %d", rc, errno, get_sl_error());
	else
		LOGI("bg_player created");

	rc = create_queued_player(&eff_player, &aeng);

	if (rc == -1)
		LOGE("create eff_player: rc: %d, errno: %d, slerror: %d", rc, errno, get_sl_error());
	else
		LOGI("eff_player created");
}

void sound_create()
{
	create_players();
}

void sound_destroy()
{
}

void sound_play(enum game_sound sound) {
	struct player* p;
	int rc;

	p = (sound == sound_bg) ? &bg_player : &eff_player;

	rc = play_data(p, bg_s16, bg_s16_len);
	if (rc)
		LOGE("play_data error: %d", rc);
}
