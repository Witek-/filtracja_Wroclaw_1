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
	///  £aduje dane pojazdów z danego katalogu z opcjonaln¹ normalizacj¹ prêdkoœci i przyspieszenia dla kierunku z negatywnymi pozycjami
	/// </summary>
	/// <param name="pozycja_dodatnia_predkosc_ujemna">Czy normalizowaæ prêdkoœci i przyspieszenia?</param>
	int laduj(string sciezka, bool pozycja_dodatnia_predkosc_ujemna=false);
	/// <summary>
	/// Wyznacza skrajne wartoœci prêdkoœci i przyspieszenia WSZYSTKICH pojazdów NA DANYM PASIE w podanym zakresie odleg³oœci.
	/// Pe³en zakres obserwacji nie jest dobry, bo trajektorie mogê mieæ spore b³êdy w du¿ej odleg³oœci od przejœcia.
	/// </summary>
	/// <param name="odl_min">Pocz¹tek zakresu przeszukiwania prêdkoœci (odleg³oœæ w metrach od œrodka pasów</param>
	/// <param name="odl_max">Koniec zakresu przeszukiwania prêdkoœci (odleg³oœæ w metrach od œrodka pasów</param>
	int analizuj_predkosc(float odl_min, float odl_max, float w_punkcie);
};
#endif