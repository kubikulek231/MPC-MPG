// MPC-MPG cviceni 4 2024
#include <stdlib.h>
#include <GL\glut.h>
#include <string>
#include "..\include\simpleMesh.h"

// Menu
#define MENU_RESET   1001
#define MENU_EXITOK  1002
#define MENU_EXITNO  1003 
#define MENU_BEZIER  1005
#define MENU_RECT	 1004
#define MENU_TIM_ON	 1006
#define	MENU_TIM_OFF 1007
using namespace std;

#define POCET_RIDICICH_BODU 4
int aktBod = POCET_RIDICICH_BODU;
bool bezier = true;

float xOffset = 0;
float yOffset = 0;
bool trianglestrip = true;
bool increasing = false;
bool timerOn = false;
int winWidth = 700;
int winHeight = 500;
float id_Color = 0;
bool inc = true;
float ridiciBody[POCET_RIDICICH_BODU][3] = {
	{60,400,0.0},
	{300,300,0.0},
	{150,200,0.0},
	{60,60,0.0}
};

void vykresliPole()
{

	int tempPole[2 * numVer];

	// promenne poli ulozeny v souboru simpleMesh.h

	for (int i = 0; i < numVer; i++)
	{
		tempPole[2 * i] = coords[2 * i] + xOffset;		// x souradnice
		tempPole[2 * i + 1] = coords[2 * i + 1] + yOffset;		// y souradnice
	}

	// registrace vertex poli 
	glVertexPointer(2, GL_INT, 0, &tempPole[0]);
	glColorPointer(4, GL_FLOAT, 0, &colors);

	// zapnuti vertex poli 	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// vykresleni vertex poli 
	if (trianglestrip)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, numVer); // jako obdelnik
	}
	else
	{

		glDrawArrays(GL_TRIANGLES, 0, numVer); // jako trojuhelniky
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void bitmapText()
{
	// Vypis souradnic vsech ridicich bodu
	std::string str;
	for (int i = 0; i < POCET_RIDICICH_BODU; i++)
	{
		str = "Ridici bod: " + std::to_string(i) + " X: " + std::to_string(ridiciBody[i][0]) + " Y: " + std::to_string(ridiciBody[i][1]);

		glRasterPos2i(ridiciBody[i][0] + 10, ridiciBody[i][1] + 5);
		for (int j = 0; j < str.length(); j++)
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[j]);
		}
	}
}

void vykresliBeziera()
{
	// Vypis souradnice ridicich bodu
	glColor3f(1, 1, 0);
	bitmapText();

	// Vykresleni ridicich bodu
	glColor3f(1 - id_Color, id_Color, id_Color);
	glPointSize(10);
	glBegin(GL_POINTS);
	for (int i = 0; i < POCET_RIDICICH_BODU; i++)
	{
		glVertex2i(ridiciBody[i][0], ridiciBody[i][1]);
	}
	glEnd();


	// Vykresleni ridiciho polygonu

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(5, 0b1010101010101010);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < POCET_RIDICICH_BODU; i++)
	{
		glVertex2i(ridiciBody[i][0], ridiciBody[i][1]);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	// Vykresleni beziera
	glEnable(GL_MAP1_VERTEX_3);
	glMap1f(GL_MAP1_VERTEX_3, 0, 100, 3, 4, *ridiciBody);
	glBegin(GL_LINE_STRIP);
	glColor3f(1 - id_Color, 0.5 * id_Color, id_Color);
	glLineWidth(2);
	for (int i = 0; i <= 100; i++)
		glEvalCoord1f(i);
	glEnd();

}

void onSpecial(int key, int mx, int my)
{
	int jump = 5;

	// detekce modifikatoru
	switch (glutGetModifiers()) {
	case GLUT_ACTIVE_SHIFT: jump = 5; break;
	case GLUT_ACTIVE_CTRL:	jump = 10; break;
	case GLUT_ACTIVE_ALT:	jump = 20; break;
	default: jump = 1;
	}

	// zpracovani klaves
	switch (key) {
	case GLUT_KEY_UP:	 yOffset += jump; break;
	case GLUT_KEY_DOWN:	 yOffset -= jump; break;
	case GLUT_KEY_LEFT:  xOffset -= jump; break;
	case GLUT_KEY_RIGHT: xOffset += jump; break;
	}

	// vynuceni prekresleni
	glutPostRedisplay();
}

