#ifndef _filtracja_h_
#define _filtracja_h_

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <opencv2\opencv.hpp>

#include <omp.h>
#include <math.h>
#include <cmath>

#include "pugixml.hpp"
#include "ExcelFormat.h"

using namespace ExcelFormat;
using namespace std;
using namespace pugi;

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <crtdbg.h>

#else // _WIN32

#define	FW_NORMAL	400
#define	FW_BOLD		700

#endif // _WIN32



extern fstream plik_wy;


bool  operator < (const SYSTEMTIME& st1,const SYSTEMTIME& st2);
bool  operator > (const SYSTEMTIME& st1,const SYSTEMTIME& st2);
bool  operator <= (const SYSTEMTIME& st1,const SYSTEMTIME& st2);
bool  operator >= (const SYSTEMTIME& st1,const SYSTEMTIME& st2);
bool  operator == (const SYSTEMTIME& st1,const SYSTEMTIME& st2);
bool  operator != (const SYSTEMTIME& st1,const SYSTEMTIME& st2);
double  operator - (const SYSTEMTIME& st1,const SYSTEMTIME& st2);
ostream& operator<< (ostream &out, SYSTEMTIME &st);
struct Frame
{
	SYSTEMTIME ts;
	struct Pos
	{
		float x;
		float y;
		bool na_pasach; //zmienna okreœlaj¹ca czy pieszy znajduje siê na pasach, ignorowana dla samochodów
	Pos(): na_pasach(false) {}
	} pos;
	struct Vel
	{
		float x;
		float y;
	} vel;
	struct Acc
	{
		float x;
		float y;
	} acc;
};
bool  operator < (const Frame& f1,const Frame& f2);
SYSTEMTIME systemtime_z_daty(string text_time);
///<summary>
///Zwraca datê w formacie YYYYMMDD dnia nastêpnego po danym
///</summary>
string nastepny_dzien(string data);
string data_z_systemtime(SYSTEMTIME st1);
int liczba_dni_miedzy(string t1, string t2);
#endif