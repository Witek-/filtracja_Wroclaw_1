#include"poligon.h"

const double INF = 1.e100;

extern struct Wspolrzedne_przejscia
	{
		float x1, x2;
		float y1, y2;

	}wp;

double roundToNearest(double num) {
	return (num > 0.0) ? floor(num*100 + 0.5)/100 : ceil(num*100 - 0.5)/100;
}

double interpolate(vector<pair<double, double> > t, double x) {
	// Assumes that "table" is sorted by .first
	// Check if x is out of bound
	if (x > t.back().first) return INF;
	if (x < t[0].first) return -INF;
	vector<pair<double, double> >::iterator it, it2;
	// INFINITY is defined in math.h in the glibc implementation
	it = lower_bound(t.begin(), t.end(), make_pair(x, -INF));
	// Corner case
	if (it == t.begin()) return it->second;
	it2 = it;
	--it2;
	return it2->second + (it->second - it2->second)*(x - it2->first)/(it->first - it2->first);
}
vector <double> interpolate2(Pojazd p, vector<double> new_x)
{
	vector <double> result;
	vector<pair<double, double> > table;

	for(int q=0;q<p.frame.size();q++)
		table.push_back(make_pair(p.frame[q].pos.x, sqrt(p.frame[q].vel.x*p.frame[q].vel.x+p.frame[q].vel.y*p.frame[q].vel.y)));



	std::sort(table.begin(), table.end());
	for(int i=0;i<new_x.size();i++)
		result.push_back( roundToNearest(interpolate(table, new_x[i])));

	return(result);
}



Poligon::Poligon(string xml)
{
	xml_document doc;
	xml_parse_result result = doc.load_file(xml.c_str());
	if(result.status!=status_ok)
	{
		cout << result.description() <<endl;
		cout <<"Blad wczytywania pliku XML z parametrami, nacisnij ENTER by zakonczyc"<< endl;
		cin.ignore(1);
		exit(-1);
	}


	//odczyt sciezek

	xml_node object = doc.child("filtracja").child("ustawienia");
	/*
	//przeliczenie zakresu przetwarzania na format czasowy - chyba zbedne
	string data=object.child("zakres_przetwarzania").child_value("poczatek");
	zakres_przetwarzania.poczatek.wYear=atoi(data.substr(0,4).c_str());
	zakres_przetwarzania.poczatek.wMonth=atoi(data.substr(4,2).c_str());
	zakres_przetwarzania.poczatek.wDay=atoi(data.substr(6,2).c_str());
	data=object.child("zakres_przetwarzania").child_value("koniec");
	zakres_przetwarzania.koniec.wYear=atoi(data.substr(0,4).c_str());
	zakres_przetwarzania.koniec.wMonth=atoi(data.substr(4,2).c_str());
	zakres_przetwarzania.koniec.wDay=atoi(data.substr(6,2).c_str());*/
	zakres_przetwarzania.poczatek=object.child("zakres_przetwarzania").child_value("poczatek");
	zakres_przetwarzania.koniec=object.child("zakres_przetwarzania").child_value("koniec");

	//jeœli nie ma zakresu, przetwarzamy kolejne daty
	zakres_przetwarzania.nazwa=object.child("zakres_przetwarzania").child_value("nazwa");
	if(zakres_przetwarzania.poczatek=="")
	{
		string daty=object.child("zakres_przetwarzania").child_value("daty");
		stringstream ss(daty);
		string tmp;

		while(ss>>tmp)
			zakres_przetwarzania.daty.push_back(tmp);

	}



	sciezka_bazowa = object.child_value("sciezka_bazowa");
	liczba_pasow = atoi(object.child_value("liczba_pasow"));
	sciezka_prawy_pas = object.child_value("prawy_pas");
	sciezka_lewy_pas = object.child_value("lewy_pas");
	sciezka_piesi = object.child_value("piesi");
	katalog_wyjsciowy=object.child_value("nazwa_katalogu_wyjsciowego");


	//wpisanie wspolrzednych - na razie z palca, nie z pliku
	if(sciezka_bazowa.find("CEN")!=std::string::npos)
	{
		wspolrzedne_przejscia.x1=0; 
		wspolrzedne_przejscia.x2=6.0; 
		wspolrzedne_przejscia.y1=-1.0; 
		wspolrzedne_przejscia.y2=wspolrzedne_przejscia.y1+7.5;
	}
	else
		if(sciezka_bazowa.find("SWO")!=std::string::npos)
		{
			wspolrzedne_przejscia.x1=0.0; 
			wspolrzedne_przejscia.x2=6.0; 
			wspolrzedne_przejscia.y1=-0.5; 
			wspolrzedne_przejscia.y2=wspolrzedne_przejscia.y1+7.5;
		}
		else
		{

			cout <<"Zla nazwa poligonu (dopuszczalne CEN i SWO), nacisnij ENTER by zakonczyc"<< endl;
			cin.ignore(1);
			exit(-1);
		}

		//zmiennq globalna aby wygodny dostep miala funkcja analizy trajektorii samochodow
		wp.x1=wspolrzedne_przejscia.x1;
		wp.x2=wspolrzedne_przejscia.x2;
		wp.y1=wspolrzedne_przejscia.y1;
		wp.y2=wspolrzedne_przejscia.y2;

		//odczyt operacji do wykonania

		//filtry.wykrywanie_duchow.wlaczone=atoi(doc.child("filtracja").child("wykrywanie_duchow").child_value("wlaczone"));
		//filtry.wykrywanie_duchow.przebyta_odleglosc.mniej_niz=atof(doc.child("filtracja").child("wykrywanie_duchow").child("przebyta_odleglosc").child_value("mniej_niz"));
		//filtry.wykrywanie_duchow.przebyta_odleglosc.wiecej_niz=atof(doc.child("filtracja").child("wykrywanie_duchow").child("przebyta_odleglosc").child_value("wiecej_niz"));
		//filtry.wykrywanie_duchow.minimalna_odleglosc_w_pionie=atof(doc.child("filtracja").child("wykrywanie_duchow").child_value("minimalna_odleglosc_w_pionie"));
		//filtry.wykrywanie_duchow.maksymalna_odleglosc_w_poziomie=atof(doc.child("filtracja").child("wykrywanie_duchow").child_value("maksymalna_odleglosc_w_poziomie"));


		if((string)doc.child("filtracja").child("w_wynikach_filtracji_maja_byc").child_value("poprawne")=="tak") 
			filtry.w_wynikach_maja_byc.poprawne=true;
		else
			filtry.w_wynikach_maja_byc.poprawne=false;

		if((string)doc.child("filtracja").child("w_wynikach_filtracji_maja_byc").child_value("niepoprawne")=="tak") 
			filtry.w_wynikach_maja_byc.niepoprawne=true;
		else
			filtry.w_wynikach_maja_byc.niepoprawne=false;

		if((string)doc.child("filtracja").child("w_wynikach_filtracji_maja_byc").child_value("konflikty_odleglosc")=="tak") 
			filtry.w_wynikach_maja_byc.konflikty_odleglosc=true;
		else
			filtry.w_wynikach_maja_byc.konflikty_odleglosc=false;

		if((string)doc.child("filtracja").child("w_wynikach_filtracji_maja_byc").child_value("konflikty_hamowanie")=="tak") 
			filtry.w_wynikach_maja_byc.konflikty_hamowanie=true;
		else
			filtry.w_wynikach_maja_byc.konflikty_hamowanie=false;


		if((string)doc.child("filtracja").child("w_wynikach_filtracji_maja_byc").child_value("wszystko")=="tak") 
			filtry.w_wynikach_maja_byc.wszystko=true;
		else
			filtry.w_wynikach_maja_byc.wszystko=false;



		//wczytanie kryteriów filtracji konfliktów

		filtry.kryteria.hamowanie.opoznienie=atof(doc.child("filtracja").child("kryteria_filtracji_konfliktow").child("hamowanie").child_value("opoznienie_ponizej")); 
		filtry.kryteria.hamowanie.predkosc=atof(doc.child("filtracja").child("kryteria_filtracji_konfliktow").child("hamowanie").child_value("predkosc_ponizej")); 
		filtry.kryteria.hamowanie.roznica_predkosci=atof(doc.child("filtracja").child("kryteria_filtracji_konfliktow").child("hamowanie").child_value("roznica_predkosci_powyzej")); 
		filtry.kryteria.hamowanie.odleglosc_od_pieszego=atof(doc.child("filtracja").child("kryteria_filtracji_konfliktow").child("hamowanie").child_value("odleglosc_od_pieszego")); 
		filtry.kryteria.odleglosc_i_predkosc.odleglosc_od_pieszego=atof(doc.child("filtracja").child("kryteria_filtracji_konfliktow").child("odleglosc_i_predkosc").child_value("odleglosc_od_pieszego")); 
		filtry.kryteria.odleglosc_i_predkosc.predkosc_pojazdu=atof(doc.child("filtracja").child("kryteria_filtracji_konfliktow").child("odleglosc_i_predkosc").child_value("predkosc_powyzej")); 


		statystyki_dodatkowe.predkosc_pojazdow_w_punkcie.wspolrzedna_x_punktu = atof(doc.child("filtracja").child("statystyki_dodatkowe").child("predkosc_pojazdow_w_punkcie").child_value("wspolrzedna_x_punktu")); 
		
		//klasyfikator stary ogolny
		forest.load( "piesi20131015-20131015_2.xml" );
		if( forest.get_tree_count() == 0 )
		{

			cout <<"Blad wczytywania klasyfikatora pieszych, nacisnij ENTER by zakonczyc"<< endl;
			cin.ignore(1);
			exit(-1);
		}
		//klasyfikatory dedykowane
		cen_dx.load("piesi_cen9.1.dx.xml");
		cen_dsk.load("piesi_cen9.1.dsk.xml");
		swo_dx.load("piesi_swo9.1.dx.xml");
		swo_dsk.load("piesi_swo9.1.dsk.xml");
		if( cen_dx.get_tree_count() == 0 || cen_dsk.get_tree_count() == 0 || swo_dx.get_tree_count() == 0 || swo_dsk.get_tree_count() == 0)
		{

			cout <<"Blad wczytywania ktoregos z klasyfikatorw dedykowanych, nacisnij ENTER by zakonczyc"<< endl;
			cin.ignore(1);
			exit(-1);
		}



		sprawdz_katalogi_danych();

		DWORD windows_t1=GetTickCount();
		wczytaj_dane();
		DWORD windows_t2=GetTickCount()-windows_t1;

		if(windows_t2/1000<120)
			printf("\nCzas wczytywania danych [s]: %d\n", windows_t2/1000);
		else
			printf("\nCzas wczytywania danych [min]: %.1f\n", 1./60*(windows_t2/1000));

		cout << "\nDane wczytane...";



};

