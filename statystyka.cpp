#include "statystyka.h"

//Statystyka::Statystyka()
//{
//	
//}

void Statystyka::zapisz_pieszych(string plik)
{
	fstream plik_wy;
	plik_wy.open(plik.c_str(),ios::out);
	if( plik_wy.good() == false )
	{
		cout << "Nie moge pisac w pliku wyjsciowym!" << endl;
		cout <<"nacisnij ENTER by zakonczyc"<< endl;
		cin.ignore(1);
		exit(-1);
	}

	//statystyka godzinowa z podsumowaniem dziennym
	plik_wy<<"Statystyka godzinowa pieszych z podsumowaniem dziennym"<<endl;
	//plik_wy<<";Liczba wykrytych pieszych;;;;Liczba pieszych wedlug rodzaju trajektorii"<<endl;
	plik_wy<<";Liczba wykrytych pieszych"<<endl;
	int numer_dnia=0;
	while(dzienne[numer_dnia].data!="000000")
	{
		//plik_wy<<"data; godzina; poprawnych; niepoprawnych; ; dobre; krotkie; ; duchy; na_samochodzie" << endl;
		plik_wy<<"data; godzina; poprawnych; niepoprawnych;" << endl;
		plik_wy<<dzienne[numer_dnia].data;
		for(int i=0;i<24;i++)
		{
			plik_wy<<";"<<i<<";"<<dzienne[numer_dnia].na_godzine[i].piesi.razem_poprawna<<";"<<dzienne[numer_dnia].na_godzine[i].piesi.razem_niepoprawna;
			//plik_wy<<";;"<<dzienne[numer_dnia].na_godzine[i].piesi.dobra<<";"<<dzienne[numer_dnia].na_godzine[i].piesi.krotka;
			//plik_wy<<";;"<<dzienne[numer_dnia].na_godzine[i].piesi.duch<<";"<<dzienne[numer_dnia].na_godzine[i].piesi.pieszy_na_samochodzie;
			plik_wy<<endl;
		}
		plik_wy<<"NA CALY DZIEN;"<<dzienne[numer_dnia].na_dzien.piesi.razem_poprawna+dzienne[numer_dnia].na_dzien.piesi.razem_niepoprawna<<";";
		plik_wy<<dzienne[numer_dnia].na_dzien.piesi.razem_poprawna<<";"<<dzienne[numer_dnia].na_dzien.piesi.razem_niepoprawna;
		//plik_wy<<";;"<<dzienne[numer_dnia].na_dzien.piesi.dobra<<";"<<dzienne[numer_dnia].na_dzien.piesi.krotka;
		//plik_wy<<";;"<<dzienne[numer_dnia].na_dzien.piesi.duch<<";"<<dzienne[numer_dnia].na_dzien.piesi.pieszy_na_samochodzie;
		plik_wy<<endl;
		numer_dnia++;
		plik_wy<<endl;
	}

	//statystyka dzienna
	plik_wy<<"Statystyka dzienna pieszych"<<endl;
	//plik_wy<<";Liczba wykrytych pieszych;;;;Liczba pieszych wedlug rodzaju trajektorii"<<endl;
	//plik_wy<<"data; wszystkich; poprawnych; niepoprawnych; ; dobre; krotkie; ; duchy; na_samochodzie" << endl;
	plik_wy<<";Liczba wykrytych pieszych"<<endl;
	plik_wy<<"data; wszystkich; poprawnych; niepoprawnych;" << endl;
	numer_dnia=0;
	while(dzienne[numer_dnia].data!="000000")
	{

		plik_wy<<dzienne[numer_dnia].data<<";";
		plik_wy<<dzienne[numer_dnia].na_dzien.piesi.razem_poprawna+dzienne[numer_dnia].na_dzien.piesi.razem_niepoprawna<<";"<<dzienne[numer_dnia].na_dzien.piesi.razem_poprawna<<";"<<dzienne[numer_dnia].na_dzien.piesi.razem_niepoprawna;
		//plik_wy<<";;"<<dzienne[numer_dnia].na_dzien.piesi.dobra<<";"<<dzienne[numer_dnia].na_dzien.piesi.krotka;
		//plik_wy<<";;"<<dzienne[numer_dnia].na_dzien.piesi.duch<<";"<<dzienne[numer_dnia].na_dzien.piesi.pieszy_na_samochodzie;<<
		plik_wy<<endl;
		numer_dnia++;
	}

	plik_wy.close();
}

