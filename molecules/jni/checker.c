#include <stdio.h>
#include "app.h"
#include "log.h"

#include <jni.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>

struct engine {
	struct android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	int animating;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
};

static struct engine engine;
static NativeWindowType displayWindow;

static int
engine_init_display(struct engine* engine)
{
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_NONE
	};
	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;
	EGLBoolean rc;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	printf("eglGetDisplay: %p\n", display);
	
	displayWindow = 1; //(NativeWindowType)android_createDisplaySurface();
	printf("displayWindow: %p\n", displayWindow);

	rc = eglInitialize(display, 0, 0);
	printf("eglInitialize: %p\n", rc);

	/* Here, the application chooses the configuration it desires. In this
	 * sample, we have a very simplified selection process, where we pick
	 * the first EGLConfig that matches our criteria */
	rc = eglChooseConfig(display, attribs, &config, 1, &numConfigs);
	printf("eglChooseConfig: %p\n", rc);

//	setEGLConfigChooser(8, 8, 8, 8, 0, 0);

	surface = eglCreateWindowSurface(display, config, displayWindow, NULL);
	printf("surface: %p\n", surface);


	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	 * As soon as we picked a EGLConfig, we can safely reconfigure the
	 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	rc = eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
	printf("eglGetConfigAttrib: %p, format: %d\n", rc, format);

	int rci = ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);
	printf("ANativeWindow_setBuffersGeometry: %p\n", rci);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	printf("surface 2: %p\n", surface);

	context = eglCreateContext(display, config, NULL, NULL);
	printf("context: %p\n", context);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		printf("Unable to eglMakeCurrent");
		return -1;
	}

	rc = eglQuerySurface(display, surface, EGL_WIDTH, &w);
	printf("eglQuerySurface: %p, width: %d\n", rc, w);

	eglQuerySurface(display, surface, EGL_HEIGHT, &h);
	printf("eglQuerySurface: %p, height: %d\n", rc, h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;

	// Initialize GL state.
//    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
//    glEnable(GL_CULL_FACE);
//    glShadeModel(GL_SMOOTH);
//    glDisable(GL_DEPTH_TEST);

	app_init(w, h);

	return 0;
}

int main()
{
	printf("checker started\n");
	engine_init_display(&engine);
	printf("display inited\n");
	sleep(5);
	printf("checker stopped\n");

	return 0;
}
