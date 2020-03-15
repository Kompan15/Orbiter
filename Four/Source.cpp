include <iostream>
#include "SDL.h"
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <string>
#include <typeinfo>

#define PI 3.1415926535897
#define DI 10000 
#define PXX 800
#define PXY 600
#define G 0.00000000006

using namespace std;

long long int masam, masaM, fiPocz, Vpocz, VxPocz, VyPocz, Vmax, Vmin, rmax, rmin;
long double V = 0, Vx = 1, Vy = 1, Afx = 0, Afy = 0, fi; 
long long int xm, ym, xM, yM; 
bool fiW = 0; 
long long int a, b, c, d, e, f; 
long long int odleglosc_koncowa = 0, droga_calkowita = 0;

void wprowadzanie(){
	cout << "jak wprowadzac dane?"
		<< "\n\n program generuje okno w rozdzielczosci 800X600 pixeli\n Program traktuje je jako 'odwrocony' kartezjanski uklad wspolrzednych\n"
		<< "znaczy to tyle, ze y = 0  znajduje sie u gory ukladu, x pozostaje bez zmian\n" << endl;
	system("Pause");

	cout << "\nwprowadz modul predkosci np: ~3000 [m/s]:\n";
	do{
		do{
			cin.clear(); cin.ignore(256, '\n');
			cout << "\nV:"; cin >> Vpocz;
			if (cin.fail() == 1){ cout << "podana zmienna jest zlego typu"; }
		} while (cin.fail() != 0);
		if (Vpocz > 20000 || Vpocz < 0){ cout << "wez nie przeginaj.."; }
	} while (Vpocz > 20000|| Vpocz < 0);
	system("cls");
	cout << "\nwprowadz kat fi pomiedzy nim a osia x (0-360) wiekszy/mniejszy zostanie przeskalowany np: ~320:";
	do{
		cin.clear(); cin.ignore(256, '\n');
		cout << "\nfi:"; cin >> fiPocz;
		if (cin.fail() == 1){ cout << "nieodpowiedni typ zmiennej"; }
	} while (cin.fail() != 0);
	system("cls");

	cout << "\npolozenie orbitowanego obiektu\n\n	planeta: 400X300,\n	jednostka rozdzielczosci: " << DI << " metrow/pixel,\n	rozmiary okna" << PXX << "x" << PXY;
	do{
		do{
			cin.clear(); cin.ignore(256, '\n');
			cout << "\nmx np: ~230:";  cin >> xm; cout << "\nym np: ~150:"; cin >> ym;
			if (cin.fail() == 1){ cout << "nieodpowiedni typ zmiennej"; }
		} while (cin.fail() != 0);
		if (xm>PXX || ym > PXY || xm<0 || ym<0){ cout << "przekroczyles dopuszczalna rozdzielczosc"; }
	} while (xm>PXX || ym > PXY || xm<0 || ym<0);

	xm *= DI; ym *= DI; system("cls");

	cout << "\nMasa ciala (jako wielokrotnosc masy ziemi) np: ~1, 2 ,3... \n";

	do{
		do{
			cin.clear(); cin.ignore(256, '\n');
			cout << "\nM:"; cin >> masaM;
			if (cin.fail() == 1){ cout << "nieodpowiedni typ zmiennej"; }
		} while (cin.fail() != 0);
		if (masaM > 10000 || masaM <0){ cout << "wez nie przeginaj.."; }
	} while (masaM > 10000 || masaM <0);
	masaM *= 60000000000000;

	cout << "\nobiekt umieszczony w przestrzeni: np: ~15000[kg]\n";

	do{
		do{
			cin.clear(); cin.ignore(256, '\n');
			cout << "\nm:"; cin >> masam;
			if (cin.fail() == 1){ cout << "nieodpowiedni typ zmiennej"; }
		} while (cin.fail() != 0);
		if (masam > masaM / 1000 || masam <0){ cout << "wez nie przeginaj.."; }
	} while (masam > masaM / 1000 || masam <0);
	system("cls");
}

