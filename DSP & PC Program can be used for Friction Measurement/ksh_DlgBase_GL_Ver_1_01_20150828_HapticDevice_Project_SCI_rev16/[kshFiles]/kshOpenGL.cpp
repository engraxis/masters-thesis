#include <windows.h>
#include <gl\gl.h>			
#include <gl\glu.h>		
#include <gl\glaux.h>	
#include <gl\glut.h>
#include "kshOpenGL.h"



BOOL CreateGLWindow(HWND* hWnd, HDC* hDC, HGLRC* hRC) {
	GLuint		PixelFormat;				// Holds The Results After Searching For A Match
	static	PIXELFORMATDESCRIPTOR pfd=		// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),		// Size Of This Pixel Format Descriptor
		1,									// Version Number
		PFD_DRAW_TO_WINDOW |				// Format Must Support Window
		PFD_SUPPORT_OPENGL |				// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,					// Must Support Double Buffering
		PFD_TYPE_RGBA,						// Request An RGBA Format
		32,									// Select Our Color Depth
		0, 0, 0, 0, 0, 0,					// Color Bits Ignored
		0,									// No Alpha Buffer
		0,									// Shift Bit Ignored
		0,									// No Accumulation Buffer
		0, 0, 0, 0,							// Accumulation Bits Ignored
		16,									// 16Bit Z-Buffer (Depth Buffer)  
		0,									// No Stencil Buffer
		0,									// No Auxiliary Buffer
		PFD_MAIN_PLANE,						// Main Drawing Layer
		0,									// Reserved
		0, 0, 0								// Layer Masks Ignored
	};

	// Did We Get A Device Context?
	if (!(*hDC=GetDC(*hWnd))) {			
		KillGLWindow(hDC, hRC);	
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	// Did Windows Find A Matching Pixel Format?
	if (!(PixelFormat=ChoosePixelFormat(*hDC, &pfd))) {		
		KillGLWindow(hDC, hRC);
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	// Are We Able To Set The Pixel Format?
	if(!SetPixelFormat(*hDC, PixelFormat, &pfd)) {		
		KillGLWindow(hDC, hRC);	
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	// Are We Able To Get A Rendering Context?
	if (!(*hRC = wglCreateContext(*hDC))) {				
		KillGLWindow(hDC, hRC);
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	// Try To Activate The Rendering Context
	if(!wglMakeCurrent(*hDC, *hRC)) {					
		KillGLWindow(hDC, hRC);
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	//ShowWindow(*hWnd, SW_SHOW);		// Show The Window


//	SetForegroundWindow(hWnd1);		// Slightly Higher Priority
//	SetFocus(hWnd1);				// Sets Keyboard Focus To The Window

	return TRUE;
}


GLvoid KillGLWindow(HDC* hDC, HGLRC* hRC)	{			// Properly Kill The Window
	if (*hRC) {										// Do We Have A Rendering Context?	
		if (!wglMakeCurrent(NULL,NULL))	{				// Are We Able To Release The DC And RC Contexts?		
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(*hRC))	{				// Are We Able To Delete The RC?		
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		*hRC=NULL;										// Set RC To NULL
	}
}











