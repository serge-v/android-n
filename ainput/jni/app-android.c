#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "../common/app.h"
#include "../common/log.h"

int g_log_level = 0;
const char* g_appname = "ainput";

struct saved_state
{
    float angle;
    int32_t x;
    int32_t y;
};

struct engine
{
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int32_t width;
    int32_t height;
    struct saved_state state;
};

static void dump_motion_event(const AInputEvent* e)
{
    float x, y, xoffs, yoffs, xprec, yprec, rawx, rawy, press, msize, touch_major, touch_minor, tool_major, tool_minor, orient;
    float hrawx, hrawy, histx, histy, hpress, hmsize, htouch_major, htouch_minor, htool_major, htool_minor, horient;
    int32_t action, flags, meta_state, edge_flags, ptrid;
    int64_t down_time, event_time, hist_event_time;
    size_t pointer_count, hist_size;
    size_t i, j;

    action = AMotionEvent_getAction(e);
    flags = AMotionEvent_getFlags(e);
    meta_state = AMotionEvent_getMetaState(e);
    edge_flags = AMotionEvent_getEdgeFlags(e);
    down_time = AMotionEvent_getDownTime(e);
    event_time = AMotionEvent_getEventTime(e);
    xoffs = AMotionEvent_getXOffset(e);
    yoffs = AMotionEvent_getYOffset(e);
    xprec = AMotionEvent_getXPrecision(e);
    yprec = AMotionEvent_getYPrecision(e);
    pointer_count = AMotionEvent_getPointerCount(e);
    LOGT("  motion: ");
    LOGT("    time: %ld, action: %d, flags: %x, mstate: %d, eflags: %x",
        event_time, action, flags, meta_state, edge_flags);
    LOGT("    dtime: %ld, offs: %.2f,%2f, prec: %.2f,%.2f, points: %ld",
        down_time, xoffs, yoffs, xprec, yprec, pointer_count);
   
    for (i = 0; i < pointer_count; i++)
    {
        ptrid = AMotionEvent_getPointerId(e, i);
        x = AMotionEvent_getX(e, i);
        y = AMotionEvent_getY(e, i);
        rawx = AMotionEvent_getRawX(e, i);
        rawy = AMotionEvent_getRawY(e, i);
        press = AMotionEvent_getPressure(e, i);
        msize = AMotionEvent_getSize(e, i);
        touch_major = AMotionEvent_getTouchMajor(e, i);
        touch_minor = AMotionEvent_getTouchMinor(e, i);
        tool_major = AMotionEvent_getToolMajor(e, i);
        tool_minor = AMotionEvent_getToolMinor(e, i);
        orient = AMotionEvent_getOrientation(e, i);
        hist_size = AMotionEvent_getHistorySize(e);

        LOGT("    pointer[%d]", i);
        LOGT("      id: %d, xy: %.2f,%.2f, rawxy: %.2f, %.2f, press: %.2f, msize: %.2f",
            ptrid, x, y, rawx, rawy, press, msize);
        LOGT("      touch: %.2f,%.2f, tool: %.2f,%.2f, orient: %.2f, hist_size: %d",
            touch_major, touch_minor, tool_major, tool_minor, orient, hist_size);
        
        for (j = 0; j < hist_size; j++)
        {
            hist_event_time = AMotionEvent_getHistoricalEventTime(e, j);
//            hrawx = AMotionEvent_getHistoricalRawX(e, i, j);
//            hrawy = AMotionEvent_getHistoricalRawY(e, i, j);
            histx = AMotionEvent_getHistoricalX(e, i, j);
            histy = AMotionEvent_getHistoricalY(e, i, j);
            hpress = AMotionEvent_getHistoricalPressure(e, i, j);
            hmsize = AMotionEvent_getHistoricalSize(e, i, j);
//            htouch_major = AMotionEvent_getHistoricalTouchMajor(e, i, j);
//            htouch_minor = AMotionEvent_getHistoricalTouchMinor(e, i, j);
//            htool_major = AMotionEvent_getHistoricalToolMajor(e, i, j);
//            htool_minor = AMotionEvent_getHistoricalToolMinor(e, i, j);
//            horient = AMotionEvent_getHistoricalOrientation(e, i, j);
            LOGT("      hist[%d]: time: %ld, hist: %.2f,%.2f, press: %.2f, msize: %.2f",
                j, hist_event_time, histx, histy, hpress, hmsize);
//            LOGT("      hist[%d]: time: %ld, hraw: %.2f,%.2f, hist: %.2f,%.2f, press: %.2f, msize: %.2f, touch: %.2f,%.2f, tool: %.2f,%.2f, orient: %.2f",
//                j, hist_event_time, hrawx, hrawy, histx, histy, hpress, hmsize, htouch_major, htouch_minor, htool_major, htool_minor, horient);
        }
    }
}

