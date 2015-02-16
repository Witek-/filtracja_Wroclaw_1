#include"piesi.h"

Piesi::Piesi()
{
	liczba_plikow_do_zaladowania=0;
	liczba_plikow_zaladowanych=0;
	liczba_pieszych_bez_samochodow=0;
	liczba_pieszych_z_samochodami=0;
}


int Piesi:: licz_pliki_w_katalogu(string sciezka)
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


int Piesi::laduj(string sciezka)
{
	//pieszy.clear();
	//pieszy.reserve(100000);
	WIN32_FIND_DATAA found; 
	HANDLE hfind;

	//przeszukiwanie katalogu, tym razem z zapisywaniem danych pieszych z wybranych dni w pamieci
	Pieszy p;
	string sciezka_wlasciwa=sciezka+"*.xml";
	hfind=FindFirstFileA(sciezka_wlasciwa.c_str(),&found);
	if(hfind != INVALID_HANDLE_VALUE)
		do{
			string nazwa=sciezka+found.cFileName;
			p.laduj(nazwa);
			liczba_plikow_zaladowanych++;
			pieszy.push_back(p);

		} while(::FindNextFileA(hfind, &found) == TRUE);
	return 1;
}
