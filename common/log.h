#ifndef LOG_H_
#define LOG_H_

#include <android/log.h>

extern const char* g_appname;
extern int g_log_level;

#define LOGT(...)   { if (g_log_level <= ANDROID_LOG_VERBOSE) { ((void)__android_log_print(ANDROID_LOG_VERBOSE,  g_appname, __VA_ARGS__)); } }
#define LOGD(...)   { if (g_log_level <= ANDROID_LOG_DEBUG)   { ((void)__android_log_print(ANDROID_LOG_DEBUG,    g_appname, __VA_ARGS__)); } }
#define LOGI(...)   { if (g_log_level <= ANDROID_LOG_INFO)    { ((void)__android_log_print(ANDROID_LOG_INFO,     g_appname, __VA_ARGS__)); } }
#define LOGW(...)   { if (g_log_level <= ANDROID_LOG_WARN)    { ((void)__android_log_print(ANDROID_LOG_WARN,     g_appname, __VA_ARGS__)); } }
#define LOGE(...)   { if (g_log_level <= ANDROID_LOG_ERROR)   { ((void)__android_log_print(ANDROID_LOG_ERROR,    g_appname, __VA_ARGS__)); } }
#define LOGGLE(...) \
{ \
  GLenum error = glGetError(); \
  if (error != GL_NO_ERROR) \
  { \
      ((void)__android_log_print(ANDROID_LOG_ERROR, g_appname, __VA_ARGS__)); \
      ((void)__android_log_print(ANDROID_LOG_ERROR, g_appname, "glerror: %d[0x%X]\n", error, error)); \
  } \
}

#endif // LOG_H_