int wczytaj_warunki_poczatkowe(){
	int s;
	cout << "1 - prosta orbita \n2 - odkrzywienie toru \n3 - orbita kolizyjna \n4 - wprowadz wlasne dane";
	do{
		do{
			cin.clear(); cin.ignore(256, '\n');
			cout << "\nopcja:"; cin >> s;
			if (cin.fail() == 1){ cout << "podana zmienna jest zlego typu"; }
		} while (cin.fail() != 0);
		if (s > 4 || s < 1){ cout << "Wybierz jedna z opcji!"; }
	} while (s > 4 || s < 1);
	switch (s){

	case 1:
		Vpocz = 5700;
		fiPocz = 270;
		xm = 270; xm *= DI;
		ym = 300; ym *= DI;
		masaM = 60000000000000;
		masam = 15000;
		break;
	case 2:
		Vpocz = 13000;
		fiPocz = 210;
		xm = 700; xm *= DI;
		ym = 550; ym *= DI;
		masaM = 70000000000000;
		masam = 700000000000;
		break;
	case 3:
		Vpocz = 3000;
		fiPocz = 30;
		xm = 200; xm *= DI;
		ym = 500; ym *= DI;
		masaM = 60000000000000;
		masam = 700000000000;
		break;
	case 4:
		wprowadzanie();
		break;
	}
		return 0;
}


long long int odleglosc(){
	long long int a = abs(xM - xm);
	long long int b = abs(yM - ym);
	long long r = sqrt(a*a + b*b);
	return r;
}

int kwadrant_triangulacja(){ 
	int cwf = 0;
	if (ym<yM && xm<xM) cwf = 0;
	else if (ym<yM && xm>xM) cwf = 1;
	else if (ym>yM && xm>xM) cwf = 2;
	else if (ym>yM && xm<xM) cwf = 3;
	return cwf;
}

class timer {
private:
	unsigned long begTime;
public:
	void start() {
		begTime = clock();
	}

	unsigned long elapsedTime() {
		return ((unsigned long)clock() - begTime) / CLOCKS_PER_SEC;
	}

	bool isTimeout(unsigned long seconds) {
		return seconds >= elapsedTime();
	}
};

class sila{
	
public:
	
	double angle(){
		double phi;
		double a = abs(yM - ym);
		phi = sin((a) / odleglosc()); 
		double b = ((phi * 180) / PI); 
		return b;
	}

	
	long double force(){
		double f = (masaM*masam) / (odleglosc()*odleglosc());
		return f;
	}
	
	long double przyspieszenie(){
		double a = (masaM) / (odleglosc()*odleglosc());
		return a;
	}
	
	long double afx(){
		long double ax = przyspieszenie()*cos(angle()*PI / 180);
		long double a = abs(ax);
		if (kwadrant_triangulacja() == 1 || kwadrant_triangulacja() == 2){
			a *= (-1); Afx = a; return a;
		}
		else
			Afx = a;
		return a;
	}

	long double afy(){
		long double ay = przyspieszenie()*sin(angle()*PI / 180);
		long double b = abs(ay);
		if (kwadrant_triangulacja() == 2 || kwadrant_triangulacja() == 3){
			b *= (-1); Afy = b; return b;
		}
		else
			Afy = b;
		return b;
	}

};

class predkosc{
public: 
	int kwadrant_predkosci(){ 
		int cwiartka;
		if (fi>360){ fi = abs(fi - 360); }
		else if (fi<0){ fi = abs(360 + fi); }
		if (fi <= 90 && fi >= 0) cwiartka = 0;
		else if (fi>90 && fi <= 180) cwiartka = 1;
		else if (fi>180 && fi <= 270) cwiartka = 2;
		else if (fi>270 && fi <= 360) cwiartka = 3;
		return cwiartka;
	}

	double angle(){
		double alfa = atan2(Vy, Vx); 
		double b = abs(alfa);
		double theta = (b * 180 / PI);
		return theta + skladowe_cwiartkaV() * 90;
	}

	
	double skladowe_cwiartkaV(){
		double cwiartkav;
		if (Vx > 0 && Vy > 0){ return cwiartkav = 0; }
		else if (Vx < 0 && Vy > 0){ return cwiartkav = 1; }
		else if (Vx < 0 && Vy < 0){ return cwiartkav = 2; }
		else if (Vx > 0 && Vy < 0){ return cwiartkav = 3; }
	}