void Statystyka::zapisz_pieszych2(string plik)
{
	BasicExcel  e;
	e.New(1); //automatycznie tworzy 1 arkusz
	e.RenameWorksheet("Sheet1","Zbiorcza");


	int numer_dnia=0;
	BasicExcelWorksheet* sheet = e.GetWorksheet("Zbiorcza");
	sheet->Cell(0,0)->SetString("Statystyka dzienna pieszych");
	sheet->Cell(1,1)->SetString("Liczba wykrytych pieszych");
	sheet->Cell(0,6)->SetString("Srednia predkosc");
	sheet->Cell(1,6)->SetString("na pasach");
	sheet->Cell(1,7)->SetString("odch. stand.");
	sheet->Cell(1,8)->SetString("z trajektorii");
	sheet->Cell(1,9)->SetString("l. ostrych hamowan");


	sheet->Cell(2,0)->SetString("data");
	sheet->Cell(2,1)->SetString("wszystkich");
	sheet->Cell(2,2)->SetString("poprawnych");
	sheet->Cell(2,3)->SetString("niepoprawnych");
	sheet->Cell(2,4)->SetString("poprawnych z samochodem");
	sheet->Cell(2,5)->SetString("poprawnych bez samochodu");



	while(dzienne[numer_dnia].data!="000000")
	{

		sheet->Cell(3+numer_dnia,0)->SetInteger(atoi(dzienne[numer_dnia].data.c_str()));
		sheet->Cell(3+numer_dnia,1)->SetInteger(dzienne[numer_dnia].na_dzien.piesi.razem_poprawna+dzienne[numer_dnia].na_dzien.piesi.razem_niepoprawna);
		sheet->Cell(3+numer_dnia,2)->SetInteger(dzienne[numer_dnia].na_dzien.piesi.razem_poprawna);
		sheet->Cell(3+numer_dnia,3)->SetInteger(dzienne[numer_dnia].na_dzien.piesi.razem_niepoprawna);
		sheet->Cell(3+numer_dnia,4)->SetInteger(dzienne[numer_dnia].na_dzien.piesi.razem_poprawna_z_samochodem);
		sheet->Cell(3+numer_dnia,5)->SetInteger(dzienne[numer_dnia].na_dzien.piesi.razem_poprawna_bez_samochodu);
		
		
		sheet->Cell(3+numer_dnia,6)->Set(dzienne[numer_dnia].na_dzien.piesi.predkosc.srednia);
		sheet->Cell(3+numer_dnia,7)->Set(dzienne[numer_dnia].na_dzien.piesi.predkosc.odch_st);
		sheet->Cell(3+numer_dnia,8)->Set(dzienne[numer_dnia].na_dzien.piesi.predkosc.srednia_z_pozycji);
		sheet->Cell(3+numer_dnia,9)->Set(dzienne[numer_dnia].na_dzien.liczba_ostrych_hamowan);


		numer_dnia++;
	}

	numer_dnia=0;
	while(dzienne[numer_dnia].data!="000000")
	{
		e.AddWorksheet(dzienne[numer_dnia].data.c_str());
		BasicExcelWorksheet* sheet = e.GetWorksheet(dzienne[numer_dnia].data.c_str());
		sheet->Cell(0,0)->SetString("Statystyka godzinowa pieszych");
		sheet->Cell(1,1)->SetString("Liczba wykrytych pieszych");
		sheet->Cell(1,6)->SetString("Srednia predkosc (2 metody)");
		sheet->Cell(2,0)->SetString("godzina");
		sheet->Cell(2,1)->SetString("wszystkich");
		sheet->Cell(2,2)->SetString("poprawnych");
		sheet->Cell(2,3)->SetString("niepoprawnych");
		sheet->Cell(2,4)->SetString("poprawnych z samochodem");
		sheet->Cell(2,5)->SetString("poprawnych bez samochodu");
		sheet->Cell(2,6)->SetString("srednia");
		sheet->Cell(2,7)->SetString("odch. stand.");
		sheet->Cell(2,8)->SetString("srednia z pozycji");
		sheet->Cell(2,9)->SetString("l. ostrych hamowan");
		int i;
		for(i=0;i<24;i++)
		{
			sheet->Cell(3+i,0)->SetInteger(i);
			sheet->Cell(3+i,1)->SetInteger(dzienne[numer_dnia].na_godzine[i].piesi.razem_poprawna+dzienne[numer_dnia].na_godzine[i].piesi.razem_niepoprawna);
			sheet->Cell(3+i,2)->SetInteger(dzienne[numer_dnia].na_godzine[i].piesi.razem_poprawna);
			sheet->Cell(3+i,3)->SetInteger(dzienne[numer_dnia].na_godzine[i].piesi.razem_niepoprawna);
			sheet->Cell(3+i,4)->SetInteger(dzienne[numer_dnia].na_godzine[i].piesi.razem_poprawna_z_samochodem);
			sheet->Cell(3+i,5)->SetInteger(dzienne[numer_dnia].na_godzine[i].piesi.razem_poprawna_bez_samochodu);
			sheet->Cell(3+i,6)->Set(dzienne[numer_dnia].na_godzine[i].piesi.predkosc.srednia);
			sheet->Cell(3+i,7)->Set(dzienne[numer_dnia].na_godzine[i].piesi.predkosc.odch_st);

			sheet->Cell(3+i,8)->Set(dzienne[numer_dnia].na_godzine[i].piesi.predkosc.srednia_z_pozycji);
			sheet->Cell(3+i,9)->Set(dzienne[numer_dnia].na_godzine[i].liczba_ostrych_hamowan);
		}
		sheet->Cell(3+i,0)->Set("Razem");
		sheet->Cell(3+i,1)->Set(dzienne[numer_dnia].na_dzien.piesi.razem_poprawna+dzienne[numer_dnia].na_dzien.piesi.razem_niepoprawna);
		sheet->Cell(3+i,2)->Set(dzienne[numer_dnia].na_dzien.piesi.razem_poprawna);
		sheet->Cell(3+i,3)->Set(dzienne[numer_dnia].na_dzien.piesi.razem_niepoprawna);
		sheet->Cell(3+i,4)->Set(dzienne[numer_dnia].na_dzien.piesi.razem_poprawna_z_samochodem);
		sheet->Cell(3+i,5)->Set(dzienne[numer_dnia].na_dzien.piesi.razem_poprawna_bez_samochodu);

		numer_dnia++;

	}
	
	if(e.SaveAs(plik.c_str())==false)
	{
		cout << "\nNie moge pisac w pliku wyjsciowym: "<<plik << endl;
		cout <<"nacisnij ENTER by zakonczyc"<< endl;
		cin.ignore(1);
		exit(-1);
	}
	
}


