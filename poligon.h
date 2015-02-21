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

	CvRTrees forest; //stary klasyfikator ogolny dla Warszawy
	//klasyfikatory dedykowane dla Wroclawia
	CvRTrees cen_dx, cen_dsk, swo_dx, swo_dsk;
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
				float opoznienie; /*najwi�ksze zarejestrowane w badanym zakresie op�nienie pojazdu konfliktowego musi by� mniejsze*/
				float predkosc; /*najmniejsza zarejestrowana w badanym zakresie pr�dko�� pojazdu konfliktowego musi by� mniejsza*/
				float roznica_predkosci; /*najwi�ksza zarejestrowana w badanym zakresie r�nica pr�dko�ci pojazdu konfliktowego musi by� wi�ksza*/
				float odleglosc_od_pieszego; /*najmniejsza odleg�o�� pojazdu konfliktowego od pieszego W CA�YM ZAKRESIE musi by� mniejsza*/
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
	/// Inicjalizuje g��wn� klas� zawieraj�c� wszystkie dane i metody. Wczytuje parametry przetwarzania, w tym �cie�ki i czasowy zakres danych.
	/// Sprawdza czy istniej� odpowiednie katalogi, je�li tak, �aduje dane.
	/// </summary>
	/// <param name="xml">Nazwa pliku, z kt�rego zostan� wczytane parametry przetwarzania.</param>
	Poligon(string xml);
	/// <summary>
	///  Liczy katalogi z danymi. Katalogi s� nazwane datami i zawieraj� 3 podkatalogi z odpowiednimi danymi
	/// </summary>
	void sprawdz_katalogi_danych(); 

	void licz_pliki_do_zaladowania();
	int wczytaj_dane();
	/// <summary>
	///  Dokonuje analizy trajektorii pieszych, klasyfikuje je na poprawne i niepoprawne itd.
	/// </summary>
	int analizuj_trajektorie_pieszych();
	/// <summary>
	/// Wyznacza list� pojazd�w widocznych w czasie przej�cia JEDNEGO pieszego przez jezdni�
	/// Nimery wszystkich widocznych pojazd�w na ka�dym pasie zapisywane s� w strukturze Pojazdy obiektu Pieszy
	/// </summary>
	/// <param name="nr_pieszego">Numer pieszego do analizy</param>
	int znajdz_pojazdy_dla_pieszego(int nr_pieszego);
	/// <summary>
	/// Wyznacza list� pojazd�w widocznych w czasie przej�cia WSZYSTKICH pieszych przez jezdni�
	/// Oblicza tak�e liczb� pieszych z samochodami i bez
	/// </summary>
	int znajdz_pojazdy_dla_pieszych();
	/// <summary>
	/// Wyznacza minimaln� odleg�o�� pomi�dzy WSZYSTKIMI pieszymi, a WSZYSTKIMI mijaj�cymi ich pojazdami
	/// 
	/// </summary>
	int znajdz_minimalna_odleglosc_od_pojazdow();
	int znajdz_maksymalne_opoznienie_pojazdow(float odl_min, float odl_max);
	/// <summary>
/// Wyznacza skrajne warto�ci pr�dko�ci i przyspieszenia WSZYSTKICH POJAZD�W w podanym zakresie odleg�o�ci.
/// Pe�en zakres obserwacji nie jest dobry, bo trajektorie mog� mie� spore b��dy w du�ej odleg�o�ci od przej�cia.
/// Liczy tak�e pojazdy z dziurami i daleko od przej�cia
/// </summary>
/// <param name="odl_min">Pocz�tek zakresu przeszukiwania pr�dko�ci (odleg�o�� w metrach od �rodka pas�w</param>
/// <param name="odl_max">Koniec zakresu przeszukiwania pr�dko�ci (odleg�o�� w metrach od �rodka pas�w</param>
	int analizuj_trajektorie_pojazdow(float odl_min, float odl_max);
	/// <summary>
	///Po analizie trajektorii pojazdow liczy ile pojazd�w ma dziury w trajektoriach i ile "nie dojecha�o" do przej�cia
	/// </summary>
	/// <param name="rozmiar_dziury">Granica (w metrach) uznania, �e dana trajektoria ma dziur� definiowan� jako maksymalna odleg�o�� pomi�dzy kolejnymi pozycjami</param>
	/// <param name="odleglosc_od_przejscia">Granica (w metrach) uznania, �e dany pojazd nie doje�d�a do pas�w, bo jego trajektoria urywa si� dalej ni� tyle metr�w od �rodka pas�w</param>
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
