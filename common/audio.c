#include <jni.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "audio.h"

static SLresult sl_error = 0;

SLresult get_sl_error()
{
    SLresult ret = sl_error;
    sl_error = 0;
    return ret;
}

static const SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

#define RECORDER_FRAMES (16000 * 5) // 5 seconds of recorded audio at 16 kHz mono, 16-bit signed little endian
static short recorderBuffer[RECORDER_FRAMES];
static unsigned recorderSize = 0;
static SLmilliHertz recorderSR;

// pointer and size of the next player buffer to enqueue, and number of remaining buffers
static short *nextBuffer;
static unsigned nextSize;
static int nextCount;

void bq_player_callback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
    SLresult rc;
    struct player* p = (struct player*)context;
    bq;
    context;
    rc = (*p->play)->SetPlayState(p->play, SL_PLAYSTATE_STOPPED);
}


void bq_recorder_callback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
    SLresult rc;
    struct recorder* p = (struct recorder*)context;
    bq;
    context;

    rc = (*p->record)->SetRecordState(p->record, SL_RECORDSTATE_STOPPED);
}

void destroy_player(struct player* p)
{
    if (p->object)
        (*p->object)->Destroy(p->object);

    memset(p, 0, sizeof(struct player));
}

void destroy_recorder(struct recorder* p)
{
    if (p->object)
        (*p->object)->Destroy(p->object);

    memset(p, 0, sizeof(struct recorder));
}

void destroy_engine(struct aengine* ae)
{
    if (ae->mix)
        (*ae->mix)->Destroy(ae->mix);
    
    if (ae->object)
        (*ae->object)->Destroy(ae->object);
    
    memset(ae, 0, sizeof(struct aengine));
}

int create_engine(struct aengine* ae)
{
    SLInterfaceID ids[1] = { SL_IID_ENVIRONMENTALREVERB };
    SLboolean req[1] = { SL_BOOLEAN_FALSE };

    SLresult rc;
    
    memset(ae, 0, sizeof(struct aengine));

    rc = slCreateEngine(&ae->object, 0, NULL, 0, NULL, NULL);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*ae->object)->Realize(ae->object, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*ae->object)->GetInterface(ae->object, SL_IID_ENGINE, &ae->engine);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*ae->engine)->CreateOutputMix(ae->engine, &ae->mix, 1, ids, req);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*ae->mix)->Realize(ae->mix, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*ae->mix)->GetInterface(ae->mix, SL_IID_ENVIRONMENTALREVERB, &ae->reverb);
    if (SL_RESULT_SUCCESS == rc)
        (*ae->reverb)->SetEnvironmentalReverbProperties(ae->reverb, &reverbSettings);

    return 0;
    
error:

    destroy_engine(ae);
    sl_error = rc;

    return -1;
}

int create_queued_player(struct player* p, struct aengine* ae)
{
    SLresult rc;

    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
        SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
        2
    };
    
    SLDataFormat_PCM format_pcm = {
        SL_DATAFORMAT_PCM,
        1,
        SL_SAMPLINGRATE_16,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_SPEAKER_FRONT_CENTER,
        SL_BYTEORDER_LITTLEENDIAN
    };

    SLDataSource audioSrc = {
        &loc_bufq,
        &format_pcm
    };

    SLDataLocator_OutputMix loc_outmix = {
        SL_DATALOCATOR_OUTPUTMIX,
        ae->mix
    };
    
    SLDataSink audioSnk = {
        &loc_outmix,
        NULL
    };

    SLInterfaceID ids[3] = {
        SL_IID_BUFFERQUEUE,
        SL_IID_EFFECTSEND,
        SL_IID_VOLUME
    };

    SLboolean req[3] = {
        SL_BOOLEAN_TRUE,
        SL_BOOLEAN_TRUE,
        SL_BOOLEAN_TRUE
    };

    memset(p, 0, sizeof(struct player));

    rc = (*ae->engine)->CreateAudioPlayer(ae->engine, &p->object, &audioSrc, &audioSnk, 3, ids, req);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->Realize(p->object, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->GetInterface(p->object, SL_IID_PLAY, &p->play);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->GetInterface(p->object, SL_IID_BUFFERQUEUE, &p->queue);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->queue)->RegisterCallback(p->queue, bq_player_callback, p);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->GetInterface(p->object, SL_IID_EFFECTSEND, &p->effect_send);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->GetInterface(p->object, SL_IID_VOLUME, &p->volume);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->play)->SetPlayState(p->play, SL_PLAYSTATE_PLAYING);
    if (SL_RESULT_SUCCESS != rc)
        goto error;
        
    return 0;
    
error:
    
    destroy_player(p);
    sl_error = rc;
    
    return -1;
}