void dump_key_event(AInputEvent* e)
{
    int32_t action, flags, key_code, scan_code, meta_state, repeat_count;
    int64_t down_time, event_time;
        
    action = AKeyEvent_getAction(e);
    flags = AKeyEvent_getFlags(e);
    key_code = AKeyEvent_getKeyCode(e);
    scan_code = AKeyEvent_getScanCode(e);
    meta_state = AKeyEvent_getMetaState(e);
    repeat_count = AKeyEvent_getRepeatCount(e);
    down_time = AKeyEvent_getDownTime(e);
    event_time = AKeyEvent_getEventTime(e);
    
    LOGT("  key: time: %ld, action: %d, flags: %x, key_code: %d, scan_code: %d, mstate: %d, rcount: %d, dtime: %ld",
        event_time, action, flags, key_code, scan_code, meta_state, repeat_count, down_time);
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
    
    LOGT("type: %d, devid: %d, src: %d", type, devid, src);
  
    if (type == AINPUT_EVENT_TYPE_MOTION)
    {
        LOGT("event type: AINPUT_EVENT_TYPE_MOTION[%d]", type);
        dump_motion_event(e);
    }
    else if (type == AINPUT_EVENT_TYPE_KEY)
    {
        LOGT("event type: AINPUT_EVENT_TYPE_KEY[%d]", type);
        dump_key_event(e);
    }
    else
    {
        LOGT("event type: %d", type);
    }

    return 0;
}

static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    struct engine* engine = (struct engine*)app->userData;
    LOGT("cmd: %d", cmd);

    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;

        case APP_CMD_INIT_WINDOW:
            if (engine->app->window != NULL)
            {
//                engine_init_display(engine);
//                engine_draw_frame(engine);
            }
            break;

        case APP_CMD_TERM_WINDOW:
            break;

        case APP_CMD_GAINED_FOCUS:
            if (engine->accelerometerSensor != NULL)
            {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue, engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue, engine->accelerometerSensor, (1000L/60)*1000);
            }
            break;
        
        case APP_CMD_LOST_FOCUS:
            if (engine->accelerometerSensor != NULL)
            {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue, engine->accelerometerSensor);
            }
            
            break;
    }
}

void android_main(struct android_app* state)
{
    struct engine engine;

    app_dummy();
    LOGT("android_main started");

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    // Prepare to monitor accelerometer
    engine.sensorManager = ASensorManager_getInstance();
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
            state->looper, LOOPER_ID_USER, NULL, NULL);

    if (state->savedState != NULL)
    {
        engine.state = *(struct saved_state*)state->savedState;
    }

    // loop waiting for stuff to do.

    while (1)
    {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        while ((ident = ALooper_pollAll(-1, NULL, &events, (void**)&source)) >= 0) 
        {
            if (source != NULL)
            {
                source->process(state, source);
            }

            if (ident == LOOPER_ID_USER)
            {
                if (engine.accelerometerSensor != NULL)
                {
                    ASensorEvent e;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue, &e, 1) > 0)
                    {
                        LOGT("accelerometer: x=%f y=%f z=%f", e.acceleration.x, e.acceleration.y, e.acceleration.z);
                    }
                }
            }

            if (state->destroyRequested != 0)
            {
                return;
            }
        }
    }
}