void Poligon::sprawdz_katalogi_danych()
{
	WIN32_FIND_DATAA found; 
	if(zakres_przetwarzania.poczatek!="")
	{
		cout << "Zakres przetwarzania\nLiczba dni: "<<liczba_dni_miedzy(zakres_przetwarzania.poczatek,zakres_przetwarzania.koniec) << endl;
		cout << "Zakres dat: "<< zakres_przetwarzania.poczatek << " - " << zakres_przetwarzania.koniec << endl;
	}
	else
		if(zakres_przetwarzania.daty.size()!=0)
		{
			cout << "Zakres przetwarzania\nLiczba dni: "<<zakres_przetwarzania.daty.size() << endl;
			cout << "Konkretne daty: "<< endl;
			for(int i=0;i<zakres_przetwarzania.daty.size();i++) 
				cout << zakres_przetwarzania.daty[i]<<", ";
			cout<<endl;
		}
		else
		{
			cout <<"Blad w zakresie przetwarzania, nacisnij ENTER by zakonczyc"<< endl;
			cin.ignore(1);
			exit(-1);
		}

		//generowanie listy katalogów
		Katalog_wejsciowy k;
		//przetwarzanie standardowe - zakres dat
		if(zakres_przetwarzania.poczatek!="")
		{
			string data=zakres_przetwarzania.poczatek;
			do
			{
				k.data=data;
				k.nazwa=sciezka_bazowa+"/"+data;
				katalog_wejsciowy.push_back(k);
				data=nastepny_dzien(data);
			}while(data<=zakres_przetwarzania.koniec);
		}
		//przetwarzanie konkretnych dat
		else
		{
			int i=0;

			do
			{
				string data=zakres_przetwarzania.daty[i];
				k.data=data;
				k.nazwa=sciezka_bazowa+"/"+data;
				katalog_wejsciowy.push_back(k);
				data=zakres_przetwarzania.daty[i];
				i++;
			}while(i<zakres_przetwarzania.daty.size());
		}

		cout << "Skanowanie katalogu: " << sciezka_bazowa << endl;
		//sprawdzanie czy wszystkie wygenerowane katalogi istniej¹ i czy s¹ ok
		for(int i=0;i<katalog_wejsciowy.size();i++)
		{
			if(FindFirstFileA(katalog_wejsciowy[i].nazwa.c_str(),&found)!=INVALID_HANDLE_VALUE && found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//znaleziono katalog odpowiadaj¹cy danej dacie
				//najpierw liczenie katalogow podrzednych
				WIN32_FIND_DATAA found2; 
				HANDLE hfind2;
				int lkp=-2; //ignorujemy katalog bie¿¹cy i nadrzêdny (. i ..)
				hfind2=FindFirstFileA((katalog_wejsciowy[i].nazwa+"/*").c_str(),&found2);
				do 
				{
					if (found2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	lkp++;
				} while (FindNextFileA(hfind2, &found2) != 0);

				if(lkp==liczba_pasow+1) //jest ok, znaleziono o 1 wiecej katalogow (hum) niz jest pasow 
				{
					if(lkp==3) //znaleziono 3 podkatalogi
					{
						//sprawdzenie czy znalezione 3 podkatalogi s¹ poprawne (bez sprawdzania ich zawartoœci!
						if(FindFirstFileA((katalog_wejsciowy[i].nazwa+"/"+sciezka_lewy_pas).c_str(),&found2)==INVALID_HANDLE_VALUE ||
							FindFirstFileA((katalog_wejsciowy[i].nazwa+"/"+sciezka_prawy_pas).c_str(),&found2)==INVALID_HANDLE_VALUE ||
							FindFirstFileA((sciezka_bazowa+"/"+found.cFileName+"/"+sciezka_piesi).c_str(),&found2)==INVALID_HANDLE_VALUE)
						{
							cout << "\nW katalogu " <<katalog_wejsciowy[i].nazwa<< " nie znaleziono przynajmniej 1 podkatalogu z danymi\nNacisnij Enter by zakonczyc program";
							cin.ignore(1);
							exit(1);
						}
					}
					else
						if(lkp==2) //znaleziono 2 podkatalogi
						{
							//sprawdzenie czy znalezione 2 podkatalogi s¹ poprawne (bez sprawdzania ich zawartoœci!
							if(FindFirstFileA((katalog_wejsciowy[i].nazwa+"/"+sciezka_prawy_pas).c_str(),&found2)==INVALID_HANDLE_VALUE ||
								FindFirstFileA((sciezka_bazowa+"/"+found.cFileName+"/"+sciezka_piesi).c_str(),&found2)==INVALID_HANDLE_VALUE)
							{
								cout << "\nW katalogu " <<katalog_wejsciowy[i].nazwa<< " nie znaleziono przynajmniej 1 podkatalogu z danymi\nNacisnij Enter by zakonczyc program";
								cin.ignore(1);
								exit(1);
							}
						}

						else
						{
							cout << "\nKatalog " <<katalog_wejsciowy[i].nazwa<< " nie zawiera "<< liczba_pasow+1 <<" podkatalogow z danymi\nNacisnij Enter by zakonczyc program";
							cin.ignore(1);
							exit(1);
						}
				}
				else
				{
					cout << "\nKatalog " <<katalog_wejsciowy[i].nazwa<< " zawiera inna liczbe podkatalogow z danymi niz powinien\nNacisnij Enter by zakonczyc program";
					cin.ignore(1);
					exit(1);
				}
			}
			//obsluga katalogow "brak danych" i "dane niekompletne"
			else
				if(FindFirstFileA((katalog_wejsciowy[i].nazwa+" brak danych").c_str(),&found)!=INVALID_HANDLE_VALUE && found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					cout << "\nKatalog \"" <<sciezka_bazowa+"/"+found.cFileName<< "\" bedzie ignorowany, bo nie zawiera danych\n";
					katalog_wejsciowy[i].przetwarzac=false;
				}
				else
					if(FindFirstFileA((katalog_wejsciowy[i].nazwa+" dane niepelne").c_str(),&found)!=INVALID_HANDLE_VALUE && found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						cout << "\nKatalog \"" <<sciezka_bazowa+"/"+found.cFileName<< "\" bedzie ignorowany, bo nie zawiera kompletu danych\n";
						katalog_wejsciowy[i].przetwarzac=false;
					}

					else
					{
						cout << "\nKatalog " <<(katalog_wejsciowy[i].nazwa).c_str()<< " nie istnieje\nNacisnij Enter by zakonczyc program";
						cin.ignore(1);
						exit(1);
					}


		}

		cout << "\nKatalogi z danymi wydaja sie byc w porzadku...";

}

void Poligon::licz_pliki_do_zaladowania()
{
	cout << endl;

	//policzenie ile jest katalogow do przetworzenia
	int k=0;
	for(int i=0;i<katalog_wejsciowy.size();i++)
		if(katalog_wejsciowy[i].przetwarzac!=false) 
			k++;

	for(int i=0;i<katalog_wejsciowy.size();i++)
	{
		if(katalog_wejsciowy[i].przetwarzac==false) continue;
		if(liczba_pasow==1 || liczba_pasow==2)
			katalog_wejsciowy[i].licznik_pp = pojazdy_pp.licz_pliki_w_katalogu(katalog_wejsciowy[i].nazwa+"/"+sciezka_prawy_pas+"/");
		if(liczba_pasow==2)
			katalog_wejsciowy[i].licznik_lp = pojazdy_lp.licz_pliki_w_katalogu(katalog_wejsciowy[i].nazwa+"/"+sciezka_lewy_pas+"/");
		katalog_wejsciowy[i].licznik_pieszych = piesi.licz_pliki_w_katalogu(katalog_wejsciowy[i].nazwa+"/"+sciezka_piesi+"/");
		cout << "\rZliczanie plikow do zaladowania..." << setw(3) <<100*(i+1)/k<<"%";

	}

}

int Poligon::wczytaj_dane()
{
	licz_pliki_do_zaladowania();
	if(zakres_przetwarzania.poczatek!="")
		cout << "\n\nZakres przetwarzania\nLiczba dni: "<<liczba_dni_miedzy(zakres_przetwarzania.poczatek,zakres_przetwarzania.koniec);
	else
		cout << "\n\nZakres przetwarzania\nLiczba dni: "<<zakres_przetwarzania.daty.size();
	//liczenie dni ignorowanych
	int li=0;
	for(int i=0;i<katalog_wejsciowy.size();i++)
		if(katalog_wejsciowy[i].przetwarzac==false) li++;

	cout << ", w tym dni pominietych: "<<li << endl;

	if(zakres_przetwarzania.poczatek!="")
		cout << "Zakres dat: "<< zakres_przetwarzania.poczatek << " - " << zakres_przetwarzania.koniec << endl;
	else
		cout << "Zakres dat: "<< zakres_przetwarzania.daty[0] << " - " << zakres_przetwarzania.daty[zakres_przetwarzania.daty.size()-1] << endl;
	cout<<"\nLiczba obiektow do zaladowania:";
	if(liczba_pasow==1 || liczba_pasow==2)
		cout<<"\nPojazdy, prawy pas: "<<pojazdy_pp.liczba_plikow_do_zaladowania;
	if(liczba_pasow==2)
		cout<<"\nPojazdy, lewy pas: "<<pojazdy_lp.liczba_plikow_do_zaladowania;

	cout<<"\nPiesi: "<<piesi.liczba_plikow_do_zaladowania;
	cout<<"\n\nWczytywanie danych...to moze chwile potrwac\n";
	//wersja rownolegla
#pragma omp parallel sections
	{ 
#pragma omp section 
		{
			if(liczba_pasow==1 || liczba_pasow==2)
				for(int i=0;i<katalog_wejsciowy.size();i++)
					if(katalog_wejsciowy[i].przetwarzac==true)
						if(sciezka_bazowa.find("POW")!=std::string::npos || sciezka_bazowa.find("CEN")!=std::string::npos || sciezka_bazowa.find("SWO")!=std::string::npos)
							pojazdy_pp.laduj(katalog_wejsciowy[i].nazwa+"/"+sciezka_prawy_pas+"/",true);
						else
							pojazdy_pp.laduj(katalog_wejsciowy[i].nazwa+"/"+sciezka_prawy_pas+"/",false);
			//cout<<"\nWczytano dane dla pojazdow z prawego pasa";
		}

#pragma omp section 
		{
			if(liczba_pasow==2)
				for(int i=0;i<katalog_wejsciowy.size();i++)
					if(katalog_wejsciowy[i].przetwarzac==true)
						if(sciezka_bazowa.find("POW")!=std::string::npos || sciezka_bazowa.find("CEN")!=std::string::npos || sciezka_bazowa.find("SWO")!=std::string::npos)
							pojazdy_lp.laduj(katalog_wejsciowy[i].nazwa+"/"+sciezka_lewy_pas+"/",true);
						else
							pojazdy_lp.laduj(katalog_wejsciowy[i].nazwa+"/"+sciezka_lewy_pas+"/",false);

			//cout<<"\nWczytano dane dla pojazdow z lewego pasa";
		}
#pragma omp section 
		{
			for(int i=0;i<katalog_wejsciowy.size();i++)
				if(katalog_wejsciowy[i].przetwarzac==true)
					piesi.laduj(katalog_wejsciowy[i].nazwa+"/"+sciezka_piesi+"/");
			//cout<<"\nWczytano dane dla pieszych";
		}
#pragma omp section 
		{
			while(piesi.liczba_plikow_zaladowanych!=piesi.liczba_plikow_do_zaladowania
				|| pojazdy_pp.liczba_plikow_zaladowanych!=pojazdy_pp.liczba_plikow_do_zaladowania
				||pojazdy_lp.liczba_plikow_zaladowanych!=pojazdy_lp.liczba_plikow_do_zaladowania)
			{
				Sleep(200);
				cout << "\rPojazdy, prawy pas: " << setw(3) <<100*pojazdy_pp.liczba_plikow_zaladowanych/pojazdy_pp.liczba_plikow_do_zaladowania <<"%";
				if(liczba_pasow==2)
					cout << " Pojazdy, lewy pas: " << setw(3) <<100*pojazdy_lp.liczba_plikow_zaladowanych/pojazdy_lp.liczba_plikow_do_zaladowania<<"%";
				cout << " Piesi: " << setw(3) <<100*piesi.liczba_plikow_zaladowanych/piesi.liczba_plikow_do_zaladowania<<"%";
			}

		}
	}


	return 1;
}


int Poligon::analizuj_trajektorie_pieszych()
{
	//int licznik=0;


//#pragma omp parallel for
	for(int i=0;i<piesi.pieszy.size();i++)
	{
		Pieszy *p=&piesi.pieszy[i];

		//liczenie 6 wskaznikow
		//kryterium 1 - odleg³oœæ miêdzy pozycj¹ pocz¹tkow¹ a koñcow¹
		int ostatnia=p->frame.size()-1;
		p->trajektoria.dlugosc.efektywna=abs(sqrt( (p->frame[0].pos.x-p->frame[ostatnia].pos.x)*(p->frame[0].pos.x-p->frame[ostatnia].pos.x) + (p->frame[0].pos.y-p->frame[ostatnia].pos.y)*(p->frame[0].pos.y-p->frame[ostatnia].pos.y) ));


		//kryterium 2 - przebyta odleg³oœæ w pionie
		float min_y, max_y;
		min_y=p->frame[0].pos.y;
		max_y=min_y;
		for(int l=1;l<p->frame.size();l++)
		{
			if(p->frame[l].pos.y<min_y) min_y=p->frame[l].pos.y;
			if(p->frame[l].pos.y>max_y) max_y=p->frame[l].pos.y;
		}
		p->trajektoria.dlugosc.w_pionie=max_y-min_y;

		//kryterium 3 - przebyta odleg³oœæ w poziomie
		float min_x, max_x;
		min_x=p->frame[0].pos.x;
		max_x=min_x;
		for(int l=1;l<p->frame.size();l++)
		{
			if(p->frame[l].pos.x<min_x) min_x=p->frame[l].pos.x;
			if(p->frame[l].pos.x>max_x) max_x=p->frame[l].pos.x;
		}
		p->trajektoria.dlugosc.w_poziomie=max_x-min_x;

		//kryterium 4 - calkowita przebyta odleglosc
		p->trajektoria.dlugosc.calkowita=0;
		for(int l=1;l<p->frame.size();l++)
			p->trajektoria.dlugosc.calkowita+=abs(sqrt((p->frame[l].pos.x-p->frame[l-1].pos.x)*(p->frame[l].pos.x-p->frame[l-1].pos.x) + (p->frame[l].pos.y-p->frame[l-1].pos.y)*(p->frame[l].pos.y-p->frame[l-1].pos.y) ));

		//kryterium 5 - liczba zmian znaku predkosci pionowej (jak bardzo trzesie sie pieszy)
		p->trajektoria.wspolczynnik_drgan_w_pionie=0;
		for(int l=2;l<p->frame.size();l++)
			if( (p->frame[l].pos.y-p->frame[l-1].pos.y) * (p->frame[l-1].pos.y-p->frame[l-2].pos.y) <0 )
				p->trajektoria.wspolczynnik_drgan_w_pionie++;

		//kryterium 6 - liczba zmian znaku predkosci poziomej (jak bardzo trzesie sie pieszy)
		p->trajektoria.wspolczynnik_drgan_w_poziomie=0;
		for(int l=2;l<p->frame.size();l++)
			if( (p->frame[l].pos.x-p->frame[l-1].pos.x) * (p->frame[l-1].pos.x-p->frame[l-2].pos.x) <0 )
				p->trajektoria.wspolczynnik_drgan_w_poziomie++;



		//dodatkowe parametry
		//analiza trajektorii


		int licznik_zliczen=0; //zlicza ile razy liczylismy œrednie prêdkoœci, a te liczymy tylko w obrêbie pasów (dla wsp Y, X dowolna)
		for(int l=0;l<p->frame.size();l++)
		{
			if(p->frame[l].pos.x<p->trajektoria.pozycja.pozioma.minimalna) p->trajektoria.pozycja.pozioma.minimalna=p->frame[l].pos.x;
			if(p->frame[l].pos.x>p->trajektoria.pozycja.pozioma.maksymalna) p->trajektoria.pozycja.pozioma.maksymalna=p->frame[l].pos.x;
			p->trajektoria.pozycja.pozioma.srednia+=p->frame[l].pos.x;

			if(p->frame[l].pos.y<p->trajektoria.pozycja.pionowa.minimalna) p->trajektoria.pozycja.pionowa.minimalna=p->frame[l].pos.y;
			if(p->frame[l].pos.y>p->trajektoria.pozycja.pionowa.maksymalna) p->trajektoria.pozycja.pionowa.maksymalna=p->frame[l].pos.y;
			p->trajektoria.pozycja.pionowa.srednia+=p->frame[l].pos.y;

			//pieszy musi byæ na pasach; 
			if(p->frame[l].pos.y>=wspolrzedne_przejscia.y1 && p->frame[l].pos.y<=wspolrzedne_przejscia.y2)
			{
				//wpisujemy do pozycji pieszego, ¿e tu jest na pasach - przyda siê póŸniej
				piesi.pieszy[i].frame[l].pos.na_pasach=true;
				float predkosc_chwilowa=sqrt(p->frame[l].vel.x*p->frame[l].vel.x+p->frame[l].vel.y*p->frame[l].vel.y);
				if(predkosc_chwilowa>4) continue; //odrzucamy tak¿e nieprawdopodobne prêdkoœci
				p->trajektoria.predkosc.srednia+=predkosc_chwilowa;
				if(predkosc_chwilowa>p->trajektoria.predkosc.maksymalna) p->trajektoria.predkosc.maksymalna=predkosc_chwilowa;
				if(predkosc_chwilowa<p->trajektoria.predkosc.minimalna) p->trajektoria.predkosc.minimalna=predkosc_chwilowa;
				licznik_zliczen++;
			}
			else
				piesi.pieszy[i].frame[l].pos.na_pasach=false; // nie jest na pasach
		}
		float srednia=p->trajektoria.predkosc.srednia/licznik_zliczen;
		if(licznik_zliczen>0)
			p->trajektoria.predkosc.srednia/=licznik_zliczen;
		else
			p->trajektoria.predkosc.srednia=0;

		p->trajektoria.pozycja.pozioma.srednia/=p->frame.size();
		p->trajektoria.pozycja.pionowa.srednia/=p->frame.size();

		//teraz liczenie wariancji prêdkoœci pieszego
		float wariancja=0;
		licznik_zliczen=0;
		for(int l=0;l<p->frame.size();l++)
			if(p->frame[l].pos.y>=wspolrzedne_przejscia.y1 && p->frame[l].pos.y<=wspolrzedne_przejscia.y2)
			{
				float predkosc_chwilowa=sqrt(p->frame[l].vel.x*p->frame[l].vel.x+p->frame[l].vel.y*p->frame[l].vel.y);
				wariancja+=(predkosc_chwilowa-p->trajektoria.predkosc.srednia)*(predkosc_chwilowa-p->trajektoria.predkosc.srednia);
				licznik_zliczen++;
			}

			if(licznik_zliczen>0)
				wariancja/=licznik_zliczen;
			else
				wariancja=0;
			p->trajektoria.predkosc.odch_st=sqrt(wariancja); //odchylenie standardowe

			//przy okazji notujemy chwile pojawienia sie l znikniecia
			p->t1=p->frame[0].ts;
			p->t2=p->frame[p->frame.size()-1].ts;

			//maj¹c chwile pojawienia siê l znikniêcia, mo¿emy policzyæ drug¹ œredni¹ prêdkoœæ
			//na podstawie przebytej odleg³oœci l czasu
			float t=(p->t2-p->t1)*24*60*60;
			p->trajektoria.predkosc.srednia_z_pozycji=p->trajektoria.dlugosc.calkowita/t;



			//dlugosc efektywna i calkowita trajektorii pieszego na ulicy - czesc chodnikowa jest pomijana
			int ulica1=-1, ulica2=-1;	
			for(int l=0;l<p->frame.size();l++)
			{
				//poszukiwanie pierwszej i ostatniej klatki na ulicy
				if(ulica1==-1 && p->frame[l].pos.na_pasach==true) //nie znaleziono jeszcze ramki wejscia pieszego na ulice i wlasnie wlazl
					ulica1=l;
				if(ulica1!=-1 && p->frame[l].pos.na_pasach==true) //i caly czas jest
					ulica2=l;
			}
			if(ulica1!=-1)
			{
				p->trajektoria.dlugosc.na_jezdni_efektywna=abs(sqrt( (p->frame[ulica1].pos.x-p->frame[ulica2].pos.x)*(p->frame[ulica1].pos.x-p->frame[ulica2].pos.x) + (p->frame[ulica1].pos.y-p->frame[ulica2].pos.y)*(p->frame[ulica1].pos.y-p->frame[ulica2].pos.y) ));
				for(int l=ulica1+1;l<=ulica2;l++)
					p->trajektoria.dlugosc.na_jezdni_calkowita+=abs(sqrt((p->frame[l].pos.x-p->frame[l-1].pos.x)*(p->frame[l].pos.x-p->frame[l-1].pos.x) + (p->frame[l].pos.y-p->frame[l-1].pos.y)*(p->frame[l].pos.y-p->frame[l-1].pos.y) ));
			}

	}






//#pragma omp parallel for
	for(int i=0;i<piesi.pieszy.size();i++)
	{
		//oryginalna wersja cech dla Warszawy
		/*cv::Mat p = (cv::Mat_<float>(1,10) << 
			piesi.pieszy[i].trajektoria.dlugosc.calkowita,
			piesi.pieszy[i].trajektoria.dlugosc.efektywna,
			piesi.pieszy[i].trajektoria.dlugosc.w_pionie,
			piesi.pieszy[i].trajektoria.dlugosc.w_poziomie,
			piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_pionie,
			piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_poziomie,
			piesi.pieszy[i].trajektoria.predkosc.srednia,
			piesi.pieszy[i].trajektoria.pozycja.pozioma.minimalna,
			piesi.pieszy[i].trajektoria.pozycja.pozioma.srednia,
			piesi.pieszy[i].trajektoria.pozycja.pozioma.maksymalna);

		char r=forest.predict(p);
		if(r=='d') 
			piesi.pieszy[i].trajektoria.rodzaj.dobra=true;
		else
			piesi.pieszy[i].trajektoria.rodzaj.duch=true;*/
		
		//wersja cech dla Wroclawia

		cv::Mat p = (cv::Mat_<float>(1,7) << 
		piesi.pieszy[i].trajektoria.pozycja.pionowa.maksymalna-piesi.pieszy[i].trajektoria.pozycja.pionowa.minimalna,
		(piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_efektywna>0 ? piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_calkowita/piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_efektywna : -1),
		(piesi.pieszy[i].trajektoria.pozycja.pionowa.maksymalna-piesi.pieszy[i].trajektoria.pozycja.pionowa.minimalna)/(piesi.pieszy[i].trajektoria.pozycja.pozioma.maksymalna-piesi.pieszy[i].trajektoria.pozycja.pozioma.minimalna),
		piesi.pieszy[i].trajektoria.dlugosc.efektywna,
		piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_calkowita,
		piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_efektywna,
		piesi.pieszy[i].trajektoria.predkosc.minimalna);

		
		char dx, dsk;
		if(sciezka_bazowa.find("CEN")!=std::string::npos)
		{

			dx=cen_dx.predict(p);
			if(dx=='d')
				dsk=cen_dsk.predict(p);
		}
		else
		if(sciezka_bazowa.find("SWO")!=std::string::npos)
			{

			dx=swo_dx.predict(p);
			if(dx=='d')
				dsk=swo_dsk.predict(p);
			}
		else
			{

			cout <<"Nieobslugiwany poligon (w module analizy trajektorii pieszych), nacisnij ENTER by zakonczyc"<< endl;
			cin.ignore(1);
			exit(-1);
		}

		if(dx=='d') //znaczy trajektoria dobra, ale nie wiemy dokladnie jaka
		{
			piesi.pieszy[i].trajektoria.rodzaj.dobra=true;
			if(dsk=='d')
				piesi.pieszy[i].trajektoria.rodzaj.dluga=true;
			else
				if(dsk=='s')
					piesi.pieszy[i].trajektoria.rodzaj.srednia=true;
			if(dsk=='k')
				piesi.pieszy[i].trajektoria.rodzaj.krotka=true;
			
		}
		else
			piesi.pieszy[i].trajektoria.rodzaj.duch=true;



		//czy pieszy idzie w gore czy w dol
		if(piesi.pieszy[i].frame[piesi.pieszy[i].frame.size()-1].pos.y-piesi.pieszy[i].frame[0].pos.y >0)
			piesi.pieszy[i].trajektoria.kierunek.w_gore=true;
		else
			piesi.pieszy[i].trajektoria.kierunek.w_dol=true;

	}


	return 1;
}
int Poligon::znajdz_pojazdy_dla_pieszego(int nr_pieszego)
{
	if(piesi.pieszy[nr_pieszego].trajektoria.rodzaj.duch || piesi.pieszy[nr_pieszego].trajektoria.rodzaj.pieszy_na_samochodzie) return 0;
	SYSTEMTIME p1, p2, s1, s2;
	p1=piesi.pieszy[nr_pieszego].t1;
	p2=piesi.pieszy[nr_pieszego].t2;

	//piesi.pieszy[0].wyswietlStempleCzasowe();
	//pojazdy_nr_pieszego.pojazd[0].wyswietlStempleCzasowe();

	int i;
	//prawy pas
	for(i=0;i<pojazdy_pp.pojazd.size();i++)
	{


		s1=pojazdy_pp.pojazd[i].t1;
		s2=pojazdy_pp.pojazd[i].t2;
		if(s2>p1 && s1<p2) 
		{
			//cout << "pierwszy samochod nr " <<i<<endl;
			piesi.pieszy[nr_pieszego].pojazdy.pp.p1=i;
			break;
		}
	}

	if(piesi.pieszy[nr_pieszego].pojazdy.pp.p1!=-1)
	{
		for(i=piesi.pieszy[nr_pieszego].pojazdy.pp.p1+1;i<pojazdy_pp.pojazd.size();i++)
		{
			s1=pojazdy_pp.pojazd[i].t1;
			s2=pojazdy_pp.pojazd[i].t2;
			if(s1>p2) 	break;
		}

		//cout << "ostatni samochod nr " <<i-1<<endl;
		piesi.pieszy[nr_pieszego].pojazdy.pp.p2=i-1;
		piesi.pieszy[nr_pieszego].pojazdy.pp.liczba=piesi.pieszy[nr_pieszego].pojazdy.pp.p2-piesi.pieszy[nr_pieszego].pojazdy.pp.p1+1;
	}
	//zapisanie, ¿e danemu pojazdowi towarzyszy jakiœ pieszy
	int pp1=piesi.pieszy[nr_pieszego].pojazdy.pp.p1;
	int pp2=piesi.pieszy[nr_pieszego].pojazdy.pp.p2;
	if(pp1!=-1)
		for(i=pp1;i<=pp2;i++)
			pojazdy_pp.pojazd[i].liczba_pieszych++;

	//lewy pas
	for(i=0;i<pojazdy_lp.pojazd.size();i++)
	{

		s1=pojazdy_lp.pojazd[i].t1;
		s2=pojazdy_lp.pojazd[i].t2;
		if(s2>p1 && s1<p2) 
		{
			//cout << "pierwszy samochod nr " <<i<<endl;
			piesi.pieszy[nr_pieszego].pojazdy.lp.p1=i;
			break;
		}
	}

	if(piesi.pieszy[nr_pieszego].pojazdy.lp.p1!=-1)
	{
		for(i=piesi.pieszy[nr_pieszego].pojazdy.lp.p1+1;i<pojazdy_lp.pojazd.size();i++)
		{
			s1=pojazdy_lp.pojazd[i].t1;
			s2=pojazdy_lp.pojazd[i].t2;
			if(s1>p2) 	break;
		}

		//cout << "ostatni samochod nr " <<i-1<<endl;
		piesi.pieszy[nr_pieszego].pojazdy.lp.p2=i-1;
		piesi.pieszy[nr_pieszego].pojazdy.lp.liczba=piesi.pieszy[nr_pieszego].pojazdy.lp.p2-piesi.pieszy[nr_pieszego].pojazdy.lp.p1+1;
	}

	//zapisanie, ¿e danemu pojazdowi towarzyszy jakiœ pieszy
	int lp1=piesi.pieszy[nr_pieszego].pojazdy.lp.p1;
	int lp2=piesi.pieszy[nr_pieszego].pojazdy.lp.p2;
	if(lp1!=-1)
		for(i=lp1;i<=lp2;i++)
			pojazdy_lp.pojazd[i].liczba_pieszych++;


	piesi.pieszy[nr_pieszego].pojazdy.razem=piesi.pieszy[nr_pieszego].pojazdy.lp.liczba+piesi.pieszy[nr_pieszego].pojazdy.pp.liczba;



	//zapisanie numerow wszystkich pojazdow do wektora
	for(int i=0;i<piesi.pieszy[nr_pieszego].pojazdy.lp.liczba;i++)
		piesi.pieszy[nr_pieszego].pojazdy.lp.nr.push_back(piesi.pieszy[nr_pieszego].pojazdy.lp.p1+i);
	for(int i=0;i<piesi.pieszy[nr_pieszego].pojazdy.pp.liczba;i++)
		piesi.pieszy[nr_pieszego].pojazdy.pp.nr.push_back(piesi.pieszy[nr_pieszego].pojazdy.pp.p1+i);
	//if(index_s1==-1 && index_s2==-1) cout << "brak samochodow\n";
	return 1;
}
int Poligon::znajdz_pojazdy_dla_pieszych()
{
	//#pragma omp parallel for
	for(int i=0;i<piesi.pieszy.size();i++)
		znajdz_pojazdy_dla_pieszego(i);

	//zliczanie pieszych bez i z samochodami
	//#pragma omp parallel for
	for(int i=0;i<piesi.pieszy.size();i++)
		if(piesi.pieszy[i].pojazdy.razem==0) 
			piesi.liczba_pieszych_bez_samochodow++;
		else
			piesi.liczba_pieszych_z_samochodami++;

	//zliczanie samochodów bez i z pieszymi
	//#pragma omp parallel for
	for(int i=0;i<pojazdy_pp.pojazd.size();i++)
		if(pojazdy_pp.pojazd[i].liczba_pieszych>0)
			pojazdy_pp.liczba_pojazdow_z_pieszymi++;
		else
			pojazdy_pp.liczba_pojazdow_bez_pieszych++;
	//#pragma omp parallel for
	for(int i=0;i<pojazdy_lp.pojazd.size();i++)
		if(pojazdy_lp.pojazd[i].liczba_pieszych>0)
			pojazdy_lp.liczba_pojazdow_z_pieszymi++;
		else
			pojazdy_lp.liczba_pojazdow_bez_pieszych++;
	return 1;
}
int Poligon::znajdz_minimalna_odleglosc_od_pojazdow()
{
	int i;
	//#pragma omp parallel for
	for(i=0;i<piesi.pieszy.size();i++)
	{
		//sprawdzanie wszystkich pojazdow na lewym pasie
		if(piesi.pieszy[i].pojazdy.lp.p1!=-1)
		{

			for(int j=piesi.pieszy[i].pojazdy.lp.p1;j<=piesi.pieszy[i].pojazdy.lp.p2;j++)
			{	

				//znalezienie wspolnych ramek dla pieszego i pojazdu
				vector<Frame> f; //samochody
				set_intersection(pojazdy_lp.pojazd[j].frame.begin(),pojazdy_lp.pojazd[j].frame.end(), piesi.pieszy[i].frame.begin(), piesi.pieszy[i].frame.end(),back_inserter(f));
				vector<Frame> g; //piesi
				set_intersection(piesi.pieszy[i].frame.begin(), piesi.pieszy[i].frame.end(),f.begin(), f.end(),back_inserter(g));

				float odleglosc=999;
				float predkosc=999;
				Frame::Pos pozycja_pieszego_min; //w punkcie minimalnej odleglosci
				Frame::Pos pozycja_pojazdu_min; //w punkcie minimalnej odleglosci


				for(int k=0;k<f.size();k++)
				{
					//cout << f[k].pos.x << " " <<f[k].pos.y <<" - " << g[k].pos.x << " " << g[k].pos.y << "     " <<
					//sqrt((f[k].pos.x-g[k].pos.x)*(f[k].pos.x-g[k].pos.x) + (f[k].pos.y-g[k].pos.y)*(f[k].pos.y-g[k].pos.y)) << endl;

					//obliczanie odleg³oœci tylko jak pieszy jest na pasach! jak czeka z boku to nie
					//obliczenia WSZEDZIE! gdy zakomentowane
					//if(g[k].pos.na_pasach==true)
					{
						float odl_tmp=sqrt((f[k].pos.x-g[k].pos.x)*(f[k].pos.x-g[k].pos.x) + (f[k].pos.y-g[k].pos.y)*(f[k].pos.y-g[k].pos.y));
						if(odl_tmp<odleglosc)
						{
							odleglosc=odl_tmp;
							predkosc=sqrt(f[k].vel.x*f[k].vel.x+f[k].vel.y*f[k].vel.y);
							pozycja_pieszego_min=g[k].pos;
							pozycja_pojazdu_min=f[k].pos;

						}
					}
				}
				//wpisanie minimalnej odleg³oœci dla tego pieszego i danego pojazdu
				piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc.push_back(odleglosc);
				piesi.pieszy[i].pojazdy.lp.predkosc_w_punkcie_minimalnej_odleglosci.push_back(predkosc);
				piesi.pieszy[i].pojazdy.lp.pozycja_pieszego_w_punkcie_minimalnej_odleglosci.push_back(pozycja_pieszego_min);
				piesi.pieszy[i].pojazdy.lp.pozycja_pojazdu_w_punkcie_minimalnej_odleglosci.push_back(pozycja_pojazdu_min);

				if(f.size()<1)
				{
					piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach.push_back("blad");
					continue;
				}

				Frame::Pos pozycja_pieszego_pocz=g[0].pos; //w pierwszej wspolnej klatce
				Frame::Pos pozycja_pieszego_konc=g[f.size()-1].pos; //w ostatniej wspolnej klatce
				Frame::Pos pozycja_pojazdu_pocz=f[0].pos; //w pierwszej wspolnej klatce
				Frame::Pos pozycja_pojazdu_konc=f[f.size()-1].pos; //w ostatniej wspolnej klatce


				//czy samochod przejechal po palcach czy po pietach, a moze przepuscil


				if(piesi.pieszy[i].trajektoria.kierunek.w_gore)
				{
					if(pozycja_pieszego_pocz.x<pozycja_pojazdu_pocz.x) //tak bedzie prawie zawsze
						//jesli pozycja minimalna pieszego jest mniejsza, to albo przepuscil albo po pietach
							if(pozycja_pieszego_min.x<pozycja_pojazdu_min.x) 
								//skoro ostatnia jest mniejsza, to pelne przepuszczenie
									if(pozycja_pieszego_konc.x<pozycja_pojazdu_konc.x) 
										piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach.push_back("przepuszczenie");
									else
										//jesli ostatnia nie jest mniejsza, to znaczy ze przepuscil i przejechal po pietach
										piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach.push_back("po pietach");
							else
								//jesli jednak pozycja minimalna pieszego NIE JEST MNIEJSZA niz samochodu
								//to znaczy ze jest po palcach ALBO po pietach
								if(pozycja_pieszego_min.y<pozycja_pojazdu_min.y) 
									piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach.push_back("po palcach");
								else
									piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach.push_back("po pietach");
					else
						piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach.push_back("hgw");
				}
				else
					if(piesi.pieszy[i].trajektoria.kierunek.w_dol)
					{
						if(pozycja_pieszego_pocz.x<pozycja_pojazdu_pocz.x) //tak bedzie prawie zawsze
							//jesli pozycja minimalna pieszego jest mniejsza, to albo przepuscil albo po pietach
								if(pozycja_pieszego_min.x<pozycja_pojazdu_min.x) 
									//skoro ostatnia jest mniejsza, to pelne przepuszczenie
										if(pozycja_pieszego_konc.x<pozycja_pojazdu_konc.x) 
											piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach.push_back("przepuszczenie");
										else
											//jesli ostatnia nie jest mniejsza, to znaczy ze przepuscil i przejechal po pietach
											piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach.push_back("po pietach");
								else
									//jesli jednak pozycja minimalna pieszego NIE JEST MNIEJSZA niz samochodu
									//to znaczy ze jest po palcach ALBO po pietach
									if(pozycja_pieszego_min.y>pozycja_pojazdu_min.y) 
										piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach.push_back("po palcach");
									else
										piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach.push_back("po pietach");
						else
							piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach.push_back("hgw");
					}


			}

		}

		//to samo dla prawego pasa

		if(piesi.pieszy[i].pojazdy.pp.p1!=-1)
		{
			for(int j=piesi.pieszy[i].pojazdy.pp.p1;j<=piesi.pieszy[i].pojazdy.pp.p2;j++)
			{	

				//znalezienie wspolnych ramek dla pieszego i pojazdu
				vector<Frame> f;
				set_intersection(pojazdy_pp.pojazd[j].frame.begin(),pojazdy_pp.pojazd[j].frame.end(), piesi.pieszy[i].frame.begin(), piesi.pieszy[i].frame.end(),back_inserter(f));
				vector<Frame> g;
				set_intersection(piesi.pieszy[i].frame.begin(), piesi.pieszy[i].frame.end(),f.begin(), f.end(),back_inserter(g));


				float odleglosc=999;
				float predkosc=999;
				Frame::Pos pozycja_pieszego_min; //w punkcie minimalnej odleglosci
				Frame::Pos pozycja_pojazdu_min; //w punkcie minimalnej odleglosci


				for(int k=0;k<f.size();k++)
				{
					//cout << f[k].pos.x << " " <<f[k].pos.y <<" - " << g[k].pos.x << " " << g[k].pos.y << "     " <<
					//sqrt((f[k].pos.x-g[k].pos.x)*(f[k].pos.x-g[k].pos.x) + (f[k].pos.y-g[k].pos.y)*(f[k].pos.y-g[k].pos.y)) << endl;

					//obliczanie odleg³oœci tylko jak pieszy jest na pasach! jak czeka z boku to nie
					//obliczenia WSZEDZIE! jak zakomentowane
					//if(g[k].pos.na_pasach==true)
					{
						float odl_tmp=sqrt((f[k].pos.x-g[k].pos.x)*(f[k].pos.x-g[k].pos.x) + (f[k].pos.y-g[k].pos.y)*(f[k].pos.y-g[k].pos.y));
						if(odl_tmp<odleglosc)
						{
							odleglosc=odl_tmp;
							predkosc=sqrt(f[k].vel.x*f[k].vel.x+f[k].vel.y*f[k].vel.y);
							pozycja_pieszego_min=g[k].pos;
							pozycja_pojazdu_min=f[k].pos;
						}
					}
				}
				//wpisanie minimalnej odleg³oœci dla tego pieszego i danego pojazdu
				piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc.push_back(odleglosc);
				piesi.pieszy[i].pojazdy.pp.predkosc_w_punkcie_minimalnej_odleglosci.push_back(predkosc);
				piesi.pieszy[i].pojazdy.pp.pozycja_pieszego_w_punkcie_minimalnej_odleglosci.push_back(pozycja_pieszego_min);
				piesi.pieszy[i].pojazdy.pp.pozycja_pojazdu_w_punkcie_minimalnej_odleglosci.push_back(pozycja_pojazdu_min);

				if(f.size()<1)
				{
					piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach.push_back("blad");
					continue;
				}

				Frame::Pos pozycja_pieszego_pocz=g[0].pos; //w pierwszej wspolnej klatce
				Frame::Pos pozycja_pieszego_konc=g[f.size()-1].pos; //w ostatniej wspolnej klatce
				Frame::Pos pozycja_pojazdu_pocz=f[0].pos; //w pierwszej wspolnej klatce
				Frame::Pos pozycja_pojazdu_konc=f[f.size()-1].pos; //w ostatniej wspolnej klatce



				//czy samochod przejechal po palcach czy po pietach, a moze przepuscil

				if(piesi.pieszy[i].trajektoria.kierunek.w_gore)
				{
					if(pozycja_pieszego_pocz.x<pozycja_pojazdu_pocz.x) //tak bedzie prawie zawsze
						//jesli pozycja minimalna pieszego jest mniejsza, to albo przepuscil albo po pietach
							if(pozycja_pieszego_min.x<pozycja_pojazdu_min.x) 
								//skoro ostatnia jest mniejsza, to pelne przepuszczenie
									if(pozycja_pieszego_konc.x<pozycja_pojazdu_konc.x) 
										piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach.push_back("przepuszczenie");
									else
										//jesli ostatnia nie jest mniejsza, to znaczy ze przepuscil i przejechal po pietach
										piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach.push_back("po pietach");
							else
								//jesli jednak pozycja minimalna pieszego NIE JEST MNIEJSZA niz samochodu
								//to znaczy ze jest po palcach ALBO po pietach
								if(pozycja_pieszego_min.y<pozycja_pojazdu_min.y) 
									piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach.push_back("po palcach");
								else
									piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach.push_back("po pietach");
					else
						piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach.push_back("hgw");
				}
				else
					if(piesi.pieszy[i].trajektoria.kierunek.w_dol)
					{
						if(pozycja_pieszego_pocz.x<pozycja_pojazdu_pocz.x) //tak bedzie prawie zawsze
							//jesli pozycja minimalna pieszego jest mniejsza, to albo przepuscil albo po pietach
								if(pozycja_pieszego_min.x<pozycja_pojazdu_min.x) 
									//skoro ostatnia jest mniejsza, to pelne przepuszczenie
										if(pozycja_pieszego_konc.x<pozycja_pojazdu_konc.x) 
											piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach.push_back("przepuszczenie");
										else
											//jesli ostatnia nie jest mniejsza, to znaczy ze przepuscil i przejechal po pietach
											piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach.push_back("po pietach");
								else
									//jesli jednak pozycja minimalna pieszego NIE JEST MNIEJSZA niz samochodu
									//to znaczy ze jest po palcach ALBO po pietach
									if(pozycja_pieszego_min.y>pozycja_pojazdu_min.y) 
										piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach.push_back("po palcach");
									else
										piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach.push_back("po pietach");
						else
							piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach.push_back("hgw");
					}
			}
		}
		//znalezienie najmniejszej odlegloœci ze wszystkich dla danego pieszego

		if(piesi.pieszy[i].pojazdy.lp.p1!=-1)
			for(int k=0;k<piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc.size();k++)
				if(piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc[k]<piesi.pieszy[i].konfliktowy_pojazd.minimalna_odleglosc)
				{
					piesi.pieszy[i].konfliktowy_pojazd.minimalna_odleglosc=piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc[k];
					Pojazd p=pojazdy_lp.pojazd[piesi.pieszy[i].pojazdy.lp.nr[k]];
					piesi.pieszy[i].konfliktowy_pojazd.predkosc_najblizszego_na_pasach=p.vel.na_pasach;
				}


				if(piesi.pieszy[i].pojazdy.pp.p1!=-1)
					for(int k=0;k<piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc.size();k++)
						if(piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[k]<piesi.pieszy[i].konfliktowy_pojazd.minimalna_odleglosc)
						{
							piesi.pieszy[i].konfliktowy_pojazd.minimalna_odleglosc=piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[k];
							Pojazd p=pojazdy_pp.pojazd[piesi.pieszy[i].pojazdy.pp.nr[k]];
							piesi.pieszy[i].konfliktowy_pojazd.predkosc_najblizszego_na_pasach=p.vel.na_pasach;
						}


	}
	return 1;
}

int Poligon::znajdz_maksymalne_opoznienie_pojazdow(float odl_min, float odl_max)
{

	int i;
#pragma omp parallel for
	for(i=0;i<piesi.pieszy.size();i++)
	{
		if(piesi.pieszy[i].pojazdy.razem==0) continue;
		//sprawdzanie wszystkich pojazdow na lewym pasie
		if(piesi.pieszy[i].pojazdy.lp.p1!=-1)
		{

			for(int j=piesi.pieszy[i].pojazdy.lp.p1;j<=piesi.pieszy[i].pojazdy.lp.p2;j++)
			{	

				//znalezienie wspolnych ramek dla pieszego i pojazdu
				vector<Frame> f;
				set_intersection(pojazdy_lp.pojazd[j].frame.begin(),pojazdy_lp.pojazd[j].frame.end(), piesi.pieszy[i].frame.begin(), piesi.pieszy[i].frame.end(),back_inserter(f));
				vector<Frame> g;
				set_intersection(piesi.pieszy[i].frame.begin(), piesi.pieszy[i].frame.end(),f.begin(), f.end(),back_inserter(g));

				float opoznienie=999;
				float min_predkosc=999;
				float max_predkosc=-999;
				for(int k=0;k<f.size();k++)
					if(f[k].pos.x<odl_max && f[k].pos.x>odl_min)
					{
						opoznienie=min(opoznienie,f[k].acc.x);
						min_predkosc=min(min_predkosc,f[k].vel.x);
						max_predkosc=max(max_predkosc,f[k].vel.x);
					}

					//wpisanie minimalnej predkosci, roznicy predkosci i opoznienia dla tego pieszego i danego pojazdu
					piesi.pieszy[i].pojazdy.lp.maksymalne_opoznienie.push_back(opoznienie);
					piesi.pieszy[i].pojazdy.lp.minimalna_predkosc.push_back(min_predkosc);
					piesi.pieszy[i].pojazdy.lp.maksymalna_predkosc.push_back(max_predkosc);
					piesi.pieszy[i].pojazdy.lp.roznica_predkosci.push_back(max_predkosc-min_predkosc);


			}

		}

		//to samo dla prawego pasa
		if(piesi.pieszy[i].pojazdy.pp.p1!=-1)
		{

			for(int j=piesi.pieszy[i].pojazdy.pp.p1;j<=piesi.pieszy[i].pojazdy.pp.p2;j++)
			{	

				//znalezienie wspolnych ramek dla pieszego i pojazdu
				vector<Frame> f;
				set_intersection(pojazdy_pp.pojazd[j].frame.begin(),pojazdy_pp.pojazd[j].frame.end(), piesi.pieszy[i].frame.begin(), piesi.pieszy[i].frame.end(),back_inserter(f));
				vector<Frame> g;
				set_intersection(piesi.pieszy[i].frame.begin(), piesi.pieszy[i].frame.end(),f.begin(), f.end(),back_inserter(g));

				float opoznienie=999;
				float min_predkosc=999;
				float max_predkosc=-999;
				for(int k=0;k<f.size();k++)
					if(f[k].pos.x<odl_max && f[k].pos.x>odl_min)
					{
						opoznienie=min(opoznienie,f[k].acc.x);
						min_predkosc=min(min_predkosc,f[k].vel.x);
						max_predkosc=max(max_predkosc,f[k].vel.x);
					}

					//wpisanie minimalnej predkosci, roznicy predkosci i opoznienia dla tego pieszego i danego pojazdu
					piesi.pieszy[i].pojazdy.pp.maksymalne_opoznienie.push_back(opoznienie);
					piesi.pieszy[i].pojazdy.pp.minimalna_predkosc.push_back(min_predkosc);
					piesi.pieszy[i].pojazdy.pp.maksymalna_predkosc.push_back(max_predkosc);
					piesi.pieszy[i].pojazdy.pp.roznica_predkosci.push_back(max_predkosc-min_predkosc);


			}
		}
		//znalezienie najgorszegosamochodu, czyli maj¹cego najwiêksze ujemne opoznienie	
		//znalezienie najmniejszego opoznienia ze wszystkich dla danego pieszego

		int nr_pojazdu_lp=-1; //który samochód najbardziej hamowa³ na lewym pasie
		if(piesi.pieszy[i].pojazdy.lp.p1!=-1)
			for(int k=0;k<piesi.pieszy[i].pojazdy.lp.maksymalne_opoznienie.size();k++)
				if(piesi.pieszy[i].pojazdy.lp.maksymalne_opoznienie[k]<piesi.pieszy[i].konfliktowy_pojazd.maksymalne_opoznienie)
				{
					piesi.pieszy[i].konfliktowy_pojazd.maksymalne_opoznienie=piesi.pieszy[i].pojazdy.lp.maksymalne_opoznienie[k];
					nr_pojazdu_lp=k;
				}

				int nr_pojazdu_pp=-1; //który samochód najbardziej hamowa³ na prawym pasie
				if(piesi.pieszy[i].pojazdy.pp.p1!=-1)
					for(int k=0;k<piesi.pieszy[i].pojazdy.pp.maksymalne_opoznienie.size();k++)
						if(piesi.pieszy[i].pojazdy.pp.maksymalne_opoznienie[k]<piesi.pieszy[i].konfliktowy_pojazd.maksymalne_opoznienie)
						{
							piesi.pieszy[i].konfliktowy_pojazd.maksymalne_opoznienie=piesi.pieszy[i].pojazdy.pp.maksymalne_opoznienie[k];
							nr_pojazdu_pp=k;
						}

						//przepisanie najmniejszej predkosci oraz roznicy predkosci z najbardziej hamuj¹cego samochodu do parametrów pieszego
						if(nr_pojazdu_lp!=-1) //znaczy, ¿e na tym pasie by³o najwiêksze hamowanie
						{
							piesi.pieszy[i].konfliktowy_pojazd.minimalna_predkosc_h=piesi.pieszy[i].pojazdy.lp.minimalna_predkosc[nr_pojazdu_lp];
							piesi.pieszy[i].konfliktowy_pojazd.roznica_predkosci_h=piesi.pieszy[i].pojazdy.lp.maksymalna_predkosc[nr_pojazdu_lp]-piesi.pieszy[i].pojazdy.lp.minimalna_predkosc[nr_pojazdu_lp];

						}
						else
							if(nr_pojazdu_pp!=-1) //jeœli oba ujemne, to znaczy ¿e w wybranym zakresie (3-15) samochodu nie by³o
							{
								piesi.pieszy[i].konfliktowy_pojazd.minimalna_predkosc_h=piesi.pieszy[i].pojazdy.pp.minimalna_predkosc[nr_pojazdu_pp];
								piesi.pieszy[i].konfliktowy_pojazd.roznica_predkosci_h=piesi.pieszy[i].pojazdy.pp.maksymalna_predkosc[nr_pojazdu_pp]-piesi.pieszy[i].pojazdy.pp.minimalna_predkosc[nr_pojazdu_pp];
							}


	}
	return 1;
}


int Poligon:: analizuj_trajektorie_pojazdow(float odl_min, float odl_max)
{

	pojazdy_pp.analizuj_predkosc(odl_min, odl_max, statystyki_dodatkowe.predkosc_pojazdow_w_punkcie.wspolrzedna_x_punktu);
	pojazdy_lp.analizuj_predkosc(odl_min, odl_max, statystyki_dodatkowe.predkosc_pojazdow_w_punkcie.wspolrzedna_x_punktu);
	licz_pojazdy_z_dziurami_i_daleko_od_przejscia();

	return 1;
}

void Poligon::licz_pojazdy_z_dziurami_i_daleko_od_przejscia(float rozmiar_dziury, float odleglosc_od_przejscia)
{
#pragma omp parallel for
	for(int i=0;i<pojazdy_pp.pojazd.size();i++)
	{
		if(pojazdy_pp.pojazd[i].dziura_w_trajektorii.rozmiar_x>=rozmiar_dziury)
			pojazdy_pp.liczba_pojazdow_z_dziurami++;
		if(pojazdy_pp.pojazd[i].dziura_w_trajektorii.ostatnia_pozycja_x>odleglosc_od_przejscia)
			pojazdy_pp.liczba_pojazdow_z_dala_od_pasow++;

	}
#pragma omp parallel for
	for(int i=0;i<pojazdy_lp.pojazd.size();i++)
	{
		if(pojazdy_lp.pojazd[i].dziura_w_trajektorii.rozmiar_x>rozmiar_dziury)
			pojazdy_lp.liczba_pojazdow_z_dziurami++;
		if(pojazdy_lp.pojazd[i].dziura_w_trajektorii.ostatnia_pozycja_x>odleglosc_od_przejscia)
			pojazdy_lp.liczba_pojazdow_z_dala_od_pasow++;
	}

}
void Poligon::zbieraj_statystyki_pieszych()
{

	auto it = piesi.pieszy.begin();
	int numer_dnia=-1;

	while(it != piesi.pieszy.end()) 
	{
		numer_dnia++;
		string data=data_z_systemtime(it->t1);
		statystyka.dzienne[numer_dnia].data=data; //zapisanie daty do statystyki
		do
		{
			if(data!=data_z_systemtime(it->t1)) break;
			if(it->trajektoria.rodzaj.duch==false && it->trajektoria.rodzaj.pieszy_na_samochodzie==false)
			{
				int godzina=it->t1.wHour;
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.razem_poprawna++;
				statystyka.dzienne[numer_dnia].na_dzien.piesi.razem_poprawna++;
				statystyka.calosciowe.piesi.razem_poprawna++;
				if(it->pojazdy.razem>0)
				{
					statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.razem_poprawna_z_samochodem++;
					statystyka.dzienne[numer_dnia].na_dzien.piesi.razem_poprawna_z_samochodem++;
				}
				else
				{
					statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.razem_poprawna_bez_samochodu++;
					statystyka.dzienne[numer_dnia].na_dzien.piesi.razem_poprawna_bez_samochodu++;
				}

				float srednia=it->trajektoria.predkosc.srednia;
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.predkosc.srednia+=it->trajektoria.predkosc.srednia;
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.predkosc.odch_st+=it->trajektoria.predkosc.odch_st;
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.predkosc.srednia_z_pozycji+=it->trajektoria.predkosc.srednia_z_pozycji;
				statystyka.dzienne[numer_dnia].na_dzien.piesi.predkosc.srednia+=it->trajektoria.predkosc.srednia;
				statystyka.dzienne[numer_dnia].na_dzien.piesi.predkosc.odch_st+=it->trajektoria.predkosc.odch_st;
				statystyka.dzienne[numer_dnia].na_dzien.piesi.predkosc.srednia_z_pozycji+=it->trajektoria.predkosc.srednia_z_pozycji;

				if(filtry.w_wynikach_maja_byc.konflikty_hamowanie==true)
				{
					if(it->konfliktowy_pojazd.maksymalne_opoznienie<filtry.kryteria.hamowanie.opoznienie
						//&& it->konfliktowy_pojazd.minimalna_odleglosc<filtry.kryteria.hamowanie.odleglosc_od_pieszego //nie, bo dotyczy niekoniecznie najbardziej hamuj¹cego samochodu
							&& it->konfliktowy_pojazd.minimalna_predkosc_h<filtry.kryteria.hamowanie.predkosc
							&& it->konfliktowy_pojazd.roznica_predkosci_h>filtry.kryteria.hamowanie.roznica_predkosci
							)
					{
						//wy¿ej dodatkowe filtry, ¿eby fa³szywe ostre hamowania wywaliæ

						//aktualizacja statystyk
						statystyka.dzienne[numer_dnia].na_dzien.liczba_ostrych_hamowan++;
						statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].liczba_ostrych_hamowan++;

						//zapisanie, ¿e dany pieszy jest w konflikcie
						it->konflikt.hamowanie=true;
					}
				}
				else
					if(filtry.w_wynikach_maja_byc.konflikty_odleglosc==true)
					{
						if(it->konfliktowy_pojazd.minimalna_odleglosc<filtry.kryteria.odleglosc_i_predkosc.odleglosc_od_pieszego
							&&it->konfliktowy_pojazd.predkosc_najblizszego_na_pasach>filtry.kryteria.odleglosc_i_predkosc.predkosc_pojazdu
							)
						{
							//aktualizacja statystyk
							statystyka.dzienne[numer_dnia].na_dzien.liczba_ostrych_hamowan++;
							statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].liczba_ostrych_hamowan++;
							//zapisanie, ¿e dany pieszy jest w konflikcie
							it->konflikt.smigniecie =true;

						}
					}

			}
			else
			{
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.razem_niepoprawna++;
				statystyka.dzienne[numer_dnia].na_dzien.piesi.razem_niepoprawna++;
				statystyka.calosciowe.piesi.razem_niepoprawna++;
			}
			if(it->trajektoria.rodzaj.dobra==true)	
			{
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.dobra++;
				statystyka.dzienne[numer_dnia].na_dzien.piesi.dobra++;
				statystyka.calosciowe.piesi.dobra++;
			}
			else
				if(it->trajektoria.rodzaj.krotka==true)
				{
					statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.krotka++;
					statystyka.dzienne[numer_dnia].na_dzien.piesi.krotka++;
					statystyka.calosciowe.piesi.krotka++;
				}
				/*else
				if(it->trajektoria.rodzaj.bardzo_krotka==true)
				{
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.bardzo_krotka++;
				statystyka.dzienne[numer_dnia].na_dzien.piesi.bardzo_krotka++;
				}
				else
				if(it->trajektoria.rodzaj.inna==true)	
				{
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.inna++;
				statystyka.dzienne[numer_dnia].na_dzien.piesi.inna++;
				}*/
				else
					if(it->trajektoria.rodzaj.duch==true)	
					{
						statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.duch++;
						statystyka.dzienne[numer_dnia].na_dzien.piesi.duch++;
						statystyka.calosciowe.piesi.duch++;
					}
					else
						if(it->trajektoria.rodzaj.pieszy_na_samochodzie==true)	
						{
							statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].piesi.pieszy_na_samochodzie++;
							statystyka.dzienne[numer_dnia].na_dzien.piesi.pieszy_na_samochodzie++;
							statystyka.calosciowe.piesi.pieszy_na_samochodzie++;
						}

						it++;
		}
		while(it != piesi.pieszy.end());

	}
	//aby policzyæ œredni¹ prêdkoœæ trzeba teraz podzieliæ zsumowane prêdkoœci przez liczbê pieszych
	//dla ka¿dego dnia i ka¿dej godziny


	numer_dnia=0;

	while(statystyka.dzienne[numer_dnia].data!="000000")
	{
		for(int g=0;g<24;g++)
		{
			if(statystyka.dzienne[numer_dnia].na_godzine[g].piesi.razem_poprawna>0)
			{
				statystyka.dzienne[numer_dnia].na_godzine[g].piesi.predkosc.srednia /=statystyka.dzienne[numer_dnia].na_godzine[g].piesi.razem_poprawna;
				statystyka.dzienne[numer_dnia].na_godzine[g].piesi.predkosc.odch_st /=statystyka.dzienne[numer_dnia].na_godzine[g].piesi.razem_poprawna;
				statystyka.dzienne[numer_dnia].na_godzine[g].piesi.predkosc.srednia_z_pozycji /=statystyka.dzienne[numer_dnia].na_godzine[g].piesi.razem_poprawna;
			}


		}
		statystyka.dzienne[numer_dnia].na_dzien.piesi.predkosc.srednia/=statystyka.dzienne[numer_dnia].na_dzien.piesi.razem_poprawna;
		statystyka.dzienne[numer_dnia].na_dzien.piesi.predkosc.odch_st/=statystyka.dzienne[numer_dnia].na_dzien.piesi.razem_poprawna;

		statystyka.dzienne[numer_dnia].na_dzien.piesi.predkosc.srednia_z_pozycji/=statystyka.dzienne[numer_dnia].na_dzien.piesi.razem_poprawna;

		numer_dnia++;
	}
}

