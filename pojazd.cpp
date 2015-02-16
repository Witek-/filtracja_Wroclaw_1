#include"pojazd.h"

Pojazd::Pojazd()
{
	liczba_pieszych=0;
	wypisany_w_statystykach=false;
}

int Pojazd::laduj(string nazwa_pliku, bool pozycja_dodatnia_predkosc_ujemna)
{
	frame.clear();
	xml_document doc;
	xml_parse_result result = doc.load_file(nazwa_pliku.c_str());
	if(result.status!=status_ok)
	{
		cout << result.description() <<endl;
		cout <<"nacisnij ENTER by zakonczyc"<< endl;
		cin.ignore(1);
		exit(-1);
	}
	sciezka=nazwa_pliku.substr(0, nazwa_pliku.find_last_of("/"));
	//odczyt parametrow pojazdu

	xml_node object = doc.child("root").child("object");
	if(object==NULL) 
	{
		cout << result.description() <<endl;
		cout <<"Nieprawidlowy plik: "<< nazwa_pliku << endl;
		cout <<" nacisnij ENTER by zakonczyc"<< endl;
		cin.ignore(1);
		exit(-1);
	}
	symbol = object.child_value("symbol");
	manufacturer=object.child_value("manufacturer");
	type=object.child_value("type");
	id = object.child_value("id");
	xml_node n_frames = doc.child("root").child("frames");


	//normalizacja
	if(pozycja_dodatnia_predkosc_ujemna==true)
		for (xml_node n_frame = n_frames.first_child(); n_frame; n_frame = n_frame.next_sibling())
	{
		struct Frame f;
		f.pos.x=atof(n_frame.child("pos").child_value("x"));
		f.pos.y=atof(n_frame.child("pos").child_value("y"));
		f.vel.x=-atof(n_frame.child("vel").child_value("x"));
		f.vel.y=atof(n_frame.child("vel").child_value("y"));
		f.acc.x=-atof(n_frame.child("acc").child_value("x"));
		f.acc.y=atof(n_frame.child("acc").child_value("y"));
		f.ts=systemtime_z_daty(n_frame.child_value("ts"));
		frame.push_back(f);
	}
	else
	for (xml_node n_frame = n_frames.first_child(); n_frame; n_frame = n_frame.next_sibling())
	{
		struct Frame f;
		f.pos.x=-atof(n_frame.child("pos").child_value("x"));
		f.pos.y=atof(n_frame.child("pos").child_value("y"));
		f.vel.x=atof(n_frame.child("vel").child_value("x"));
		f.vel.y=atof(n_frame.child("vel").child_value("y"));
		f.acc.x=atof(n_frame.child("acc").child_value("x"));
		f.acc.y=atof(n_frame.child("acc").child_value("y"));
		f.ts=systemtime_z_daty(n_frame.child_value("ts"));
		frame.push_back(f);
	}
	//przy okazji notujemy chwile pojawienia sie i znikniecia
	t1=frame[0].ts;
	t2=frame[frame.size()-1].ts;
	return 1;
}
///Wyznacza skrajne warto�ci pr�dko�ci i przyspieszenia 
/// <summary>
/// Wyznacza skrajne warto�ci pr�dko�ci i przyspieszenia danego pojazdu w podanym zakresie odleg�o�ci.
/// Pe�en zakres obserwacji nie jest dobry, bo trajektorie mog� mie� spore b��dy w du�ej odleg�o�ci od przej�cia.
/// </summary>
/// <param name="odl_min">Pocz�tek zakresu przeszukiwania pr�dko�ci (odleg�o�� w metrach od �rodka pas�w</param>
/// <param name="odl_max">Koniec zakresu przeszukiwania pr�dko�ci (odleg�o�� w metrach od �rodka pas�w</param>
/// <param name="w_punkcie">Odleg�o�� od �rodka pas�w dla kt�rej wyznaczana jest pr�dko�� samochodu</param>
int Pojazd::analizuj_predkosc(float odl_min, float odl_max, float w_punkcie)
{
	//szuka dziur w trajektorii czyli najwiekszych przerw w zarejestrowanych pozycjach, a co za tym idzie pr�dko�ciach itd
	//pozycje mog� mie� znak ujemny lub dodatni

	
	{
		for(int i=1;i<frame.size();i++)
		{
			if(frame[i-1].pos.x-frame[i].pos.x>	dziura_w_trajektorii.rozmiar_x)
			{
				dziura_w_trajektorii.rozmiar_x=frame[i-1].pos.x-frame[i].pos.x;
				dziura_w_trajektorii.pozycja_x=frame[i-1].pos.x;
				dziura_w_trajektorii.rozmiar_czas=(frame[i-1].ts-frame[i].ts)*(1*24*60*60);
			}
		}
		//przy okazji zapami�tywana jest ostatnia zarejestrowana pozycja, bo to te� jest swoista dziura, je�li ostatnio samoch�d zarejestrowano
		//z dala od pas�w
		dziura_w_trajektorii.ostatnia_pozycja_x=frame[frame.size()-1].pos.x;

		//zapisuje czy pojazd jest na pasach czy poza nimi (chodzi o rejestracje!)
		for(int i=0;i<frame.size();i++)
			if(frame[i].pos.x>=-2 && frame[i].pos.x<=2) frame[i].pos.na_pasach=true;
			else
				frame[i].pos.na_pasach=false;


		//liczy skrajne warto�ci pr�dko�ci i przyspiesze�
		for(int i=0;i<frame.size();i++)
		{
			//pr�dko�ci i przyspieszenia zosta�y znormalizowane
			if(frame[i].pos.x>odl_max || frame[i].pos.x<odl_min) continue;
			if(frame[i].vel.x>vel.max_x) vel.max_x=frame[i].vel.x;
			if(frame[i].vel.y>vel.max_y) vel.max_y=frame[i].vel.y;
			if(frame[i].vel.x<vel.min_x) vel.min_x=frame[i].vel.x;
			if(frame[i].vel.y<vel.min_y) vel.min_y=frame[i].vel.y;
			if(frame[i].acc.x>acc.max_x) acc.max_x=frame[i].acc.x;
			if(frame[i].acc.y>acc.max_y) acc.max_y=frame[i].acc.y;
			if(frame[i].acc.x<acc.min_x) acc.min_x=frame[i].acc.x;
			if(frame[i].acc.y<acc.min_y) acc.min_y=frame[i].acc.y;
		}
		//wyznaczenie pr�dko�ci w punkcie jako �redniej pr�dko�ci tu� za i tu� przed


		for(int i=0;i<frame.size();i++)
		{

			if(frame[i].pos.x>=w_punkcie) continue;
			else
				if(i>0)
				{
					vel.w_punkcie=abs(frame[i-1].vel.x+frame[i].vel.x)/2;
					break;
				}
				else
					if(i==0)
					{
						vel.w_punkcie=abs(frame[i].vel.x);
					break;
					}

		}

		//wyznaczenie w identyczny sposob pr�dko�ci na �rodku pas�w

		for(int i=0;i<frame.size();i++)
		{

			if(frame[i].pos.x>=0) continue;
			else
				if(i>0)
				{
					vel.na_pasach=abs(frame[i-1].vel.x+frame[i].vel.x)/2;
					break;
				}
				else
					if(i==0)
					{
						vel.na_pasach=abs(frame[i].vel.x);
					break;
					}

		}
	}
	


	return 1;
}
void Pojazd::wyswietlStempleCzasowe()
{
	for(int i=0;i<frame.size();i++)
		cout << frame[i].ts << ", ";
	cout <<endl;
}