
#include "glwindow.h"

#include "../utils/memoryUtils.h"

#include <gl\GL.h>
#include <gl\GLU.h>

typedef struct GlWindow_t
{
	HGLRC		m_hGlrc;
	HDC			m_hDc;
	HWND		m_hWnd;
	HINSTANCE	m_hInstance;
	LPCWSTR		m_lpWindowClass;
	BOOL		m_windowActive;
	BOOL		m_windowDone;
} GlWindow;

// Private Interface //

LRESULT CALLBACK GlWindow_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
GLvoid GlWindow_Resize(GLsizei width, GLsizei height);
void GlWindow_HandleConstructionFailure(GlWindow* pWindow);

// temporary gl setup functions - move out to a scene or renderer //
int GlWindow_InitGl();

// Implementation //

GlWindow* GlWindow_Create(GlWindowInit *pInit)
{
	GlWindow* pWindow = ALLOCATE(GlWindow);

	GLuint pixelFormat;
	WNDCLASS wc;		// window class
	DWORD dwExStyle;	// extended style
	DWORD dwStyle;		// window style

	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),								// size
		1,															// version
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,	//format
		PFD_TYPE_RGBA,												// rgba format
		0,															// color depth
		0, 0, 0, 0, 0, 0,											// color bits ignored
		0,															// alpha buffer
		0,															// shift bit
		0,															// accum buffer
		0, 0, 0, 0,													// accum bits ignored
		16,															// z-buffer
		0,															// stencil buffer
		0,															// aux buffer
		PFD_MAIN_PLANE,												// main drawing layer
		0,															// reserved
		0, 0, 0														// layer masks ignored
	};

	// Describe the window size
	RECT WindowRect;
	WindowRect.left = 0L;
	WindowRect.right = pInit->width;
	WindowRect.top = 0L;
	WindowRect.bottom = pInit->height;
	ZERO(pWindow,GlWindow);

	pWindow->m_lpWindowClass = pInit->lpWindowClass;

	// setup the window
	pWindow->m_hInstance = GetModuleHandle(NULL);	// Window Instance
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw on move, own DC
	wc.lpfnWndProc = (WNDPROC) GlWindow_WndProc;	// WndProc
	wc.cbClsExtra = 0;								// Extra Window Data
	wc.cbWndExtra = 0;								// Extra Window Data
	wc.hInstance = pWindow->m_hInstance;			// hInstance for the window
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load default icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);		// Load a Mouse Pointer
	wc.hbrBackground = NULL;						// No background
	wc.lpszMenuName = NULL;							// No menu
	wc.lpszClassName = pInit->lpWindowClass;		// Class Name

	// register the class
	if (!RegisterClass(&wc))
	{
		MessageBox(
			NULL,
			L"Failed to register the Window Class",
			L"Startup Failure",
			MB_OK | MB_ICONEXCLAMATION);
		GlWindow_HandleConstructionFailure(pWindow);
		return NULL;
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle = WS_OVERLAPPEDWINDOW;

	// resize adjusts the window borders to not draw over the GL Scene
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	// Create the window
	pWindow->m_hWnd = CreateWindowEx(
		dwExStyle,							// Extended Style
		pInit->lpWindowClass,				// Window Class
		pInit->lpWindowTitle,				// Window Title
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,	// Window Style

		// Window Position
		CW_USEDEFAULT,
		CW_USEDEFAULT,

		// Window Size
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		
		NULL,								// Parent Window
		NULL,								// Menu
		pWindow->m_hInstance,				// hInstance
		NULL);								// pass to WM_CREATE

	// setup the associated pointer
	SetWindowLongPtr(
		pWindow->m_hWnd,
		GWLP_USERDATA,
		(LONG_PTR)pWindow);

	if (!pWindow->m_hWnd)
	{
		GlWindow_HandleConstructionFailure(pWindow);
		return NULL;
	}

	// get the device context
	pWindow->m_hDc = GetDC(pWindow->m_hWnd);
	if (!pWindow->m_hDc)
	{
		GlWindow_HandleConstructionFailure(pWindow);
		return NULL;
	}

	pfd.cColorBits = pInit->bits;

	// setup the pixel format
	pixelFormat = ChoosePixelFormat(pWindow->m_hDc, &pfd);
	if (!pixelFormat)
	{
		GlWindow_HandleConstructionFailure(pWindow);
		return NULL;
	}

	if (!SetPixelFormat(pWindow->m_hDc, pixelFormat, &pfd))
	{
		GlWindow_HandleConstructionFailure(pWindow);
		return NULL;
	}

	pWindow->m_hGlrc = wglCreateContext(pWindow->m_hDc);
	if (!pWindow->m_hGlrc)
	{
		GlWindow_HandleConstructionFailure(pWindow);
		return NULL;
	}

	if (!wglMakeCurrent(pWindow->m_hDc, pWindow->m_hGlrc))
	{
		GlWindow_HandleConstructionFailure(pWindow);
		return NULL;
	}

	ShowWindow(pWindow->m_hWnd, SW_SHOW);			// show the window
	SetForegroundWindow(pWindow->m_hWnd);			// increase its priority
	SetFocus(pWindow->m_hWnd);						// give it keyboard focus
	GlWindow_Resize(pInit->width, pInit->height);	// setup the gl scene

	GlWindow_InitGl();

	return pWindow;
}