void Poligon::zbieraj_statystyki_pojazdow()
{
	//lewy pas oraz uzupe³nienie statystyki zbiorczej
	auto it = pojazdy_lp.pojazd.begin();
	int numer_dnia=-1;
	string data;

	while(it != pojazdy_lp.pojazd.end()) 
	{
		numer_dnia++;
		data=data_z_systemtime(it->t1);
		statystyka.dzienne[numer_dnia].data=data; //zapisanie daty do statystyki
		do
		{
			if(data!=data_z_systemtime(it->t1)) break;

			statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.razem++;
			statystyka.dzienne[numer_dnia].na_dzien.pojazdy.razem++;
			if(it->vel.w_punkcie!=-999) //jeœli nie uda³o siê wyznaczyæ prêdkoœci w punkcie to nie dodawaj jej do œredniej
			{
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp_w_punkcie.razem++;
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.predkosc_w_punkcie+=it->vel.w_punkcie;
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy.predkosc_w_punkcie+=it->vel.w_punkcie;
			}


			statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_lp.razem++;
			statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.razem++;
			if(it->vel.w_punkcie!=-999) //jeœli nie uda³o siê wyznaczyæ prêdkoœci w punkcie to nie dodawaj jej do œredniej
			{
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_lp_w_punkcie.razem++;
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_lp.predkosc_w_punkcie+=it->vel.w_punkcie;
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.predkosc_w_punkcie+=it->vel.w_punkcie;
			}
			statystyka.calosciowe.pojazdy_lp.razem++;
			statystyka.calosciowe.pojazdy.razem++;

			if(it->type=="car")
			{
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_lp.osobowe++;
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.osobowe++;
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.osobowe++;
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy.osobowe++;
				statystyka.calosciowe.pojazdy_lp.osobowe++;
				statystyka.calosciowe.pojazdy.osobowe++;

			}
			else
				if(it->type=="bus")
				{
					statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_lp.autobusy++;
					statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.autobusy++;
					statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.autobusy++;
					statystyka.dzienne[numer_dnia].na_dzien.pojazdy.autobusy++;
					statystyka.calosciowe.pojazdy_lp.autobusy++;
					statystyka.calosciowe.pojazdy.autobusy++;
				}
				else
					if(it->type=="pickup")
					{
						statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_lp.polciezarowe++;
						statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.polciezarowe++;
						statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.polciezarowe++;
						statystyka.dzienne[numer_dnia].na_dzien.pojazdy.polciezarowe++;
						statystyka.calosciowe.pojazdy_lp.polciezarowe++;
						statystyka.calosciowe.pojazdy.polciezarowe++;
					}
					else
						if(it->type=="truck")
						{
							statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_lp.ciezarowe++;
							statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.ciezarowe++;
							statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.ciezarowe++;
							statystyka.dzienne[numer_dnia].na_dzien.pojazdy.ciezarowe++;
							statystyka.calosciowe.pojazdy_lp.ciezarowe++;
							statystyka.calosciowe.pojazdy.ciezarowe++;
						}
						else

						{
							statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_lp.pozostale++;
							statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.pozostale++;
							statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.pozostale++;
							statystyka.dzienne[numer_dnia].na_dzien.pojazdy.pozostale++;
							statystyka.calosciowe.pojazdy_lp.pozostale++;
							statystyka.calosciowe.pojazdy.pozostale++;
						}

						it++;
		}
		while(it != pojazdy_lp.pojazd.end());
	}

	//to samo dla prawego pasa
	//prawy pas oraz uzupe³nienie statystyki zbiorczej
	it = pojazdy_pp.pojazd.begin();
	numer_dnia=-1;

	while(it != pojazdy_pp.pojazd.end()) 
	{
		numer_dnia++;
		data=data_z_systemtime(it->t1);
		statystyka.dzienne[numer_dnia].data=data; //zapisanie daty do statystyki
		do
		{
			if(data!=data_z_systemtime(it->t1)) break;


			statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.razem++;
			statystyka.dzienne[numer_dnia].na_dzien.pojazdy.razem++;
			if(it->vel.w_punkcie!=-999) //jeœli nie uda³o siê wyznaczyæ prêdkoœci w punkcie to nie dodawaj jej do œredniej
			{
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp_w_punkcie.razem++;
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.predkosc_w_punkcie+=it->vel.w_punkcie;
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy.predkosc_w_punkcie+=it->vel.w_punkcie;
			}
			statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp.razem++;
			statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.razem++;
			if(it->vel.w_punkcie!=-999) //jeœli nie uda³o siê wyznaczyæ prêdkoœci w punkcie to nie dodawaj jej do œredniej
			{
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp_w_punkcie.razem++;
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp.predkosc_w_punkcie+=it->vel.w_punkcie;
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.predkosc_w_punkcie+=it->vel.w_punkcie;
			}
			statystyka.calosciowe.pojazdy_pp.razem++;
			statystyka.calosciowe.pojazdy.razem++;

			if(it->type=="car")
			{
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp.osobowe++;
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.osobowe++;
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.osobowe++;
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy.osobowe++;
				statystyka.calosciowe.pojazdy_pp.osobowe++;
				statystyka.calosciowe.pojazdy.osobowe++;
			}
			else
				if(it->type=="bus")
				{
					statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp.autobusy++;
					statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.autobusy++;
					statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.autobusy++;
					statystyka.dzienne[numer_dnia].na_dzien.pojazdy.autobusy++;
					statystyka.calosciowe.pojazdy_pp.autobusy++;
					statystyka.calosciowe.pojazdy.autobusy++;
				}
				else
					if(it->type=="pickup")
					{
						statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp.polciezarowe++;
						statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.polciezarowe++;
						statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.polciezarowe++;
						statystyka.dzienne[numer_dnia].na_dzien.pojazdy.polciezarowe++;
						statystyka.calosciowe.pojazdy_pp.polciezarowe++;
						statystyka.calosciowe.pojazdy.polciezarowe++;
					}
					else
						if(it->type=="truck")
						{
							statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp.ciezarowe++;
							statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.ciezarowe++;
							statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.ciezarowe++;
							statystyka.dzienne[numer_dnia].na_dzien.pojazdy.ciezarowe++;
							statystyka.calosciowe.pojazdy_pp.ciezarowe++;
							statystyka.calosciowe.pojazdy.ciezarowe++;
						}
						else
						{
							statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp.pozostale++;
							statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.pozostale++;
							statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.pozostale++;
							statystyka.dzienne[numer_dnia].na_dzien.pojazdy.pozostale++;
							statystyka.calosciowe.pojazdy_pp.pozostale++;
							statystyka.calosciowe.pojazdy.pozostale++;
						}		

						it++;
		}
		while(it != pojazdy_pp.pojazd.end());
	}



	//teraz trzeba podzielic srednie predkosci w punkcie przez liczbe pojazdow
	numer_dnia=0;

	while(statystyka.dzienne[numer_dnia].data!="000000")
	{
		for(int g=0;g<24;g++)
		{
			if(statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_lp_w_punkcie.razem+statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_pp_w_punkcie.razem>0)
			{
				if(statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_lp_w_punkcie.razem>0)
					statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_lp.predkosc_w_punkcie/=statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_lp_w_punkcie.razem;
				if(statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_pp_w_punkcie.razem>0)
					statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_pp.predkosc_w_punkcie/=statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_pp_w_punkcie.razem;

				statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy.predkosc_w_punkcie/=(statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_lp_w_punkcie.razem+statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_pp_w_punkcie.razem);
			}


		}

		statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.predkosc_w_punkcie/=statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp_w_punkcie.razem;	
		statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.predkosc_w_punkcie/=statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp_w_punkcie.razem;
		statystyka.dzienne[numer_dnia].na_dzien.pojazdy.predkosc_w_punkcie/=(statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp_w_punkcie.razem+statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp_w_punkcie.razem);

		numer_dnia++;
	}

	//srednie predkosci w punkcie godzinowe i dzienne policzone
	//teraz wariancja

	//lewy pas
	it = pojazdy_lp.pojazd.begin();
	numer_dnia=-1;
	while(it != pojazdy_lp.pojazd.end()) 
	{
		numer_dnia++;
		data=data_z_systemtime(it->t1);
		do
		{
			if(data!=data_z_systemtime(it->t1)) break;

			float godz=it->t1.wHour;
			float pre=it->vel.w_punkcie;
			if(it->vel.w_punkcie!=-999) //jeœli nie uda³o siê wyznaczyæ prêdkoœci w punkcie to nie dodawaj jej do œredniej
			{
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.wariancja_w_punkcie+=
					(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.predkosc_w_punkcie)*(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.predkosc_w_punkcie);
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy.wariancja_w_punkcie+=
					(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.predkosc_w_punkcie)*(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.predkosc_w_punkcie);

			}

			if(it->vel.w_punkcie!=-999) //jeœli nie uda³o siê wyznaczyæ prêdkoœci w punkcie to nie dodawaj jej do œredniej
			{
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_lp.wariancja_w_punkcie+=
					(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_lp.predkosc_w_punkcie)*(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_lp.predkosc_w_punkcie);
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.wariancja_w_punkcie+=
					(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.predkosc_w_punkcie)*(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.predkosc_w_punkcie);
			}


			it++;
		}
		while(it != pojazdy_lp.pojazd.end());
	}

	//prawy pas
	it = pojazdy_pp.pojazd.begin();
	numer_dnia=-1;
	while(it != pojazdy_pp.pojazd.end()) 
	{
		numer_dnia++;
		data=data_z_systemtime(it->t1);
		do
		{
			if(data!=data_z_systemtime(it->t1)) break;

			float godz=it->t1.wHour;
			float pre=it->vel.w_punkcie;
			if(it->vel.w_punkcie!=-999) //jeœli nie uda³o siê wyznaczyæ prêdkoœci w punkcie to nie dodawaj jej do œredniej
			{
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.wariancja_w_punkcie+=
					(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.predkosc_w_punkcie)*(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.predkosc_w_punkcie);
				statystyka.dzienne[numer_dnia].na_dzien.pojazdy.wariancja_w_punkcie+=
					(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_dzien.pojazdy.predkosc_w_punkcie)*(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_dzien.pojazdy.predkosc_w_punkcie);

			}

			if(it->vel.w_punkcie!=-999) //jeœli nie uda³o siê wyznaczyæ prêdkoœci w punkcie to nie dodawaj jej do œredniej
			{
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp.wariancja_w_punkcie+=
					(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp.predkosc_w_punkcie)*(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp.predkosc_w_punkcie);
				statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy.wariancja_w_punkcie+=
					(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp.predkosc_w_punkcie)*(it->vel.w_punkcie-statystyka.dzienne[numer_dnia].na_godzine[it->t1.wHour].pojazdy_pp.predkosc_w_punkcie);
			}


			it++;
		}
		while(it != pojazdy_pp.pojazd.end());
	}

	//odchylenie standardowe

	numer_dnia=0;

	while(statystyka.dzienne[numer_dnia].data!="000000")
	{
		for(int g=0;g<24;g++)
		{
			if(statystyka.dzienne[numer_dnia].na_godzine[g].piesi.razem_poprawna>0)
			{
				if(statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_lp_w_punkcie.razem>0)
					statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_lp.wariancja_w_punkcie /=statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_lp_w_punkcie.razem;
				if(statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_pp_w_punkcie.razem>0)
					statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_pp.wariancja_w_punkcie /=statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_pp_w_punkcie.razem;
				statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_lp.odch_st_w_punkcie =sqrt(statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_lp.wariancja_w_punkcie);
				statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_pp.odch_st_w_punkcie =sqrt(statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_pp.wariancja_w_punkcie);				

				if((statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_lp_w_punkcie.razem+statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_pp_w_punkcie.razem)>0)
					statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy.wariancja_w_punkcie /=(statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_lp_w_punkcie.razem+statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy_pp_w_punkcie.razem);
				statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy.odch_st_w_punkcie =sqrt(statystyka.dzienne[numer_dnia].na_godzine[g].pojazdy.wariancja_w_punkcie);


			}


		}
		if(statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp_w_punkcie.razem>0)
			statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.wariancja_w_punkcie /=statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp_w_punkcie.razem;
		if(statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp_w_punkcie.razem>0)
			statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.wariancja_w_punkcie /=statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp_w_punkcie.razem;
		statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.odch_st_w_punkcie = sqrt(statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp.wariancja_w_punkcie);
		statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.odch_st_w_punkcie =sqrt(statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp.wariancja_w_punkcie);

		if((statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp_w_punkcie.razem+statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp_w_punkcie.razem)>0)
			statystyka.dzienne[numer_dnia].na_dzien.pojazdy.wariancja_w_punkcie /=(statystyka.dzienne[numer_dnia].na_dzien.pojazdy_lp_w_punkcie.razem+statystyka.dzienne[numer_dnia].na_dzien.pojazdy_pp_w_punkcie.razem);
		statystyka.dzienne[numer_dnia].na_dzien.pojazdy.odch_st_w_punkcie =sqrt(statystyka.dzienne[numer_dnia].na_dzien.pojazdy.wariancja_w_punkcie);

		numer_dnia++;
	}

}

