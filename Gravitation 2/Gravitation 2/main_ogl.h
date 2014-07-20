
# ifndef _MAIN_OGL_H
# define _MAIN_OGL_H
 
// ������, ����������� ��� ������ ���������
# include <windows.h>
# include <stdio.h>
# include <string.h>
# include <math.h>
# include <Mmsystem.h>
# include <WinUser.h>
# include "GL\GL.H"
# include "GL\GLU.H"
# include "GL\glut.h"
# include "GL\GLAux.h"

# pragma comment ( lib, "winmm.lib" )

// ������� ���������� ����������, ������, ������ � ������� ����� ������
# define SCREEN_WIDTH 800
# define SCREEN_HEIGHT 600
# define SCREEN_DEPTH 16

// ���������� ��������� ����; ����� �������� �� ������ ������:
extern HWND  g_hWnd;
extern RECT  g_rRect;
extern HDC   g_hDC;
extern HGLRC g_hRC;
extern HINSTANCE g_hInstance;

//���������� ������.
	#include "Physics.h"
	using namespace Physics;
//

// ������� - ������� ���� ���������
int MainLoop();

// �������, ��������������� ������ �������
bool bSetupPixelFormat(HDC hdc);
 
// �������� �������, ��������������� ������� ���� OpenGL
void SizeOpenGLScreen(int width, int height);
 
// �������, ���������������� OpenGL
void InitializeOpenGL(int width, int height);
 
// ����� �������������
void InitOpenGL(HWND hWnd);

// �������, ������� ���������� ������ �����
void RenderScene(LPSYSSTATE,LPELLIPSES);
 
// ��-�������������
void DeInit();
 
# endif