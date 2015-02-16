#ifndef _poligon_h_
#define _poligon_h_

#include "filtracja.h"
#include "pojazdy.h"
#include "piesi.h"
#include "statystyka.h"

struct Katalog_wejsciowy
	{
		string data;
		string nazwa;
		bool przetwarzac;
		int licznik_pp;
		int licznik_lp;
		int licznik_pieszych;
		Katalog_wejsciowy(): data(""), nazwa(""), przetwarzac(true), licznik_pp(0), licznik_lp(0), licznik_pieszych(0) {}
	};

class Poligon
{
public:
	string sciezka_bazowa, sciezka_prawy_pas, sciezka_lewy_pas, sciezka_piesi, katalog_wyjsciowy;
	int liczba_pasow;
	vector<Katalog_wejsciowy> katalog_wejsciowy;
	struct Wspolrzedne_przejscia
	{
		float x1, x2;
		float y1, y2;

	}wspolrzedne_przejscia;

	CvRTrees forest;
	struct Zakres_przetwarzania
	{
		string poczatek;
		string koniec;
		vector<string>daty;
		string nazwa;
		//SYSTEMTIME poczatek;
		//SYSTEMTIME koniec;
	}zakres_przetwarzania;
	struct Filtry
	{
		struct Wykrywanie_duchow
		{
			int wlaczone;
			struct Przebyta_odleglosc
			{
				float mniej_niz;
				float wiecej_niz;
			}przebyta_odleglosc;
			float minimalna_odleglosc_w_pionie;
			float maksymalna_odleglosc_w_poziomie;
		}wykrywanie_duchow;
		struct W_wynikach_maja_byc
		{
			bool poprawne;
			bool niepoprawne;
			bool konflikty_odleglosc;
			bool konflikty_hamowanie;
			bool wszystko;
			W_wynikach_maja_byc() : poprawne(false), niepoprawne(false), konflikty_odleglosc(false), konflikty_hamowanie(false), wszystko(false) {}
		}w_wynikach_maja_byc;
		struct Kryteria
		{
			struct Hamowanie
			{
				float opoznienie; /*najwiêksze zarejestrowane w badanym zakresie opóŸnienie pojazdu konfliktowego musi byæ mniejsze*/
				float predkosc; /*najmniejsza zarejestrowana w badanym zakresie prêdkoœæ pojazdu konfliktowego musi byæ mniejsza*/
				float roznica_predkosci; /*najwiêksza zarejestrowana w badanym zakresie ró¿nica prêdkoœci pojazdu konfliktowego musi byæ wiêksza*/
				float odleglosc_od_pieszego; /*najmniejsza odleg³oœæ pojazdu konfliktowego od pieszego W CA£YM ZAKRESIE musi byæ mniejsza*/
			}hamowanie;
			struct Odleglosc_i_predkosc
			{
				float odleglosc_od_pieszego;
				float predkosc_pojazdu;
			}odleglosc_i_predkosc;
		}kryteria;
	}filtry;
	struct Statystyki_dodatkowe
	{
		struct Predkosc_pojazdow_w_punkcie
		{
			float wspolrzedna_x_punktu;
		}predkosc_pojazdow_w_punkcie;
	}statystyki_dodatkowe;
	Pojazdy pojazdy_pp, pojazdy_lp;
	Piesi piesi; 
	Statystyka statystyka;
	/// <summary>
	/// Inicjalizuje g³ówn¹ klasê zawieraj¹c¹ wszystkie dane i metody. Wczytuje parametry przetwarzania, w tym œcie¿ki i czasowy zakres danych.
	/// Sprawdza czy istniej¹ odpowiednie katalogi, jeœli tak, ³aduje dane.
	/// </summary>
	/// <param name="xml">Nazwa pliku, z którego zostan¹ wczytane parametry przetwarzania.</param>
	Poligon(string xml);
	/// <summary>
	///  Liczy katalogi z danymi. Katalogi s¹ nazwane datami i zawieraj¹ 3 podkatalogi z odpowiednimi danymi
	/// </summary>
	void sprawdz_katalogi_danych(); 

	void licz_pliki_do_zaladowania();
	int wczytaj_dane();
	/// <summary>
	///  Dokonuje analizy trajektorii pieszych, klasyfikuje je na poprawne i niepoprawne itd.
	/// </summary>
	int analizuj_trajektorie_pieszych();
	/// <summary>
	/// Wyznacza listê pojazdów widocznych w czasie przejœcia JEDNEGO pieszego przez jezdniê
	/// Nimery wszystkich widocznych pojazdów na ka¿dym pasie zapisywane s¹ w strukturze Pojazdy obiektu Pieszy
	/// </summary>
	/// <param name="nr_pieszego">Numer pieszego do analizy</param>
	int znajdz_pojazdy_dla_pieszego(int nr_pieszego);
	/// <summary>
	/// Wyznacza listê pojazdów widocznych w czasie przejœcia WSZYSTKICH pieszych przez jezdniê
	/// Oblicza tak¿e liczbê pieszych z samochodami i bez
	/// </summary>
	int znajdz_pojazdy_dla_pieszych();
	/// <summary>
	/// Wyznacza minimaln¹ odleg³oœæ pomiêdzy WSZYSTKIMI pieszymi, a WSZYSTKIMI mijaj¹cymi ich pojazdami
	/// 
	/// </summary>
	int znajdz_minimalna_odleglosc_od_pojazdow();
	int znajdz_maksymalne_opoznienie_pojazdow(float odl_min, float odl_max);
	/// <summary>
/// Wyznacza skrajne wartoœci prêdkoœci i przyspieszenia WSZYSTKICH POJAZDÓW w podanym zakresie odleg³oœci.
/// Pe³en zakres obserwacji nie jest dobry, bo trajektorie mogê mieæ spore b³êdy w du¿ej odleg³oœci od przejœcia.
/// Liczy tak¿e pojazdy z dziurami i daleko od przejœcia
/// </summary>
/// <param name="odl_min">Pocz¹tek zakresu przeszukiwania prêdkoœci (odleg³oœæ w metrach od œrodka pasów</param>
/// <param name="odl_max">Koniec zakresu przeszukiwania prêdkoœci (odleg³oœæ w metrach od œrodka pasów</param>
	int analizuj_trajektorie_pojazdow(float odl_min, float odl_max);
	/// <summary>
	///Po analizie trajektorii pojazdow liczy ile pojazdów ma dziury w trajektoriach i ile "nie dojecha³o" do przejœcia
	/// </summary>
	/// <param name="rozmiar_dziury">Granica (w metrach) uznania, ¿e dana trajektoria ma dziurê definiowan¹ jako maksymalna odleg³oœæ pomiêdzy kolejnymi pozycjami</param>
	/// <param name="odleglosc_od_przejscia">Granica (w metrach) uznania, ¿e dany pojazd nie doje¿d¿a do pasów, bo jego trajektoria urywa siê dalej ni¿ tyle metrów od œrodka pasów</param>
	void licz_pojazdy_z_dziurami_i_daleko_od_przejscia(float rozmiar_dziury=4, float odleglosc_od_przejscia=4);

	float znajdz_najwieksze_opoznienie_pojazdu_dla_pieszego(int nr_pieszego);

	void zbieraj_statystyki_pieszych();
	void zbieraj_statystyki_pojazdow();
	void zbieraj_statystyki_dodatkowe();
	void zbieraj_statystyki();

	void zapisz_wyniki_filtracji();
	void zapisz_pieszych();
	void zapisz_pojazdy();


};
#endif