void Poligon::zbieraj_statystyki_dodatkowe()
{

}

void Poligon::zbieraj_statystyki()
{
	zbieraj_statystyki_pieszych();
	zbieraj_statystyki_pojazdow();
	//zbieraj_statystyki_dodatkowe();
}

float Poligon::znajdz_najwieksze_opoznienie_pojazdu_dla_pieszego(int nr_pieszego)
{
	float acc_min_total=999;
#pragma omp parallel for
	for(int k=0;k<piesi.pieszy[nr_pieszego].pojazdy.pp.liczba;k++)
	{
		int nr_pojazdu=piesi.pieszy[nr_pieszego].pojazdy.pp.nr[k];
		if(pojazdy_pp.pojazd[nr_pojazdu].acc.min_x<acc_min_total) acc_min_total=pojazdy_pp.pojazd[nr_pojazdu].acc.min_x;
	}
#pragma omp parallel for
	for(int k=0;k<piesi.pieszy[nr_pieszego].pojazdy.lp.liczba;k++)
	{
		int nr_pojazdu=piesi.pieszy[nr_pieszego].pojazdy.lp.nr[k];
		if(pojazdy_lp.pojazd[nr_pojazdu].acc.min_x<acc_min_total) acc_min_total=pojazdy_lp.pojazd[nr_pojazdu].acc.min_x;
	}

	return(acc_min_total);
}


