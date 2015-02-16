#ifndef _pojazdy_h_
#define _pojazdy_h_

#include"pojazd.h"

class Pojazdy
{
public:

	vector<Pojazd> pojazd;

	int liczba_plikow_do_zaladowania;
	int liczba_plikow_zaladowanych;
	int liczba_pojazdow_bez_pieszych;
	int liczba_pojazdow_z_pieszymi;
	int liczba_pojazdow_z_dziurami;
	int liczba_pojazdow_z_dala_od_pasow;

	Pojazdy();
	int licz_pliki_w_katalogu(string sciezka);
	/// <summary>
	///  �aduje dane pojazd�w z danego katalogu z opcjonaln� normalizacj� pr�dko�ci i przyspieszenia dla kierunku z negatywnymi pozycjami
	/// </summary>
	/// <param name="pozycja_dodatnia_predkosc_ujemna">Czy normalizowa� pr�dko�ci i przyspieszenia?</param>
	int laduj(string sciezka, bool pozycja_dodatnia_predkosc_ujemna=false);
	/// <summary>
	/// Wyznacza skrajne warto�ci pr�dko�ci i przyspieszenia WSZYSTKICH pojazd�w NA DANYM PASIE w podanym zakresie odleg�o�ci.
	/// Pe�en zakres obserwacji nie jest dobry, bo trajektorie mog� mie� spore b��dy w du�ej odleg�o�ci od przej�cia.
	/// </summary>
	/// <param name="odl_min">Pocz�tek zakresu przeszukiwania pr�dko�ci (odleg�o�� w metrach od �rodka pas�w</param>
	/// <param name="odl_max">Koniec zakresu przeszukiwania pr�dko�ci (odleg�o�� w metrach od �rodka pas�w</param>
	int analizuj_predkosc(float odl_min, float odl_max, float w_punkcie);
};
#endif