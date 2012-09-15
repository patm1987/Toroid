
#ifndef _GLWINDOW_H
#define _GLWINDOW_H

#include <stdint.h>
#include <Windows.h>

struct GlWindow_t;
typedef struct GlWindow_t GlWindow;

typedef struct GlWindowInit_t
{
	int32_t width;
	int32_t height;
	int32_t bits;
	LPCWSTR lpWindowClass;
	LPCWSTR lpWindowTitle;
} GlWindowInit;

// Public Interface //
GlWindow* GlWindow_Create(GlWindowInit *pInit);
void GlWindow_Destroy(GlWindow* pGlWindow);
BOOL GlWindow_isActive(GlWindow* pGlWindow);
BOOL GlWindow_isDone(GlWindow* pGlWindow);
void GlWindow_setDone(GlWindow* pGlWindow, BOOL done);
void GlWindow_swapBuffers(GlWindow* pGlWindow);

// temporary gl functions - move out to a scene or renderer //
int GlWindow_DrawGlScene();

#endif//_GLWINDOW_H