void Statystyka::zapisz_pojazdy(string plik)
{
	fstream plik_wy;
	plik_wy.open(plik.c_str(),ios::out);
	if( plik_wy.good() == false )
	{
		cout << "Nie moge pisac w pliku wyjsciowym!" << endl;
		cout <<"nacisnij ENTER by zakonczyc"<< endl;
		cin.ignore(1);
		exit(-1);
	}

	//statystyka godzinowa z podsumowaniem dziennym
	plik_wy<<"Statystyka godzinowa pojazdow ZARJESTROWANYCH WRAZ Z PIESZYMI z podsumowaniem dziennym"<<endl;
	plik_wy<<"WSZYSTKIE SAMOCHODY NA OBU PASACH"<<endl;
	int numer_dnia=0;
	while(dzienne[numer_dnia].data!="000000")
	{
		plik_wy<<"data; godzina; wszystkich; osobowych; polciezarowych; ciezarowych; autobusow; pozostalych" << endl;
		plik_wy<<dzienne[numer_dnia].data;
		for(int i=0;i<24;i++)
		{
			plik_wy<<";"<<i<<";"<<dzienne[numer_dnia].na_godzine[i].pojazdy.razem<<";"<<dzienne[numer_dnia].na_godzine[i].pojazdy.osobowe;
			plik_wy<<";"<<dzienne[numer_dnia].na_godzine[i].pojazdy.polciezarowe<<";"<<dzienne[numer_dnia].na_godzine[i].pojazdy.ciezarowe;
			plik_wy<<";"<<dzienne[numer_dnia].na_godzine[i].pojazdy.autobusy<<";"<<dzienne[numer_dnia].na_godzine[i].pojazdy.pozostale<<endl;
		}
		plik_wy<<"NA CALY DZIEN;;";
		plik_wy<<dzienne[numer_dnia].na_dzien.pojazdy.razem<<";"<<dzienne[numer_dnia].na_dzien.pojazdy.osobowe;
		plik_wy<<";"<<dzienne[numer_dnia].na_dzien.pojazdy.polciezarowe<<";"<<dzienne[numer_dnia].na_dzien.pojazdy.ciezarowe;
		plik_wy<<";"<<dzienne[numer_dnia].na_dzien.pojazdy.autobusy<<";"<<dzienne[numer_dnia].na_dzien.pojazdy.pozostale<<endl;


		numer_dnia++;
		plik_wy<<endl;
	}

	//statystyka dzienna
	plik_wy<<"Statystyka dzienna pojazdow ZARJESTROWANYCH WRAZ Z PIESZYMI"<<endl;
	plik_wy<<"WSZYSTKIE SAMOCHODY NA OBU PASACH"<<endl;
	plik_wy<<"data; ; wszystkich; osobowych; polciezarowych; ciezarowych; autobusow; pozostalych" << endl;
	numer_dnia=0;
	while(dzienne[numer_dnia].data!="000000")
	{

		plik_wy<<dzienne[numer_dnia].data<<";;";
		plik_wy<<dzienne[numer_dnia].na_dzien.pojazdy.razem<<";"<<dzienne[numer_dnia].na_dzien.pojazdy.osobowe;
		plik_wy<<";"<<dzienne[numer_dnia].na_dzien.pojazdy.polciezarowe<<";"<<dzienne[numer_dnia].na_dzien.pojazdy.ciezarowe;
		plik_wy<<";"<<dzienne[numer_dnia].na_dzien.pojazdy.autobusy<<";"<<dzienne[numer_dnia].na_dzien.pojazdy.pozostale<<endl;
		numer_dnia++;
	}

	plik_wy.close();
}

