#ifndef _statystyka_h_
#define _statystyka_h_

#include"filtracja.h"



class Statystyka
{
private:
	struct Predkosc
		{
			float minimalna;
			float maksymalna;
			float srednia; /*œrednia z prêdkoœci chwilowych pieszego*/
			float odch_st; /*odchylenie standardowe prêdkoœci pieszego na pasach*/
			float srednia_z_pozycji; /*œrednia wyliczana jako d³ugoœæ pokonanej drogi do czasu jej pokonania*/
			Predkosc() : minimalna(0), maksymalna(0), srednia(0), srednia_z_pozycji(0), odch_st(0) {}
		}predkosc;

	typedef struct Piesi
		{
			int dobra; 
			int krotka;
			//int bardzo_krotka;
			int duch;
			int pieszy_na_samochodzie;
			//int inna;
			int razem_poprawna;
			int razem_niepoprawna;
			int razem_poprawna_z_samochodem;
			int razem_poprawna_bez_samochodu;
			Predkosc predkosc;
			Piesi() : dobra(0), krotka(0), /*bardzo_krotka(0),*/ duch(0), pieszy_na_samochodzie(0), /*inna(0),*/ razem_poprawna(0), razem_niepoprawna(0), razem_poprawna_z_samochodem(0), razem_poprawna_bez_samochodu(0) {}
		}Piesi;

	typedef	struct Pojazdy
		{
			int osobowe;
			int polciezarowe;
			int ciezarowe;
			int autobusy;
			int pozostale;
			int razem;
			float predkosc_w_punkcie;
			float wariancja_w_punkcie;
			float odch_st_w_punkcie;
			Pojazdy() : osobowe(0), polciezarowe(0), ciezarowe(0), autobusy(0), pozostale(0), razem(0), predkosc_w_punkcie(0), wariancja_w_punkcie(0), odch_st_w_punkcie(0) {}
		}Pojazdy;

	typedef struct Zliczenia
	{
		struct Piesi piesi;
		struct Pojazdy pojazdy_lp;
		struct Pojazdy pojazdy_pp;
		struct Pojazdy pojazdy_lp_w_punkcie; //niezbêdne do prawid³owego policzenia œredniej prêdkoœci w punkcie
		struct Pojazdy pojazdy_pp_w_punkcie; //gdy¿ nie da siê jej wyznaczyæ dla wszystkich pojazdów
		struct Pojazdy pojazdy;
		int liczba_ostrych_hamowan;
		Zliczenia() : liczba_ostrych_hamowan(0) {}
	}Zliczenia;

	typedef struct Dzienne
	{
		string data;
		Zliczenia na_godzine[24];
		Zliczenia na_dzien;
		Dzienne() : data("000000") {}
	}Dzienne;


//void zapisz_pojazdy_z_puli(string plik, );
public:
	
Dzienne dzienne[60];
Zliczenia calosciowe;

//Zliczenia na_dzien[30];
//Zliczenia na_dzien_tygodnia[7];

void zapisz_pieszych(string plik);
void zapisz_pieszych2(string plik);
void zapisz_pojazdy(string plik);
void zapisz_pojazdy2(string plik);



	
//Statystyka();

	
};

#endif