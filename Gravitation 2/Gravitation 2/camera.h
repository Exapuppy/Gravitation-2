
# ifndef _CAMERA_H
# define _CAMERA_H

# include "main_ogl.h"

class CVector3 
{
	public:
		float x, y, z;
		CVector3 ( ) { }
		CVector3 ( float X, float Y, float Z )
		{
			x = X;
			y = Y;
			z = Z;
		}
		CVector3 operator + ( CVector3 vVector )
		{
			return CVector3 ( vVector.x + x, vVector.y + y, vVector.z + z );
		}
		CVector3 operator - ( CVector3 vVector )
		{
			return CVector3 ( x - vVector.x, y - vVector.y, z - vVector.z );
		}
		CVector3 operator * ( float num )
		{
			return CVector3 ( x * num, y * num, z * num );
		}
		CVector3 operator / ( float num )
		{
			return CVector3 ( x / num, y / num, z / num );
		}		
};
class CCamera
{
	private:
		CVector3 m_vPosition;
		CVector3 m_vView;
		CVector3 m_vUpVector;
		CVector3 m_vStrafe;
	public:		
		CCamera ( );
		CVector3 Position ( )	{ return m_vPosition;	}
		CVector3 View ( )		{ return m_vView;		}
		CVector3 UpVector ( )	{ return m_vUpVector;	}
		CVector3 Strafe ( )		{ return m_vStrafe;		}
		void PositionCamera ( float PositionX, float PositionY, float PositionZ,
							float ViewX, float ViewY, float ViewZ,
							float upVectorX, float upVectorY, float upVectorZ );
		void StrafeCamera (	float speed );
		void StrafeCameray ( float speed );
		void MoveCamera ( float speed );
		void RotateView ( float angle, float x, float y, float z );
		void CheckForMovement ( GLfloat kSpeed );
		void Update ( GLfloat kSpeed, HWND hWnd );
		void Look ( );
		void SetViewByMouse ( HWND hWnd );
		void RotateAroundPoint ( CVector3 vCenter, float angle, float x, float y, float z );
};

# endif