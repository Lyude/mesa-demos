/*
 * Copyright (C) 2016, VMware, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <windows.h>
#include <stdlib.h>
#include <GL/gl.h>

int
main(int argc, char *argv[])
{
   WNDCLASS wc;
   HWND hwnd;
   HDC hdc;
   PIXELFORMATDESCRIPTOR pfd;
   int iPixelFormat;
   HGLRC hglrc;

   ZeroMemory(&wc, sizeof wc);
   wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc = DefWindowProc;
   wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
   wc.lpszClassName = "wglfont";

   if (!RegisterClass(&wc)) {
      abort();
   }

   hwnd = CreateWindowEx(0,
                         wc.lpszClassName,
                         "wglfont",
                         WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TILEDWINDOW,
                         CW_USEDEFAULT, CW_USEDEFAULT, 512, 512,
                         NULL, NULL,
                         wc.hInstance,
                         NULL);
   if (!hwnd) {
      abort();
   }

   hdc = GetDC(hwnd);
   if (!hdc) {
      abort();
   }

   ZeroMemory(&pfd, sizeof pfd);
   pfd.nSize = sizeof pfd;
   pfd.nVersion = 1;
   pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
   pfd.iPixelType = PFD_TYPE_RGBA;
   pfd.cColorBits = 24;
   pfd.cDepthBits = 0;
   pfd.iLayerType = PFD_MAIN_PLANE;

   iPixelFormat = ChoosePixelFormat(hdc, &pfd);
   if (!iPixelFormat) {
      abort();
   }

   if (!SetPixelFormat(hdc, iPixelFormat, &pfd)) {
      abort();
   }

   hglrc = wglCreateContext(hdc);
   if (!hglrc) {
      abort();
   }

   wglMakeCurrent(hdc, hglrc);

   glClearColor(0.0, 0.0, 0.0, 1.0);
   glClear(GL_COLOR_BUFFER_BIT);

   HFONT hFont;
   hFont = CreateFont(72,			// Height Of Font
                      0,			// Width Of Font
                      0,			// Angle Of Escapement
                      0,			// Orientation Angle
                      FW_NORMAL,		// Font Weight
                      FALSE,			// Italic
                      FALSE,			// Underline
                      FALSE,			// Strikeout
                      ANSI_CHARSET,		// Character Set Identifier
                      OUT_TT_PRECIS,		// Output Precision
                      CLIP_DEFAULT_PRECIS,	// Clipping Precision
                      ANTIALIASED_QUALITY,	// Output Quality
                      FF_DONTCARE|DEFAULT_PITCH,	// Family And Pitch
                      "Arial");			// Font Name
   if (!hFont) {
      abort();
   }

   SelectObject(hdc, hFont);

   GLYPHMETRICSFLOAT agmf[256];
   wglUseFontOutlinesA(hdc, 0, 256, 1000, 0.0f, 1.0f, WGL_FONT_POLYGONS, agmf);

   glShadeModel(GL_SMOOTH);

   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);

   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHTING);
   glEnable(GL_COLOR_MATERIAL);
   glColor3f(1.0f, 0.5, 0.0);

   glTranslatef(-0.8f, -0.0f, -1.0f);
   glScalef(0.25f, 0.25f, 0.25f);

   glListBase(1000);

   glCallLists(12, GL_UNSIGNED_BYTE, "Hello World!");

   SwapBuffers(hdc);

   Sleep(1000);

   wglMakeCurrent(NULL, NULL);

   wglDeleteContext(hglrc);

   ReleaseDC(hwnd, hdc);

   DestroyWindow(hwnd);

   return 0;
}
