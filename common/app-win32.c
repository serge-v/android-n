#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <time.h>
#include <jni.h>
#include <GLES/gl.h>
#include <share.h>
#include "app.h"
#include "log.h"

int g_app_alive = 1;
static FILE* g_log = NULL;

static HINSTANCE sInstance;
static const wchar_t sAppName[] = L"test";
HWND g_hwnd;
HWND g_glwnd;
static int sWindowWidth = 1280;
static int sWindowHeight = 800;

static const char* levels[] = {
	"LEVEL0",
	"LEVEL1",
	"TRACE",
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL"
};

int print_level(int level)
{
	time_t now = time(NULL);
	struct tm* t = gmtime(&now);
	return fprintf(g_log, "%04d-%02d-%02d %02d:%02d:%02d %s ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, levels[level]);
}

void check_glerrors()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		wchar_t errorString[32];
		swprintf(errorString, 32, L"0x%04x", error);
		MessageBoxW(NULL, errorString, L"GL Error", MB_OK);
	}
}

static LRESULT CALLBACK wnd_proc(HWND wnd, UINT message,
                                 WPARAM wParam, LPARAM lParam)
{
	RECT rc;
	POINT pt;
	int useDefWindowProc = 0;

	switch (message) {
	case WM_PAINT:
		break;
	case WM_CLOSE:
		DestroyWindow(wnd);
		g_app_alive = 0;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		g_app_alive = 0;
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE || wParam == VK_RETURN)
			g_app_alive = 0;
		else
			app_key(wParam);
		useDefWindowProc = 1;
		break;

	case WM_KEYUP:
		useDefWindowProc = 1;
		break;

	case WM_SIZE:
		if (GetClientRect(g_hwnd, &rc)) {
			sWindowWidth = rc.right;
			sWindowHeight = rc.bottom;
		}
		break;

	case WM_LBUTTONDOWN:
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		MapWindowPoints(g_hwnd, g_glwnd, &pt, 1);
		LOGT("WM_LBUTTONDOWN: x, y = %d, %d", pt.x, pt.y);
		SetCapture(g_hwnd);
		app_touch(1, pt.x, sWindowHeight - pt.y);
		useDefWindowProc = 0;
		break;

	case WM_LBUTTONUP:
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		MapWindowPoints(g_hwnd, g_glwnd, &pt, 1);
		LOGT("WM_LBUTTONUP: x, y = %d, %d", pt.x, pt.y);
		ReleaseCapture();
		app_touch(2, pt.x, sWindowHeight - pt.y);
		useDefWindowProc = 0;
		break;

	case WM_MOUSEMOVE:
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		MapWindowPoints(g_hwnd, g_glwnd, &pt, 1);
//		LOGT("WM_MOUSEMOVE: x, y = %d, %d", x, y);
		app_touch(0, pt.x, sWindowHeight - pt.y);
		useDefWindowProc = 0;
		break;

	default:
		useDefWindowProc = 1;
	}

	if (useDefWindowProc)
		return DefWindowProc(wnd, message, wParam, lParam);
	return 0;
}

void enable_opengl(HWND hWnd, HDC * hDC, HGLRC * hRC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;
	BOOL rc;

	*hDC = GetDC(hWnd);

	// set the pixel format for the DC
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	iFormat = ChoosePixelFormat(*hDC, &pfd);
	SetPixelFormat(*hDC, iFormat, &pfd);

	*hRC = wglCreateContext(*hDC);
	rc = wglMakeCurrent(*hDC, *hRC);
	if (!rc)
		check_glerrors();
}

int create_window(HINSTANCE instance, int cmd_show)
{
	WNDCLASSW wc;
	DWORD windowStyle;

	sInstance = instance;

	// register class
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)wnd_proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = sInstance;
	wc.hIcon = NULL;
	wc.hCursor =  LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = sAppName;
	if (!RegisterClassW(&wc))
		return -1;

	// init instance
	windowStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE;
	windowStyle |= WS_OVERLAPPEDWINDOW;

	g_hwnd = CreateWindowW(sAppName, sAppName, windowStyle,
	                       100, 100,
	                       1600, 900,
	                       NULL, NULL, instance, NULL);
	if (!g_hwnd)
		return -1;

	g_glwnd = CreateWindowW(L"Static", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 1280, 800, g_hwnd, NULL, NULL, NULL);

	create_controls();

	ShowWindow(g_hwnd, cmd_show);
	UpdateWindow(g_hwnd);
	return 0;
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	MSG msg;
	HDC hDC;
	HGLRC hRC;
	int rc;

	prevInstance;
	cmdLine;

	g_log = fopen("1.log", "wt");
	LOGI("log opened");

	if (create_window(instance, cmdShow) == -1)
		return 1;

	LOGI("window created");
	enable_opengl(g_glwnd, &hDC, &hRC);
	LOGI("opengl enabled");
	app_init(sWindowWidth, sWindowHeight);
	check_glerrors();
	LOGI("app inited");
	/*
	while (g_app_alive)
	{
	    while (PeekMessage(&msg, g_hwnd, 0, 0, PM_NOREMOVE))
	    {
	        if (GetMessage(&msg, g_hwnd, 0, 0))
	        {
	            TranslateMessage(&msg);
	            DispatchMessage(&msg);
	        }
	        else
	            g_app_alive = 0;
	    }

	    if (g_app_alive)
	    {
	        app_render(GetTickCount(), sWindowWidth, sWindowHeight);
	        check_glerrors();
	        SwapBuffers(hDC);
	        check_glerrors();
	    }
	}
	*/
	while ((rc = GetMessage(&msg, g_hwnd, 0, 0)) != 0) {
		if (rc == -1)
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_PAINT) {
			app_render(GetTickCount(), sWindowWidth, sWindowHeight);
			check_glerrors();
			SwapBuffers(hDC);
			check_glerrors();
		}
	}

	app_deinit();
	LOGI("app deinited");

	return 0;
}

int __android_log_print(int prio, const char *tag,  const char *fmt, ...)
{
	va_list marker;
	int n;
	tag;

	va_start(marker, fmt);
	n = print_level(prio);
	n += vfprintf(g_log, fmt, marker);
	n += fprintf(g_log, "\n");
	fflush(g_log);
	return n;
}
