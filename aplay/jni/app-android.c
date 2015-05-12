//#include <android/sensor.h>
//#include <android/log.h>
#include <android_native_app_glue.h>
#include <stdlib.h>
#include <errno.h>

#include "../common/app.h"
#include "../common/log.h"
#include "../common/audio.h"

int g_log_level = 0;
const char* g_appname = "aplay";

struct saved_state {
	float angle;
	int32_t x;
	int32_t y;
};

struct engine {
	struct android_app* app;
	int32_t width;
	int32_t height;
	struct saved_state state;
};

static struct aengine aeng;
static struct player bplayer;
static struct player fplayer;
static struct recorder arecorder;

static void create_aengine()
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
}

void play_file()
{
	int rc;

	if (!fplayer.object) {
		rc = create_file_player(&fplayer, "/sdcard/aplay.mp3", &aeng);

		if (rc == -1)
			LOGE("create_file_player: rc: %d, errno: %d, slerror: %d", rc, errno, get_sl_error());
		else
			LOGI("file player created");
	}

	rc = set_play_state(&fplayer, 1);
	if (rc == -1)
		LOGE("set_play_state: rc: %d, errno: %d, slerror: %d", rc, errno, get_sl_error());
	else
		LOGI("set_play_state(1)");
}

static void record_buffer()
{
	int rc;

	if (!arecorder.object) {
		rc = create_recorder(&arecorder, &aeng);

		if (rc == -1)
			LOGE("create_recorder: rc: %d, errno: %d, slerror: %d", rc, errno, get_sl_error());
		else
			LOGI("recorder created");
	}

	rc = start_recording(&arecorder);

	if (rc == -1)
		LOGE("start_recording: rc: %d, errno: %d, slerror: %d", rc, errno, get_sl_error());
	else
		LOGI("start_recording ok");
}

static void play_buffer()
{
	int rc;

	if (!bplayer.object) {
		rc = create_queued_player(&bplayer, &aeng);

		if (rc == -1)
			LOGE("create_queued_player: rc: %d, errno: %d, slerror: %d", rc, errno, get_sl_error());
		else
			LOGI("qplayer created");
	}

	rc =  play_recording(&bplayer);

	if (rc == -1)
		LOGE("play_recording: rc: %d, errno: %d, slerror: %d", rc, errno, get_sl_error());
	else
		LOGI("play_recording ok");

	rc = set_play_state(&bplayer, 1);

	if (rc == -1)
		LOGE("set_play_state: rc: %d, errno: %d, slerror: %d", rc, errno, get_sl_error());
	else
		LOGI("set_play_state(1)");
}

static void destroy_all()
{
	destroy_recorder(&arecorder);
	destroy_player(&fplayer);
	destroy_player(&bplayer);
	destroy_engine(&aeng);
}

void process_key_event(AInputEvent* e)
{
	int rc;
	int32_t key_code, action;
	int64_t event_time;

	key_code = AKeyEvent_getKeyCode(e);
	event_time = AKeyEvent_getEventTime(e);
	action = AKeyEvent_getAction(e);

	LOGT("key: time: %ld, key_code: %d, action: %d", event_time, key_code, action);

	if (action != AKEY_EVENT_ACTION_DOWN)
		return;

	switch (key_code) {
	case AKEYCODE_E:
		create_aengine();
		break;

	case AKEYCODE_D:
		destroy_all();
		break;

	case AKEYCODE_F:
		play_file();
		break;

	case AKEYCODE_R:
		record_buffer();
		break;

	case AKEYCODE_B:
		play_buffer();
		break;
	}
}

static int32_t engine_handle_input(struct android_app* app, AInputEvent* e)
{
	struct engine* engine;
	int32_t type, devid, src;
	float x, y;

	engine = (struct engine*)app->userData;

	type = AInputEvent_getType(e);
	devid = AInputEvent_getDeviceId(e);
	src = AInputEvent_getSource(e);

	if (type == AINPUT_EVENT_TYPE_KEY)
		process_key_event(e);

	return 0;
}

static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
	struct engine* engine = (struct engine*)app->userData;
	LOGT("cmd: %d", cmd);

	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		engine->app->savedState = malloc(sizeof(struct saved_state));
		*((struct saved_state*)engine->app->savedState) = engine->state;
		engine->app->savedStateSize = sizeof(struct saved_state);
		break;

	case APP_CMD_INIT_WINDOW:
		if (engine->app->window != NULL) {
		}
		break;

	case APP_CMD_TERM_WINDOW:
		break;

	case APP_CMD_GAINED_FOCUS:
		break;

	case APP_CMD_LOST_FOCUS:
		break;
	}
}

void android_main(struct android_app* state)
{
	struct engine engine;

	app_dummy();
	LOGT("android_main started");

	memset(&aeng, 0, sizeof(struct aengine));
	memset(&fplayer, 0, sizeof(struct player));
	memset(&bplayer, 0, sizeof(struct player));
	memset(&arecorder, 0, sizeof(struct recorder));

	memset(&engine, 0, sizeof(engine));
	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	engine.app = state;

	if (state->savedState != NULL)
		engine.state = *(struct saved_state*)state->savedState;

	while (1) {
		int ident;
		int events;
		struct android_poll_source* source;

		while ((ident = ALooper_pollAll(-1, NULL, &events, (void**)&source)) >= 0) {
			if (source != NULL)
				source->process(state, source);

			if (state->destroyRequested != 0)
				return;
		}
	}
}
