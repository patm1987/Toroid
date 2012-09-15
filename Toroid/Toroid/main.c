
#include "glwindow\glwindow.h"

void StartUpdateLoop(GlWindow* pWindow);

int main(int argc, char** argv)
{
	GlWindow *pWindow = NULL;
	GlWindowInit init =
	{
		640,
		480,
		32,
		L"OpenGL",
		L"GL Test"
	};
	pWindow = GlWindow_Create(&init);

	// 3d context
	// scene
	// game start

	if (pWindow)
	{
		StartUpdateLoop(pWindow);
	}

	GlWindow_Destroy(pWindow);
}

void StartUpdateLoop(GlWindow* pWindow)
{
	MSG msg;

	while (!GlWindow_isDone(pWindow))
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				GlWindow_setDone(pWindow, TRUE);
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if (GlWindow_isActive(pWindow))
		{
			GlWindow_DrawGlScene();
			GlWindow_swapBuffers(pWindow);
		}
	}
}