#define WYPIS	plik_wy << "<br>" << licznik << ". " << piesi.pieszy[i].id << ":  nr: " << i << " czas: " << piesi.pieszy[i].t1<<" razem: "<< piesi.pieszy[i].pojazdy.razem <<" prawy: " <<piesi.pieszy[i].pojazdy.pp.liczba << " lewy: " << piesi.pieszy[i].pojazdy.lp.liczba<<endl; \
	/*plik_wy << "<br>przebyta odleglosc calkowita: "<< piesi.pieszy[i].trajektoria.dlugosc.calkowita << ", efektywna: " << piesi.pieszy[i].trajektoria.dlugosc.efektywna <<", w pionie: " << piesi.pieszy[i].trajektoria.dlugosc.w_pionie<< ", w poziomie: " << piesi.pieszy[i].trajektoria.dlugosc.w_poziomie <<endl;*/ \
	/*plik_wy << " wsp. drgan w pionie: " << piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_pionie<< " wsp. drgan w poziomie: " << piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_poziomie <<" srednia predkosc: "<<piesi.pieszy[i].trajektoria.predkosc.srednia <<endl;*/ \
	/*plik_wy << " polozenie poziome min: " <<piesi.pieszy[i].trajektoria.pozycja.pozioma.minimalna << " polozenie poziome sr: " << piesi.pieszy[i].trajektoria.pozycja.pozioma.srednia << " max. polozenie: " <<piesi.pieszy[i].trajektoria.pozycja.pozioma.maksymalna <<endl;*/ \
	plik_wy << "<br> Minimalna odleglosc: " << piesi.pieszy[i].konfliktowy_pojazd.minimalna_odleglosc; \
	plik_wy << "<br>Kolejne pojazdy na prawym pasie: "; \
	for(int k=0;k<piesi.pieszy[i].pojazdy.pp.liczba;k++) \
{ \
	string nazwa_wlasciwa = pojazdy_pp.pojazd[piesi.pieszy[i].pojazdy.pp.nr[k]].sciezka +"/"+ pojazdy_pp.pojazd[piesi.pieszy[i].pojazdy.pp.nr[k]].id; \
	plik_wy << "<a href=\"" << nazwa_wlasciwa << ".xml" << "\">" << "XML" <<"</a>, " << "<a href=\"" << nazwa_wlasciwa << ".mkv" << "\">" << "MKV" <<"</a> "<< " "; \
} \
	plik_wy << "<br>Kolejne pojazdy na lewym pasie: "; \
	for(int k=0;k<piesi.pieszy[i].pojazdy.lp.liczba;k++) \
{ \
	string nazwa_wlasciwa = pojazdy_lp.pojazd[piesi.pieszy[i].pojazdy.lp.nr[k]].sciezka +"/"+ pojazdy_lp.pojazd[piesi.pieszy[i].pojazdy.lp.nr[k]].id; \
	plik_wy << "<a href=\"" << nazwa_wlasciwa << ".xml" << "\">" << "XML" <<"</a>, " << "<a href=\"" << nazwa_wlasciwa << ".mkv" << "\">" << "MKV" <<"</a> "<< " "; \
} \
	plik_wy << "<br>Minimalne odleglosci od kolejnych samochodow na prawym pasie: "; \
	for(int k=0;k<piesi.pieszy[i].pojazdy.pp.liczba;k++) \
	plik_wy << piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[k] << "  "; \
	plik_wy << "<br>Minimalne odleglosci od kolejnych samochodow na lewym pasie: "; \
	for(int k=0;k<piesi.pieszy[i].pojazdy.lp.liczba;k++) \
	plik_wy << piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc[k] << "  "; \
	plik_wy <<"<br> <video width=\"1000\"  controls>" <<endl<< "<source src=\"" << piesi.pieszy[i].sciezka<< "/"<<piesi.pieszy[i].id <<".mkv\"></video><br>"<<endl; \
	licznik++; \
	if(licznik%50==0) \
{ \
	plik_wy << "<br><br>Nastepna czesc: <a href=\""<<nazwa_pliku_wyjsciowego << licznik_plikow << ".htm\">"<<nazwa_pliku_wyjsciowego << licznik_plikow << ".htm" <<"</a> "; \
	plik_wy << "</body></html>"; \
	plik_wy.close(); \
	stringstream ss; \
	ss<<katalog_wyjsciowy<<"/"<<nazwa_pliku_wyjsciowego<<licznik_plikow<<".htm"; \
	plik_wy.open( ss.str(),ios::out); \
	licznik_plikow++; \
} \


