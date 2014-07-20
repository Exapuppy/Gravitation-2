	
# include "stdafx.h"
# include "glelements_and_texture.h"

void CTexture :: InitTextures ( CTexture *Texture )
{	
	Texture -> LoadTexture ( IL_BMP, "00.bmp", &textures [ 0 ] );
	Texture -> LoadTexture ( IL_BMP, "01.bmp", &textures [ 1 ] );
	Texture -> LoadTexture ( IL_BMP, "02.bmp", &textures [ 2 ] );
	Texture -> LoadTexture ( IL_BMP, "03.bmp", &textures [ 3 ] );

	Texture -> LoadTexture ( IL_BMP, "04.bmp", &textures [ 4 ] );
	Texture -> LoadTexture ( IL_BMP, "05.bmp", &textures [ 5 ] );
	Texture -> LoadTexture ( IL_BMP, "06.bmp", &textures [ 6 ] );
	Texture -> LoadTexture ( IL_BMP, "07.bmp", &textures [ 7 ] );

	Texture -> LoadTexture ( IL_BMP, "08.bmp", &textures [ 8 ] );
	Texture -> LoadTexture ( IL_BMP, "09.bmp", &textures [ 9 ] );
	Texture -> LoadTexture ( IL_BMP, "10.bmp", &textures [ 10 ] );
	Texture -> LoadTexture ( IL_BMP, "11.bmp", &textures [ 11 ] );
} 
void CTexture :: LoadTexture ( ILenum FileType, char *filename, TextureImage *texture )
{	
	ilLoad ( FileType, ( LPCWSTR ) filename );
 	
	int err=ilGetError ( );
	if ( err != IL_NO_ERROR )
	{
		const wchar_t* strError = iluErrorString(err);
		MessageBox ( NULL, (LPCWSTR)strError, L"Ошибка при загрузке!", MB_OK );
		exit ( 1 );
	} 
	
	texture->width = ilGetInteger(IL_IMAGE_WIDTH);			// Ширина
	texture->height = ilGetInteger(IL_IMAGE_HEIGHT);		// Высота
	texture->bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);	// Байт на пиксель
 
	// Загружаем данные в нашу текстуру
	texture->imageData = ilGetData();
 
	ilEnable(IL_CONV_PAL);
 
	// Тип данных изображения
	unsigned int type = ilGetInteger(IL_IMAGE_FORMAT);
 
	// Генерируем текстуру
	glGenTextures(1, &texture->texID);
 
	// Привязываем данные текстуры к ID
	glBindTexture(GL_TEXTURE_2D,texture->texID);
 
	// биндим мип-мапы
	gluBuild2DMipmaps(GL_TEXTURE_2D, texture->bpp, texture->width,
		texture->height, type, GL_UNSIGNED_BYTE, texture->imageData);
 
	// Устанавливаем качество текстур
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}
void GLelements :: DrawQ ( CTexture *Texture, int TEXT_ID, GLfloat sizePlanet )
{	
	glBindTexture ( GL_TEXTURE_2D, Texture -> textures [ TEXT_ID ].texID );
	qobj = gluNewQuadric ( );
	glPushName ( TEXT_ID );
		gluQuadricTexture ( qobj, GL_TRUE );
		gluQuadricDrawStyle ( qobj, GLU_FILL );			
			//glRotated ( -90, 1, 0, 0 );
			gluSphere ( qobj, sizePlanet, 32, 32 );
			//glRotated ( 90, 1, 0, 0 );
	glPopName ( );
		gluDeleteQuadric ( qobj );
}
void GLelements :: Traectory ( LPELLIPSES traect )
{	
	glPushMatrix ( );	
	//glEnable ( GL_LINE_SMOOTH );	
	for ( int i = 0; i < 9; i++ )
	{			
		glBegin ( GL_LINE_STRIP );
			glColor3ub ( 40, 197, 255 );
			for ( int j = 0; j < traect -> Size [ i ]; j++ )
				glVertex3f ( traect->El[i][j].y*1e-8, traect->El[i][j].x*1e-8, traect->El[i][j].z*1e-8 );
		glEnd ( );
	}
	glColor3ub ( 255, 255, 255 );
	//glDisable ( GL_LINE_SMOOTH );	
}
void GLelements :: GenParticles ( bool &PointFlag )
{		
	srand ( time(NULL) );
	glPushMatrix ( );	
	//glEnable ( GL_POINT_SMOOTH );
	
		glRotated ( rotParticles, 1, 0, 1 );
		rotParticles += 0.0025f;
	
	glBegin ( GL_POINTS );	
	for(int i=0; i<5000; i++)
	{
		if ( PointFlag )
		{
			Point.pointX [ i ] = rand()%-400 - 200;
			Point.pointY [ i ] = rand()%-400 - 200;
			Point.pointZ [ i ] = rand()%-400 - 200;			
		}
		glColor3ub ( 255, 255, 255 );
		glVertex3f ( Point.pointX [ i ], Point.pointY [ i ], Point.pointZ [ i ] );
	}
	if ( PointFlag )
		PointFlag = !PointFlag;
	glEnd ( );
	glColor3ub ( 255, 255, 255 );
	//glDisable ( GL_POINT_SMOOTH );
	glPopMatrix ( );	
}