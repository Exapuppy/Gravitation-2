#ifndef PHYSIC_H
#define PHYSIC_H

namespace Physics
{
	#define _USE_MATH_DEFINES
	#include <math.h>

	#define G 0.0000000000667259
	#define	LENGTH 72000
	#define h 1e-2
	typedef class Vector 
	{
	public:
		float x, y, z;	
		
		inline float norm();
		// ����� ��������
		inline float norm_xy();
		inline float norm_xz();
		inline float norm_yz();
	}VECTOR,*LPVECTOR;

	// �����, ���������� ������ ���� 8�� ������,
	typedef class Ellipses
	{
	public:
		VECTOR El[9][LENGTH];
		int Size[9];

		Ellipses();
		void push(int PlanetNum, VECTOR c);
		bool full(int PlanetNum);
		void reset();
	}ELLIPSES,*LPELLIPSES;

	typedef class Planet
	{
	public:
		// ������ ���������� ����� �����o� ���a � ������
		VECTOR r;//km.
		// ������ ��������
		VECTOR v;//km/days.
	}PLANET, *LPPLANET;

	//��������� ��������� ������� � ������������ ������ �������
	typedef struct SystemState
	{
		PLANET planets[9];
	}SYSSTATE,*LPSYSSTATE;

	// �������� �����, ���������� ��������� ������� � ������� ������ � ������ ���������� ����������, ��������, ������� �����.
	typedef class System 
	{
	private:
	// ������ ��������� �������
		bool BufferIndex;
		LPSYSSTATE buf1;
		LPSYSSTATE buf2;
	//
	// ���������� �������
		Vector Sun_coord;
		inline Vector F1( Vector r, float M1, float M2, Vector rO);
		void F_Ailer (int PlanetNum, int ObjectNum);
		void F_Ailer_for_Sun (int PlanetNum);
	//
	// ��������� ������
		float PtMass[10];
		int fi[9];
		float v[9];
		float sl[9];
		float p[9];
	//
	// ������ ��������� ������
		//������ ���������� ��������� �������
		void SysStep_El();//�� ������ ��������� ������
	//
	// ��������� ����
		// ��������� ���
		double delta_t;//sec.
		// ��������� ����, �� ������� ������������ ������� ������ ���� �� 0.1 �������.
		double _01grad_t[9];
	//
	// ������ ����������
		// ��������� � traect2 ������� ���������� ������.
		// ����� ������� ���������� ������������� �������� ���������� ������ ��������.
		void SetCurrentState();
	//	
	public:
	// ������ ����������
		// ������� ������
		float PtRad[10];
		//
		// ������������ ������ � ������� ��������� ������. �������� �� � traect.
		void SetStandartOrbits();
	//
	// ���������� �����
		double t;//hours.
		long year, month, day, hour;
		//�������, ������� ������������� � ������� �������� �� �����
		void SetSysTime(long year, long month, long day, long hour);
		double timespeed;
	//
	// ������� �������� ������� � 4 ��������� - ���, �����,���� � ���.
		void CalculateTimeToDate();
	//
	// ������ ��� �����������
		//����������, ������������ � ������� ��������� ������.
		ELLIPSES traect;
	//
	// ����������� � ����������
		//������������ ��������� � ������������ ������
		System();
		~System();
	//
	// �������, ������� ���������� � �������� ����� (MainLoop)
		//����������� ����� ��������� ������, ������� �� �������� � ������� ����������.
		//����������� ����� � ��������� ��� �� ���� ��� ������ � ����
		void SystemStep();
	//
	// ������������� ������� ���, ��� ��� ���� 1.1.00 00:00 (��������)
		// ���������� ��� ������ �������, � ����� � ������ ������ ���������, �����
		// ���������� ������� �� ������.
		void SetSysFirstState();
	//
	// ���������� ������� ������������ ��������� �������.
		// ���� �� ����� ����������, ������ �������� �������
		SYSSTATE* GetBuffer();
	//
	}SYS,*LPSYS;
};

#endif