
# ifndef _FONT_H
# define _FONT_H

# include "main_ogl.h"
# include <wchar.h>

# include "ftgl\FTFont.h"
# include "ftgl\FTGLBitmapFont.h"
# include "ftgl\FTGLOutlineFont.h"

class CFont
{
	public:
		CFont ( char *ttf, int FSize, int FDepth );
		~ CFont ( );
		FTFont *Font;
		void Print ( float x, float y, const wchar_t *text );
};

# endif