	long double zlozenie_skladowych(){
		long double Vi = abs(sqrt(Vx*Vx + Vy*Vy));
		V = Vi;
		return Vi;
	}
};


double agreguj_i_transferuj(){ 
	Vx += Afx; 
	Vy += Afy;

	xm += Vx; 
	xm = abs(xm);
	ym += Vy;
	ym = abs(ym);

	droga_calkowita += abs(V);
	return 0;
}

int inicjacja_kwadrantu_predkosci(){ 
	int cwiartka; 
	if (fiPocz > 360){
		fiPocz = abs(fiPocz - 360);
		if (fiPocz <= 90 && fiPocz >= 0){ Vx *= 1;    Vy *= 1;    cwiartka = 0; }
		else if (fiPocz > 90 && fiPocz <= 180){ Vx *= (-1); Vy *= 1;    cwiartka = 1; }
		else if (fiPocz > 180 && fiPocz <= 270){ Vx *= (-1); Vy *= (-1); cwiartka = 2; }
		else if (fiPocz > 270 && fiPocz <= 360){ Vx *= 1, Vy *= (-1); cwiartka = 3; }
		return cwiartka;
	}
	else if (fiPocz	<	0){
		fiPocz = abs(360 + fiPocz);
		if (fiPocz <= 90 && fiPocz >= 0){ Vx *= 1;    Vy *= 1;    cwiartka = 0; }
		else if (fiPocz > 90 && fiPocz <= 180){ Vx *= (-1); Vy *= 1;    cwiartka = 1; }
		else if (fiPocz > 180 && fiPocz <= 270){ Vx *= (-1); Vy *= (-1); cwiartka = 2; }
		else if (fiPocz > 270 && fiPocz <= 360){ Vx *= 1, Vy *= (-1); cwiartka = 3; }
		return cwiartka;
	}
}

double inicjacja(){ 
	V = Vpocz;
	fi = fiPocz;
	
	Vx *= V*cos(fi*PI / 180); 
	Vy *= V*sin(fi*PI / 180);

	xM = PXX / 2; xM *= DI; 
	yM = PXY / 2; yM *= DI;
	c = xM; d = yM;
	a = xm; b = ym;
	VxPocz = Vx; 
	VyPocz = Vy;
	
	return 0;
}

void wypisz_warunki_poczatkowe(){
	cout << "\n*****\nMpocz xM,yM:" << c << "," << d
		<< "\nmPocz:xm,ym:" << a << "," << b
		<< "\nrPocz:" << e
		<< "\nVPocz:" << Vpocz
		<< "\nfiPocz:" << fiPocz
		<< "\nVxPocz:" << VxPocz
		<< "\nvyPocz:" << VyPocz
		<< "\nkwadrant p." << f << endl;
}

void zapisz_maksyma(){
	Vmin = Vpocz;
	rmin = e;
	if (V > Vmax) Vmax = V;
	if (V < Vmin) Vmin = V;
	if (odleglosc() > rmax) rmax = odleglosc();
	if (odleglosc() < rmin) rmin = odleglosc();
}

void inne_wielkosci(){
	
	long double  P1, P2;
	P1 = -((masaM*G) / e); 
	P2 = -((masaM*G) / odleglosc_koncowa);

	long double praca = masam*abs(P2 - P1);
	cout <<"\npraca wykonana przez sily w centralnym polu grawitacyjnym: " << praca << "[J]"
		<< "\n\npredkosc maksymalna: " << Vmax <<"[m/s]\n\npredkosc minimalna: " << Vmin << "[m/s]"
		<< "\n\nPerycentrum (peryapsis): "<< rmin << "[m], " << "\n\nApocentrum (apoapsis): " << rmax <<"[m]\n"<< endl;
}

