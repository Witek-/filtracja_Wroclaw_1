#ifndef _pojazd_h_
#define _pojazd_h_

#include"filtracja.h"


class Pojazd
{
public: 

	SYSTEMTIME t1,t2;
	string id; /*symbol identyfikacyjny(nazwa) pliku z pojazdem*/
	string symbol;
	string manufacturer;
	string type;
	string model;
	vector<Frame> frame;
	string sciezka; /*sciezka do pliku*/
	int liczba_pieszych;
	bool wypisany_w_statystykach;

	struct Dziura_w_trajektorii
	{
		float rozmiar_x;
		float rozmiar_czas; //w sekundach
		float pozycja_x;
		float ostatnia_pozycja_x; /* ostatnia zarejestrowana pozycja samochodu (odleg³oœæ od œrodka pasów) */

		Dziura_w_trajektorii(): rozmiar_x(-999), rozmiar_czas(-999), pozycja_x(-999) {}
	}dziura_w_trajektorii;
	struct Vel
	{
		float max_x;
		float min_x;
		float max_y;
		float min_y;
		float w_punkcie; /* predkoœæ dla konkretnej wspó³rzêdnej x */
		float na_pasach; /*prêdkoœæ pojazdu na œrodku pasów, dla x=0*/
		Vel() : max_x(-999),min_x(999), max_y(-999), min_y(999), w_punkcie(-999), na_pasach(-999) {}
	} vel;
	struct Acc
	{
		float max_x;
		float min_x;
		float max_y;
		float min_y;
		float w_punkcie; /*przyspieszenie dla konkretnej wspó³rzêdnej x*/
		Acc() : max_x(-999),min_x(999), max_y(-999), min_y(999), w_punkcie(-999) {}
	} acc;

	Pojazd();
	/// <summary>
	///  £aduje dane pojazdu z pliku z opcjonaln¹ normalizacj¹ prêdkoœci i przyspieszenia dla kierunku z negatywnymi pozycjami
	/// </summary>
	/// <param name="pozycja_dodatnia_predkosc_ujemna">Czy normalizowaæ prêdkoœci i przyspieszenia?</param>
	int laduj(string nazwa_pliku, bool pozycja_dodatnia_predkosc_ujemna=false);
	///Wyznacza skrajne wartoœci prêdkoœci i przyspieszenia 
	int analizuj_predkosc(float odl_min, float odl_max, float w_punkcie);
	void wyswietlStempleCzasowe();



};

#endif