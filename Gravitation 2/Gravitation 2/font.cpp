
# include "stdafx.h"
# include "font.h"

CFont :: CFont ( char *ttf, int FSize, int FDepth )
{
	this -> Font = new FTGLBitmapFont ( ttf );

	if ( !Font -> FaceSize ( FSize ) )
	{
		MessageBox ( NULL, L"Can't set font FaceSize", L"Error", MB_OK );
		exit ( 1 );
	}

	Font -> Depth ( (float)FDepth );
	Font -> CharMap ( ft_encoding_unicode );
}

void CFont :: Print ( float x, float y, const wchar_t *text )
{
	glPushMatrix ( );
	glLoadIdentity ( );
		glDisable ( GL_TEXTURE_2D );
			glTranslatef ( x, y, -1 );
			glRasterPos2f ( -1, 0.5 );
			Font -> Render ( text );
		glEnable ( GL_TEXTURE_2D );
	glPopMatrix ( );
}