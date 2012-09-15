
#include "glwindow/glwindow.h"

#include "messaging/messaging.h"
#include "input/keyboard.h"

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

	Messaging_Init();
	pWindow = GlWindow_Create(&init);
	Keyboard_Init();

	// 3d context
	// scene
	// game start

	if (pWindow)
	{
		StartUpdateLoop(pWindow);
	}

	Keyboard_Cleanup();
	GlWindow_Destroy(pWindow);
	Messaging_Cleanup();
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
