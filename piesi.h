#ifndef _piesi_h_
#define _piesi_h_

#include"pieszy.h"

class Piesi
{
public:
	vector<Pieszy> pieszy;

	int liczba_plikow_do_zaladowania;
	int liczba_plikow_zaladowanych;
	int liczba_pieszych_bez_samochodow;
	int liczba_pieszych_z_samochodami;
	Piesi();
	int licz_pliki_w_katalogu(string sciezka);

	int laduj(string sciezka);
};

#endif

