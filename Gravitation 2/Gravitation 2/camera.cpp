# include "stdafx.h"
# include "camera.h"

float g_FrameInterval = 0.0f;

void GetFrameTime ( )
{
	static float frameTime = 0.0f;

	float currentTime = timeGetTime ( ) * 0.001f;

	g_FrameInterval = currentTime - frameTime;
	frameTime = currentTime;
}
CVector3 Cross ( CVector3 vVector1, CVector3 vVector2 )
{
	CVector3 vNormal;

	vNormal.x = ( ( vVector1.y * vVector2.z ) - ( vVector1.z * vVector2.y ) );
	vNormal.y = ( ( vVector1.z * vVector2.x ) - ( vVector1.x * vVector2.z ) );
	vNormal.z = ( ( vVector1.x * vVector2.y ) - ( vVector1.y * vVector2.x ) );

	return vNormal;
}
float Magnitude ( CVector3 vNormal )
{
	return ( float ) sqrt ( ( vNormal.x * vNormal.x) + ( vNormal.y * vNormal.y ) + ( vNormal.z * vNormal.z ) );
}
CVector3 Normalize ( CVector3 vVector )
{
	float magnitude = Magnitude ( vVector );

	vVector = vVector / magnitude;

	return vVector;
}
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
void CCamera :: SetViewByMouse ( HWND hWnd )
{
	register POINT mousePos; 
	register RECT rt;
	GetWindowRect ( hWnd, &rt );
	int middleX = ( rt.left + rt.right ) >> 1;
	int middleY = ( rt.top + rt.bottom ) >> 1;
	float angleY= 0.0f;
	float angleZ = 0.0f;
	static float currentRotX = 0.0f;

	mousePos = get_cursor_pos ( hWnd );

	if ( ( mousePos.x == middleX) && ( mousePos.y == middleY ) )
		return;

	SetCursorPos ( middleX, middleY );

	angleY = ( float ) ( ( middleX - mousePos.x ) ) / 1000.0f;
	angleZ = ( float ) ( ( middleY - mousePos.y ) ) / 1000.0f;

	static float lastRotX = 0.0f;
	lastRotX = currentRotX;

	if ( currentRotX > 1.0f )
	{
		currentRotX = 1.0f;
		if ( lastRotX != 1.0f )
		{
			CVector3 vAxis = Cross ( m_vView - m_vPosition, m_vUpVector );
			vAxis = Normalize ( vAxis );
			RotateView ( 1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z );
		}
	}
	else	
	{
		if ( currentRotX < -1.0f )
		{
			currentRotX = -1.0f;
			if ( lastRotX != -1.0f )
			{
				CVector3 vAxis = Cross ( m_vView - m_vPosition, m_vUpVector );
				vAxis = Normalize ( vAxis );
				RotateView ( -1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z );
			}
		}
		else
		{
			CVector3 vAxis = Cross ( m_vView - m_vPosition, m_vUpVector );
			vAxis = Normalize ( vAxis );
			RotateView ( angleZ, vAxis.x, vAxis.y, vAxis.z );
		}
	}

	RotateView ( angleY, 0, 1, 0 );
}
void CCamera :: StrafeCamera ( float speed )
{
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}
void CCamera :: CheckForMovement ( GLfloat kSpeed)
{
	float speed = kSpeed * g_FrameInterval;

	if ( ( GetKeyState ( VK_UP ) & 0x80 ) || ( GetKeyState ( 'W' ) & 0x80 ) )
		MoveCamera ( speed );
	if ( ( GetKeyState ( VK_DOWN ) & 0x80 ) || ( GetKeyState ( 'S' ) & 0x80 ) )
		MoveCamera ( - speed );
	if ( ( GetKeyState ( VK_LEFT ) & 0x80 ) || ( GetKeyState ( 'A' ) & 0x80 ) )
		StrafeCamera (-speed);
	if ( ( GetKeyState ( VK_RIGHT ) & 0x80 ) || ( GetKeyState ( 'D' ) & 0x80 ) )
		StrafeCamera (speed);
}
void CCamera :: Update ( GLfloat kSpeed, HWND hWnd )
{
	CVector3 vCross = Cross ( m_vView - m_vPosition, m_vUpVector );

	m_vStrafe = Normalize ( vCross );

	SetViewByMouse ( hWnd );

	CheckForMovement ( kSpeed );

	GetFrameTime ( );
}
void CCamera :: Look ( )
{
	gluLookAt ( m_vPosition.x,	m_vPosition.y,	m_vPosition.z,
				m_vView.x,		m_vView.y,		m_vView.z,
				m_vUpVector.x,	m_vUpVector.y,	m_vUpVector.z);
}
CCamera :: CCamera ( )
{
	CVector3 vZero = CVector3 ( 0.0, 0.0, 0.0 );
	CVector3 vView = CVector3 ( 0.0, 1.0, 0.5 );
	CVector3 vUp   = CVector3 ( 0.0, 0.0, 1.0 );

	m_vPosition = vZero;
	m_vView = vView;
	m_vUpVector = vUp;
}
GLvoid CCamera :: PositionCamera ( float PositionX, float PositionY, float PositionZ,
								   float ViewX, float ViewY, float ViewZ,
								   float upVectorX, float upVectorY, float upVectorZ )
{
	CVector3 vPosition	= CVector3 (  PositionX, PositionY, PositionZ );
	CVector3 vView		= CVector3 (  ViewX, ViewY, ViewZ );
	CVector3 vUpVector	= CVector3 (  upVectorX, upVectorY, upVectorZ );

	m_vPosition = vPosition;
	m_vView = vView;
	m_vUpVector = vUpVector;
}
void CCamera :: RotateView ( float angle, float x, float y, float z )
{
	CVector3 vNewView;
	CVector3 vView;

	vView.x = m_vView.x - m_vPosition.x;
	vView.y = m_vView.y - m_vPosition.y;
	vView.z = m_vView.z - m_vPosition.z;

	float cosTheta = ( float ) cos ( angle );
	float sinTheta = ( float ) sin ( angle );

	vNewView.x	 = ( cosTheta + ( 1 - cosTheta ) * x * x ) * vView.x;
	vNewView.x	+= ( ( 1 - cosTheta ) * x * y - z * sinTheta ) * vView.y;
	vNewView.x	+= ( ( 1 - cosTheta ) * x * z + y * sinTheta ) * vView.z;

	vNewView.y	= ( ( 1 - cosTheta ) * x * y + z * sinTheta ) * vView.x;
	vNewView.y += ( cosTheta + ( 1 - cosTheta ) * y * y ) * vView.y;
	vNewView.y += ( ( 1 - cosTheta ) * y * z - x * sinTheta ) * vView.z;

	vNewView.z	= ( ( 1 - cosTheta ) * x * z - y * sinTheta ) * vView.x;
	vNewView.z += ( ( 1 - cosTheta ) * y * z + x * sinTheta ) * vView.y;
	vNewView.z += ( cosTheta + ( 1 - cosTheta ) * z * z ) * vView.z;

	m_vView.x = m_vPosition.x + vNewView.x;
	m_vView.y = m_vPosition.y + vNewView.y;
	m_vView.z = m_vPosition.z + vNewView.z;
}
void CCamera :: MoveCamera ( float speed )
{
	CVector3 vVector = m_vView - m_vPosition;	
	
	vVector = Normalize ( vVector );
	
	m_vPosition.x += vVector.x * speed;
	m_vPosition.y += vVector.y * speed;
	m_vPosition.z += vVector.z * speed;
	m_vView.x += vVector.x * speed;
	m_vView.y += vVector.y * speed;
	m_vView.z += vVector.z * speed;
}