void Statystyka::zapisz_pojazdy2(string plik)
{
	BasicExcel  e;
	e.New(1); //automatycznie tworzy 1 arkusz
	e.RenameWorksheet("Sheet1","Zbiorcza");
	
	
	int numer_dnia=0;
	BasicExcelWorksheet* sheet = e.GetWorksheet("Zbiorcza");


	//statystyka dzienna
	sheet->Cell(0,0)->SetString("Statystyka dzienna pojazdow ZARJESTROWANYCH WRAZ Z PIESZYMI");
	sheet->Cell(1,0)->SetString("WSZYSTKIE SAMOCHODY NA OBU PASACH");
	sheet->Cell(2,0)->SetString("data");
	sheet->Cell(2,1)->SetString("wszystkich");
	sheet->Cell(2,2)->SetString("osobowych");
	sheet->Cell(2,3)->SetString("polciezarowych");
	sheet->Cell(2,4)->SetString("ciezarowych");
	sheet->Cell(2,5)->SetString("autobusow");
	sheet->Cell(2,6)->SetString("pozostalych");
	sheet->Cell(2,7)->SetString("srednia predkosc w punkcie");
	sheet->Cell(2,8)->SetString("odch. stand.");

	numer_dnia=0;
	while(dzienne[numer_dnia].data!="000000")
	{

		sheet->Cell(3+numer_dnia,0)->Set(atoi(dzienne[numer_dnia].data.c_str()));
		sheet->Cell(3+numer_dnia,1)->Set(dzienne[numer_dnia].na_dzien.pojazdy.razem);
		sheet->Cell(3+numer_dnia,2)->Set(dzienne[numer_dnia].na_dzien.pojazdy.osobowe);
		sheet->Cell(3+numer_dnia,3)->Set(dzienne[numer_dnia].na_dzien.pojazdy.polciezarowe);
		sheet->Cell(3+numer_dnia,4)->Set(dzienne[numer_dnia].na_dzien.pojazdy.ciezarowe);
		sheet->Cell(3+numer_dnia,5)->Set(dzienne[numer_dnia].na_dzien.pojazdy.autobusy);
		sheet->Cell(3+numer_dnia,6)->Set(dzienne[numer_dnia].na_dzien.pojazdy.pozostale);
		sheet->Cell(3+numer_dnia,7)->Set(dzienne[numer_dnia].na_dzien.pojazdy.predkosc_w_punkcie);
		sheet->Cell(3+numer_dnia,8)->Set(dzienne[numer_dnia].na_dzien.pojazdy.odch_st_w_punkcie);

		numer_dnia++;
	}


	//statystyka godzinowa z podsumowaniem dziennym
	numer_dnia=0;
	while(dzienne[numer_dnia].data!="000000")
	{
		e.AddWorksheet(dzienne[numer_dnia].data.c_str());
		BasicExcelWorksheet* sheet = e.GetWorksheet(dzienne[numer_dnia].data.c_str());

	sheet->Cell(0,0)->SetString("Statystyka godzinowa pojazdow ZARJESTROWANYCH WRAZ Z PIESZYMI");
	sheet->Cell(1,0)->SetString("WSZYSTKIE SAMOCHODY NA OBU PASACH");
	sheet->Cell(2,0)->SetString("godzina");
	sheet->Cell(2,1)->SetString("wszystkich");
	sheet->Cell(2,2)->SetString("osobowych");
	sheet->Cell(2,3)->SetString("polciezarowych");
	sheet->Cell(2,4)->SetString("ciezarowych");
	sheet->Cell(2,5)->SetString("autobusow");
	sheet->Cell(2,6)->SetString("pozostalych");
	sheet->Cell(2,7)->SetString("srednia predkosc w punkcie");
	sheet->Cell(2,8)->SetString("odch. stand.");

		int i;
		for(i=0;i<24;i++)
		{
		sheet->Cell(3+i,0)->Set(i);
		sheet->Cell(3+i,1)->Set(dzienne[numer_dnia].na_godzine[i].pojazdy.razem);
		sheet->Cell(3+i,2)->Set(dzienne[numer_dnia].na_godzine[i].pojazdy.osobowe);
		sheet->Cell(3+i,3)->Set(dzienne[numer_dnia].na_godzine[i].pojazdy.polciezarowe);
		sheet->Cell(3+i,4)->Set(dzienne[numer_dnia].na_godzine[i].pojazdy.ciezarowe);
		sheet->Cell(3+i,5)->Set(dzienne[numer_dnia].na_godzine[i].pojazdy.autobusy);
		sheet->Cell(3+i,6)->Set(dzienne[numer_dnia].na_godzine[i].pojazdy.pozostale);
		sheet->Cell(3+i,7)->Set(dzienne[numer_dnia].na_godzine[i].pojazdy.predkosc_w_punkcie);
		sheet->Cell(3+i,8)->Set(dzienne[numer_dnia].na_godzine[i].pojazdy.odch_st_w_punkcie);
		//

		}
		sheet->Cell(3+i,0)->Set("razem");
		sheet->Cell(3+i,1)->Set(dzienne[numer_dnia].na_dzien.pojazdy.razem);
		sheet->Cell(3+i,2)->Set(dzienne[numer_dnia].na_dzien.pojazdy.osobowe);
		sheet->Cell(3+i,3)->Set(dzienne[numer_dnia].na_dzien.pojazdy.polciezarowe);
		sheet->Cell(3+i,4)->Set(dzienne[numer_dnia].na_dzien.pojazdy.ciezarowe);
		sheet->Cell(3+i,5)->Set(dzienne[numer_dnia].na_dzien.pojazdy.autobusy);
		sheet->Cell(3+i,6)->Set(dzienne[numer_dnia].na_dzien.pojazdy.pozostale);
		


		numer_dnia++;
	
	}

	

	if(e.SaveAs(plik.c_str())==false)
	{
		cout << "\nNie moge pisac w pliku wyjsciowym: "<<plik << endl;
		cout <<"nacisnij ENTER by zakonczyc"<< endl;
		cin.ignore(1);
		exit(-1);
	}
}

