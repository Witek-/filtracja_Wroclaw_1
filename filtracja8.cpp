#include "poligon.h"
#include "statystyka.h"
#include <locale.h>

//normalizacja prêdkoœci
//wszystkie prêdkoœci w kierunku przejœcia s¹ dodatnie, a w przeciwnym ujemne
//ujemne opóŸnienie oznacza faktyczne hamowanie
//pozycje te¿ s¹ normalizowane
//dla kier POW oryginalnie prêdkoœci s¹ ujemne a opóŸnienia dodatnie, powoduje to k³opoty
//dla kier RAD jest w porz¹dku, tylko pozycje s¹ ujemne, s¹ wiêc normalizowane
//normalizacja dotyczy tylko horyzontalnych danych, wertykalne s¹ ok


//znormalizowac pozycje pieszych!!





int main( int argc, char *argv[] )
{
	//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);


	Poligon mickiewicza(argv[1] );


	cout << "\nAnaliza trajektorii pieszych...";
	mickiewicza.analizuj_trajektorie_pieszych();


	//zapisanie cech pieszych do pliku dla potrzeb budowy klasyfikatora trajektorii
	
	/*	
		ofstream plik_trajektorie("piesi_cenaaa.csv",ios::out);
		for(int i=0;i<mickiewicza.piesi.pieszy.size();i++)
		{

			//plik_trajektorie<<mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.calkowita<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.efektywna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.w_pionie<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.w_poziomie<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_pionie<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_poziomie<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.predkosc.srednia<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.minimalna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.srednia<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.maksymalna<<";"<<
			//mickiewicza.piesi.pieszy[i].frame[0].pos.x<<";"<<
			//mickiewicza.piesi.pieszy[i].frame[mickiewicza.piesi.pieszy[i].frame.size()-1].pos.x<<";"<<
			//mickiewicza.piesi.pieszy[i].frame[0].pos.y<<";"<<
			//mickiewicza.piesi.pieszy[i].frame[mickiewicza.piesi.pieszy[i].frame.size()-1].pos.y<<endl;
			

			//kombinacyjne + probkowanie
			//	plik_trajektorie<<mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.maksymalna-mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.minimalna<<";"<<
			//(mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_efektywna>0 ? mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_calkowita/
			//mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_efektywna : -1)<<";";
			//////inne cechy probkowanie trajektorii
			//int nf=mickiewicza.piesi.pieszy[i].frame.size(); //liczba ramek pieszego
			//int ns=3; //liczba probek
			//if(nf>=ns) //ramek przynajmniej tyle ile probek
			//	for(int l=0;l<ns;l++)
			//		plik_trajektorie<<mickiewicza.piesi.pieszy[i].frame[l*(nf-1)/(ns-1)].pos.x<<";"<<
			//		mickiewicza.piesi.pieszy[i].frame[l*(nf-1)/(ns-1)].pos.y<<";";
			//else
			//{
			//	int l;
			//	for(l=0;l<nf;l++)
			//		plik_trajektorie<<mickiewicza.piesi.pieszy[i].frame[l].pos.x<<";"<<
			//		mickiewicza.piesi.pieszy[i].frame[l].pos.y<<";";
			//	for(l;l<ns;l++)
			//		plik_trajektorie<<mickiewicza.piesi.pieszy[i].frame[nf-1].pos.x<<";"<<
			//		mickiewicza.piesi.pieszy[i].frame[nf-1].pos.y<<";";
			//}
			//plik_trajektorie<<endl;

			//jeszcze inne
			
			//plik_trajektorie<<
			//mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.calkowita<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.efektywna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_calkowita<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_efektywna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.predkosc.minimalna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.predkosc.srednia<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.predkosc.srednia_z_pozycji<<";"<<
			////mickiewicza.piesi.pieszy[i].trajektoria.predkosc.maksymalna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.minimalna<<";"<<
			////mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.srednia<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.maksymalna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.minimalna<<";"<<
			////mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.srednia<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.maksymalna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.maksymalna-mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.minimalna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.maksymalna-mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.minimalna<<";"<<
			////mickiewicza.piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_pionie<<";"<<
			////mickiewicza.piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_poziomie<<";"<<
			////mickiewicza.piesi.pieszy[i].t1.wHour<<

			//endl;
			

			//kombinacyjne
			//5 ma wszystko
			//6 bez kombinacji
			//7 takze bez predkosci
			//8 bez pozycji
			//9 pierwsze 10 bez 5 - czyli 9
			
			plik_trajektorie<<
			//1
			mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.maksymalna-mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.minimalna<<";"<<
			//2
			(mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_efektywna>0 ? mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_calkowita/
			mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_efektywna : -1)<<";"<<
			//3
			(mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.maksymalna-mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.minimalna)/
			(mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.maksymalna-mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.minimalna)<<";"<<
			//4
			mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.efektywna<<";"<<	
			//5
			//mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.calkowita<<";"<<
			//6
			mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_calkowita<<";"<<
			//7
			mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.na_jezdni_efektywna<<";"<<
			
			//8
			mickiewicza.piesi.pieszy[i].trajektoria.predkosc.minimalna<<";"<<
			//9
			mickiewicza.piesi.pieszy[i].trajektoria.predkosc.srednia<<";"<<
			//10
			mickiewicza.piesi.pieszy[i].trajektoria.predkosc.srednia_z_pozycji<<";"<<
			//godzina tylko do statystyk, nie jako cecha!
			mickiewicza.piesi.pieszy[i].t1.wHour<<
		
			//mickiewicza.piesi.pieszy[i].trajektoria.predkosc.maksymalna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.predkosc.odch_st<<";"<<
			//
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.minimalna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.srednia<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.maksymalna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.minimalna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.srednia<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.maksymalna<<";"<<

			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.maksymalna-mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.minimalna<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.maksymalna-mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pionowa.minimalna<<";"<<

			//mickiewicza.piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_pionie<<";"<<
			//mickiewicza.piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_poziomie<<";"<<
			//mickiewicza.piesi.pieszy[i].t1.wHour<<
			
			endl;
		}
		plik_trajektorie.close();
    
*/
	//koniec

	cout << "\nAnaliza predkosci (i przyspieszenia)...";
	mickiewicza.analizuj_trajektorie_pojazdow(3,15);



	cout << "\nWyszukiwanie samochodow widocznych wraz z pieszymi...";
	mickiewicza.znajdz_pojazdy_dla_pieszych();


	

	cout << "\nWyznaczania minimalnej odleglosci pieszy-samochod...";
	mickiewicza.znajdz_minimalna_odleglosc_od_pojazdow();
	mickiewicza.znajdz_maksymalne_opoznienie_pojazdow(3,15);


	mickiewicza.zbieraj_statystyki();


	cout<<"\nZapisywanie statystyk pieszych..."<<endl;
	mickiewicza.zapisz_pieszych();	
	cout<<"Zapisywanie statystyk pojazdow..."<<endl;
	mickiewicza.zapisz_pojazdy();		
	cout<<"Zapisywanie pliku przegladowego wynikow filtracji..."<<endl;
	mickiewicza.zapisz_wyniki_filtracji();
	cout<<"\nZrobione...nacisnij Enter by zakonczyc program"<<endl;


	//plik_wy.open(nazwa_pliku_wyjsciowego+"0.htm",ios::out);
	//if( plik_wy.good() == false )
	//	cout << "Nie moge pisac w pliku wyjsciowym!" << endl;
	//else
	//	plik_wy << "<html><body>" <<endl;

	////drugi plik z danymi o trajektoriach pieszych
	//	//fstream plik_trajektorie;




	//cout << endl << "Liczba wykrytych pieszych nieprawidlowych (duchy i na samochodzie): " << mickiewicza.statystyka.calosciowe.piesi.razem_niepoprawna <<endl;
	//cout << endl << "Procent wykrytych pieszych nieprawidlowych: " << 100*mickiewicza.statystyka.calosciowe.piesi.razem_niepoprawna/mickiewicza.piesi.pieszy.size() <<endl;

	//cout << endl << "Liczba wykrytych pieszych bez samochodu: " << mickiewicza.piesi.liczba_pieszych_bez_samochodow <<endl;
	//cout << endl << "Procent wykrytych pieszych bez samochodu: " << 100*mickiewicza.piesi.liczba_pieszych_bez_samochodow/mickiewicza.piesi.pieszy.size() <<endl;

	//if(plik_wy.good())
	//{
	//	plik_wy << endl << "<br>Liczba wykrytych pieszych nieprawidlowych (duchy i na samochodzie): " << mickiewicza.statystyka.calosciowe.piesi.razem_niepoprawna <<"<br>"<<endl;
	//	plik_wy << endl << "<br>Procent wykrytych pieszych nieprawidlowych: " << 100*mickiewicza.statystyka.calosciowe.piesi.razem_niepoprawna/mickiewicza.piesi.pieszy.size() <<"<br>"<<endl;
	//	plik_wy << endl << "<br>Liczba wykrytych pieszych bez samochodu: " << mickiewicza.piesi.liczba_pieszych_bez_samochodow <<endl;
	//	plik_wy << endl << "<br>Procent wykrytych pieszych bez samochodu: " << 100*mickiewicza.piesi.liczba_pieszych_bez_samochodow/mickiewicza.piesi.pieszy.size() <<endl;
	//}


	//int licznik=1;
	//int licznik_plikow=1;
	//int sprawdzaj_przyspieszenie=1;

	//if(false)
	//{
	//	
	//	plik_trajektorie.open(nazwa_pliku_wyjsciowego+".csv",ios::out);
	//	plik_trajektorie<<"nr; id; odl. calk.; odl. efekt.; pion; poziom; drg. pion; drg. poziom; sr predk.; xmin; xsr; xmax\n";
	//	for(int i=0;i<mickiewicza.piesi.pieszy.size();i++)
	//	{
	//		//if(mickiewicza.piesi.pieszy[i].trajektoria.rodzaj.dobra==true)
	//		{

	//			plik_trajektorie << i << ";" << mickiewicza.piesi.pieszy[i].id;
	//			plik_trajektorie << ";"<< mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.calkowita << ";" << mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.efektywna <<";" << mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.w_pionie<< ";" << mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.w_poziomie;
	//			plik_trajektorie << ";" << mickiewicza.piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_pionie<< ";" << mickiewicza.piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_poziomie <<";"<<mickiewicza.piesi.pieszy[i].trajektoria.predkosc.srednia;
	//			plik_trajektorie << ";" <<mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.minimalna << ";" << mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.srednia << ";" <<mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.maksymalna <<endl;




	//			cout << "\n" << licznik << ". " << mickiewicza.piesi.pieszy[i].id << ":\n  nr: " << i << " czas: " << mickiewicza.piesi.pieszy[i].t1<<" razem: "<< mickiewicza.piesi.pieszy[i].pojazdy.razem <<" prawy: " <<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba << " lewy: " << mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba;
	//			cout << "\n minimalna odleglosc: " << mickiewicza.piesi.pieszy[i].konfliktowy_pojazd.minimalna_odleglosc;

	//			plik_wy << "<br>" << licznik << ". " << mickiewicza.piesi.pieszy[i].id << ":  nr: " << i << " czas: " << mickiewicza.piesi.pieszy[i].t1<<" razem: "<< mickiewicza.piesi.pieszy[i].pojazdy.razem <<" prawy: " <<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba << " lewy: " << mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba<<endl;
	//			plik_wy << "<br>przebyta odleglosc calkowita: "<< mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.calkowita << ", efektywna: " << mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.efektywna <<", w pionie: " << mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.w_pionie<< ", w poziomie: " << mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.w_poziomie <<endl;
	//			plik_wy << " wsp. drgan w pionie: " << mickiewicza.piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_pionie<< " wsp. drgan w poziomie: " << mickiewicza.piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_poziomie <<" srednia predkosc: "<<mickiewicza.piesi.pieszy[i].trajektoria.predkosc.srednia <<endl;
	//			plik_wy << " polozenie poziome min: " <<mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.minimalna << " polozenie poziome sr: " << mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.srednia << " max. polozenie: " <<mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.maksymalna <<endl;
	//			plik_wy << "<br> Minimalna odleglosc: " << mickiewicza.piesi.pieszy[i].konfliktowy_pojazd.minimalna_odleglosc;
	//			plik_wy << "<br>Kolejne pojazdy na prawym pasie: ";
	//			for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba;k++)
	//			{
	//				string nazwa_wlasciwa = mickiewicza.pojazdy_pp.pojazd[mickiewicza.piesi.pieszy[i].pojazdy.pp.nr[k]].sciezka +"/"+ mickiewicza.pojazdy_pp.pojazd[mickiewicza.piesi.pieszy[i].pojazdy.pp.nr[k]].id;
	//				plik_wy << "<a href=\"" << nazwa_wlasciwa << ".xml" << "\">" << "XML" <<"</a>, " << "<a href=\"" << nazwa_wlasciwa << ".mkv" << "\">" << "MKV" <<"</a> "<< " ";
	//			}
	//			plik_wy << "<br>Kolejne pojazdy na lewym pasie: ";
	//			for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba;k++)
	//			{
	//				string nazwa_wlasciwa = mickiewicza.pojazdy_lp.pojazd[mickiewicza.piesi.pieszy[i].pojazdy.lp.nr[k]].sciezka +"/"+ mickiewicza.pojazdy_lp.pojazd[mickiewicza.piesi.pieszy[i].pojazdy.lp.nr[k]].id;
	//				plik_wy << "<a href=\"" << nazwa_wlasciwa << ".xml" << "\">" << "XML" <<"</a>, " << "<a href=\"" << nazwa_wlasciwa << ".mkv" << "\">" << "MKV" <<"</a> "<< " ";
	//			}
	//			plik_wy << "<br>Minimalne odleglosci od kolejnych samochodow na prawym pasie: ";
	//			for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba;k++)
	//				plik_wy << mickiewicza.piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[k] << "  ";
	//			plik_wy << "<br>Minimalne odleglosci od kolejnych samochodow na lewym pasie: ";
	//			for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba;k++)
	//				plik_wy << mickiewicza.piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc[k] << "  ";

	//			plik_wy <<"<br> <video width=\"1000\"  controls>" <<endl<< "<source src=\"" << mickiewicza.piesi.pieszy[i].sciezka<< "/"<<mickiewicza.piesi.pieszy[i].id <<".mkv\"></video><br>"<<endl;

	//			licznik++;
	//			if(licznik%50==0)
	//			{
	//				plik_wy << "<br><br>Nastepna czesc: <a href=\""<<nazwa_pliku_wyjsciowego << licznik_plikow << ".htm\">"<<nazwa_pliku_wyjsciowego << licznik_plikow << ".htm" <<"</a> ";
	//				plik_wy << "</body></html>";
	//				plik_wy.close();
	//				stringstream ss;
	//				ss<<nazwa_pliku_wyjsciowego<<licznik_plikow<<".htm";
	//				plik_wy.open( ss.str(),ios::out);
	//				licznik_plikow++;
	//			}
	//		}
	//	}
	//}
	//else


	//	if(mickiewicza.filtry.w_wynikach_maja_byc.duchy)
	//		for(int i=0;i<mickiewicza.piesi.pieszy.size();i++)
	//		{
	//			if(mickiewicza.piesi.pieszy[i].trajektoria.rodzaj.duch==true)
	//			{
	//				cout << "\n" << licznik << ". " << mickiewicza.piesi.pieszy[i].id << ":\n  nr: " << i << " czas: " << mickiewicza.piesi.pieszy[i].t1<<" razem: "<< mickiewicza.piesi.pieszy[i].pojazdy.razem <<" prawy: " <<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba << " lewy: " << mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba;
	//				cout << "\n minimalna odleglosc: " << mickiewicza.piesi.pieszy[i].konfliktowy_pojazd.minimalna_odleglosc;

	//				plik_wy << "<br>" << licznik << ". " << mickiewicza.piesi.pieszy[i].id << ":  nr: " << i << " czas: " << mickiewicza.piesi.pieszy[i].t1<<" razem: "<< mickiewicza.piesi.pieszy[i].pojazdy.razem <<" prawy: " <<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba << " lewy: " << mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba<<endl;
	//				plik_wy << "<br>przebyta odleglosc calkowita: "<< mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.calkowita << ", efektywna: " << mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.efektywna <<", w pionie: " << mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.w_pionie<< ", w poziomie: " << mickiewicza.piesi.pieszy[i].trajektoria.dlugosc.w_poziomie <<endl;
	//				plik_wy << " wsp. drgan w pionie: " << mickiewicza.piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_pionie<< " wsp. drgan w poziomie: " << mickiewicza.piesi.pieszy[i].trajektoria.wspolczynnik_drgan_w_poziomie <<" srednia predkosc: "<<mickiewicza.piesi.pieszy[i].trajektoria.predkosc.srednia <<endl;
	//				plik_wy << " polozenie poziome min: " <<mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.minimalna << " polozenie poziome sr: " << mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.srednia << " max. polozenie: " <<mickiewicza.piesi.pieszy[i].trajektoria.pozycja.pozioma.maksymalna <<endl;
	//				plik_wy << "<br> Minimalna odleglosc: " << mickiewicza.piesi.pieszy[i].konfliktowy_pojazd.minimalna_odleglosc;
	//				plik_wy << "<br>Kolejne pojazdy na prawym pasie: ";
	//				for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba;k++)
	//				{
	//					string nazwa_wlasciwa = mickiewicza.pojazdy_pp.pojazd[mickiewicza.piesi.pieszy[i].pojazdy.pp.nr[k]].sciezka +"/"+ mickiewicza.pojazdy_pp.pojazd[mickiewicza.piesi.pieszy[i].pojazdy.pp.nr[k]].id;
	//					plik_wy << "<a href=\"" << nazwa_wlasciwa << ".xml" << "\">" << "XML" <<"</a>, " << "<a href=\"" << nazwa_wlasciwa << ".mkv" << "\">" << "MKV" <<"</a> "<< " ";
	//				}
	//				plik_wy << "<br>Kolejne pojazdy na lewym pasie: ";
	//				for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba;k++)
	//				{
	//					string nazwa_wlasciwa = mickiewicza.pojazdy_lp.pojazd[mickiewicza.piesi.pieszy[i].pojazdy.lp.nr[k]].sciezka +"/"+ mickiewicza.pojazdy_lp.pojazd[mickiewicza.piesi.pieszy[i].pojazdy.lp.nr[k]].id;
	//					plik_wy << "<a href=\"" << nazwa_wlasciwa << ".xml" << "\">" << "XML" <<"</a>, " << "<a href=\"" << nazwa_wlasciwa << ".mkv" << "\">" << "MKV" <<"</a> "<< " ";
	//				}
	//				plik_wy << "<br>Minimalne odleglosci od kolejnych samochodow na prawym pasie: ";
	//				for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba;k++)
	//					plik_wy << mickiewicza.piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[k] << "  ";
	//				plik_wy << "<br>Minimalne odleglosci od kolejnych samochodow na lewym pasie: ";
	//				for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba;k++)
	//					plik_wy << mickiewicza.piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc[k] << "  ";

	//				plik_wy <<"<br> <video width=\"1000\"  controls>" <<endl<< "<source src=\"" << mickiewicza.piesi.pieszy[i].sciezka<< "/"<<mickiewicza.piesi.pieszy[i].id <<".mkv\"></video><br>"<<endl;

	//				licznik++;
	//				if(licznik%50==0)
	//				{
	//					plik_wy << "<br><br>Nastepna czesc: <a href=\""<<nazwa_pliku_wyjsciowego << licznik_plikow << ".htm\">"<<nazwa_pliku_wyjsciowego << licznik_plikow << ".htm" <<"</a> ";
	//					plik_wy << "</body></html>";
	//					plik_wy.close();
	//					stringstream ss;
	//					ss<<nazwa_pliku_wyjsciowego<<licznik_plikow<<".htm";
	//					plik_wy.open( ss.str(),ios::out);
	//					licznik_plikow++;
	//				}
	//			}
	//		}
	//	else
	//		if(mickiewicza.filtry.w_wynikach_maja_byc.konflikty)
	//		{
	//			for(int i=0;i<mickiewicza.piesi.pieszy.size();i++)
	//			{
	//				//pominiecie nieprawidlowo wykrytych pieszych
	//				if(mickiewicza.piesi.pieszy[i].trajektoria.rodzaj.duch==true)
	//					continue;
	//				if(mickiewicza.piesi.pieszy[i].trajektoria.rodzaj.pieszy_na_samochodzie==true)
	//					continue;
	//				//pominiecie pieszych bez samochodu
	//				if(mickiewicza.piesi.pieszy[i].pojazdy.razem==0) continue;
	//				//sprawdzenie czy ktorykolwiek pojazd przekroczyl opoznienie
	//				float md=mickiewicza.piesi.pieszy[i].maksymalne_opoznienie;
	//				float md2=mickiewicza.znajdz_najwieksze_opoznienie_pojazdu_dla_pieszego(i);
	//				//maksymalne opoznienie w ogole
	//				//if(mickiewicza.znajdz_najwieksze_opoznienie_pojazdu_dla_pieszego(i)>-4) continue;
	//				//maksymalne opoznienie w zakresie widocznosci pieszego
	//				if(mickiewicza.piesi.pieszy[i].maksymalne_opoznienie>-4) continue;
	//				cout << "\n" << licznik << ". " << mickiewicza.piesi.pieszy[i].id << ":\n  nr: " << i << " czas: " << mickiewicza.piesi.pieszy[i].t1<<" razem: "<< mickiewicza.piesi.pieszy[i].pojazdy.razem <<" prawy: " <<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba << " lewy: " << mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba;
	//				cout << "\n minimalna odleglosc: " << mickiewicza.piesi.pieszy[i].konfliktowy_pojazd.minimalna_odleglosc;


	//				if(plik_wy.good())
	//				{
	//					plik_wy << "<br>" << licznik << ". " << mickiewicza.piesi.pieszy[i].id << ":  nr: " << i << " czas: " << mickiewicza.piesi.pieszy[i].t1<<" razem: "<< mickiewicza.piesi.pieszy[i].pojazdy.razem <<" prawy: " <<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba << " lewy: " << mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba<<endl;
	//					plik_wy << "<br> Minimalna odleglosc: " << mickiewicza.piesi.pieszy[i].konfliktowy_pojazd.minimalna_odleglosc;
	//					plik_wy << "<br>Kolejne pojazdy na prawym pasie: ";
	//					for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba;k++)
	//					{
	//						string nazwa_wlasciwa = mickiewicza.pojazdy_pp.pojazd[mickiewicza.piesi.pieszy[i].pojazdy.pp.nr[k]].sciezka +"/"+ mickiewicza.pojazdy_pp.pojazd[mickiewicza.piesi.pieszy[i].pojazdy.pp.nr[k]].id;
	//						plik_wy << "<a href=\"" << nazwa_wlasciwa << ".xml" << "\">" << "XML" <<"</a>, " << "<a href=\"" << nazwa_wlasciwa << ".mkv" << "\">" << "MKV" <<"</a> "<< " ";

	//					}
	//					plik_wy << "<br>Kolejne pojazdy na lewym pasie: ";
	//					for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba;k++)
	//					{
	//						string nazwa_wlasciwa = mickiewicza.pojazdy_lp.pojazd[mickiewicza.piesi.pieszy[i].pojazdy.lp.nr[k]].sciezka +"/"+ mickiewicza.pojazdy_lp.pojazd[mickiewicza.piesi.pieszy[i].pojazdy.lp.nr[k]].id;
	//						plik_wy << "<a href=\"" << nazwa_wlasciwa << ".xml" << "\">" << "XML" <<"</a>, " << "<a href=\"" << nazwa_wlasciwa << ".mkv" << "\">" << "MKV" <<"</a> "<< " ";
	//					}
	//					plik_wy << "<br>Minimalne odleglosci od kolejnych samochodow na prawym pasie: ";
	//					for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba;k++)
	//						plik_wy << mickiewicza.piesi.pieszy[i].pojazdy.pp.minimalna_odleglosc[k] << "  ";
	//					plik_wy << "<br>Minimalne odleglosci od kolejnych samochodow na lewym pasie: ";
	//					for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba;k++)
	//						plik_wy << mickiewicza.piesi.pieszy[i].pojazdy.lp.minimalna_odleglosc[k] << "  ";

	//					if(sprawdzaj_przyspieszenie)
	//					{
	//						plik_wy << "<br>Maksymalne opoznienia (oraz delta Vx, Vxmax-Vxmin) kolejnych samochodow na prawym pasie: ";
	//						for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.pp.liczba;k++)
	//						{
	//							int nr_pojazdu=mickiewicza.piesi.pieszy[i].pojazdy.pp.nr[k];
	//							if(mickiewicza.pojazdy_pp.pojazd[nr_pojazdu].vel.min_x<.2)
	//								plik_wy<<"<font color=\"red\">";
	//							plik_wy << mickiewicza.pojazdy_pp.pojazd[nr_pojazdu].acc.min_x << "("<< mickiewicza.pojazdy_pp.pojazd[nr_pojazdu].vel.max_x-mickiewicza.pojazdy_pp.pojazd[nr_pojazdu].vel.min_x <<", "<<mickiewicza.pojazdy_pp.pojazd[nr_pojazdu].vel.max_x<<"-"<<mickiewicza.pojazdy_pp.pojazd[nr_pojazdu].vel.min_x <<")  ";

	//							if(mickiewicza.pojazdy_pp.pojazd[nr_pojazdu].vel.min_x<.2)
	//								plik_wy<<"</font>";


	//						}
	//						plik_wy << "<br>Maksymalne opoznienia (oraz delta Vx, Vxmax-Vxmin) kolejnych samochodow na lewym pasie: ";
	//						for(int k=0;k<mickiewicza.piesi.pieszy[i].pojazdy.lp.liczba;k++)
	//						{
	//							int nr_pojazdu=mickiewicza.piesi.pieszy[i].pojazdy.lp.nr[k];
	//							if(mickiewicza.pojazdy_lp.pojazd[nr_pojazdu].vel.min_x<.2)
	//								plik_wy<<"<font color=\"red\">";
	//							plik_wy << mickiewicza.pojazdy_lp.pojazd[nr_pojazdu].acc.min_x << "("<< mickiewicza.pojazdy_lp.pojazd[nr_pojazdu].vel.max_x-mickiewicza.pojazdy_lp.pojazd[nr_pojazdu].vel.min_x<<", "<<mickiewicza.pojazdy_lp.pojazd[nr_pojazdu].vel.max_x<<"-"<<mickiewicza.pojazdy_lp.pojazd[nr_pojazdu].vel.min_x<<")  ";
	//							if(mickiewicza.pojazdy_lp.pojazd[nr_pojazdu].vel.min_x<.2)
	//								plik_wy<<"</font>";


	//						}
	//					}

	//					plik_wy <<"<br> <video width=\"1000\"  controls>" <<endl<< "<source src=\"" << mickiewicza.piesi.pieszy[i].sciezka<< "/"<<mickiewicza.piesi.pieszy[i].id <<".mkv\"></video><br>"<<endl;
	//				}
	//				licznik++;
	//				if(licznik%50==0)
	//				{
	//					plik_wy << "<br><br>Nastepna czesc: <a href=\""<<nazwa_pliku_wyjsciowego << licznik_plikow << ".htm\">"<<nazwa_pliku_wyjsciowego << licznik_plikow << ".htm" <<"</a> ";
	//					plik_wy << "</body></html>";
	//					plik_wy.close();
	//					stringstream ss;
	//					ss<<nazwa_pliku_wyjsciowego<<licznik_plikow<<".htm";
	//					plik_wy.open( ss.str(),ios::out);
	//					licznik_plikow++;
	//				}
	//			}




	//		}


	//		plik_wy << "</body></html>";
	//		plik_wy.close();
	//		plik_trajektorie.close();



			if(argc>2)
				cin.ignore(1);
			cout<<"Zwalnianie pamieci....okno konsoli zaraz samo sie zamknie";
}






