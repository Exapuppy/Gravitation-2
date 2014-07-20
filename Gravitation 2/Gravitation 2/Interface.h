#ifndef INTERFACE_H
#define INTERFACE_H

#include "stdafx.h"

namespace Interface

{
	#include "Resource.h"
    #define _USE_MATH_DEFINES
	#include <math.h>

#pragma region CONSTANTS
	//Font's
	#define MS_SANS_SERIF	L"MS Sans Serif"
	#define COURIER_NEW		L"Courier New"
	#define TIMES_NEW_ROMAN L"Times New Roman"
	#define DENG_XIAN		L"DengXian"
	#define IRIS_UPS		L"IrisUPS"
	//

	#define OGL_ID 1
	//#set time window control id
	  #define STW_ID 2
		#define STW_Y_ID 3
		#define STW_M_ID 4
		#define STW_D_ID 5
		#define STW_H_ID 6
		#define STW_Ok_ID 7//Okay     }
		#define STW_Cn_ID 8//Cancel  }  - Buttons
	//#end
	//
		#define TSL_ID 9
		#define TSR_ID 10
	//
		#define BPause_ID 11
		#define CBGoToObj_ID 12
	//
		#define BPlSzUp_ID 14
		#define BPlSzDn_ID 15
	//
		#define BKmSpUp_ID 16
		#define BKmSpDn_ID 17
	//
#pragma endregion

	typedef class tagWindow
	{
	public:
		wchar_t lpch[20];
		int par;
		int maxX, maxY;
		HWND hWnd;
		HDC hdc;
	} WINDOW, *LPWINDOW;
	typedef class tagLogo: public tagWindow
	{
		HWND hWndLogo;
		HINSTANCE hInst;
		LARGE_INTEGER b_start,b_stop,freq;
		void DrawBackground(HWND hWndBGR, int ID_BGR, HINSTANCE hInst)
		{
			HDC dc, bits;
			RECT rt;
			GetClientRect(hWndBGR, &rt);
			int maxX = rt.right, maxY = rt.bottom;
			dc = GetDC(hWndBGR);
			bits = CreateCompatibleDC(dc);
			HBITMAP BGR = LoadBitmap(hInst, MAKEINTRESOURCE(ID_BGR));
			SelectObject(bits, BGR);
			StretchBlt(dc, 0, 0, maxX, maxY, bits, 0, 0, maxX, maxY, SRCCOPY);
			DeleteDC(bits);
			ReleaseDC(hWndBGR, dc);
		};
	public:
		tagLogo(HINSTANCE hInst):hInst(hInst)
		{
			WNDCLASS wc = {0};
			wc.lpszClassName = L"logo";
			wc.lpfnWndProc = DefWindowProc;
			wc.hCursor	= LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_3DFACE);
			wc.hIcon	= (HICON)0;
			RegisterClass(&wc);

			hWndLogo = CreateWindow(L"logo", NULL, WS_POPUPWINDOW, GetSystemMetrics(SM_CXSCREEN) / 2.0f - 295, GetSystemMetrics(SM_CYSCREEN) / 2.0f - 200,
									590, 400, NULL, NULL, hInst, 0);
		}
		void Show()
		{
			// замер времени
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&b_start);

			// отображение заставки.
			ShowWindow(hWndLogo, 1	);
			DrawBackground( hWndLogo, IDB_LOGO, hInst);
		}
		void LogoLoop()
		{
			MSG msg;
			while(1)
			{
				if ( PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) )
				{
					if ( msg.message == WM_QUIT )				
						break;
					// выходит по нажатию клавиши(любой).
					if ( msg.message == WM_KEYDOWN || msg.message == WM_MBUTTONDOWN )
					{
						DestroyWindow(hWndLogo);
						break;
					}
					TranslateMessage ( &msg );
					DispatchMessage ( &msg );
				}
				// выходит после 10 секунд
				QueryPerformanceCounter(&b_stop);
				b_stop.QuadPart = (b_stop.QuadPart - b_start.QuadPart)/(freq.QuadPart);
				if(b_stop.QuadPart == 1000)
				{
					DestroyWindow(hWndLogo);
					break;
				}
			}
		}
	} LOGO, *LPLOGO;
	typedef class tagMAINWINDOW: public tagWindow
	{
	private:
	#pragma region Внутренние параметры
		int FmaxX, FmaxY;
		wchar_t *lpchiter;
	#pragma endregion
	#pragma region Font
		HFONT hFont;
		void set_Font( __in LPCWSTR font_name, __in int Height, __in int Weight)
		{
			hFont = CreateFont(	Height, Weight, 0, 0, 500, 0, 0, 0, 1, 0, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, font_name);
		};
		void set_Font( __in LPCWSTR font_name, __in int Height)
		{
			hFont = CreateFont(	Height, 0, 0, 0, 500, 0, 0, 0, 1, 0, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, font_name);
		};
		inline void setFontToC(HWND Ctrl)
		{
			SendMessage( Ctrl, WM_SETFONT, (WPARAM) hFont, 0 );
		};
	#pragma endregion

	#pragma region Контролы
		//Static controls
		HWND StYearNum, StYearText;
		HWND StMonthNum;
		HWND StDayNum;
		HWND StHourNum, StMonthDayHourText;
		HWND dv1,dv2;
		//
		HWND GoToObjName;
		
	#pragma endregion
	public:
		//Timespeed Panel
		HWND StTimeSpeedName, StTimeSpeedNum;
		HWND BTimeSpeedFast, BTimeSpeedSlow;
		bool GoParam;
		HWND BPause;
		//
		HWND CBGoToObj;
		//
		//Planet Size
		HWND PlanetSizeName, PlanetSizeNum;
		HWND BPlSzUp, BPlSzDn;
		//
		//KamSpeed
		HWND KamSpeedName, KamSpeedNum;
		HWND BKmSpUp, BKmSpDn;
		//
	#pragma region SetTimeWindow
		bool SetNewDate;//true сразу после установки новой даты
		HWND SetTimeWindow;//основное окошечко
		HWND STW_Ok,STW_Cn,STWY,STWM,STWD,STWH;//поля ввода и кнопки
		bool stwy,stwm,stwd,stwh;//какой из элементов был изменен
		long pary,parm,pard,parh;//содержат значения полей ввода
		wchar_t lpchy[20],lpchm[20],lpchd[20],lpchh[20];//содержат текст полей ввода
		bool KeyFocusSTW[4];//на каком элементе стоит фокус клавиатуры
	#pragma endregion
		HWND OGLhWnd;
		void SetSysTime(long, long, long, long);
	#pragma region Обработка сообщений
		void MoveAllWnd();
		static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
		LRESULT CALLBACK STW_WM_COMMAND_reaction(HWND, UINT, WPARAM, LPARAM);
		LRESULT CALLBACK STW_WM_KEYDOWN_reaction(HWND, UINT, WPARAM, LPARAM);
		LRESULT CALLBACK WM_COMMAND_reaction(HWND, UINT, WPARAM, LPARAM);
	#pragma endregion
		void InitCtrlInterface(HINSTANCE, WNDPROC, WNDPROC, WNDPROC);
		tagMAINWINDOW();
	}MAINWINDOW, *LPMAINWINDOW;
}
#endif