void onKeyboard(unsigned char key, int mx, int my)
{
	// prevod na mala pismena
	key = (key > 'A' && key <= 'Z') ? key + 'a' - 'A' : key;

	// Ukol 2
	// zpracovani klaves
	// Pridejte pomoci switch case zmenu trianglestrip
	switch (key) {
	case 'w': trianglestrip = true; break;
	case 's': trianglestrip = false; break;
	}

	// vynuceni prekresleni
	glutPostRedisplay();

}


//void onIdle(void)
//{
//
//	if (id_Color >= 1) {
//		inc = false;
//	}
//	if (id_Color <= 0) {
//		inc = true;
//	}
//
//	if (inc == true) {
//		id_Color += 0.001;
//	}
//	else
//	{
//		id_Color -= 0.001;
//	}
//	
//
//	glutPostRedisplay();
//
//}


void onReshape(int w, int h)            // event handler pro zmenu velikosti okna
{
	glViewport(0, 0, w, h);             // OpenGL: nastaveni rozmenu viewportu
	glMatrixMode(GL_PROJECTION);        // OpenGL: matice bude typu projekce
	glLoadIdentity();                   // OpenGL: matice bude identicka (jen jednicky v hlavni diagonale)
	glOrtho(0, w, 0, h, -1, 1);         // OpenGL: mapovani abstraktnich souradnic do souradnic okna
}

void onDisplay(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	if (bezier == true)
	{
		vykresliBeziera();
	}
	else
	{
		vykresliPole();
	}

	glFlush();
	glutSwapBuffers();
}





// obsluha pohybu mysi (aktivniho)
void onMotion(int mx, int my)
{
	// posun y souradnice aby byl pocatek vlevo dole
	my = glutGet(GLUT_WINDOW_HEIGHT) - my;

	if (aktBod < POCET_RIDICICH_BODU)
	{
		// Ukol 3
		// Doplnte kod pro prepocet souradnic bodu, ktery je tazen
		ridiciBody[aktBod][0] = mx;
		ridiciBody[aktBod][1] = my;

	}

	glutPostRedisplay();
}

void onTimer(int value)
{

	if (increasing)
		xOffset += 1;
	else
		xOffset -= 1;

	int	lenRect = coords[2 * numVer - 2] - coords[0];

	if (xOffset > winWidth - lenRect)
	{
		xOffset = winWidth - lenRect;
		increasing = false;
	}
	else if (xOffset < 0)
	{
		xOffset = 0;
		increasing = true;
	};

	glutPostRedisplay();

	if (timerOn)
		glutTimerFunc(5, onTimer, value);
}

// obsluha tlacitek mysi
void onMouse(int button, int state, int mx, int my)
{
	// posun y souradnice aby byl pocatek vlevo dole
	my = glutGet(GLUT_WINDOW_HEIGHT) - my;

	// detekce povoleneho pohybu ( left button only )
	if (button == GLUT_LEFT_BUTTON && bezier == true) {
		if (state == GLUT_UP)
		{
			aktBod = 100;
		}
		else {
			// Ukol 3
			// Doplnte kod pro nastaveni aktivniho bodu, na ktery bude kliknuto 
			for (int i = 0; i < POCET_RIDICICH_BODU; i++)
			{
				if ((ridiciBody[i][0] - 10) < mx && mx < (ridiciBody[i][0] + 10))
				{
					if ((ridiciBody[i][1] - 10) < my && my < (ridiciBody[i][1] + 10))
					{
						aktBod = i;
					}
				}
			}

		}
	}
	else
	{
		aktBod = 100;
	}


	//// Pozastaveni timeru pri chyceni objektu - Bonus
	//if (button == GLUT_LEFT_BUTTON && bezier == false) {
	//	if (state == GLUT_DOWN)
	//	{
	//		// Doplnte kod pro nastaveni aktivniho bodu, na ktery bude kliknuto 
	//		if ((10 + xOffset - 10) < mx && mx < (250 + xOffset + 10))
	//		{
	//			if ((10 + yOffset - 10) < my && my < (50 + yOffset + 10))
	//			{
	//				if (!timerOn)
	//				{
	//					timerOn = true;
	//					glutTimerFunc(5, onTimer, 0);
	//				}
	//				else
	//				{
	//					timerOn = false;
	//				}
	//			}
	//		}
	//	}
	//}

}


