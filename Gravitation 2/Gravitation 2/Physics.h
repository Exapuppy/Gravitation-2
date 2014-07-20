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
		// нормы проекций
		inline float norm_xy();
		inline float norm_xz();
		inline float norm_yz();
	}VECTOR,*LPVECTOR;

	// класс, содержащий орбиты всех 8ми планет,
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
		// вектор расстояния между центрoм телa и Солнца
		VECTOR r;//km.
		// вектор скорости
		VECTOR v;//km/days.
	}PLANET, *LPPLANET;

	//структура состояния системы в определенный момент времени
	typedef struct SystemState
	{
		PLANET planets[9];
	}SYSSTATE,*LPSYSSTATE;

	// основной класс, содержащий состояние системы в текущий момент и другие физические переменные, например, текущее время.
	typedef class System 
	{
	private:
	// Буферы состояний системы
		bool BufferIndex;
		LPSYSSTATE buf1;
		LPSYSSTATE buf2;
	//
	// Физические расчеты
		Vector Sun_coord;
		inline Vector F1( Vector r, float M1, float M2, Vector rO);
		void F_Ailer (int PlanetNum, int ObjectNum);
		void F_Ailer_for_Sun (int PlanetNum);
	//
	// Параметры планет
		float PtMass[10];
		int fi[9];
		float v[9];
		float sl[9];
		float p[9];
	//
	// Расчет положения планет
		//расчет следующего состояния системы
		void SysStep_El();//на основе табличных данных
	//
	// Временные шаги
		// временной шаг
		double delta_t;//sec.
		// временные шаги, за которые определенная планета меняет фазу на 0.1 градуса.
		double _01grad_t[9];
	//
	// Расчет траекторий
		// добавляет в traect2 текущие координаты планет.
		// Таким образом постепенно выстраивается реальная траектория полета объектов.
		void SetCurrentState();
	//	
	public:
	// Расчет траектории
		// Радиусы планет
		float PtRad[10];
		//
		// рассчитывает орбиты с помощью табличных данных. помещает их в traect.
		void SetStandartOrbits();
	//
	// Внутреннее время
		double t;//hours.
		long year, month, day, hour;
		//Функция, которая устанавливает в системе поданное ей время
		void SetSysTime(long year, long month, long day, long hour);
		double timespeed;
	//
	// Функция перевода времени в 4 параметра - Год, Месяц,День и Час.
		void CalculateTimeToDate();
	//
	// Орбита для отображения
		//траектории, рассчитанные с помощью табличных данных.
		ELLIPSES traect;
	//
	// Конструктор и деструктор
		//динамическое выделение и освобождение памяти
		System();
		~System();
	//
	// Функция, которая вызывается в основном цикле (MainLoop)
		//расчитывает новые положения планет, заносит их значения в массивы траекторий.
		//увеличивает время и разбивает его на часы дни месяцы и годы
		void SystemStep();
	//
	// Устанавливает планеты так, как это было 1.1.00 00:00 (примерно)
		// вызывается при сбросе времени, а также в начале работы программы, чтобы
		// расставить планеты по местам.
		void SetSysFirstState();
	//
	// Возвращает текущее расположение элементов системы.
		// пока не нашел применения, просто красивая функция
		SYSSTATE* GetBuffer();
	//
	}SYS,*LPSYS;
};

#endif