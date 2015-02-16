#include"pojazdy.h"

Pojazdy::Pojazdy()
{
	liczba_plikow_do_zaladowania=0;
	liczba_plikow_zaladowanych=0;
	liczba_pojazdow_bez_pieszych=0;
	liczba_pojazdow_z_pieszymi=0;
	liczba_pojazdow_z_dziurami=0;
	liczba_pojazdow_z_dala_od_pasow=0;

}


int Pojazdy:: licz_pliki_w_katalogu(string sciezka)
{
	WIN32_FIND_DATAA found; 
	HANDLE hfind;

	//cout << "\nSkanowanie katalogu: " << sciezka << endl;

	//liczenie plikow o danym rozszerzeniu
	string sciezka_wlasciwa=sciezka+"*.xml";
	int licznik_plikow=0;
	
	hfind=FindFirstFileA(sciezka_wlasciwa.c_str(),&found);
	if(hfind!=INVALID_HANDLE_VALUE)
	{
		licznik_plikow++;
		while(FindNextFileA(hfind,&found))
			licznik_plikow++;
	}
	if(licznik_plikow==0)
	{
		cout << "\nBrak plikow do zaladowania w katalogu: "<< sciezka <<"\nNacisnij Enter by zakonczyc program";
		cin.ignore(1);
		exit(1);
	}

		//cout<<"Liczba znalezionych plikow: "<< licznik_plikow<< endl;

		liczba_plikow_do_zaladowania+=licznik_plikow;
	
	return licznik_plikow;
}
int Pojazdy:: laduj(string sciezka, bool pozycja_dodatnia_predkosc_ujemna)
{
	//pojazd.clear();
	//pojazd.reserve(100000);
	WIN32_FIND_DATAA found; 
	HANDLE hfind;

	//przeszukiwanie katalogu, tym razem z zapisywaniem danych pojazdow z wybranych dni w pamieci
	Pojazd p;
	string sciezka_wlasciwa=sciezka+"*.xml";
	hfind=FindFirstFileA(sciezka_wlasciwa.c_str(),&found);
	if(hfind != INVALID_HANDLE_VALUE)
		do{
			string nazwa=sciezka+found.cFileName;
			p.laduj(nazwa, pozycja_dodatnia_predkosc_ujemna);
			liczba_plikow_zaladowanych++;
			//str.find(str2) != string::npos
			//if(p.symbol.find("W")!=string::npos)
			pojazd.push_back(p);
		} while(::FindNextFileA(hfind, &found) == TRUE);
	return 1;
}


int Pojazdy:: analizuj_predkosc(float odl_min, float odl_max, float w_punkcie)
{
	#pragma omp parallel for
	for(int i=0;i<pojazd.size();i++)
		pojazd[i].analizuj_predkosc(odl_min, odl_max, w_punkcie);

	return 1;
}