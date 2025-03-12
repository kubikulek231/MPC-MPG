﻿// MPC-MPG Lab 2: OpenGL, uvod do vykreslovani
#include <iostream>
#include <GL/glut.h>
// #include <...\packages\nupengl.core.0.1.0.1\build\native\include\GL\freeglut.h> //  include souboru freeglut pri instalaci NuGet balicku

#define PI 3.14 // protože proč ne...

int window1, window2, window3; // deklarace proměnný pro indexy oken


void vykresliDomecek()
{
	glLineWidth(5); // nastavení šířky čáry na 5 pixelů

	glBegin(GL_LINE_STRIP); // GL_LINE_STRIP označuje primitivu, je to lomená (neuzavřená) čára
	glVertex2i(50, 50);
	glVertex2i(150, 50);
	glVertex2i(50, 150);
	glVertex2i(150, 150);
	glVertex2i(100, 200);
        
	// Úkol 1
	// doplněný kód
	glVertex2i(50, 150);
	glVertex2i(50, 50);
	glVertex2i(150, 150);
	glVertex2i(150, 50);
	// konec doplněného kódu


    glEnd(); // každá primitiva se musí opět ukončit
}

void vykresliKruh(int numVerticies, int sx, int sy, int r)
{
	float t = 0;

	glBegin(GL_POLYGON);
	for (int i = 0; i < numVerticies; i++)
	{
		glVertex3f(sx + r * cos(2 * PI * t), sy + r * sin(2 * PI * t), 0.0);
		t += 1. / numVerticies;
	}
	glEnd();
}

void vykresliKrivku()
{
	glEnable(GL_LINE_SMOOTH);		   // aktivace antialiasingu - funguje pouze společně se zapnutím průhlednosti!!!
	glHint(GL_LINE_SMOOTH, GL_NICEST); // nastaveni algoritmu
	glEnable(GL_BLEND);                // aktivace míchání barev
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // nastavení způsobu počítání průhlednosti

	// x = 150 + 150t, y = 300t^2, z = 0, -1 ≤ t ≤ 1

	// Úkol 2
	// doplněný kód
	int numVerticies = 50;
	float t = -1.0;

	// pozor, na rozdíl od kružncie je zde jiný rozsah t, proto začínáme na t = -1
	// protože maximální t = +1, v cyklu níže vystupuje v inkrementaci proměnné t hodnota 2 jakožto délka intervalu, ve kterém počítáme t

	glLineWidth(5);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < numVerticies; i++)
	{
		glVertex3f((150 + 150 * t), 300 * (pow(t, 2)), 0.0);
		t += 2. / numVerticies;
	}
	glEnd();
	// konec doplněného kódu

	glDisable(GL_LINE_SMOOTH); // je dobrým zvykem deaktivovat, co jsme předtím aktivovali
	glDisable(GL_BLEND);
}

void vykresliPruhlednost()
{
	glLineWidth(1);
	int r1 = 100;
	int r2 = 50;
	int sx = 150;
	int sy = 150;
	int numVerticies = 50;

	// Úkol 3
	// doplněný kód
	// kruhy - bez průhlednosti
	glColor4f(1., 0., 0., 0.3); // nastavení barvy, kterou se bude kreslit, čtvrtým parametrem v nastavení barvy je faktor průhlednosti
	vykresliKruh(numVerticies, sx, sy, r1);
	glColor4f(1., 1., 1., 0.6);
	vykresliKruh(numVerticies, sx, sy, r2);
	
	// trouhelnik - zapínáme průhlednost
	glEnable(GL_BLEND); // aktivace míchání barev
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // nastavení způsobu počítání průhlednosti

	glColor4f(0., 0., 1., 0.8);
	glBegin(GL_TRIANGLES); // vykresluji trojúhelníky; protože zadávám pouze 3 body, bude to jeden trojúhelník
	int triangleOffset = 10;
	glVertex2i(sx - r1 -triangleOffset, sy);
	glVertex2i(sx + r1 + triangleOffset, sy);
	glVertex2i(sx, sy + r1 + triangleOffset);
	glEnd();

	// vypnutí průhlednosti
	glDisable(GL_BLEND);
	// konec doplněného kódu
}