int create_file_player(struct player* p, const char* fname, struct aengine* ae)
{
    SLresult rc;
    int fd;
    struct stat st;
    
    memset(p, 0, sizeof(struct player));

    fd = stat(fname, &st);
    if (fd <= 0)
        return -1;

    fd = open(fname, O_RDONLY);
    if (fd <= 0)
        return -1;

    SLDataLocator_AndroidFD loc_fd = {
        SL_DATALOCATOR_ANDROIDFD,
        fd, 
        0, 
        st.st_size
    };
    
    SLDataFormat_MIME format_mime = {
        SL_DATAFORMAT_MIME,
        NULL,
        SL_CONTAINERTYPE_UNSPECIFIED
    };
    
    SLDataSource audioSrc = {
        &loc_fd,
        &format_mime
    };

    SLDataLocator_OutputMix loc_outmix = {
        SL_DATALOCATOR_OUTPUTMIX, 
        ae->mix
    };
    
    SLDataSink audioSnk = {
        &loc_outmix,
        NULL
    };

    const SLInterfaceID ids[3] = {
        SL_IID_SEEK, 
        SL_IID_MUTESOLO, 
        SL_IID_VOLUME
    };
    
    const SLboolean req[3] = {
        SL_BOOLEAN_TRUE, 
        SL_BOOLEAN_TRUE, 
        SL_BOOLEAN_TRUE
    };
    
    rc = (*ae->engine)->CreateAudioPlayer(ae->engine, &p->object, &audioSrc, &audioSnk, 3, ids, req);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->Realize(p->object, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->GetInterface(p->object, SL_IID_PLAY, &p->play);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->GetInterface(p->object, SL_IID_SEEK, &p->seek);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->GetInterface(p->object, SL_IID_MUTESOLO, &p->solo);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->GetInterface(p->object, SL_IID_VOLUME, &p->volume);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->seek)->SetLoop(p->seek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    return 0;

error:

    destroy_player(p);
    sl_error = rc;

    return -1;
}

int set_play_state(struct player* p, int isPlaying)
{
    SLresult rc;

    if (!p->play)
        return -1;

    rc = (*p->play)->SetPlayState(p->play, isPlaying ? SL_PLAYSTATE_PLAYING : SL_PLAYSTATE_PAUSED);
    if (SL_RESULT_SUCCESS == rc)
        return 0;

    sl_error = rc;
    return -1;
}

int create_recorder(struct recorder* p, struct aengine* ae)
{
    SLresult rc;

    SLDataLocator_IODevice loc_dev = {
        SL_DATALOCATOR_IODEVICE, 
        SL_IODEVICE_AUDIOINPUT,
        SL_DEFAULTDEVICEID_AUDIOINPUT, 
        NULL
    };
    
    SLDataSource audioSrc = {
        &loc_dev, 
        NULL
    };

    SLDataLocator_AndroidSimpleBufferQueue loc_bq = {
        SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
        2
    };
    
    SLDataFormat_PCM format_pcm = {
        SL_DATAFORMAT_PCM, 
        1, 
        SL_SAMPLINGRATE_16,
        SL_PCMSAMPLEFORMAT_FIXED_16, 
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_SPEAKER_FRONT_CENTER, 
        SL_BYTEORDER_LITTLEENDIAN
    };
    
    SLDataSink audioSnk = {
        &loc_bq, 
        &format_pcm
    };

    const SLInterfaceID id[1] = {
        SL_IID_ANDROIDSIMPLEBUFFERQUEUE
    };
    
    const SLboolean req[1] = {
        SL_BOOLEAN_TRUE
    };
    
    rc = (*ae->engine)->CreateAudioRecorder(ae->engine, &p->object, &audioSrc, &audioSnk, 1, id, req);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->Realize(p->object, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->GetInterface(p->object, SL_IID_RECORD, &p->record);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->object)->GetInterface(p->object, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &p->queue);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->queue)->RegisterCallback(p->queue, bq_recorder_callback, p);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    return 0;

error:

    destroy_recorder(p);
    sl_error = rc;

    return -1;
}

int start_recording(struct recorder* p)
{
    SLresult rc;

    rc = (*p->record)->SetRecordState(p->record, SL_RECORDSTATE_STOPPED);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->queue)->Clear(p->queue);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    recorderSize = RECORDER_FRAMES * sizeof(short);

    rc = (*p->queue)->Enqueue(p->queue, recorderBuffer, recorderSize);
    if (SL_RESULT_SUCCESS != rc)
        goto error;

    rc = (*p->record)->SetRecordState(p->record, SL_RECORDSTATE_RECORDING);
    if (SL_RESULT_SUCCESS != rc)
        goto error;
    
    return 0;

error:
    
    sl_error = rc;
    
    return -1;
}

int play_recording(struct player* p)
{
    SLresult rc;
    
    rc = (*p->queue)->Enqueue(p->queue, nextBuffer, nextSize);
    
    if (SL_RESULT_SUCCESS != rc)
    {
        sl_error = rc;
        return -1;
    }
    
    return 0;
}

int play_data(struct player* p, unsigned char* data, int size)
{
    SLresult rc;
    
    rc = (*p->queue)->Enqueue(p->queue, data, size);
    
    if (SL_RESULT_SUCCESS != rc)
    {
        sl_error = rc;
        return -1;
    }
    
    return 0;
}
