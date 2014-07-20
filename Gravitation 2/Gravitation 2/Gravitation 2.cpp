#include "stdafx.h"
#include "Gravitation 2.h"

# pragma comment (lib, "opengl32.lib")
# pragma comment (lib, "glu32.lib")
# pragma comment (lib, "glaux.lib")
# pragma comment (lib, "ftgl_dynamic_MT.lib")

# pragma comment (lib, "DevIL.lib")
# pragma comment (lib, "ilut.lib")
# pragma comment (lib, "ilu.lib")


# include "main_ogl.h"
# include "camera.h"
# include "glelements_and_texture.h"
# include "font.h"

float		kSpeed = 1.0f;
unsigned	PlanetSize = 100;
int			CamPlanetInd;
bool		CamToPlanet = false;
LPSYSSTATE  tempstate;

CTexture *Texture;
CCamera g_Camera;

GLfloat rotPlanet = 0.0f;
bool PointFlag = true;

float g_FogDensity = 0.00005f;
float fogColor[4] = {0.0f,0.0f,0.0f,1.0f};

CFont *Font;
GLelements GLE;

// Необходимые дескрипторы:
HWND  g_hWnd;
RECT  g_rRect;
HDC   g_hDC;
HGLRC g_hRC;
HINSTANCE g_hInstance;

//залипание курсора
bool flag = false;
HCURSOR hCursor;

#define MAX_LOADSTRING 100
MSG msg;

//класс интерфейса.
	#include "Interface.h"
	using namespace Interface;

	LPMAINWINDOW window;
//
//физическая модель.
	#include "Physics.h"
	using namespace Physics;

	LPSYS solar_sys;