// první přístup: jedna společná funkce onDisplay, uvnitř rozlišíme, do kterého okna zrovna kreslíme
void onDisplay()
{
	int window = glutGetWindow();
	if(window == window1)
	{
		glClearColor(0.,0.,0.,0); // nastavení barvy, kterou bude okno mazáno
		glClear(GL_COLOR_BUFFER_BIT); // vymazání bitových rovin barvového bufferu

		vykresliDomecek();
	}
	else
	if(window == window2)
	{
		glClearColor(0.,0.,0.,0);
		glClear(GL_COLOR_BUFFER_BIT);

		glColor4f(0, 1, 1, 1);
        vykresliKrivku();
		
		glColor4f(1, 1, 1, 0);
		vykresliKruh(50,100,100,50);
	}
	else if(window == window3)
	{
		glClearColor(1.,1.,1.,0);
		glClear(GL_COLOR_BUFFER_BIT);

		vykresliPruhlednost();
	}
    glFlush(); // provedení a vykreslení změn
}

// druhý přístup: pro každé okno zvláštní vykreslovací funkce
void displayUkol1()
{
	glClearColor(0., 0., 0., 0); // nastavení barvy, kterou bude okno mazáno
	glClear(GL_COLOR_BUFFER_BIT); // vymazání bitových rovin barvového bufferu

	vykresliDomecek();

	glFlush(); // provedení a vykreslení změn
}

void displayUkol2()
{
	glClearColor(0., 0., 0., 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor4f(0, 1, 1, 1);
	vykresliKrivku();

	glColor4f(1, 1, 1, 0);
	vykresliKruh(50, 100, 100, 50);

	glFlush(); // provedení a vykreslení změn
}

void displayUkol3()
{
	glClearColor(1., 1., 1., 0);
	glClear(GL_COLOR_BUFFER_BIT);

	vykresliPruhlednost();

	glFlush(); // provedení a vykreslení změn
}

void onResize(int w, int h)
{
    glViewport(0, 0, w, h);      // viditelná oblast přes celé okno
    glMatrixMode(GL_PROJECTION); // začátek modifikace projekční matice
    glLoadIdentity();            // vymazání projekční matice (přesněji nastavení identity)
    glOrtho(0, w, 0, h, -1, 1);  // mapování abstraktních souřadnic do souřadnic okna
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); // inicializace glutu
	glutInitWindowSize(300,350); // defaultní velikost okna

	glutInitWindowPosition(0,0); // pozice okna
	window1 = glutCreateWindow("Podokno 1"); // vytvoření okna, uložení jeho indexu (window1) a nastavení názvu
	glutSetWindow(window1); // nastavení okna window1 jako aktivního
	// glutDisplayFunc(onDisplay); // definice toho, jaká událost se má vyvolat při překreslení okna
	glutDisplayFunc(displayUkol1);
	glutReshapeFunc(onResize); // definice toho, jaká událost se má vyvolat při změně velikosti okna

	glutInitWindowPosition(520,200);
	window2 = glutCreateWindow("Podokno 2");
	glutSetWindow(window2);
	// glutDisplayFunc(onDisplay);
	glutDisplayFunc(displayUkol2);
	glutReshapeFunc(onResize);

	glutInitWindowPosition(900,200);
	window3 = glutCreateWindow("Podokno 3");
	glutSetWindow(window3);
	// glutDisplayFunc(onDisplay);
	glutDisplayFunc(displayUkol3);
	glutReshapeFunc(onResize);

	glutMainLoop(); // v tuto chvíli máme všechno nastavené, tudíž předáváme vedení glutu a necháváme všechno na něm

	return 0; // tohle je nutné proto, že funkce main má vracet hodnotu integer, tak třeba 0
}