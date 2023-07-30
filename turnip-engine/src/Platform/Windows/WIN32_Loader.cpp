#include "pch.h"
#ifdef TUR_PLATFORM_WINDOWS

#include "WIN32_Window.h"
#include "WIN32_Loader.h"
#include "Platform/OpenGL/OGL_API.h"

#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023
#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

namespace
{
	typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext,
		const int* attribList);
	wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;

	typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int* piAttribIList,
		const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
	wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;
}

namespace tur
{
	void WIN32_Loader_OGL::PreInitialize()
	{
		CreateDummyContext();
	}

	void WIN32_Loader_OGL::PostInitialize(Window* window, IGraphicsAPI* api)
	{
		WIN32_Window* win32Window = static_cast<WIN32_Window*>(window);
		
		HDC hdc = GetDC(win32Window->GetHandle());

		int pixelFormatAttributes[] = {
			WGL_DRAW_TO_WINDOW_ARB,		GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,		GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,		GL_TRUE,
			WGL_PIXEL_TYPE_ARB,			WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,			32,
			WGL_DEPTH_BITS_ARB,			24,
			WGL_STENCIL_BITS_ARB,		8,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			0, // End
		};

		int pixelFormat = 0;
		UINT numFormats = 0;
		wglChoosePixelFormatARB(hdc, pixelFormatAttributes, 0, 1, &pixelFormat, &numFormats);

		if (!numFormats)
		{
			TUR_CORE_ERROR("Failed to choose the OpenGL 3.0 pixel format.");
			return;
		}

		PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
		DescribePixelFormat(hdc, pixelFormat, sizeof(pixelFormatDescriptor), &pixelFormatDescriptor);

		if (!SetPixelFormat(hdc, pixelFormat, &pixelFormatDescriptor)) 
		{
			TUR_CORE_CRITICAL("Failed to set the OpenGL 3.0 pixel format.");
			return;
		}

		OGL_API_WIN32* glAPI = static_cast<OGL_API_WIN32*>(api);
		int versionMajor = glAPI->GetAPIData().versionMajor;
		int versionMinor = glAPI->GetAPIData().versionMinor;
		
		int openGLAttributes[] = 
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, versionMajor,
			WGL_CONTEXT_MINOR_VERSION_ARB, versionMinor,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0,
		};

		HGLRC context = wglCreateContextAttribsARB(hdc, 0, openGLAttributes);
		if (!context)
		{
			TUR_CORE_CRITICAL("Failed to create a OpenGL {}.{} context", versionMajor, versionMinor);
			return;
		}

		if (!wglMakeCurrent(hdc, context))
		{
			TUR_CORE_CRITICAL("Failed to set the OpenGL context as current.");
			return;
		}
	}

	void WIN32_Loader_OGL::CreateDummyContext()
	{
		constexpr const wchar_t* dummyClassName = L"TurnipDummyWindowClass";

		WNDCLASSEX wndDummyClass = {};
		wndDummyClass.cbSize = sizeof(wndDummyClass);
		wndDummyClass.style = CS_OWNDC;
		wndDummyClass.lpfnWndProc = DefWindowProc;
		wndDummyClass.lpszClassName = dummyClassName;

		if (FAILED(RegisterClassEx(&wndDummyClass)))
		{
			TUR_CORE_ERROR("Failed to create dummy WIN32 class for OpenGL context creation. \
				| Error Code: {}", GetLastError());
			return;
		}

		HWND dummyHandle = CreateWindowEx(
			0,
			dummyClassName,
			L"",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			nullptr, nullptr, nullptr, this
		);

		if (!dummyHandle)
		{
			TUR_CORE_ERROR("Failed to create dummy WIN32 window for OpenGL context creation. \
				| Error Code: {}", GetLastError());
			return;
		}

		HDC dummyDevice = GetDC(dummyHandle);

		PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
		pixelFormatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pixelFormatDescriptor.nVersion = 1;
		pixelFormatDescriptor.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
		pixelFormatDescriptor.cColorBits = 32;
		pixelFormatDescriptor.cDepthBits = 24;
		pixelFormatDescriptor.cAlphaBits = 8;
		pixelFormatDescriptor.cStencilBits = 8;
		pixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE;

		int pixelFormat = ChoosePixelFormat(dummyDevice, &pixelFormatDescriptor);
		SetPixelFormat(dummyDevice, pixelFormat, &pixelFormatDescriptor);

		HGLRC dummyContext = wglCreateContext(dummyDevice);
		if (!dummyContext)
		{
			TUR_CORE_ERROR("Failed to create dummy WIN32 OpenGL context. | Error Code: {}", GetLastError());
			return;
		}

		wglMakeCurrent(dummyDevice, dummyContext);

		if (!gladLoadWGL(dummyDevice))
		{
			TUR_CORE_ERROR("Failed to load WGL OpenGL functions.");
			return;
		}

		if (!gladLoadGL())
		{
			TUR_CORE_ERROR("Failed to load OpenGL functions.");
			return;
		}

		wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress(
			"wglCreateContextAttribsARB");

		wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress(
			"wglChoosePixelFormatARB");

		wglMakeCurrent(dummyDevice, 0);
		wglDeleteContext(dummyContext);
		ReleaseDC(dummyHandle, dummyDevice);
		DestroyWindow(dummyHandle);
	}
}
#endif // TUR_PLATFORM_WINDOWS