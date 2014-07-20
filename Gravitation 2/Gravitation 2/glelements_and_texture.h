
# ifndef _GLELEMENTS_AND_TEXTURE_H
# define _GLELEMENTS_AND_TEXTURE_H

# include "main_ogl.h"
# include "Physics.h"

# include "devil\include\IL\il.h"
# include "devil\include\IL\ilu.h"

struct TextureImage
{
	GLubyte		*imageData;		// Данные текстуры
	GLuint		bpp;			// Байт на пиксел
	GLuint		width,height;	// Высота и ширина
	GLuint		texID;			// ID текстуры
};
class CTexture
{
	public:
		TextureImage textures [ 12 ];
		void InitTextures ( CTexture *Texture );
		CTexture ( )
		{	
			ilInit ( );
			iluInit ( );
		}
		~CTexture ( ) { }
		void LoadTexture ( ILenum FileType, char *filename, TextureImage *texture );
		void FreeTexture ( TextureImage *texture ) { }
};
struct PointMass
{
	int pointX [ 5000 ];
	int pointY [ 5000 ];
	int pointZ [ 5000 ];
};
class GLelements
{	
	private:
		PointMass Point;
		GLfloat rotParticles;
	public:	
		GLUquadricObj *qobj;
		GLelements ( )
		{
			rotParticles = 0.0f;
		}
		void DrawQ ( CTexture *Texture, int TEXT_ID, GLfloat sizePlanet );	
		void Traectory ( LPELLIPSES traect );
		void GenParticles ( bool &PointFlag );
};

# endif