// vytvoreni menu
void createMenu(void(*func)(int value))
{
	// Doplnte prepinac mezi vykreslenim Bezierovy krivky a obrazku

	// vytvoreni podmenu
	int idSub = glutCreateMenu(func);
	glutAddMenuEntry("Ano", MENU_EXITOK);
	glutAddMenuEntry("Ne", MENU_EXITNO);

	// Ukol 1 - pridejte submenu pro zmenu mezi Bezier a Rect
	int idSub2 = glutCreateMenu(func);
	glutAddMenuEntry("Bezier", MENU_BEZIER);
	glutAddMenuEntry("Rectangle", MENU_RECT);

	// Ukol 4 -  pridejte submenu pro zmenu zapnuti/vypnuti timeru
	int idSub3 = glutCreateMenu(func);
	glutAddMenuEntry("On", MENU_TIM_ON);
	glutAddMenuEntry("Off", MENU_TIM_OFF);

	// vytvoreni hlavniho menu
	glutCreateMenu(func);

	glutAddMenuEntry("Reset pozice", MENU_RESET);

	// Ukol 1 aktivujte submenu pro prepinani Bezier a Rect
	glutAddSubMenu("Vyber scenu", idSub2);

	glutAddSubMenu("Timer", idSub3);

	glutAddSubMenu("Konec", idSub);

	// prirazeni hlavniho menu na tlacitko mysi
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// obsluha menu
void onMenu(int value)
{
	switch (value)
	{
	case MENU_RESET:
		ridiciBody[0][0] = 60;
		ridiciBody[0][1] = 400;
		ridiciBody[1][0] = 300;
		ridiciBody[1][1] = 300;
		ridiciBody[2][0] = 150;
		ridiciBody[2][1] = 200;
		ridiciBody[3][0] = 60;
		ridiciBody[3][1] = 60;
		break;
	case MENU_EXITOK: exit(1); break;
	case MENU_EXITNO: break;

		// Ukol 1
		// Doplnte polozky menu - vykresleni obdelniku nebo beziera
		// Solution

	case MENU_BEZIER: bezier = true; break;
	case MENU_RECT: bezier = false; break;


		// Ukol 4
		// 
	case MENU_TIM_ON:
		// doplnte kod
		if (!timerOn)
		{
			timerOn = true;
			glutTimerFunc(5, onTimer, value);
		}
		break;
	case MENU_TIM_OFF:
		// doplnte kod
		timerOn = false;
		break;

	}

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);               // inicializace knihovny GLUT

	glutInitDisplayMode(GLUT_DOUBLE);

	glutInitWindowSize(winWidth, winHeight);        // nastaveni pocatecni velikosti dale oteviranych oken
	glutInitWindowPosition(200, 200);    // nastaveni pocatecniho umisteni dale oteviranych oken


	glutCreateWindow("Bezier/Rect");	 // vytvoreni okna
	glutDisplayFunc(onDisplay);          // registrace funkce volane pri prekreslovani aktualniho okna
	glutReshapeFunc(onReshape);          // registrace funkce volane pri zmene velikosti aktualniho okna
	glutMouseFunc(onMouse);
	glutMotionFunc(onMotion);
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(onSpecial);

	createMenu(onMenu);
	glutTimerFunc(5, onTimer, 1);
	//glutIdleFunc(onIdle);
	glutMainLoop();                      // nekonecna smycka, interakce uz jen pomoci event handleru
	return 0;                            // ukonceni programu
}