void Poligon::zapisz_wyniki_filtracji()
{
	fstream plik_wy;
	CreateDirectoryA(katalog_wyjsciowy.c_str(),0);
	string nazwa_pliku_wyjsciowego;
	if(filtry.w_wynikach_maja_byc.wszystko==true)
		nazwa_pliku_wyjsciowego="wszystko_";
	else
		if(filtry.w_wynikach_maja_byc.poprawne==true)
			nazwa_pliku_wyjsciowego="poprawne_";
		else
			if(filtry.w_wynikach_maja_byc.niepoprawne==true)
				nazwa_pliku_wyjsciowego="niepoprawne_";
			else
				if(filtry.w_wynikach_maja_byc.konflikty_odleglosc==true)
					nazwa_pliku_wyjsciowego="konflikty_odleglosc_";
				else
					if(filtry.w_wynikach_maja_byc.konflikty_hamowanie==true)
						nazwa_pliku_wyjsciowego="konflikty_hamowanie_";
	if(zakres_przetwarzania.poczatek!="")
		nazwa_pliku_wyjsciowego+=zakres_przetwarzania.poczatek+"-"+zakres_przetwarzania.koniec+"_";
	else
		nazwa_pliku_wyjsciowego+=zakres_przetwarzania.nazwa+"_";

	plik_wy.open(katalog_wyjsciowy+"/"+nazwa_pliku_wyjsciowego+"0.htm",ios::out);
	if( plik_wy.good() == false )
		cout << "Nie moge pisac w pliku wyjsciowym!" << endl;
	else
		plik_wy << "<html><body>" <<endl;

	if(plik_wy.good())
	{
		plik_wy << endl << "<br>Liczba wykrytych pieszych nieprawidlowych: " << statystyka.calosciowe.piesi.razem_niepoprawna <<"<br>"<<endl;
		plik_wy << endl << "<br>Procent wykrytych pieszych nieprawidlowych: " << 100*statystyka.calosciowe.piesi.razem_niepoprawna/piesi.pieszy.size() <<"<br>"<<endl;
		plik_wy << endl << "<br>Liczba wykrytych pieszych bez samochodu: " << piesi.liczba_pieszych_bez_samochodow <<endl;
		plik_wy << endl << "<br>Procent wykrytych pieszych bez samochodu: " << 100*piesi.liczba_pieszych_bez_samochodow/piesi.pieszy.size() <<endl;
	}

	int licznik=1;
	int licznik_plikow=1;
	int sprawdzaj_przyspieszenie=1;

	int licznik_konfliktow=0;

	if(filtry.w_wynikach_maja_byc.wszystko==true)
	{

		for(int i=0;i<piesi.pieszy.size();i++)
		{
			WYPIS
		}
	}
	else


		if(filtry.w_wynikach_maja_byc.niepoprawne==true)
			for(int i=0;i<piesi.pieszy.size();i++)
			{
				if(piesi.pieszy[i].trajektoria.rodzaj.duch==true)
				{
					WYPIS
				}
			}
		else


			if(filtry.w_wynikach_maja_byc.poprawne==true)
				for(int i=0;i<piesi.pieszy.size();i++)
				{
					if(piesi.pieszy[i].trajektoria.rodzaj.dobra==true)
					{
						WYPIS
					}
				}
			else
#pragma region Hamowanie
				if(filtry.w_wynikach_maja_byc.konflikty_hamowanie)
				{

					for(int i=0;i<piesi.pieszy.size();i++)
					{
						//pominiecie pieszych bez konfliktów typu hamowanie
						if(piesi.pieszy[i].konflikt.hamowanie==false)
							continue;

						licznik_konfliktow++;

						if(plik_wy.good())
						{
							plik_wy << "<br>" << licznik << ". " << piesi.pieszy[i].id << ":  nr: " << i << " czas: " << piesi.pieszy[i].t1<<" razem: "<< piesi.pieszy[i].pojazdy.razem <<" prawy: " <<piesi.pieszy[i].pojazdy.pp.liczba << " lewy: " << piesi.pieszy[i].pojazdy.lp.liczba<<endl;
							plik_wy << "<br> Minimalna odleglosc: " << piesi.pieszy[i].konfliktowy_pojazd.minimalna_odleglosc;
							plik_wy << "<br>Kolejne pojazdy na prawym pasie: ";
							for(int k=0;k<piesi.pieszy[i].pojazdy.pp.liczba;k++)
							{
								string nazwa_wlasciwa = pojazdy_pp.pojazd[piesi.pieszy[i].pojazdy.pp.nr[k]].sciezka +"/"+ pojazdy_pp.pojazd[piesi.pieszy[i].pojazdy.pp.nr[k]].id;
								plik_wy << "<a href=\"" << nazwa_wlasciwa << ".xml" << "\">" << "XML" <<"</a>, " << "<a href=\"" << nazwa_wlasciwa << ".mkv" << "\">" << "MKV" <<"</a> "<< " ";

							}
							plik_wy << "<br>Kolejne pojazdy na lewym pasie: ";
							for(int k=0;k<piesi.pieszy[i].pojazdy.lp.liczba;k++)
							{
								string nazwa_wlasciwa = pojazdy_lp.pojazd[piesi.pieszy[i].pojazdy.lp.nr[k]].sciezka +"/"+ pojazdy_lp.pojazd[piesi.pieszy[i].pojazdy.lp.nr[k]].id;
								plik_wy << "<a href=\"" << nazwa_wlasciwa << ".xml" << "\">" << "XML" <<"</a>, " << "<a href=\"" << nazwa_wlasciwa << ".mkv" << "\">" << "MKV" <<"</a> "<< " ";
							}
							plik_wy << "<br>Minimalne odleglosci od kolejnych samochodow na prawym pasie: ";
							for(int k=0;k<piesi.pieszy[i].pojazdy.pp.liczba;k++)
								plik_wy << piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[k] << "  ";
							plik_wy << "<br>Minimalne odleglosci od kolejnych samochodow na lewym pasie: ";
							for(int k=0;k<piesi.pieszy[i].pojazdy.lp.liczba;k++)
								plik_wy << piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc[k] << "  ";

							plik_wy << "<br>Maksymalne opoznienia (oraz delta Vx, Vxmax-Vxmin) kolejnych samochodow na prawym pasie: ";
							for(int k=0;k<piesi.pieszy[i].pojazdy.pp.liczba;k++)
							{
								int nr_pojazdu=piesi.pieszy[i].pojazdy.pp.nr[k];

								if(pojazdy_pp.pojazd[nr_pojazdu].vel.min_x<.2)
									plik_wy<<"<font color=\"red\">";
								plik_wy << pojazdy_pp.pojazd[nr_pojazdu].acc.min_x << "("<< pojazdy_pp.pojazd[nr_pojazdu].vel.max_x-pojazdy_pp.pojazd[nr_pojazdu].vel.min_x <<", "<<pojazdy_pp.pojazd[nr_pojazdu].vel.max_x<<"-"<<pojazdy_pp.pojazd[nr_pojazdu].vel.min_x <<")  ";

								if(pojazdy_pp.pojazd[nr_pojazdu].vel.min_x<.2)
									plik_wy<<"</font>";


							}
							plik_wy << "<br>Maksymalne opoznienia (oraz delta Vx, Vxmax-Vxmin) kolejnych samochodow na lewym pasie: ";
							for(int k=0;k<piesi.pieszy[i].pojazdy.lp.liczba;k++)
							{
								int nr_pojazdu=piesi.pieszy[i].pojazdy.lp.nr[k];
								if(pojazdy_lp.pojazd[nr_pojazdu].vel.min_x<.2)
									plik_wy<<"<font color=\"red\">";
								plik_wy << pojazdy_lp.pojazd[nr_pojazdu].acc.min_x << "("<< pojazdy_lp.pojazd[nr_pojazdu].vel.max_x-pojazdy_lp.pojazd[nr_pojazdu].vel.min_x<<", "<<pojazdy_lp.pojazd[nr_pojazdu].vel.max_x<<"-"<<pojazdy_lp.pojazd[nr_pojazdu].vel.min_x<<")  ";
								if(pojazdy_lp.pojazd[nr_pojazdu].vel.min_x<.2)
									plik_wy<<"</font>";
							}


							plik_wy <<"<br> <video width=\"1000\"  controls>" <<endl<< "<source src=\"" << piesi.pieszy[i].sciezka<< "/"<<piesi.pieszy[i].id <<".mkv\"></video><br>"<<endl;
						}
						licznik++;
						if(licznik%50==0)
						{
							plik_wy << "<br><br>Nastepna czesc: <a href=\""<<nazwa_pliku_wyjsciowego << licznik_plikow << ".htm\">"<<nazwa_pliku_wyjsciowego << licznik_plikow << ".htm" <<"</a> ";
							plik_wy << "</body></html>";
							plik_wy.close();
							stringstream ss;
							ss<<nazwa_pliku_wyjsciowego<<licznik_plikow<<".htm";
							plik_wy.open(katalog_wyjsciowy+"/"+ ss.str(),ios::out);
							licznik_plikow++;
						}
					}
				}  
#pragma endregion
				else
					if(filtry.w_wynikach_maja_byc.konflikty_odleglosc)
					{




						//zliczenie konfliktów
						int licznik_konfliktow=0;


						if(plik_wy.good())
						{
							plik_wy<<"\nLiczba wykrytych pieszych: "<<piesi.pieszy.size()<<endl;

							plik_wy<<"Liczba wykrytych konfliktow: "<<licznik_konfliktow<<endl;


							//wypisanie kolejnych konfliktów
							for(int i=0;i<piesi.pieszy.size();i++)
							{
								if(piesi.pieszy[i].konflikt.smigniecie==false
									|| piesi.pieszy[i].trajektoria.rodzaj.dobra==false
									)
									continue;


								//wypisanie
								WYPIS
							}
						}
					}




					plik_wy << "</body></html>";
					plik_wy.close();

}

