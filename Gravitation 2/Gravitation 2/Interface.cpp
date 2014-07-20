#include "stdafx.h"
#include "Interface.h"

#ifdef INTERFACE_H

#pragma comment(lib,"comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace Interface
{
	inline POINT get_cursor_pos(HWND hWnd)
	{
	
		register POINT cp;
		register RECT r;
		GetClientRect(hWnd, &r);
		GetCursorPos(&cp);
		cp.x -= r.left;
		cp.y -= r.top;
		return cp;
	}
	tagMAINWINDOW::tagMAINWINDOW()
	{
		SetNewDate = stwy = stwm = stwd = stwh = false;
		pary = parm = pard = parh = 0;
		ZeroMemory(KeyFocusSTW,sizeof(KeyFocusSTW));
		GoParam = true;
	}
	//инициализация контролов
	void tagMAINWINDOW::InitCtrlInterface(HINSTANCE hInst, WNDPROC WndProc, WNDPROC WndProc_OGL, WNDPROC WndProc_STW)
	{
	#pragma region Main Window Generate
		TCHAR szWindowClass[100];
		WNDCLASSEX wcex = {0};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.lpfnWndProc	= WndProc;
		wcex.hInstance		= hInst;
		wcex.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_GRAVITATION2));
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= CreateSolidBrush(RGB(192,192,192));
		wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GRAVITATION2);
		wcex.lpszClassName	= szWindowClass;
		wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
		RegisterClassEx(&wcex);
		hWnd = CreateWindow(szWindowClass, L"Gravitation 2: Solar System", WS_OVERLAPPEDWINDOW, 
			 (int)(GetSystemMetrics(SM_CXSCREEN) / 2.0f) - 500, (int)(GetSystemMetrics(SM_CYSCREEN) / 2.0f) - 300, 1000, 600, NULL, NULL, hInst, NULL);
		hdc = GetDC(hWnd);
		//Window size
		RECT rt;
		GetClientRect (hWnd, &rt);
		maxX = rt.right, maxY = rt.bottom;
		FmaxX = maxX; FmaxY = maxY;
	#pragma endregion
	#pragma region OGL Generate
		WNDCLASS wndclass;
		memset ( &wndclass, 0, sizeof ( WNDCLASS ) );
		wndclass.style			= CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc	= WndProc_OGL;
		wndclass.hbrBackground	= CreateSolidBrush(RGB(255,255,255));
		wndclass.lpszClassName	= L"OGLWNDCL";
		wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		RegisterClass ( &wndclass );

		OGLhWnd = CreateWindow( L"OGLWNDCL", NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE, 
								0, 0, maxX - 230, maxY,	hWnd, (HMENU) OGL_ID, hInst, NULL);
	#pragma endregion
	#pragma region Controls Generate
		#pragma region time system
		set_Font( TIMES_NEW_ROMAN, 48);
		StYearNum =  CreateWindow(L"static",NULL,	WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY,	maxX - 215, 45, 200, 46,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(StYearNum);
		set_Font( TIMES_NEW_ROMAN, 42);
			StMonthNum = CreateWindow(L"static",NULL,	WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY,		maxX - 185, 130, 39, 40,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(StMonthNum);
				dv2   =	CreateWindow(L"static",L":",	WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY,		maxX - 145, 130,  9, 40,	hWnd,(HMENU)0,hInst,NULL);
				setFontToC(dv2);
			StDayNum  =	CreateWindow(L"static",NULL,	WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY,		maxX - 135, 130, 39, 40,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(StDayNum);
				dv1   =	CreateWindow(L"static",L":",	WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY,		maxX - 95, 130,  9, 40,	hWnd,(HMENU)0,hInst,NULL);
				setFontToC(dv1);
			StHourNum = CreateWindow(L"static",NULL,	WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY,		maxX - 85, 130, 36, 40,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(StHourNum);
		set_Font( TIMES_NEW_ROMAN, 14);
			StYearText = CreateWindow(L"static",L"год",	WS_CHILD|WS_VISIBLE|SS_CENTER,	maxX - 130, 95, 80, 20,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(StYearText);
			StMonthDayHourText = CreateWindow(L"static",L"месяц         день           час",	WS_CHILD|WS_VISIBLE|SS_CENTER,	maxX - 220, 175, 200, 20,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(StMonthDayHourText);
		#pragma endregion
		#pragma region SetTimeWindow
			WNDCLASS wc = {0};
			wc.lpszClassName = L"logo";
			wc.lpfnWndProc = WndProc_STW;
			wc.hCursor	= LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_BACKGROUND+11);
			wc.hIcon	= LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
			RegisterClass(&wc);


			SetTimeWindow = CreateWindow(L"logo",NULL,WS_POPUPWINDOW, 0,0,131,58, NULL,NULL,hInst,NULL);
		
			STW_Ok = CreateWindow(L"button",L"Ok",	WS_CHILD|WS_CLIPSIBLINGS|BS_PUSHBUTTON|WS_VISIBLE,4,40,55,15,SetTimeWindow,(HMENU)STW_Ok_ID,hInst,NULL);
				set_Font( TIMES_NEW_ROMAN, 12);
				setFontToC(STW_Ok);
			STW_Cn = CreateWindow(L"button",L"Cancel",	WS_CHILD|WS_CLIPSIBLINGS|BS_PUSHBUTTON|WS_VISIBLE,67,40,55,15,SetTimeWindow,(HMENU)STW_Cn_ID,hInst,NULL);
				setFontToC(STW_Cn);

			STWY = CreateWindow(L"edit",L"Год",WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_BORDER,	3,3,126,15,	SetTimeWindow,(HMENU)STW_Y_ID, hInst, NULL);
				setFontToC(STWY);
			STWM = CreateWindow(L"edit",L"Месяц",WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_BORDER,	3,20,39,15,	SetTimeWindow,(HMENU)STW_M_ID, hInst, NULL);
				setFontToC(STWM);
			STWD = CreateWindow(L"edit",L"День",WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_BORDER,	45,20,39,15,	SetTimeWindow,(HMENU)STW_D_ID, hInst, NULL);
				setFontToC(STWD);
			STWH = CreateWindow(L"edit",L"Час",WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_BORDER,	88,20,39,15,	SetTimeWindow,(HMENU)STW_H_ID, hInst, NULL);
				setFontToC(STWH);
		#pragma endregion
		#pragma region time speed
		set_Font( TIMES_NEW_ROMAN, 24);
			StTimeSpeedNum =  CreateWindow(L"static",L"1.8e+002 х",	WS_CHILD|WS_VISIBLE|SS_CENTER,	maxX - 160, 240, 90, 24,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(StTimeSpeedNum);
		set_Font( TIMES_NEW_ROMAN, 16);
			BTimeSpeedFast =  CreateWindow(	L"button",L">",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,maxX - 60, 240, 20, 24, hWnd,(HMENU)TSR_ID,hInst,NULL);
			setFontToC(BTimeSpeedFast);
			BTimeSpeedSlow =  CreateWindow(	L"button",L"<",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,maxX - 186, 240, 20, 24, hWnd,(HMENU)TSL_ID,hInst,NULL);
			setFontToC(BTimeSpeedSlow);
		set_Font( TIMES_NEW_ROMAN, 14);
			StTimeSpeedName =  CreateWindow(L"static",L"Скорость течения времени:",	WS_CHILD|WS_VISIBLE|SS_CENTER,	maxX - 200, 220, 170, 20,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(StTimeSpeedName);
		set_Font( TIMES_NEW_ROMAN, 16);
			BPause =  CreateWindow(	L"button",L"Пауза",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,maxX - 163, 270, 100, 25, hWnd,(HMENU)BPause_ID,hInst,NULL);
			setFontToC(BPause);
		#pragma endregion
		#pragma region GoToObject
		set_Font( TIMES_NEW_ROMAN, 14);
			GoToObjName =  CreateWindow(L"static",L"Перейти к:",	WS_CHILD|WS_VISIBLE|SS_CENTER,	maxX - 160, 320, 75, 15,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(GoToObjName);
			CBGoToObj = CreateWindow(L"combobox",NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_VSCROLL|CBS_DROPDOWNLIST|CBS_AUTOHSCROLL,	maxX - 160, 340, 85, 25,	hWnd,(HMENU)CBGoToObj_ID,hInst,NULL);
			setFontToC(CBGoToObj);
			SendMessage(CBGoToObj, CB_ADDSTRING, 0, (LPARAM)L"Меркурий");
			SendMessage(CBGoToObj, CB_ADDSTRING, 0, (LPARAM)L"Венера");
			SendMessage(CBGoToObj, CB_ADDSTRING, 0, (LPARAM)L"Земля");
			SendMessage(CBGoToObj, CB_ADDSTRING, 0, (LPARAM)L"Марс");
			SendMessage(CBGoToObj, CB_ADDSTRING, 0, (LPARAM)L"Юпитер");
			SendMessage(CBGoToObj, CB_ADDSTRING, 0, (LPARAM)L"Сатурн");
			SendMessage(CBGoToObj, CB_ADDSTRING, 0, (LPARAM)L"Уран");
			SendMessage(CBGoToObj, CB_ADDSTRING, 0, (LPARAM)L"Нептун");
			SendMessage(CBGoToObj, CB_ADDSTRING, 0, (LPARAM)L"Плутон");
		#pragma endregion
		#pragma region PlanetSize
		set_Font( TIMES_NEW_ROMAN, 24);
			PlanetSizeNum =  CreateWindow(L"static",L"100х",	WS_CHILD|WS_VISIBLE|SS_CENTER,	maxX - 160, 400, 90, 24,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(PlanetSizeNum);
		set_Font( TIMES_NEW_ROMAN, 16);
			BPlSzUp =  CreateWindow(	L"button",L">",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,maxX - 60, 400, 20, 24, hWnd,(HMENU)BPlSzUp_ID,hInst,NULL);
			setFontToC(BPlSzUp);
			BPlSzDn =  CreateWindow(	L"button",L"<",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,maxX - 186, 400, 20, 24, hWnd,(HMENU)BPlSzDn_ID,hInst,NULL);
			setFontToC(BPlSzDn);
		//set_Font( TIMES_NEW_ROMAN, 16);
			PlanetSizeName =  CreateWindow(L"static",L"Размер планет:",	WS_CHILD|WS_VISIBLE|SS_CENTER,	maxX - 200, 380, 170, 20,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(PlanetSizeName);
		#pragma endregion
		#pragma region KamSpeed
		set_Font( TIMES_NEW_ROMAN, 24);
			KamSpeedNum =  CreateWindow(L"static",L"1х",	WS_CHILD|WS_VISIBLE|SS_CENTER,	maxX - 160, 460, 90, 24,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(KamSpeedNum);
		set_Font( TIMES_NEW_ROMAN, 16);
			BKmSpUp =  CreateWindow(	L"button",L">",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,maxX - 60, 460, 20, 24, hWnd,(HMENU)BKmSpUp_ID,hInst,NULL);
			setFontToC(BKmSpUp);
			BKmSpDn =  CreateWindow(	L"button",L"<",		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,maxX - 186, 460, 20, 24, hWnd,(HMENU)BKmSpDn_ID,hInst,NULL);
			setFontToC(BKmSpDn);
		//set_Font( TIMES_NEW_ROMAN, 16);
			KamSpeedName =  CreateWindow(L"static",L"Скорость камеры:",	WS_CHILD|WS_VISIBLE|SS_CENTER,	maxX - 200, 440, 170, 20,	hWnd,(HMENU)0,hInst,NULL);
			setFontToC(KamSpeedName);
		#pragma endregion
	#pragma endregion
	}
	//установка системного времени
	void tagMAINWINDOW::SetSysTime(long Year, long Month, long Day, long Hour)
	{
		_itow_s(Year, lpch, 10);
		SetWindowText(StYearNum, lpch);
		_itow_s(Month, lpch, 10);
		if(Month<10){lpch[1]=lpch[0];lpch[0]=L'0';lpch[2]=L'\0';}
		SetWindowText(StMonthNum, lpch);
		_itow_s(Day, lpch, 10);
		if(Day<10){lpch[1]=lpch[0];lpch[0]='0';lpch[2]=L'\0';}
		SetWindowText(StDayNum, lpch);
		_itow_s(Hour, lpch, 10);
		if(Hour<10){lpch[1]=lpch[0];lpch[0]=L'0';lpch[2]=L'\0';}
		SetWindowText(StHourNum, lpch);
	}
	//вызывается при изменении размеров главного окна
	void tagMAINWINDOW::MoveAllWnd()
	{
		RECT rt;
		int maxX, maxY;
		GetClientRect (hWnd, &rt);
		maxX = rt.right, maxY = rt.bottom;

		MoveWindow(OGLhWnd, 0, 0,maxX - 230, maxY,	true);
		#pragma region time system
		MoveWindow(StYearNum,	maxX - 215, 45, 200, 46, true);
		MoveWindow(StMonthNum,	maxX - 185, 130, 39, 40, true);
			MoveWindow(dv2,		maxX - 145, 130,  9, 40, true);
		MoveWindow(StDayNum,	maxX - 135, 130, 39, 40, true);
			MoveWindow(dv1,		maxX - 95, 130,   9, 40, true);
		MoveWindow(StHourNum,	maxX - 85, 130,  36, 40, true);
		MoveWindow(StYearText,	maxX - 130, 95, 80, 20, true);
		MoveWindow(StMonthDayHourText,	maxX - 220, 175, 200, 20, true);
		#pragma endregion
		#pragma region time speed
		MoveWindow(StTimeSpeedName,	maxX - 200, 220, 170, 20, true);
		MoveWindow(BTimeSpeedFast,	maxX - 60, 240, 20, 24, true);
		MoveWindow(BTimeSpeedSlow,	maxX - 186, 240, 20, 24, true);
		MoveWindow(StTimeSpeedNum,	maxX - 160, 240, 90, 24, true);
		MoveWindow(BPause,	maxX - 163, 270, 100, 25, true);		
		#pragma endregion
		#pragma region GoToObject
		MoveWindow(GoToObjName, maxX - 160, 320, 75, 15,	true);
		MoveWindow(CBGoToObj, maxX - 160, 340, 85, 25,	true);
		#pragma endregion
		#pragma region PlanetSize
		MoveWindow(PlanetSizeNum,	maxX - 160, 390, 90, 24, true);
		MoveWindow(BPlSzUp,	maxX - 60, 390, 20, 24, true);
		MoveWindow(BPlSzDn,	maxX - 186, 390, 20, 24, true);
		MoveWindow(PlanetSizeName,	maxX - 200, 370, 170, 20, true);	
		#pragma endregion
		#pragma region KamSpeed
		MoveWindow(KamSpeedNum,	maxX - 160, 440, 90, 24, true);
		MoveWindow(BKmSpUp,	maxX - 60, 440, 20, 24, true);
		MoveWindow(BKmSpDn,	maxX - 186, 440, 20, 24, true);
		MoveWindow(KamSpeedName,	maxX - 200, 420, 170, 20, true);	
		#pragma endregion
	}
	INT_PTR CALLBACK tagMAINWINDOW::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (message)
		{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}
		return (INT_PTR)FALSE;
	}
	// реакция окна ввода даты на сообщение WM_COMMAND
	LRESULT CALLBACK tagMAINWINDOW::STW_WM_COMMAND_reaction(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		int wmId    = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		switch (wmId)
		{
////////////
		case STW_Ok_ID:
			CloseWindow(hWnd);
			if(stwy)	SetWindowText(this->StYearNum,	lpchy);
			if(stwm)	SetWindowText(this->StMonthNum,	lpchm);
			if(stwd)	SetWindowText(this->StDayNum,	lpchd);
			if(stwh)	SetWindowText(this->StHourNum,	lpchh);
			SetNewDate = true;
			ZeroMemory(KeyFocusSTW,sizeof(KeyFocusSTW));
			break;
		case STW_Cn_ID:
			pary = parm = pard = parh = 0;
			CloseWindow(hWnd);
			ZeroMemory(KeyFocusSTW,sizeof(KeyFocusSTW));
			break;
////////////
		case STW_Y_ID:
			switch(wmEvent)
			{
			case EN_SETFOCUS:
				GetWindowText((HWND)lParam, lpchy, 20);
				for(lpchiter = lpchy;	*lpchiter > 47 && *lpchiter < 58 ;	lpchiter ++);
				*lpchiter = L'\0';
				SetWindowText ((HWND)lParam, lpchy);
				KeyFocusSTW[0] = true;
				break;
			case EN_KILLFOCUS:
				GetWindowText((HWND)lParam, lpchy, 20);
				for(lpchiter = lpchy;	*lpchiter > 47 && *lpchiter < 58 ;	lpchiter ++);
				*lpchiter = L'\0';
				pary = _wtoi(lpchy);
				stwy = true;
				swprintf_s(lpch, L"%d год", pary);
				SetWindowText((HWND)lParam, lpch);
				KeyFocusSTW[0] = false;
				break;
			case EN_CHANGE:
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case STW_M_ID:
			switch(wmEvent)
			{
			case EN_SETFOCUS:
				GetWindowText((HWND)lParam,lpchm,20);
				for(lpchiter=lpchm;	*lpchiter>47 && *lpchiter<58 ;	lpchiter++);
				*lpchiter=L'\0';
				SetWindowText((HWND)lParam,lpchm);
				KeyFocusSTW[1] = true;
				break;
			case EN_KILLFOCUS:
				GetWindowText((HWND)lParam,lpchm,20);
				for(lpchiter=lpchm;	*lpchiter>47 && *lpchiter<58 ;	lpchiter++);
				*lpchiter=L'\0';
				parm = _wtoi(lpchm);
				stwm = true;
				swprintf_s(lpch,L"%d мес",parm);
				SetWindowText((HWND)lParam,lpch);
				KeyFocusSTW[1] = false;
				break;
			case EN_CHANGE:
				GetWindowText((HWND)lParam,lpchm,20);
				for(lpchiter=lpchm;	*lpchiter>47 && *lpchiter<58 ;	lpchiter++);
				*lpchiter=L'\0';
				parm = _wtoi(lpchm);
				if(parm>12)	SetWindowText((HWND)lParam,L"12");
				if(parm<1)	SetWindowText((HWND)lParam,L"01");
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case STW_D_ID:
			switch(wmEvent)
			{
			case EN_SETFOCUS:
				GetWindowText((HWND)lParam,lpchd,20);
				for(lpchiter=lpchd;	*lpchiter>47 && *lpchiter<58 ;	lpchiter++);
				*lpchiter=L'\0';
				SetWindowText((HWND)lParam,lpchd);
				KeyFocusSTW[2] = true;
				break;
			case EN_KILLFOCUS:
				GetWindowText((HWND)lParam,lpchd,20);
				for(lpchiter=lpchd;	*lpchiter>47 && *lpchiter<58 ;	lpchiter++);
				*lpchiter=L'\0';
				pard= _wtoi(lpchd);
				stwd = true;
				swprintf_s(lpch,L"%d день",pard);
				SetWindowText((HWND)lParam,lpch);
				KeyFocusSTW[2] = false;
				break;
			case EN_CHANGE:
				GetWindowText((HWND)lParam,lpchd,20);
				for(lpchiter=lpchd;	*lpchiter>47 && *lpchiter<58 ;	lpchiter++);
				*lpchiter=L'\0';
				pard = _wtoi(lpchd);
				if(pard>31)SetWindowText((HWND)lParam,L"31");
				if(pard<1)SetWindowText((HWND)lParam,L"01");
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case STW_H_ID:
			switch(wmEvent)
			{
			case EN_SETFOCUS:
				GetWindowText((HWND)lParam,lpchh,20);
				for(lpchiter=lpchh;	*lpchiter>47 && *lpchiter<58;	lpchiter++);
				*lpchiter=L'\0';
				SetWindowText((HWND)lParam,lpchh);
				KeyFocusSTW[3] = true;
				break;
			case EN_KILLFOCUS:
				GetWindowText((HWND)lParam,lpchh,20);
				for(lpchiter=lpchh;	*lpchiter>47 && *lpchiter<58 ;	lpchiter++);
				*lpchiter=L'\0';
				parh = _wtoi(lpchh);
				stwh = true;
				swprintf_s(lpch,L"%d час",parh);
				SetWindowText((HWND)lParam,lpch);
				KeyFocusSTW[3] = false;
				break;
			case EN_CHANGE:
				GetWindowText((HWND)lParam,lpchd,20);
				for(lpchiter=lpchd;	*lpchiter>47 && *lpchiter<58 ;	lpchiter++);
				*lpchiter=L'\0';
				pard = _wtoi(lpchd);
				if(pard>23)SetWindowText((HWND)lParam,L"23");
				if(pard<0)SetWindowText((HWND)lParam,L"00");
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
///////////
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
	LRESULT CALLBACK tagMAINWINDOW::WM_COMMAND_reaction(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		int wmId    = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case STN_CLICKED:
			SetWindowPos(SetTimeWindow,NULL,get_cursor_pos(hWnd).x,get_cursor_pos(hWnd).y,0,0,SWP_NOSIZE);
			UpdateWindow(SetTimeWindow);
			ShowWindow	(SetTimeWindow,1);
			SetFocus	(STWY);
			break;
		case BPause_ID:
			GoParam = !GoParam;
			if(GoParam)
				SetWindowText(BPause,L"Пауза");
			else
				SetWindowText(BPause,L"Продолжить");
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}

#endif