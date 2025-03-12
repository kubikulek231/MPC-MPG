// MPC-MPG Lab 5?
#include <stdlib.h>
#include <GL\glut.h>
#include <string>
#include "..\include\simpleMesh.h"
#include <vector>


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

float t_value = 0.5; // Výchozí hodnota uprostøed køivky
const float t_step = 0.05; // Krok pro zmìnu t

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


void deCasteljau(float t, float points[][3], int n, float result[3]) {
	std::vector<std::vector<float>> temp(n, std::vector<float>(3));

	// Copy control points into temp
	for (int i = 0; i < n; i++) {
		temp[i][0] = points[i][0];
		temp[i][1] = points[i][1];
		temp[i][2] = points[i][2];
	}

	// Compute interpolation
	for (int r = 1; r < n; r++) {
		for (int i = 0; i < n - r; i++) {
			temp[i][0] = (1 - t) * temp[i][0] + t * temp[i + 1][0];
			temp[i][1] = (1 - t) * temp[i][1] + t * temp[i + 1][1];
		}
	}

	// Store the final point
	result[0] = temp[0][0];
	result[1] = temp[0][1];
	result[2] = temp[0][2];
}

void vykresliDeCasteljauSteps(float t, float points[][3], int n) {
	std::vector<std::vector<float>> temp(n, std::vector<float>(3));
	for (int i = 0; i < n; i++) {
		temp[i][0] = points[i][0];
		temp[i][1] = points[i][1];
		temp[i][2] = points[i][2];
	}

	for (int r = 1; r < n; r++) {
		glColor3f(0.0, 0.5, 1.0); // Modrá pro pomocné úseèky
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < n - r; i++) {
			temp[i][0] = (1 - t) * temp[i][0] + t * temp[i + 1][0];
			temp[i][1] = (1 - t) * temp[i][1] + t * temp[i + 1][1];
			glVertex2f(temp[i][0], temp[i][1]);
		}
		glEnd();
	}
}

void vykresliBeziera() {
	glColor3f(1, 1, 0);
	bitmapText();

	glColor3f(1, 0, 0);
	glPointSize(6);
	glBegin(GL_POINTS);
	for (int i = 0; i < POCET_RIDICICH_BODU; i++) {
		glVertex2f(ridiciBody[i][0], ridiciBody[i][1]);
	}
	glEnd();

	glColor3f(0.5, 0.5, 0.5);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(5, 0b1010101010101010);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < POCET_RIDICICH_BODU; i++) {
		glVertex2f(ridiciBody[i][0], ridiciBody[i][1]);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	// Draw the Bézier curve using De Casteljau
	glColor3f(0, 1, 0);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for (float t = 0; t <= 1; t += 0.01) {
		float point[3];
		deCasteljau(t, ridiciBody, POCET_RIDICICH_BODU, point);
		glVertex2f(point[0], point[1]);
	}
	glEnd();

	vykresliDeCasteljauSteps(t_value, ridiciBody, POCET_RIDICICH_BODU);


	// Vykreslení aktuálního bodu na køivce
	glColor3f(1, 0, 0);  // Èervený bod
	glPointSize(8);
	float currentPoint[3];
	deCasteljau(t_value, ridiciBody, POCET_RIDICICH_BODU, currentPoint);
	glBegin(GL_POINTS);
	glVertex2f(currentPoint[0], currentPoint[1]);
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
	key = (key > 'A' && key <= 'Z') ? key + 'a' - 'A' : key;

	switch (key) {
	case 'w': trianglestrip = true; break;
	case 's': trianglestrip = false; break;
	case '+':
		if (t_value + t_step <= 1.0) t_value += t_step;
		break;
	case '-':
		if (t_value - t_step >= 0.0) t_value -= t_step;
		break;
	}

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