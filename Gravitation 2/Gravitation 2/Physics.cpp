#include "stdafx.h"
#include "Physics.h"

#ifdef PHYSIC_H

namespace Physics
{
#pragma region Vector
		//vector's metod's
		inline float Vector::norm()
		{
			return sqrt((this->x)*(this->x) + (this->y)*(this->y) + (this->z)*(this->z));
		};
		// нормы проекций
		inline float Vector::norm_xy()
		{
			return sqrt((this->x)*(this->x) + (this->y)*(this->y));
		};
		inline float Vector::norm_xz()
		{
			return sqrt((this->x)*(this->x) + (this->z)*(this->z));
		};
		inline float Vector::norm_yz()
		{
			return sqrt((this->y)*(this->y) + (this->z)*(this->z));
		};
		// operator's
		inline static Vector operator +(Vector lop, Vector rop) 
		{
			Vector t;
			t.x = lop.x + rop.x;
			t.y = lop.y + rop.y;
			t.z = lop.z + rop.z;
			return t;
		};
		inline static Vector operator -(Vector lop, Vector rop) 
		{
			Vector t;
			t.x = lop.x - rop.x;
			t.y = lop.y - rop.y;
			t.z = lop.z - rop.z;
			return t;
		};
		inline static Vector operator -(Vector a) 
		{
			Vector t;
			t.x = - a.x;
			t.y = - a.y;
			t.z = - a.z;
			return t;
		};
		inline static Vector operator *(float lop, Vector rop) 
		{
			Vector t;
			t.x = lop * rop.x;
			t.y = lop * rop.y;
			t.z = lop * rop.z;
			return t;
		};
		inline static Vector operator /(Vector lop, float rop) 
		{
			Vector t;
			t.x = lop.x / rop;
			t.y = lop.y / rop;
			t.z = lop.z / rop;
			return t;
		};	
		inline static Vector operator *(Vector lop, float rop) { return rop * lop; };
		inline static Vector operator *(int lop, Vector rop) { return (float)lop * rop; };
		inline static Vector operator *(Vector lop, int rop) { return (float)rop * lop; };
		inline static Vector operator /(Vector lop, int rop) { return lop / (float)rop; };
		inline static Vector operator *(double lop, Vector rop){ return (float)lop * rop; };
		inline static Vector operator *(Vector lop, double rop) { return (float)rop * lop; };
#pragma endregion
#pragma region Ellipses
	Ellipses::Ellipses()
	{
		ZeroMemory(this->El,sizeof(this->El));
		ZeroMemory(this->Size,sizeof(this->Size));
	};
	//добавление нового эл-та
	void Ellipses::push(int PlanetNum, VECTOR c)
	{
		if(!this->full(PlanetNum))
		{
			this->El[PlanetNum][Size[PlanetNum]]=c;
			this->Size[PlanetNum]++;
		}
	}
	// проверка на заполненность
	bool Ellipses::full(int PlanetNum)
	{
		return(this->Size[PlanetNum]>=LENGTH);
	}
	//сброс
	void Ellipses::reset()
	{
		ZeroMemory(this->Size,sizeof(this->Size));
	}
#pragma endregion

#pragma region CONSTANTS

#pragma region Массы объектов
#define M_Sun		1.9891e30
#define M_Mercury	3.33022e23
#define M_Venus		4.8685e24
#define M_Earth		5.9736e24
#define M_Mars		0.64185e24
#define M_Jupiter	1876.64328e24
#define M_Saturn	5.6846e26
#define M_Uranus	86.832e24
#define M_Neptune	102.43e24
#define M_Pluto		1.305e22
#pragma endregion
#pragma region Pадиусы объектов
#define R_Sun		695510
#define R_Mercury	2440
#define R_Venus		6051
#define R_Earth		6378.137
#define R_Mars		3396.9
#define R_Jupiter	71400
#define R_Saturn	60268
#define R_Uranus	25560
#define R_Neptune	25000
#define R_Pluto		2306
#pragma endregion
#pragma region Эксцентриситеты орбит планет
#define E_Mercury	0.20530294
#define E_Venus		0.00676 
#define E_Earth		0.016710219
#define E_Mars		0.0933941
#define E_Jupiter	0.0489
#define E_Saturn	0.055723219
#define E_Uranus	0.044405586
#define E_Neptune	0.011214269
#define E_Pluto		0.24880766
#pragma endregion
#pragma region Периоды вращения(сут)
#define P_Mercury	87.969
#define P_Venus		224.70069
#define P_Earth		365.256366
#define P_Mars		686.98
#define P_Jupiter	4332.589
#define P_Saturn	10759.22
#define P_Uranus	30799.095
#define P_Neptune	60190
#define P_Pluto		90613.3055
#pragma endregion
#pragma region Средние скорости орбитального движения(км/c)
#define V_Mercury	47.87
#define V_Venus		35.02
#define V_Earth		29.783
#define V_Mars		24.077
#define V_Jupiter	13.07
#define V_Saturn	9.69
#define V_Uranus	6.81
#define V_Neptune	5.43
#define V_Pluto		4.666
#pragma endregion
#pragma region Наклоны орбит по отношению к друг другу
#define Sl_Mercury	7.0
#define Sl_Venus	3.39
#define Sl_Earth	0.0
#define Sl_Mars		1.85
#define Sl_Jupiter	1.31
#define Sl_Saturn	2.49
#define Sl_Uranus	0.77
#define Sl_Neptune	1.77
#define Sl_Pluto	17.15
#pragma endregion
#pragma region фазы планет в 01.01.00 00:00
#define fi_Mercury	27
#define fi_Venus	100
#define fi_Earth	87
#define fi_Mars		180
#define fi_Jupiter	127
#define fi_Saturn	218
#define fi_Uranus	286
#define fi_Neptune	53
#define fi_Pluto	44
#pragma endregion

//расчет коэфф. sqrt(1-E^2)
#define _f(E) (float)sqrt(1-E*E)
// Pi/180
#define M_PI_180 0.0174532925199433

#pragma endregion

#pragma region System::Traectories
	// рассчитывает орбиты с помощью табличных данных. помещает их в traect.
	void System::SetStandartOrbits()
	{
		VECTOR c;
		float a, b, grad_step = 0.005f, koef, sin_Sl;
		int i;
#pragma region Mercury
		sin_Sl = (float)-sin(Sl_Mercury*M_PI_180);
		koef = _f(E_Mercury);
		a = V_Mercury*3600 * P_Mercury*24 * M_1_PI / ( 3 + 3*koef - sqrt((3 + koef)*(1 + 3*koef)));
		b = a * koef;
		for(i=0;i<LENGTH;i++)
		{
			c.x = a * (cos(grad_step*i*M_PI_180) - E_Mercury);
			c.y = b * sin(grad_step*i*M_PI_180);
			c.z = c.y * sin_Sl;
			traect.push( 0, c );
		}
#pragma endregion
#pragma region Venus
		sin_Sl = -sin(Sl_Venus*M_PI_180);
		koef = _f(E_Venus);
		a = V_Venus*3600 * P_Venus*24 * M_1_PI / ( 3 + 3*koef - sqrt((3 + koef)*(1 + 3*koef)));
		b = a * koef;
		for(i=0;i<LENGTH;i++)
		{
			c.x = a * (cos(grad_step*i*M_PI_180) - E_Venus);
			c.y = b * sin(grad_step*i*M_PI_180);
			c.z = c.y * sin_Sl;
			traect.push( 1, c );
		}
#pragma endregion
#pragma region Earth
		sin_Sl = -sin(Sl_Earth*M_PI_180);
		koef = _f(E_Earth);
		a = V_Earth*3600 * P_Earth*24 * M_1_PI / ( 3 + 3*koef - sqrt((3 + koef)*(1 + 3*koef)));
		b = a * koef;
		for(i=0;i<LENGTH;i++)
		{
			c.x = a * (cos(grad_step*i*M_PI_180) - E_Earth);
			c.y = b * sin(grad_step*i*M_PI_180);
			c.z = c.y * sin_Sl;
			traect.push( 2, c );
		}
#pragma endregion
#pragma region Mars
		sin_Sl = -sin(Sl_Mars*M_PI_180);
		koef = _f(E_Mars);
		a = V_Mars*3600 * P_Mars*24 * M_1_PI / ( 3 + 3*koef - sqrt((3 + koef)*(1 + 3*koef)));
		b = a * koef;
		for(i=0;i<LENGTH;i++)
		{
			c.x = a * cos(grad_step*i*M_PI_180);
			c.y = b * (sin(grad_step*i*M_PI_180) - E_Mars);
			c.z = c.y * sin_Sl;
			traect.push( 3, c );
		}
#pragma endregion
#pragma region Jupiter
		sin_Sl = -sin(Sl_Jupiter*M_PI_180);
		koef = _f(E_Jupiter);
		a = V_Jupiter*3600 * P_Jupiter*24 * M_1_PI / ( 3 + 3*koef - sqrt((3 + koef)*(1 + 3*koef)));
		b = a * koef;
		for(i=0;i<LENGTH;i++)
		{
			c.x = a * cos(grad_step*i*M_PI_180);
			c.y = b * (sin(grad_step*i*M_PI_180) - E_Jupiter);
			c.z = c.y * sin_Sl;
			traect.push( 4, c );
		}
#pragma endregion
#pragma region Saturn
		sin_Sl = -sin(Sl_Saturn*M_PI_180);
		koef = _f(E_Saturn);
		a = V_Saturn*3600 * P_Saturn*24 * M_1_PI / ( 3 + 3*koef - sqrt((3 + koef)*(1 + 3*koef)));
		b = a * koef;
		for(i=0;i<LENGTH;i++)
		{
			c.x = a * cos(grad_step*i*M_PI_180);
			c.y = b * (sin(grad_step*i*M_PI_180) - E_Saturn);
			c.z = c.y * sin_Sl;
			traect.push( 5, c );
		}
#pragma endregion
#pragma region Uranus
		sin_Sl = -sin(Sl_Uranus*M_PI_180);
		koef = _f(E_Uranus);
		a = V_Uranus*3600 * P_Uranus*24 * M_1_PI / ( 3 + 3*koef - sqrt((3 + koef)*(1 + 3*koef)));
		b = a * koef;
		for(i=0;i<LENGTH;i++)
		{
			c.x = a * cos(grad_step*i*M_PI_180);
			c.y = b * (sin(grad_step*i*M_PI_180) + E_Uranus);
			c.z = c.y * sin_Sl;
			traect.push( 6, c );
		}
#pragma endregion
#pragma region Neptune
		sin_Sl = -sin(Sl_Neptune*M_PI_180);
		koef = _f(E_Neptune);
		a = V_Neptune*3600 * P_Neptune*24 * M_1_PI / ( 3 + 3*koef - sqrt((3 + koef)*(1 + 3*koef)));
		b = a * koef;
		for(i=0;i<LENGTH;i++)
		{
			c.x = a * cos(grad_step*i*M_PI_180);
			c.y = b * (sin(grad_step*i*M_PI_180) + E_Neptune);
			c.z = c.y * sin_Sl;
			traect.push( 7, c );
		}
#pragma endregion
#pragma region Pluto
		sin_Sl = -sin(Sl_Pluto*M_PI_180);
		koef = _f(E_Pluto);
		a = V_Pluto*3600 * P_Pluto*24 * M_1_PI / ( 3 + 3*koef - sqrt((3 + koef)*(1 + 3*koef)));
		b = a * koef;
		float x,y;
		for(i=0;i<LENGTH;i++)
		{
			x = a * (cos(grad_step*i*M_PI_180) + E_Pluto);
			y = b * sin(grad_step*i*M_PI_180);
			c.x=x*0.90630778703664996324255265675432 - y*0.42261826174069943618697848964773;
			c.y=x*0.42261826174069943618697848964773 + y*0.90630778703664996324255265675432;
			c.z = c.y * sin_Sl;
			traect.push( 8, c );
		}
#pragma endregion
	}

#pragma endregion
#pragma region System()&~System()
	System::System()
	{
		timespeed = 1;
		fi[0] =   fi_Mercury*200;
		fi[1] =   fi_Venus*200;
		fi[2] =   fi_Earth*200;
		fi[3] =   fi_Mars*200;
		fi[4] =   fi_Jupiter*200;
		fi[5] =   fi_Saturn*200;
		fi[6] =   fi_Uranus*200;
		fi[7] =   fi_Neptune*200;
		fi[8] =   fi_Pluto*200;
		v[0] = 	V_Mercury;
		v[1] = 	V_Venus;
		v[2] = 	V_Earth;
		v[3] = 	V_Mars;
		v[4] = 	V_Jupiter;
		v[5] = 	V_Saturn;
		v[6] = 	V_Uranus;
		v[7] = 	V_Neptune;
		v[8] = 	V_Pluto;
		sl[0] = -sin(Sl_Mercury*M_PI_180);
		sl[1] = -sin(Sl_Venus*M_PI_180);
		sl[2] = -sin(Sl_Earth*M_PI_180);
		sl[3] = -sin(Sl_Mars*M_PI_180);
		sl[4] = -sin(Sl_Jupiter*M_PI_180);
		sl[5] = -sin(Sl_Saturn*M_PI_180);
		sl[6] = -sin(Sl_Uranus*M_PI_180);
		sl[7] = -sin(Sl_Neptune*M_PI_180);
		sl[8] = -sin(Sl_Pluto*M_PI_180);

			PtMass[0]=M_Sun;
			PtMass[1]=M_Mercury;
			PtMass[2]=M_Venus;
			PtMass[3]=M_Earth;
			PtMass[4]=M_Mars;
			PtMass[5]=M_Jupiter;
			PtMass[6]=M_Saturn;
			PtMass[7]=M_Uranus;
			PtMass[8]=M_Neptune;
			PtMass[9]=M_Pluto;
			
			PtRad[0]=R_Sun;
			PtRad[1]=R_Mercury;
			PtRad[2]=R_Venus;
			PtRad[3]=R_Earth;
			PtRad[4]=R_Mars;
			PtRad[5]=R_Jupiter;
			PtRad[6]=R_Saturn;
			PtRad[7]=R_Uranus;
			PtRad[8]=R_Neptune;
			PtRad[9]=R_Pluto;

			p[0]=P_Mercury*24;
			p[1]=P_Venus*24;
			p[2]=P_Earth*24;
			p[3]=P_Mars*24;
			p[4]=P_Jupiter*24;
			p[5]=P_Saturn*24;
			p[6]=P_Uranus*24;
			p[7]=P_Neptune*24;
			p[8]=P_Pluto*24;

			BufferIndex=false;
				
			buf1 = new SYSSTATE();
			buf2 = new SYSSTATE();
				
			//traect = new ELLIPSES();
			SetStandartOrbits();
			//traect2 = new ELLIPSES();

			delta_t = h;
			t = 0;

			Sun_coord.x = 0;
			Sun_coord.y = 0;
			Sun_coord.z = 0;
	}
	System::~System()
	{
		delete buf1; 
		delete buf2;
		//delete traect; 
		//delete traect2;
	}
#pragma endregion
#pragma region System::Steps
	//расчет следующего состояния системы по табличным данным
	void System::SysStep_El()
	{
		BufferIndex=!BufferIndex;
		register int k;
		for(int i = 8; i >= 0; i--)
		{
			k = (int)(( t - p[i]*(long)(t/p[i]) ) * LENGTH/p[i]) + fi[i];
			if(k>=LENGTH)k-=LENGTH;
			(BufferIndex?(buf1):(buf2))->planets[i].r = traect.El[i][k];
		}
	}