void GlWindow_Destroy(GlWindow* pWindow)
{
	if (pWindow->m_hGlrc)
	{
		if (!wglMakeCurrent(NULL,NULL))
		{
			MessageBox(
				NULL,
				L"Failed to release DC and RC",
				L"Termination Failure",
				MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(pWindow->m_hGlrc))
		{
			MessageBox(
				NULL,
				L"Failed to release RC",
				L"Termination Failure",
				MB_OK | MB_ICONINFORMATION);
		}
		pWindow->m_hGlrc = NULL;
	}

	if (pWindow->m_hDc && !ReleaseDC(pWindow->m_hWnd, pWindow->m_hDc))
	{
		MessageBox(
			NULL,
			L"Failed to release DC",
			L"Termination Failure",
			MB_OK | MB_ICONINFORMATION);
	}
	pWindow->m_hDc = NULL;

	if (pWindow->m_hWnd && !DestroyWindow(pWindow->m_hWnd))
	{
		MessageBox(
			NULL,
			L"Could not release hWnd",
			L"Termination Failure",
			MB_OK | MB_ICONINFORMATION);
	}
	pWindow->m_hWnd = NULL;

	if (!UnregisterClass(pWindow->m_lpWindowClass, pWindow->m_hInstance))
	{
		MessageBox(
			NULL,
			L"Failed to unregister class",
			L"Termination Failure",
			MB_OK | MB_ICONINFORMATION);
	}
	pWindow->m_hInstance = NULL;

	SAFE_FREE(pWindow);
}

BOOL GlWindow_isActive(GlWindow* pGlWindow)
{
	return pGlWindow->m_windowActive;
}

BOOL GlWindow_isDone(GlWindow* pGlWindow)
{
	return pGlWindow->m_windowDone;
}

void GlWindow_setDone(GlWindow* pGlWindow, BOOL done)
{
	pGlWindow->m_windowDone = done;
}

void GlWindow_swapBuffers(GlWindow* pGlWindow)
{
	glFinish();
	SwapBuffers(pGlWindow->m_hDc);
}

LRESULT CALLBACK GlWindow_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GlWindow* pWindow = (GlWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{
	case WM_ACTIVATE:
		{
			if(!HIWORD(wParam))
			{
				pWindow->m_windowActive = TRUE;
			}
			else
			{
				pWindow->m_windowActive = FALSE;
			}
			return 0;
		}

	case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				// prevent the screensaver from starting or the monitor from sleeping
				return 0;
			}
			break;
		}
		
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

	case WM_KEYDOWN:
		{
			// key in wParam
			return 0;
		}

	case WM_KEYUP:
		{
			// key in wParam
			return 0;
		}

	case WM_SIZE:
		{
			GlWindow_Resize(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}

	case WM_QUIT:
		{
			GlWindow_setDone(pWindow, TRUE);
			return 0;
		}
	}

	// pass remaining messages up
	return DefWindowProc(hWnd, message, wParam, lParam);
}

GLvoid GlWindow_Resize(GLsizei width, GLsizei height)
{
	if ( height == 0 )
	{
		height = 1;
	}

	glViewport(0,0,width,height);

	// temporary
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.f, (GLfloat)width/(GLfloat)height, 0.1f, 100.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GlWindow_HandleConstructionFailure(GlWindow* pWindow)
{
	GlWindow_Destroy(pWindow);
	MessageBox(
		NULL,
		L"Failed to create window",
		L"Startup Failure",
		MB_OK|MB_ICONEXCLAMATION);
	SAFE_FREE(pWindow);
}

int GlWindow_InitGl()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	return TRUE;
}

int GlWindow_DrawGlScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	return TRUE;
}