int main(int argc, char ** argv)
{
	int exit = 1;
	do{
		
		int x = 0;
		int y = 0;		
		int q = 0;
		int w = 0;
		float gloriousTime = 0;		
		bool leftMouseButtonDown = false;		
		bool quit = false;	
		SDL_Event event;

		system("cls");
		
		sila VectorA;
		predkosc VectorV;
		wczytaj_warunki_poczatkowe();
		f = inicjacja_kwadrantu_predkosci();
		inicjacja();
		e = odleglosc();
		
		unsigned long czas;
		timer t;
		int a = 0, a0 = 0;
		czas = 3200;
		system("Pause");
	
		SDL_Init(SDL_INIT_VIDEO);		
		SDL_Window * window = SDL_CreateWindow("RĂłwnania pozycyjne", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, PXX, PXY, 0);		
		SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);		
		SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, PXX, PXY);
		Uint32 * pixels = new Uint32[PXX * PXY];

		q = xM / DI; 
		w = yM / DI;
	
		SDL_Surface * image = SDL_LoadBMP("bmp/DeathStar2.bmp");
		SDL_Texture * texture1 = SDL_CreateTextureFromSurface(renderer, image); 
		SDL_FreeSurface(image);
		SDL_Rect dstrect = { q - 33, w - 33, 66, 66 }; 
		
		SDL_Surface * image1 = SDL_LoadBMP("bmp/falcon-1.bmp");
		SDL_Texture * texture2 = SDL_CreateTextureFromSurface(renderer, image1);
		SDL_FreeSurface(image1);
		SDL_Point center;
		
		memset(pixels, 0, PXX * PXY * sizeof(Uint32));	
		SDL_UpdateTexture(texture, NULL, pixels, PXX * sizeof(Uint32));

		t.start();

		do{
			if (odleglosc() <= 33*DI){
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Kolizja", "Doszlo do kolizji, program teraz wroci do menu glownego.", NULL);
				break;
			}
			
			odleglosc();
			VectorV.kwadrant_predkosci();
			VectorA.force();
			VectorA.przyspieszenie(); 
			kwadrant_triangulacja();
			VectorA.angle();
			VectorA.afx();
			VectorA.afy();
	        VectorV.skladowe_cwiartkaV();
			VectorV.angle();
			VectorV.zlozenie_skladowych();
			zapisz_maksyma();
			odleglosc_koncowa = odleglosc();

			x = xm / DI; 
			y = ym / DI;

			if (x < 1){
				x = 799*DI;
				break;
			}
			if (x > PXX){
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Kolizja", "Doszlo do kolizji, program teraz wroci do menu glownego.", NULL);
				break;
			}
			if (y > PXY){
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Kolizja", "Doszlo do kolizji, program teraz wroci do menu glownego.", NULL);
				break;
			}
			if (y < 1){
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Kolizja", "Doszlo do kolizji, program teraz wroci do menu glownego.", NULL);
				break;
			}

			SDL_Rect dstrect1 = { x - 12, y - 12, 24, 24 }; 

			pixels[y * PXX + x] = 255; 
			SDL_UpdateTexture(texture, NULL, pixels, PXX * sizeof(Uint32));
			
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderCopy(renderer, texture1, NULL, &dstrect);
			SDL_RenderCopyEx(renderer, texture2, NULL, &dstrect1, VectorV.angle(), NULL, SDL_FLIP_NONE);
			SDL_RenderPresent(renderer); 
			agreguj_i_transferuj();
			a0 = t.elapsedTime();
		
			SDL_PollEvent(&event);
			if (event.type == SDL_QUIT){ quit = true; break; }

			if (a < a0)
			{
				a = a0; 		
			}

		} while (t.elapsedTime() <= czas);
		delete[] pixels;
		SDL_DestroyTexture(texture);
		SDL_DestroyTexture(texture1);
		SDL_DestroyTexture(texture2);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		system("cls");
		inne_wielkosci();
		system("Pause");
		V = 0, Vx = 1, Vy = 1, Afx = 0, Afy = 0, Vmax = 0; Vmin = 0, rmax = 0, rmin = 0;
		system("cls");
		cout << "\n Kontynuowac?  (0 - tak, 1 - nie)";
		do{
			do{
				cin.clear(); cin.ignore(256, '\n');
				cout << "\n0/1: \n"; cin >> exit;
				if (cin.fail() == 1){ cout << "\nnieodpowiedni typ zmiennej"; }
			} while (cin.fail() != 0);
			if (exit>1 || exit<0){ cout << "\npodaj jedna z dwoch wartosci"; };
		} while (exit>1 || exit<0);

	} while (exit != 1);
	return 0;
}
