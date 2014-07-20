
# ifndef _MAIN_OGL_H
# define _MAIN_OGL_H
 
// Хидеры, необходимые для работы программы
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

// Обьявим глобальные переменные, ширину, высоту и глубину цвета экрана
# define SCREEN_WIDTH 800
# define SCREEN_HEIGHT 600
# define SCREEN_DEPTH 16

// Глобальные параметры окна; будут доступны из других файлов:
extern HWND  g_hWnd;
extern RECT  g_rRect;
extern HDC   g_hDC;
extern HGLRC g_hRC;
extern HINSTANCE g_hInstance;

//физическая модель.
	#include "Physics.h"
	using namespace Physics;
//

// Функция - главный цикл программы
int MainLoop();

// Функция, устанавливающая формат пиксела
bool bSetupPixelFormat(HDC hdc);
 
// Прототип функции, устанавливающей размеры окна OpenGL
void SizeOpenGLScreen(int width, int height);
 
// Функция, инициализирующая OpenGL
void InitializeOpenGL(int width, int height);
 
// Общая инициализация
void InitOpenGL(HWND hWnd);

// Функция, которая собственно рисует сцену
void RenderScene(LPSYSSTATE,LPELLIPSES);
 
// Де-инициализация
void DeInit();
 
# endif