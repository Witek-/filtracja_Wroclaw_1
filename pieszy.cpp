#include "pieszy.h"


Pieszy::Pieszy()
{

	//minimalna_odleglosc=9999;
	//maksymalne_opoznienie=9999;


}


int Pieszy::laduj(string nazwa_pliku)
{
	frame.clear();
	frame.reserve(200);
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
	
	//odczyt parametrow pieszego
	xml_node object = doc.child("root").child("object");
		if(object==NULL) 
		{
		cout << result.description() <<endl;
		cout <<"Nieprawidlowy plik: "<< nazwa_pliku << endl;
		cout <<" nacisnij ENTER by zakonczyc"<< endl;
		cin.ignore(1);
		exit(-1);
	}
	id = object.child_value("id");
	xml_node n_frames = doc.child("root").child("frames");


	struct Frame f;
	for (xml_node n_frame = n_frames.first_child(); n_frame; n_frame = n_frame.next_sibling())
	{

		f.pos.x=atof(n_frame.child("pos").child_value("x"));
		f.pos.y=atof(n_frame.child("pos").child_value("y"));
		f.vel.x=atof(n_frame.child("vel").child_value("x"));
		f.vel.y=atof(n_frame.child("vel").child_value("y"));
		f.ts=systemtime_z_daty(n_frame.child_value("ts"));
		frame.push_back(f);

	}
	////analiza trajektorii z pominieciem pierwszego i ostatniego zapisu, bo sa chyba bledne
	//trajektoria.pozycja.pozioma.minimalna=9999;
	//trajektoria.pozycja.pozioma.maksymalna=-9999;
	//trajektoria.pozycja.pozioma.srednia=0;
	//

	//for(int i=1;i<frame.size()-2;i++)
	//{
	//	if(frame[i].pos.x<trajektoria.pozycja.pozioma.minimalna) trajektoria.pozycja.pozioma.minimalna=frame[i].pos.x;
	//	if(frame[i].pos.x>trajektoria.pozycja.pozioma.maksymalna) trajektoria.pozycja.pozioma.maksymalna=frame[i].pos.x;
	//	trajektoria.pozycja.pozioma.srednia+=frame[i].pos.x;

	//	float predkosc_chwilowa=sqrt(frame[i].vel.x*frame[i].vel.x+frame[i].vel.y*frame[i].vel.y);
	//	trajektoria.predkosc.srednia+=predkosc_chwilowa;
	//	if(predkosc_chwilowa>trajektoria.predkosc.maksymalna) trajektoria.predkosc.maksymalna=predkosc_chwilowa;
	//	if(predkosc_chwilowa<trajektoria.predkosc.minimalna) trajektoria.predkosc.minimalna=predkosc_chwilowa;
	//}
	//trajektoria.predkosc.srednia/=(frame.size()-2);
	//trajektoria.pozycja.pozioma.srednia/=(frame.size()-2);
	////przy okazji notujemy chwile pojawienia sie i znikniecia
	//t1=frame[0].ts;
	//t2=frame[frame.size()-1].ts;

	////maj¹c chwile pojawienia siê i znikniêcia, mo¿emy policzyæ drug¹ œredni¹ prêdkoœæ
	////na podstawie przebytej odleg³oœci i czasu
	//float t=(t2-t1)*24*60*60;
	//trajektoria.predkosc.srednia_z_pozycji=trajektoria.dlugosc.calkowita/t;
	return 1;
};