void Poligon::zapisz_pieszych()
{
	CreateDirectoryA(katalog_wyjsciowy.c_str(),0);
	string nazwa_pliku;
	if(zakres_przetwarzania.poczatek!="")
		nazwa_pliku=katalog_wyjsciowy+"/piesi_"+zakres_przetwarzania.poczatek+"-"+zakres_przetwarzania.koniec+".xls";
	else
		nazwa_pliku=katalog_wyjsciowy+"/piesi_"+zakres_przetwarzania.nazwa+".xls";
	statystyka.zapisz_pieszych2(nazwa_pliku);
}

void Poligon::zapisz_pojazdy()
{
	//statystyki ogolne
	CreateDirectoryA(katalog_wyjsciowy.c_str(),0);
	string nazwa_pliku;
	if(zakres_przetwarzania.poczatek!="")
		nazwa_pliku=katalog_wyjsciowy+"/pojazdy_"+zakres_przetwarzania.poczatek+"-"+zakres_przetwarzania.koniec+".xls";
	else
		nazwa_pliku=katalog_wyjsciowy+"/pojazdy_"+zakres_przetwarzania.nazwa+".xls";
	statystyka.zapisz_pojazdy2(nazwa_pliku);

	//predkosc przy minimalnej odleglosci

	BasicExcel  e;
	e.New(1); //automatycznie tworzy 1 arkusz
	e.RenameWorksheet("Sheet1","do wykresu");
	BasicExcelWorksheet* sheet = e.GetWorksheet("do wykresu");

	XLSFormatManager fmt_mgr(e);

	ExcelFont font_black_bold;
	font_black_bold._weight = FW_BOLD;
	CellFormat fmt_black_bold(fmt_mgr);
	fmt_black_bold.set_font(font_black_bold);
	fmt_black_bold.set_wrapping(true);

	ExcelFont font_red;
	CellFormat fmt_red(fmt_mgr);
	font_red.set_color_index(EGA_RED);
	fmt_red.set_font(font_red);
	//pierwsza wersja wypisywania
	/*
	sheet->Cell(0,0)->SetString("lewy pas");
	sheet->Cell(0,1)->SetString("nazwa pliku");
	sheet->Cell(0,2)->SetString("data");
	sheet->Cell(0,3)->SetString("godzina");
	sheet->Cell(0,4)->SetString("odleglosc");
	sheet->Cell(0,5)->SetString("predkosc");

	int nr_wiersza=1;
	for(int i=0;i<piesi.pieszy.size();i++)
	{
	for(int j=0;j<piesi.pieszy[i].pojazdy.lp.liczba;j++)
	{
	if(piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc[j]<999)
	{
	sheet->Cell(nr_wiersza,1)->Set(piesi.pieszy[i].id.c_str());
	stringstream ss;
	string s;

	ss << piesi.pieszy[i].t1.wYear <<"."<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wMonth<<"."<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wDay;//<<" "<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wHour<<":"<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wMinute<<":"<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wSecond<<"."<<setfill('0')<<setw(3)<<piesi.pieszy[i].t1.wMilliseconds;
	ss>>s;

	sheet->Cell(nr_wiersza,2)->Set(s.c_str());
	ss.clear();
	ss <<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wHour<<":"<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wMinute<<":"<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wSecond<<"."<<setfill('0')<<setw(3)<<piesi.pieszy[i].t1.wMilliseconds;
	ss>>s;
	sheet->Cell(nr_wiersza,3)->Set(s.c_str());
	sheet->Cell(nr_wiersza,4)->Set(piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc[j]);
	sheet->Cell(nr_wiersza,5)->Set(piesi.pieszy[i].pojazdy.lp.predkosc_w_punkcie_minimalnej_odleglosci[j]);
	nr_wiersza++;
	}
	}

	}

	sheet->Cell(0,6)->SetString("prawy pas");
	sheet->Cell(0,7)->SetString("nazwa pliku");
	sheet->Cell(0,8)->SetString("data");
	sheet->Cell(0,9)->SetString("godzina");
	sheet->Cell(0,10)->SetString("odleglosc");
	sheet->Cell(0,11)->SetString("predkosc");



	nr_wiersza=1;
	for(int i=0;i<piesi.pieszy.size();i++)
	{
	for(int j=0;j<piesi.pieszy[i].pojazdy.pp.liczba;j++)
	{
	if(piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[j]<999)
	{
	sheet->Cell(nr_wiersza,7)->Set(piesi.pieszy[i].id.c_str());
	stringstream ss;
	string s;

	ss << piesi.pieszy[i].t1.wYear <<"."<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wMonth<<"."<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wDay;//<<" "<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wHour<<":"<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wMinute<<":"<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wSecond<<"."<<setfill('0')<<setw(3)<<piesi.pieszy[i].t1.wMilliseconds;
	ss>>s;

	sheet->Cell(nr_wiersza,8)->Set(s.c_str());
	ss.clear();
	ss <<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wHour<<":"<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wMinute<<":"<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wSecond<<"."<<setfill('0')<<setw(3)<<piesi.pieszy[i].t1.wMilliseconds;
	ss>>s;
	sheet->Cell(nr_wiersza,9)->Set(s.c_str());
	sheet->Cell(nr_wiersza,10)->Set(piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[j]);
	sheet->Cell(nr_wiersza,11)->Set(piesi.pieszy[i].pojazdy.pp.predkosc_w_punkcie_minimalnej_odleglosci[j]);
	nr_wiersza++;
	}
	}

	}
	*/

	//druga wersja wypisywania
	sheet->Cell(0,0)->SetString("ponizej przykladowa sciezka bazowa do danych");
	sheet->Cell(1,0)->SetString("D:\\mobis\\mickiewicza_cen\\najnowsze\\POW");
	sheet->Cell(2,0)->SetString("Aby moc klikac na nazwy filmikow nalezy");
	sheet->Cell(3,0)->SetString("zaznaczyc kolumne B i zamienic w niej");
	sheet->Cell(4,0)->SetString("XXX   na    =HIPERLACZE(\"D:\\mobis\\mickiewicza_cen\\najnowsze\\POW");
	sheet->Cell(5,0)->SetString("WPISAC POLSKIE ZNAKI I WLASNA SCIEZKE!!!");
	sheet->Cell(5,0)->SetString("po zmianie mozna skasowac kolumne A");
	sheet->Cell(0,1)->SetString("nazwa pliku z pieszym");
	sheet->Cell(0,2)->SetString("nazwa pliku z pojazdem");
	sheet->Cell(0,3)->SetString("l. pieszych dla poj.");
	sheet->Cell(0,4)->SetString("1 wystapienie poj.");

	sheet->Cell(0,5)->SetString("rok");
	sheet->Cell(0,6)->SetString("miesiac");
	sheet->Cell(0,7)->SetString("dzien");
	sheet->Cell(0,8)->SetString("godzina");
	sheet->Cell(0,9)->SetString("minuta");
	sheet->Cell(0,10)->SetString("l. pojazdow");
	sheet->Cell(0,11)->SetString("l. poj. LP");
	sheet->Cell(0,12)->SetString("l. poj. PP");
	sheet->Cell(0,13)->SetString("ktory pas");
	sheet->Cell(0,14)->SetString("nr pojazdu");

	sheet->Cell(0,15)->SetString("marka");
	sheet->Cell(0,16)->SetString("model");
	sheet->Cell(0,17)->SetString("rejestracja");

	sheet->Cell(0,18)->SetString("odleglosc");
	sheet->Cell(0,19)->SetString("predkosc");
	
	sheet->Cell(0,20)->SetString("rodzaj trajektorii");
	
	sheet->Cell(0,21)->SetString("pieszy na pasach");
	sheet->Cell(0,22)->SetString("kierunek ruchu pieszego");

	sheet->Cell(0,23)->SetString("X pieszego");
	sheet->Cell(0,24)->SetString("Y pieszego");
	sheet->Cell(0,25)->SetString("pojazd na pasach");
	sheet->Cell(0,26)->SetString("X pojazdu");
	sheet->Cell(0,27)->SetString("Y pojazdu");
	sheet->Cell(0,28)->SetString("rodzaj konfliktu");
	sheet->Cell(0,29)->SetString("predkosc poczatkowa pojazdu");

	//formatowanie wiersza naglowkowego
	for(int c=0;c<26;c++)
		sheet->Cell(0,c)->SetFormat(fmt_black_bold);



	//konfiguracja wypisywania profili predkosci
	int kolumna_startowa=31; //od tej kolumny zaczynaja sie profile
	int kolumna_tmp=kolumna_startowa;
	double odla=30; //zakres odleglosci do profilu
	double odla_tmp=odla;
	double odlb=-2;
	double odlco=.5;
	while(odla_tmp>=odlb)
	{
		sheet->Cell(0,kolumna_tmp++)->Set(odla_tmp);
		odla_tmp-=odlco;
	}


	int nr_wiersza=1;
	for(int i=0;i<piesi.pieszy.size();i++)
	{

		//duchy olewamy!
		if(piesi.pieszy[i].trajektoria.rodzaj.duch==true) continue;
		//sprawdzenie czy w ogole by³y pojazdy
		if(piesi.pieszy[i].pojazdy.razem>0)
		{

			//teraz wypisanie pojazdow
			for(int j=0;j<piesi.pieszy[i].pojazdy.lp.liczba;j++)
			{

				//if(piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[j]<999)

				Pojazd pojazd=pojazdy_lp.pojazd[piesi.pieszy[i].pojazdy.lp.nr[j]];

				stringstream ss;
				string s;

				//sheet->Cell(nr_wiersza,0)->Set(piesi.pieszy[i].id.c_str());
				ss << "XXX\\"<<piesi.pieszy[i].t1.wYear <<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wMonth<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wDay<<"\\hum\\" <<piesi.pieszy[i].id <<".mkv\";\""<<piesi.pieszy[i].id << "\")";
				ss>>s;
				sheet->Cell(nr_wiersza,1)->Set(s.c_str());
				ss.clear();
				ss<<"";
				ss << "XXX\\"<<pojazd.t1.wYear <<setfill('0')<<setw(2)<<pojazd.t1.wMonth<<setfill('0')<<setw(2)<<pojazd.t1.wDay<<"\\l2\\" <<pojazd.id <<".mkv\";\""<<pojazd.id << "\")";
				ss>>s;
				sheet->Cell(nr_wiersza,2)->Set(s.c_str());

				sheet->Cell(nr_wiersza,3)->Set(pojazd.liczba_pieszych);
				if(pojazd.wypisany_w_statystykach==false) //jak po raz pierwszy jest wypisywany
				{
					sheet->Cell(nr_wiersza,4)->Set(1); //to wypisujemy "1"
					pojazdy_lp.pojazd[piesi.pieszy[i].pojazdy.lp.nr[j]].wypisany_w_statystykach=true; //i zapamietujemy ze juz raz sie pojawil!
				}

				sheet->Cell(nr_wiersza,5)->Set(piesi.pieszy[i].t1.wYear);
				sheet->Cell(nr_wiersza,6)->Set(piesi.pieszy[i].t1.wMonth);
				sheet->Cell(nr_wiersza,7)->Set(piesi.pieszy[i].t1.wDay);
				sheet->Cell(nr_wiersza,8)->Set(piesi.pieszy[i].t1.wHour);
				sheet->Cell(nr_wiersza,9)->Set(piesi.pieszy[i].t1.wMinute);


				sheet->Cell(nr_wiersza,10)->Set(piesi.pieszy[i].pojazdy.razem);
				sheet->Cell(nr_wiersza,11)->Set(piesi.pieszy[i].pojazdy.lp.liczba);
				sheet->Cell(nr_wiersza,12)->Set(piesi.pieszy[i].pojazdy.pp.liczba);
				

				sheet->Cell(nr_wiersza,13)->Set("lewy");
				sheet->Cell(nr_wiersza,14)->Set(j+1);

				
				sheet->Cell(nr_wiersza,15)->Set(pojazdy_lp.pojazd[piesi.pieszy[i].pojazdy.lp.nr[j]].manufacturer.c_str());
				sheet->Cell(nr_wiersza,16)->Set(pojazdy_lp.pojazd[piesi.pieszy[i].pojazdy.lp.nr[j]].model.c_str());
				sheet->Cell(nr_wiersza,17)->Set(pojazdy_lp.pojazd[piesi.pieszy[i].pojazdy.lp.nr[j]].symbol.c_str());

				sheet->Cell(nr_wiersza,18)->Set(roundToNearest(piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc[j]));
				if(piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc[j]<=3)
					sheet->Cell(nr_wiersza,18)->SetFormat(fmt_red);

				sheet->Cell(nr_wiersza,19)->Set(roundToNearest(piesi.pieszy[i].pojazdy.lp.predkosc_w_punkcie_minimalnej_odleglosci[j]));
				if(piesi.pieszy[i].pojazdy.lp.predkosc_w_punkcie_minimalnej_odleglosci[j]>=6)
					sheet->Cell(nr_wiersza,19)->SetFormat(fmt_red);

				if(piesi.pieszy[i].trajektoria.rodzaj.dluga==true)
					sheet->Cell(nr_wiersza,20)->Set("dluga");
				else
					if(piesi.pieszy[i].trajektoria.rodzaj.srednia==true)
					sheet->Cell(nr_wiersza,20)->Set("srednia");
					else
				if(piesi.pieszy[i].trajektoria.rodzaj.krotka==true)
					sheet->Cell(nr_wiersza,20)->Set("krotka");
				else
				if(piesi.pieszy[i].trajektoria.rodzaj.duch==true)
					sheet->Cell(nr_wiersza,20)->Set("duch");
				else
					sheet->Cell(nr_wiersza,20)->Set("hgw");


				if(piesi.pieszy[i].pojazdy.lp.pozycja_pieszego_w_punkcie_minimalnej_odleglosci[j].na_pasach==true)
					sheet->Cell(nr_wiersza,21)->Set("tak");
				else
					sheet->Cell(nr_wiersza,21)->Set("nie");
				if(piesi.pieszy[i].trajektoria.kierunek.w_gore)
					sheet->Cell(nr_wiersza,22)->Set("gora");
				else
					if(piesi.pieszy[i].trajektoria.kierunek.w_dol)
						sheet->Cell(nr_wiersza,22)->Set("dol");
					else
						sheet->Cell(nr_wiersza,22)->Set("hgw");
				sheet->Cell(nr_wiersza,23)->Set(roundToNearest(piesi.pieszy[i].pojazdy.lp.pozycja_pieszego_w_punkcie_minimalnej_odleglosci[j].x));
				sheet->Cell(nr_wiersza,24)->Set(roundToNearest(piesi.pieszy[i].pojazdy.lp.pozycja_pieszego_w_punkcie_minimalnej_odleglosci[j].y));

				if(piesi.pieszy[i].pojazdy.lp.pozycja_pojazdu_w_punkcie_minimalnej_odleglosci[j].na_pasach==true)
					sheet->Cell(nr_wiersza,25)->Set("tak");
				else
					sheet->Cell(nr_wiersza,25)->Set("nie");
				sheet->Cell(nr_wiersza,26)->Set(roundToNearest(piesi.pieszy[i].pojazdy.lp.pozycja_pojazdu_w_punkcie_minimalnej_odleglosci[j].x));
				sheet->Cell(nr_wiersza,27)->Set(roundToNearest(piesi.pieszy[i].pojazdy.lp.pozycja_pojazdu_w_punkcie_minimalnej_odleglosci[j].y));

				if(j<piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach.size())
					sheet->Cell(nr_wiersza,28)->Set(piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach[j].c_str());
				else
					sheet->Cell(nr_wiersza,28)->Set("?");
				/*
				if(piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach[j]=="po palcach")
				sheet->Cell(nr_wiersza,28)->Set("palce");
				else
				if(piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach[j]=="po pietach")
				sheet->Cell(nr_wiersza,28)->Set("piety");
				else
				if(piesi.pieszy[i].pojazdy.lp.po_palcach_po_pietach[j]=="przepuszczenie?")
				sheet->Cell(nr_wiersza,28)->Set("przepuszczenie?");
				else
				sheet->Cell(nr_wiersza,28)->Set("hgw");
				*/
				Frame::Vel vp=pojazdy_lp.pojazd[piesi.pieszy[i].pojazdy.lp.nr[j]].frame[0].vel;
				sheet->Cell(nr_wiersza,29)->Set(roundToNearest(sqrt(vp.x*vp.x+vp.y*vp.y)));

				// wypisanie profilu predkosci pojazdu co x cm od 30 metra do 0


				vector<double> new_x;
				for(double q=odla;q>=odlb;q-=odlco)
					new_x.push_back(q);

				vector <double>new_v=interpolate2(pojazd,new_x);

				//zapisanie
				int nr_kol=kolumna_startowa; //nr kolumny od ktorej zapisywac do Excela
				for(int q=0;q<new_v.size();q++)
					sheet->Cell(nr_wiersza,nr_kol++)->Set(new_v[q]);

				//koniec wypisania profilu

				nr_wiersza++;
			}
			for(int j=0;j<piesi.pieszy[i].pojazdy.pp.liczba;j++)
			{
				//if(piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[j]<999)

				Pojazd pojazd=pojazdy_pp.pojazd[piesi.pieszy[i].pojazdy.pp.nr[j]];
				stringstream ss;
				string s;

				//sheet->Cell(nr_wiersza,0)->Set(piesi.pieszy[i].id.c_str());
				ss << "XXX\\"<<piesi.pieszy[i].t1.wYear <<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wMonth<<setfill('0')<<setw(2)<<piesi.pieszy[i].t1.wDay<<"\\hum\\" <<piesi.pieszy[i].id <<".mkv\";\""<<piesi.pieszy[i].id << "\")";
				ss>>s;
				sheet->Cell(nr_wiersza,1)->Set(s.c_str());
				ss.clear();
				ss<<"";
				ss << "XXX\\"<<pojazd.t1.wYear <<setfill('0')<<setw(2)<<pojazd.t1.wMonth<<setfill('0')<<setw(2)<<pojazd.t1.wDay<<"\\l1\\" <<pojazd.id <<".mkv\";\""<<pojazd.id << "\")";
				ss>>s;
				sheet->Cell(nr_wiersza,2)->Set(s.c_str());
				sheet->Cell(nr_wiersza,3)->Set(pojazd.liczba_pieszych);
				if(pojazd.wypisany_w_statystykach==false) //jak po raz pierwszy jest wypisywany
				{
					sheet->Cell(nr_wiersza,4)->Set(1); //to wypisujemy "1"
					pojazdy_pp.pojazd[piesi.pieszy[i].pojazdy.pp.nr[j]].wypisany_w_statystykach=true; //i zapamietujemy ze juz raz sie pojawil!
				}

				sheet->Cell(nr_wiersza,5)->Set(piesi.pieszy[i].t1.wYear);
				sheet->Cell(nr_wiersza,6)->Set(piesi.pieszy[i].t1.wMonth);
				sheet->Cell(nr_wiersza,7)->Set(piesi.pieszy[i].t1.wDay);
				sheet->Cell(nr_wiersza,8)->Set(piesi.pieszy[i].t1.wHour);
				sheet->Cell(nr_wiersza,9)->Set(piesi.pieszy[i].t1.wMinute);


				sheet->Cell(nr_wiersza,10)->Set(piesi.pieszy[i].pojazdy.razem);
				sheet->Cell(nr_wiersza,11)->Set(piesi.pieszy[i].pojazdy.lp.liczba);
				sheet->Cell(nr_wiersza,12)->Set(piesi.pieszy[i].pojazdy.pp.liczba);


				sheet->Cell(nr_wiersza,13)->Set("prawy");
				sheet->Cell(nr_wiersza,14)->Set(j+1);

				sheet->Cell(nr_wiersza,15)->Set(pojazdy_pp.pojazd[piesi.pieszy[i].pojazdy.pp.nr[j]].manufacturer.c_str());
				sheet->Cell(nr_wiersza,16)->Set(pojazdy_pp.pojazd[piesi.pieszy[i].pojazdy.pp.nr[j]].model.c_str());
				sheet->Cell(nr_wiersza,17)->Set(pojazdy_pp.pojazd[piesi.pieszy[i].pojazdy.pp.nr[j]].symbol.c_str());


				sheet->Cell(nr_wiersza,18)->Set(roundToNearest(piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[j]));
				if(piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[j]<=3)
					sheet->Cell(nr_wiersza,18)->SetFormat(fmt_red);

				sheet->Cell(nr_wiersza,19)->Set(roundToNearest(piesi.pieszy[i].pojazdy.pp.predkosc_w_punkcie_minimalnej_odleglosci[j]));
				if(piesi.pieszy[i].pojazdy.pp.predkosc_w_punkcie_minimalnej_odleglosci[j]>=6)
					sheet->Cell(nr_wiersza,19)->SetFormat(fmt_red);

				if(piesi.pieszy[i].trajektoria.rodzaj.dluga==true)
					sheet->Cell(nr_wiersza,20)->Set("dluga");
				else
					if(piesi.pieszy[i].trajektoria.rodzaj.srednia==true)
					sheet->Cell(nr_wiersza,20)->Set("srednia");
					else
				if(piesi.pieszy[i].trajektoria.rodzaj.krotka==true)
					sheet->Cell(nr_wiersza,20)->Set("krotka");
				else
				if(piesi.pieszy[i].trajektoria.rodzaj.duch==true)
					sheet->Cell(nr_wiersza,20)->Set("duch");
				else
					sheet->Cell(nr_wiersza,20)->Set("hgw");


				if(piesi.pieszy[i].pojazdy.pp.pozycja_pieszego_w_punkcie_minimalnej_odleglosci[j].na_pasach==true)
					sheet->Cell(nr_wiersza,21)->Set("tak");
				else
					sheet->Cell(nr_wiersza,21)->Set("nie");

				if(piesi.pieszy[i].trajektoria.kierunek.w_gore)
					sheet->Cell(nr_wiersza,22)->Set("gora");
				else
					if(piesi.pieszy[i].trajektoria.kierunek.w_dol)
						sheet->Cell(nr_wiersza,22)->Set("dol");
					else
						sheet->Cell(nr_wiersza,22)->Set("hgw");

				sheet->Cell(nr_wiersza,23)->Set(roundToNearest(piesi.pieszy[i].pojazdy.pp.pozycja_pieszego_w_punkcie_minimalnej_odleglosci[j].x));
				sheet->Cell(nr_wiersza,24)->Set(roundToNearest(piesi.pieszy[i].pojazdy.pp.pozycja_pieszego_w_punkcie_minimalnej_odleglosci[j].y));

				if(piesi.pieszy[i].pojazdy.pp.pozycja_pojazdu_w_punkcie_minimalnej_odleglosci[j].na_pasach==true)
					sheet->Cell(nr_wiersza,25)->Set("tak");
				else
					sheet->Cell(nr_wiersza,25)->Set("nie");
				sheet->Cell(nr_wiersza,26)->Set(roundToNearest(piesi.pieszy[i].pojazdy.pp.pozycja_pojazdu_w_punkcie_minimalnej_odleglosci[j].x));
				sheet->Cell(nr_wiersza,27)->Set(roundToNearest(piesi.pieszy[i].pojazdy.pp.pozycja_pojazdu_w_punkcie_minimalnej_odleglosci[j].y));

				if(j<piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach.size())
					sheet->Cell(nr_wiersza,28)->Set(piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach[j].c_str());
				else
					sheet->Cell(nr_wiersza,28)->Set("?");

				/*
				if(piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach[j]=="po palcach")
				sheet->Cell(nr_wiersza,28)->Set("palce");
				else
				if(piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach[j]=="po pietach")
				sheet->Cell(nr_wiersza,28)->Set("piety");
				else
				if(piesi.pieszy[i].pojazdy.pp.po_palcach_po_pietach[j]=="przepuszczenie?")
				sheet->Cell(nr_wiersza,28)->Set("przepuszczenie?");
				else
				sheet->Cell(nr_wiersza,28)->Set("hgw");
				*/

				Frame::Vel vp=pojazdy_pp.pojazd[piesi.pieszy[i].pojazdy.pp.nr[j]].frame[0].vel;
				sheet->Cell(nr_wiersza,29)->Set(roundToNearest(sqrt(vp.x*vp.x+vp.y*vp.y)));


				// wypisanie profilu predkosci pojazdu co x cm od 30 metra do 0


				vector<double> new_x;
				for(double q=odla;q>=odlb;q-=odlco)
					new_x.push_back(q);

				vector <double>new_v=interpolate2(pojazd,new_x);

				//zapisanie
				int nr_kol=kolumna_startowa; //nr kolumny od ktorej zapisywac do Excela
				for(int q=0;q<new_v.size();q++)
					sheet->Cell(nr_wiersza,nr_kol++)->Set(new_v[q]);

				//koniec wypisania profilu

				nr_wiersza++;
			}
		}
	}


	if(e.SaveAs((katalog_wyjsciowy+"/do_wykresu.xls").c_str())==false)
	{
		cout << "\nNie moge pisac w pliku wyjsciowym: "<<"test" << endl;
		cout <<"nacisnij ENTER by zakonczyc"<< endl;
		cin.ignore(1);
		exit(-1);
	}

	//reset "wypisany_w_statystykach"
	for(int i=0;i<pojazdy_lp.pojazd.size();i++)
		pojazdy_lp.pojazd[i].wypisany_w_statystykach=false;
	for(int i=0;i<pojazdy_pp.pojazd.size();i++)
		pojazdy_pp.pojazd[i].wypisany_w_statystykach=false;


	//statystyki szczegolowe dotyczace predkosci

	//to mo¿na na razie olaæ
	/*
	fstream plik_wy;
	nazwa_pliku=katalog_wyjsciowy+"/pojazdy_predkosc_"+zakres_przetwarzania.poczatek+"-"+zakres_przetwarzania.koniec+".csv";
	plik_wy.open(nazwa_pliku.c_str(),ios::out);
	if( plik_wy.good() == false )
	{
	cout << "Nie moge pisac w pliku wyjsciowym!" << endl;
	cout <<"nacisnij ENTER by zakonczyc"<< endl;
	cin.ignore(1);
	exit(-1);
	}

	//statystyka godzinowa z podsumowaniem dziennym
	plik_wy<<"Statystyka predkosci wszystkich pojazdow"<<endl;
	plik_wy<<"Prawy pas " << endl;
	plik_wy<<"data;typ;marka;model;rejestracja;vmin;vmax;v dla x="<< statystyki_dodatkowe.predkosc_pojazdow_w_punkcie.wspolrzedna_x_punktu<<endl;
	for(int i=0;i<pojazdy_pp.pojazd.size();i++)
	{
	Pojazd p=pojazdy_pp.pojazd[i];
	plik_wy<<p.t1<<";"<<p.type<<";"<<p.manufacturer<<";"<<p.model<<";"<<p.symbol<<";"<<p.vel.min_x<<";"<<p.vel.max_x<<";"<<p.vel.w_punkcie<<endl;
	}
	plik_wy<<endl<<"Lewy pas " << endl;
	for(int i=0;i<pojazdy_lp.pojazd.size();i++)
	{
	Pojazd p=pojazdy_lp.pojazd[i];
	plik_wy<<p.id<<";"<<p.vel.w_punkcie<<endl;
	}
	plik_wy.close();
	*/
}