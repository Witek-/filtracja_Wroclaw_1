#include "filtracja.h"

bool  operator < (const SYSTEMTIME& st1,const SYSTEMTIME& st2)
{
	FILETIME ft1, ft2;
	ULARGE_INTEGER t1, t2;
	SystemTimeToFileTime(&st1, &ft1);
	SystemTimeToFileTime(&st2, &ft2);
	memcpy(&t1, &ft1, sizeof(t1));
	memcpy(&t2, &ft2, sizeof(t1));
	return t1.QuadPart<t2.QuadPart;
}
bool  operator > (const SYSTEMTIME& st1,const SYSTEMTIME& st2)
{
	FILETIME ft1, ft2;
	ULARGE_INTEGER t1, t2;
	SystemTimeToFileTime(&st1, &ft1);
	SystemTimeToFileTime(&st2, &ft2);
	memcpy(&t1, &ft1, sizeof(t1));
	memcpy(&t2, &ft2, sizeof(t1));
	return t1.QuadPart>t2.QuadPart;
}
bool  operator <= (const SYSTEMTIME& st1,const SYSTEMTIME& st2)
{
	FILETIME ft1, ft2;
	ULARGE_INTEGER t1, t2;
	SystemTimeToFileTime(&st1, &ft1);
	SystemTimeToFileTime(&st2, &ft2);
	memcpy(&t1, &ft1, sizeof(t1));
	memcpy(&t2, &ft2, sizeof(t1));
	return t1.QuadPart<=t2.QuadPart;
}
bool  operator >= (const SYSTEMTIME& st1,const SYSTEMTIME& st2)
{
	FILETIME ft1, ft2;
	ULARGE_INTEGER t1, t2;
	SystemTimeToFileTime(&st1, &ft1);
	SystemTimeToFileTime(&st2, &ft2);
	memcpy(&t1, &ft1, sizeof(t1));
	memcpy(&t2, &ft2, sizeof(t1));
	return t1.QuadPart>=t2.QuadPart;
}
bool  operator == (const SYSTEMTIME& st1,const SYSTEMTIME& st2)
{
	FILETIME ft1, ft2;
	ULARGE_INTEGER t1, t2;
	SystemTimeToFileTime(&st1, &ft1);
	SystemTimeToFileTime(&st2, &ft2);
	memcpy(&t1, &ft1, sizeof(t1));
	memcpy(&t2, &ft2, sizeof(t1));
	return t1.QuadPart==t2.QuadPart;
}
bool  operator != (const SYSTEMTIME& st1,const SYSTEMTIME& st2)
{
	FILETIME ft1, ft2;
	ULARGE_INTEGER t1, t2;
	SystemTimeToFileTime(&st1, &ft1);
	SystemTimeToFileTime(&st2, &ft2);
	memcpy(&t1, &ft1, sizeof(t1));
	memcpy(&t2, &ft2, sizeof(t1));
	return t1.QuadPart!=t2.QuadPart;
}
double  operator - (const SYSTEMTIME& st1,const SYSTEMTIME& st2)
{
	FILETIME ft1, ft2;
	ULARGE_INTEGER t1, t2;
	ULONGLONG diff;
	SystemTimeToFileTime(&st1, &ft1);
	SystemTimeToFileTime(&st2, &ft2);
	memcpy(&t1, &ft1, sizeof(t1));
	memcpy(&t2, &ft2, sizeof(t1));
	diff = (t1.QuadPart<t2.QuadPart)?(t2.QuadPart-t1.QuadPart):(t1.QuadPart-t2.QuadPart);
	return diff/(1*24*60*60*10000000.0);
}
ostream& operator<< (ostream &out, SYSTEMTIME &st)
{
	out << st.wYear <<"."<<setfill('0')<<setw(2)<<st.wMonth<<"."<<setfill('0')<<setw(2)<<st.wDay<<" "<<setfill('0')<<setw(2)<<st.wHour<<":"<<setfill('0')<<setw(2)<<st.wMinute<<":"<<setfill('0')<<setw(2)<<st.wSecond<<"."<<setfill('0')<<setw(3)<<st.wMilliseconds;
	return out;
}
bool  operator < (const Frame& f1,const Frame& f2)
{
	return f1.ts<f2.ts;
}
SYSTEMTIME systemtime_z_daty(string text_time)
{
	SYSTEMTIME st;
	int p=text_time.find_last_of('.')-2;
	if(p==-3) p=text_time.length();
	st.wYear=atoi(text_time.substr(p-19,4).c_str());
	st.wMonth=atoi(text_time.substr(p-15,2).c_str());
	st.wDay=atoi(text_time.substr(p-13,2).c_str());
	st.wHour=atoi(text_time.substr(p-10,2).c_str());
	st.wMinute=atoi(text_time.substr(p-8,2).c_str());
	st.wSecond=atoi(text_time.substr(p-6,2).c_str());
	st.wMilliseconds=atoi(text_time.substr(p-3,3).c_str());
	FILETIME ft;
	SystemTimeToFileTime( &st, &ft );
	FileTimeToSystemTime( &ft, &st );
	return(st);
}


string nastepny_dzien(string data)
{

	SYSTEMTIME st1;
	st1.wYear=atoi(data.substr(0,4).c_str());
	st1.wMonth=atoi(data.substr(4,2).c_str());
	st1.wDay=atoi(data.substr(6,2).c_str());
	st1.wDayOfWeek=0;
	st1.wHour=0;
	st1.wMinute=0;
	st1.wSecond=0;
	st1.wMilliseconds=0;
	FILETIME ft1;
	ULARGE_INTEGER t1;
	SystemTimeToFileTime(&st1, &ft1);
	memcpy(&t1, &ft1, sizeof(t1));
	t1.QuadPart+=1*24*60*60*10000000.0;
	memcpy(&ft1, &t1, sizeof(ft1));
	FileTimeToSystemTime(&ft1,&st1);
	stringstream ss;
	string s;
	ss<<st1.wYear;
	if(st1.wMonth<10) ss<<"0";
	ss<<st1.wMonth;
	if(st1.wDay<10) ss<<"0";
	ss<<st1.wDay;
	ss>>s;
	return( s );
}

string data_z_systemtime(SYSTEMTIME st1)
{
	stringstream ss;
	string s;
	ss<<st1.wYear;
	if(st1.wMonth<10) ss<<"0";
	ss<<st1.wMonth;
	if(st1.wDay<10) ss<<"0";
	ss<<st1.wDay;
	ss>>s;
	return( s );
}


int liczba_dni_miedzy(string t1, string t2)
{
	SYSTEMTIME st1, st2;
	
	st1.wYear=atoi(t1.substr(0,4).c_str());
	st1.wMonth=atoi(t1.substr(4,2).c_str());
	st1.wDay=atoi(t1.substr(6,2).c_str());
	st1.wHour=0;
	st1.wMinute=0;
	st1.wSecond=0;
	st1.wMilliseconds=0;

	st2.wYear=atoi(t2.substr(0,4).c_str());
	st2.wMonth=atoi(t2.substr(4,2).c_str());
	st2.wDay=atoi(t2.substr(6,2).c_str());
	st2.wHour=0;
	st2.wMinute=0;
	st2.wSecond=0;
	st2.wMilliseconds=0;
	return(st2-st1+1); //bo w³¹cznie
}

fstream plik_wy;