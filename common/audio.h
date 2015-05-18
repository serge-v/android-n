#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

struct aengine {
	SLObjectItf                 object;
	SLEngineItf                 engine;
	SLObjectItf                 mix;
	SLEnvironmentalReverbItf    reverb;
};

struct player {
	SLObjectItf                     object;
	SLPlayItf                       play;
	SLEffectSendItf                 effect_send;
	SLMuteSoloItf                   solo;
	SLVolumeItf                     volume;
	SLAndroidSimpleBufferQueueItf   queue;  // for buffer based player
	SLSeekItf                       seek;   // for file descriptor player
};

struct recorder {
	SLObjectItf                     object;
	SLRecordItf                     record;
	SLAndroidSimpleBufferQueueItf   queue;
};

SLresult get_sl_error();

int create_engine(struct aengine* ae);
int create_queued_player(struct player* p, struct aengine* ae);
//int create_file_player(struct player* p, const char* fname, struct aengine* ae);

//void destroy_engine(struct aengine* ae);
//void destroy_player(struct player* p);
//void destroy_recorder(struct recorder* p);

//int start_recording(struct recorder* p);
//int play_recording(struct player* p);
int play_data(struct player* p, unsigned char* data, int size);
