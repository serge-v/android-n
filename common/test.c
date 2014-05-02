#include "audio.h"
#include "log.h"
#include <jni.h>

static struct aengine ae;
int g_log_level = 0;
const char* g_appname = "test";

jint Java_mila_create_engine(JNIEnv*  env, jobject  this)
{
    LOGI("created");
    return create_engine(&ae);
}