//

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	WndProc_OGL(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	WndProc_STW(HWND, UINT, WPARAM, LPARAM);//set time window reaction
int					MainLoop();

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	InitCommonControls();
	LPLOGO logo = new LOGO(hInstance);
	logo->Show();

	// создание и запуск окна программы.
	window = new MAINWINDOW();
	window->InitCtrlInterface(hInstance, WndProc, WndProc_OGL, WndProc_STW);

	//создание физической модели
	solar_sys = new SYS();
	//задание первоначального положения планет.
	solar_sys->SetSysFirstState();
	//запуск основного цикла работы программы
	InitOpenGL ( window -> OGLhWnd );
	
	logo->LogoLoop();

	ShowWindow(window->hWnd, 1);
	UpdateWindow(window->hWnd);

	return MainLoop();
}
//основной цикл обработки сообщений и отрисовки
int MainLoop()
{
	while(1)
	{
		if ( PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
				break;
			TranslateMessage ( &msg );
			DispatchMessage ( &msg );
		}
		else
		{	
			if(flag)
			{
				g_Camera.SetViewByMouse ( window->OGLhWnd );
				g_Camera.Update ( kSpeed, window->OGLhWnd );
			}
			if(CamToPlanet)
			{
				tempstate = solar_sys->GetBuffer();
				g_Camera.PositionCamera (	(float)(tempstate->planets[CamPlanetInd].r.x*1e-8)-2.5*(CamPlanetInd+1),
											(float)(tempstate->planets[CamPlanetInd].r.z*1e-8)+10*(CamPlanetInd+1),
											(float)(tempstate->planets[CamPlanetInd].r.y*1e-8)+2.5*(CamPlanetInd+1),
													0, 0, 0,		0, 1, 0 );
			}
			//render & calkulate new system position
			//////////////////////////Пример:////////////////////////////////////////
			//ogl->Render(solar_sys->GetBuffer(), solar_sys->traect, solar_sys->traect2); 
			///////////////////////////////////////////////////////////////////////////
			// LPELLIPSES solar_sys->traect - траектория "из интернета"
			// LPSYSSTATE solar_sys->GetBuffer() - дает класс, где лежат координаты каждой планеты.
			//для того чтобы ogl понимал результат GetBuffer, нужно чтобы он подинклудил себе Physics.h, там есть описание.
			RenderScene ( solar_sys->GetBuffer(), &solar_sys->traect );
			if(window->GoParam)
			{
				solar_sys->SystemStep();
				if(solar_sys->year > 1e9)
				{
					window->GoParam = false;
					solar_sys->t = 0;
					solar_sys->CalculateTimeToDate();
				}
				window->SetSysTime(solar_sys->year, solar_sys->month, solar_sys->day, solar_sys->hour);
			}
			if(window->SetNewDate)//установка нового времени
			{
				solar_sys->SetSysTime(window->pary,window->parm,window->pard,window->parh);
				window->SetNewDate = false;
			}
		}		
	}
	return 0;
}
//функция обработки сообщений от главного окна программы
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	wchar_t lpch[20];
	switch (message)
	{
#pragma region set transparent into the static text and set text color
	case WM_CTLCOLORSTATIC:	 
	{
		SetBkColor  ( (HDC)wParam, RGB(192,192,192) );
		SetTextColor( (HDC)wParam, RGB(55, 55, 55));
		return (LONG)GetStockObject(LTGRAY_BRUSH);
	}
#pragma endregion
#pragma region set min size
	case WM_GETMINMAXINFO: //Получили сообщение от Винды
	{
        MINMAXINFO *pInfo = (MINMAXINFO *)lParam;
        POINT Min = { 800, 600 };
        pInfo->ptMinTrackSize = Min; // Установили минимальный размер
        return 0;
	}
#pragma endregion
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case TSL_ID:
			if(solar_sys->timespeed > 1.8e-8)
			{
				solar_sys->timespeed /= 10;
				swprintf_s(window->lpch,L"%.1e x",solar_sys->timespeed*1800);
				SetWindowText(window->StTimeSpeedNum,window->lpch);
			}
			break;
		case TSR_ID:
			if(solar_sys->timespeed < 1.8e+11)
			{
				solar_sys->timespeed *= 10;
				swprintf_s(window->lpch,L"%.1e x",solar_sys->timespeed*1800);
				SetWindowText(window->StTimeSpeedNum,window->lpch);
			}
			break;
		case BPlSzUp_ID:
			if(PlanetSize <	10000)
			{
				PlanetSize <<= 1;
				swprintf_s(lpch, L"%dx", PlanetSize);
				SetWindowText(window->PlanetSizeNum, lpch);
			}
			break;
		case BPlSzDn_ID:
			if(PlanetSize > 1)
			{
				PlanetSize >>= 1;
				swprintf_s(lpch, L"%dx", PlanetSize);
				SetWindowText(window->PlanetSizeNum, lpch);
			}
			break;
		case BKmSpUp_ID:
			if(kSpeed <	100)
			{
				kSpeed *= 2;
				swprintf_s(lpch, L"%.fx", kSpeed);
				SetWindowText(window->KamSpeedNum, lpch);
			}
			break;
		case BKmSpDn_ID:
			if(kSpeed > 1)
			{
				kSpeed /= 2;
				swprintf_s(lpch, L"%.fx", kSpeed);
				SetWindowText(window->KamSpeedNum, lpch);
			}
			break;
		case CBGoToObj_ID:
			if(wmEvent==LBN_SELCHANGE)
			{
				CamPlanetInd = SendMessage(window->CBGoToObj,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
				CamToPlanet = true;
			}
			break;
		default:
			window->WM_COMMAND_reaction(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		window->MoveAllWnd();
		SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));// LoWord=Width, HiWord=Height
		GetClientRect(hWnd, &g_rRect); // получаем window rectangle
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//функция обработки сообщений от OGL окна
LRESULT CALLBACK WndProc_OGL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEACTIVATE:
		flag = !flag;
		if(flag == false)
		{
			SetCursor(hCursor);
			ShowCursor(true);
		}
		else
		{
			hCursor = SetCursor(NULL);
			ShowCursor(false);
			CamToPlanet = false;
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//функция обработки сообщений от окна ввода даты
LRESULT CALLBACK WndProc_STW(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		return window->STW_WM_COMMAND_reaction(hWnd, message, wParam, lParam);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/*//-_-_-_-_-_-_\\OGL//-_-_-_-_-_-_\\*/
void RenderScene ( LPSYSSTATE Buffer, LPELLIPSES traect )
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glLoadIdentity ( );
	glFogfv(GL_FOG_COLOR, fogColor);

	glDisable(GL_LIGHTING);

	glClearColor ( 0.0f, 0.0f, 0.0f, 1 );

	glInitNames ( );
	
	g_Camera.Look ( );
	glColor3ub ( 140, 147, 155 );

	glRotated ( -90, 1, 0, 0 );

	glColor3ub ( 140, 147, 155 );
	GLE.DrawQ ( Texture, 10, 1000 );
	GLE.DrawQ ( Texture, 11, 0 );

	/*glColor3ub ( 255, 0, 0 );
	glBegin ( GL_LINES );
		glVertex3f( 0, 0, 0 );
		glVertex3f( 20, 0, 0 );
	glEnd();
	glColor3ub ( 0, 255, 0 );
	glBegin ( GL_LINES );
		glVertex3f( 0, 0, 0 );
		glVertex3f( 0, 20, 0 );
	glEnd();
	glColor3ub ( 0, 0, 255 );
	glBegin ( GL_LINES );
		glVertex3f( 0, 0, 0 );
		glVertex3f( 0, 0, 20 );
	glEnd();*/
	
	glColor3ub ( 140, 147, 155 );
	GLE.GenParticles ( PointFlag );

	GLE.Traectory ( traect );

	glPushMatrix ( );
		if ( rotPlanet > 360.0f )
			rotPlanet = 0.0f;
		glRotated ( rotPlanet, 0, 0, 1 );
		rotPlanet += 0.01f;
		GLE.DrawQ ( Texture, 0, solar_sys->PtRad[0]*1e-7 );
		glFogi(GL_FOG_MODE, GL_EXP2);
		glFogf(GL_FOG_DENSITY, g_FogDensity);
		glHint(GL_FOG_HINT, GL_FASTEST );
		glFogf(GL_FOG_START, 0);
		glFogf(GL_FOG_END, 10000.0f);
		glEnable(GL_FOG);
	glPopMatrix ( );
	
	for( int i=0; i<9; i++ )
	{
		glTranslatef ( (float)(Buffer->planets[i].r.y*1e-8), (float)(Buffer->planets[i].r.x*1e-8), (float)(Buffer->planets[i].r.z*1e-8) );
		glPushMatrix ( );	
			if ( rotPlanet > 360.0f )
				rotPlanet = 0.0f;
			glRotated ( rotPlanet, 0, 0, 1 );
			rotPlanet += 0.01f;
			GLE.DrawQ ( Texture, i + 1, solar_sys->PtRad[i+1]*1e-8*PlanetSize );
		glPopMatrix ( );
		glTranslatef ( (float)(Buffer->planets[i].r.y*1e-8*(-1)), (float)(Buffer->planets[i].r.x*1e-8*(-1)), (float)(Buffer->planets[i].r.z*1e-8*(-1)) );
	}

	// CAMERA
	//g_Camera.PositionCamera ( (float)(Buffer->planets[6].r.x*1e-6)-50*6, (float)(Buffer->planets[6].r.z*1e-6)+200*6, (float)(Buffer->planets[6].r.y*1e-6)+50*6,
	//						  0, 0, 0,
	//						  0, 1, 0 );

	glMatrixMode ( GL_PROJECTION );
	glPushMatrix ( );
	glLoadIdentity ( );
	glOrtho ( 0, 800, 0, 600, 0, 1 );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );

	//wchar_t Buffer [ 256 ];
	//glDisable ( GL_DEPTH_TEST );
	//	glColor3f ( 0.0f, 0.0f, 0.0f );		
	//	swprintf ( Buffer, sizeof ( Buffer ), L"Время: %.3f, с", 700000 );
	//	Font -> Font -> FaceSize ( 20 );
	//	Font -> Print ( 40, 45, Buffer );  
	//glEnable ( GL_DEPTH_TEST );

	glMatrixMode ( GL_PROJECTION );
	glPopMatrix ( );
	glMatrixMode ( GL_MODELVIEW );

	glFlush();

	SwapBuffers ( g_hDC );	
} 
void InitOpenGL (HWND hWnd)
{
	g_hWnd = hWnd;
	GetClientRect(g_hWnd, &g_rRect);
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);
	
	Texture = new CTexture ( );
	Texture -> InitTextures ( Texture );

	Font = new CFont ( "framd.ttf", 10, 10 );

	g_Camera.PositionCamera ( 0, 10.5f, 10,		0, 2.5f, 0,		0, 1, 0 );
}