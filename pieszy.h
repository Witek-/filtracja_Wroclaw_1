#ifndef _pieszy_h_
#define _pieszy_h_

#include"filtracja.h"


class Pieszy
{
public: 

	/// czas
	SYSTEMTIME t1; /*czas pojawienia si� pieszego*/
	SYSTEMTIME t2; /*czas znikni�cia pieszego*/
	string id; /*nazwa pliku*/
	vector<Frame> frame;
	string sciezka; /*sciezka do pliku*/


	struct Trajektoria
	{
		struct Rodzaj
		{
			bool dobra; //ogolnie ze dobra
			bool dluga; //a ponizej szczegoliwa klasyfikacja
			bool srednia;
			bool krotka;
			//bool bardzo_krotka;
			bool duch;
			bool pieszy_na_samochodzie;
			//bool inna;
			Rodzaj() : dobra(false), dluga(false), srednia(false), krotka(false), /*bardzo_krotka(false),*/ duch(false), pieszy_na_samochodzie(false)/*, inna(false)*/ {}
		}rodzaj;

		struct Dlugosc
		{
			float efektywna;
			float w_pionie;
			float w_poziomie;
			float calkowita;
			float na_jezdni_calkowita;
			float na_jezdni_efektywna;
			Dlugosc() : efektywna(0), w_pionie(0), w_poziomie(0), calkowita(0), na_jezdni_calkowita(0), na_jezdni_efektywna(0) {}
		}dlugosc;
		struct Pozycja
		{
			struct Pozioma
			{
				float minimalna;
				float maksymalna;
				float srednia; 
				Pozioma() : minimalna(9999), maksymalna(-9999), srednia(0) {}
			}pozioma;
			struct Pionowa
			{
				float minimalna;
				float maksymalna;
				float srednia; 
				Pionowa() : minimalna(9999), maksymalna(-9999), srednia(0) {}
			}pionowa;
		}pozycja;

		
		

		struct Predkosc
		{
			float minimalna;
			float maksymalna;
			float srednia; /*�rednia z pr�dko�ci chwilowych pieszego na pasach*/
			float odch_st; /*odchylenie standardowe pr�dko�ci pieszego na pasach*/
			float srednia_z_pozycji; /*�rednia wyliczana jako d�ugo�� pokonanej drogi do czasu jej pokonania*/
			Predkosc() : minimalna(9999), maksymalna(0), srednia(0), srednia_z_pozycji(0), odch_st(0) {}
		}predkosc;

		struct Kierunek
		{
			bool w_gore;
			bool w_dol;
			Kierunek() : w_gore(false), w_dol(false) {}
		}kierunek;

		int wspolczynnik_drgan_w_pionie;
		int wspolczynnik_drgan_w_poziomie;

	}trajektoria;

	struct Konflikt
	{
		bool hamowanie; //gwa�towne hamowanie samochodu
		bool smigniecie; //szybki przejazd przez pasy, jak jest na nich pieszy
		Konflikt() : hamowanie(false), smigniecie(false) {}

	}konflikt;

	struct Konfliktowy_pojazd
	{
		float minimalna_odleglosc; /*najmniejsza odleg�o�� pomi�dzy danym pieszym, a wszystkimi mijaj�cymi go samochodami*/
		float maksymalne_opoznienie; /*najwi�ksze (ujemne) zarejestrowane op�nienie dla samochod�w mijaj�cych tego pieszego*/
		float minimalna_predkosc_h; /* najmniejsza zarejestrowana pr�dko�� dla samochodu najbardziej hamuj�cego*/
		float roznica_predkosci_h; /*r�nica mi�dzy najwi�ksz�, a najmniejsz� pr�dko�ci� dla samochodu najbardziej hamuj�cego*/
		float predkosc_najblizszego_na_pasach; /*predkosc samochodu najblizszego do pieszego na srodku pasow*/
		Konfliktowy_pojazd() : minimalna_odleglosc(888), maksymalne_opoznienie(888), minimalna_predkosc_h(888), roznica_predkosci_h(888), predkosc_najblizszego_na_pasach(888) {}
	}konfliktowy_pojazd;

	/// Struktura zawieraj�ca informacje o pojazdach widzianych w czasie rejestracji pieszego
	struct Pojazdy
	{
		/// Pojazdy na prawym pasie
		struct PP
		{
			int p1; //nr pierwszego wspolnego pojazdu
			int p2; //nr ostatniego wspolnego pojazdu
			int liczba; //wspolnych pojazdow
			vector<int> nr;
			vector<float> minimalna_odleglosc; /*od pieszego dla calej trajektorii*/
			vector<float> predkosc_w_punkcie_minimalnej_odleglosci; /*pr�dko�� samochodu, w punkcie najmniejszej odleg�o�ci*/
			vector<Frame::Pos> pozycja_pieszego_w_punkcie_minimalnej_odleglosci;
			vector<Frame::Pos> pozycja_pojazdu_w_punkcie_minimalnej_odleglosci;
			vector<string> po_palcach_po_pietach;
			vector<float> maksymalne_opoznienie; /*na pewnym odcinku (3-15)*/
			vector<float> minimalna_predkosc; /*na pewnym odcinku (3-15)*/
			vector<float> maksymalna_predkosc; /*na pewnym odcinku (3-15)*/
			vector<float> roznica_predkosci; /*na pewnym odcinku (3-15)*/

			PP() : p1(-1),p2(-1),liczba(0) {}
		}pp;
		/// Pojazdy na lewym pasie
		struct LP
		{
			int p1; //nr pierwszego wspolnego pojazdu
			int p2; //nr ostatniego wspolnego pojazdu
			int liczba; //wspolnych pojazdow
			vector<int> nr;
			vector<float> minimalna_odleglosc;/*od pieszego dla calej trajektorii*/
			vector<float> predkosc_w_punkcie_minimalnej_odleglosci; /*pr�dko�� samochodu, w punkcie najmniejszej odleg�o�ci*/
			vector<Frame::Pos> pozycja_pieszego_w_punkcie_minimalnej_odleglosci;
			vector<Frame::Pos> pozycja_pojazdu_w_punkcie_minimalnej_odleglosci;
			vector<string> po_palcach_po_pietach;
			vector<float> maksymalne_opoznienie; /*na pewnym odcinku (3-15)*/
			vector<float> minimalna_predkosc; /*na pewnym odcinku (3-15)*/
			vector<float> maksymalna_predkosc; /*na pewnym odcinku (3-15)*/
			vector<float> roznica_predkosci; /*na pewnym odcinku (3-15)*/

			LP() : p1(-1),p2(-1),liczba(0) {}
		}lp;
		int razem;
		Pojazdy() : razem(0) {}
	}pojazdy;

	Pieszy();

	int laduj(string nazwa_pliku);



};


#endif