	// Функция, которая вызывается в основном цикле (MainLoop)
	//расчитывает новые положения планет, заносит их значения в массивы траекторий.
	//увеличивает время и разбивает его на часы дни месяцы и годы
	void System::SystemStep()
	{
	//рассчитывает новые положения планет
		SysStep_El();
	//увеличивает время
		t += delta_t*timespeed;
	//рассчитывает новую дату
		CalculateTimeToDate();
	}
	//
#pragma endregion

	//Устанавливает планеты так, как это было 1.1.00 00:00 (примерно)
	void System::SetSysFirstState()
	{
		for(int PlanetNum = 8; PlanetNum >= 0; PlanetNum--)
		{
			(BufferIndex?buf1:buf2)->planets[PlanetNum].r = traect.El[PlanetNum][fi[PlanetNum]];
		}
	}
	// возвращает текущее расположение элементов системы.
	SYSSTATE* System::GetBuffer()
	{
		return (BufferIndex?(buf1):(buf2));
	}
	// Функция перевода времени в 4 параметра - Год, Месяц,День и Час.
	void System::CalculateTimeToDate()
	{
		float par1, par2;
		// 8766.152784 - Hours of Year
		// 730.512732 - Hours of Month
		year = (long)(t / 8766.152784);
		par1 = (float)(t - year * 8766.152784);
		month = (long)(par1/730.512732)+1;
		par2 = (float)(par1 - (month-1)*730.512732);
		day = (long)(par2/24)+1;
		hour = (long)(par2 - (day-1)*24);
	}
	//Функция, которая устанавливает в системе поданное ей время
	void System::SetSysTime(long year, long month, long day, long hour)
	{
		// 8766.152784 - Hours of Year
		// 730.512732 - Hours of Month
		t = year*8766.152784f + (month-1)*730.512732f + (day-1)*24.0f + hour;
	}
}

#endif