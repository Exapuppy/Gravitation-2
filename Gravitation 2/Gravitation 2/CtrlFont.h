#include "stdafx.h"

namespace Font
{
#define MS_SANS_SERIF L"MS Sans Serif"
#define COURIER_NEW L"Courier New"
#define TIMES_NEW_ROMAN L"Times New Roman"
#define IRIS_UPS L"IrisUPS"

	typedef class tagFont
	{
	private:
		HFONT hFont;
		WCHAR * font_name;
		int Height;
	public:
		void set_Font( __in const WCHAR * font_name, __in int Height)
		{
			//strcpy
			register wchar_t * cp = this->font_name;
			while( *cp++ = *font_name++ );
			//
			this->Height = Height;
			hFont = CreateFont(	Height, 0, 0, 0, 500, 
								0, 0, 0, 1, 0, 
								CLIP_DEFAULT_PRECIS, PROOF_QUALITY,  
								FIXED_PITCH, font_name);
		};
		inline void set_Height(__in int Height)
		{
			this->Height = Height;
			hFont = CreateFont(	this->Height, 0, 0, 0, 500, 
								0, 0, 0, 1, 0, 
								CLIP_DEFAULT_PRECIS, PROOF_QUALITY,  
								FIXED_PITCH, this->font_name);
		};
		inline void set_to_Ctrl(HWND Ctrl)
		{
			SendMessage( Ctrl, WM_SETFONT, (WPARAM) hFont, 0 );
		};

		tagFont()
		{
			font_name = new WCHAR[20];
		};
		tagFont( __in const WCHAR * font_name, __in int Height)
		{
			this->font_name = new WCHAR[20];
			//strcpy
			register wchar_t * cp = this->font_name;
			while( *cp++ = *font_name++ );
			//
			this->Height = Height;
			hFont = CreateFont(	Height, 0, 0, 0, 500, 
								0, 0, 0, 1, 0, 
								CLIP_DEFAULT_PRECIS, PROOF_QUALITY,  
								FIXED_PITCH, font_name);
		};
		~tagFont()
		{
			delete font_name;
			DeleteObject((HGDIOBJ)hFont);
		}
	}CFONT, *LPCFONT;
};