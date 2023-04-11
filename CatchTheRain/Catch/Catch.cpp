#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

#define DEBUG 1
#define DEBUG_MSG "[DEBUG] "
#define GAME_TITLE "CATCH GAME - PRESS ANY KEY TO START!"
#define GAME_START "CATCH GAME - POINTS: 0"
#define INITAL_TIME_BETWEEN_WAVES 10000
#define WIN_WIDTH 1000
#define WIN_HEIGHT 1000
#define WIN_X_POS 500
#define WIN_Y_POS -800
#define LEFT -WIN_WIDTH / 2
#define TOP WIN_HEIGHT
#define RIGHT  WIN_WIDTH / 2
#define BOTTOM 0
#define CATCHER_BOTTOM_MARGIN 10
#define CATCHER_WIDTH 50
#define CATCHER_HEIGHT 1
#define RAIN_DROP_SIDE 5
#define RAIN_DROP_TOP_PADDING 5
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define BLACK 0, 0, 0
#define WHITE 1, 1, 1
#define MOVE_STEP 10
#define MAX_RAIN_DROPS 10

static bool game_started = false;
static int points = 0,
		   offset_x = 0,
		   offset_y = 0,
		   time_between_waves = INITAL_TIME_BETWEEN_WAVES;

void init() {
	srand(time(NULL));
	glClearColor(BLACK, 1.0);
	gluOrtho2D(LEFT, RIGHT, BOTTOM, TOP);
	glShadeModel(GL_FLAT);
}

void displayCatcher() {
	glColor3f(WHITE);
	glBegin(GL_QUADS);
	glVertex2f(-CATCHER_WIDTH / 2 + offset_x, BOTTOM + CATCHER_BOTTOM_MARGIN + offset_y);
	glVertex2f(CATCHER_WIDTH / 2 + offset_x, BOTTOM + CATCHER_BOTTOM_MARGIN + offset_y);
	glVertex2f(CATCHER_WIDTH / 2 + offset_x, BOTTOM + CATCHER_BOTTOM_MARGIN + CATCHER_HEIGHT + offset_y);
	glVertex2f(-CATCHER_WIDTH / 2 + offset_x, BOTTOM + CATCHER_BOTTOM_MARGIN + CATCHER_HEIGHT + offset_y);
	glEnd();
}

void displayRainDrop() {
	int rand_x;

	rand_x = (rand() % WIN_WIDTH) - WIN_WIDTH / 2;
	#if DEBUG
	printf("[DEBUG] Generated rain drop at %d\n", rand_x);
	#endif
	glColor3f(WHITE);
	glBegin(GL_QUADS);
	glVertex2f(rand_x, TOP - RAIN_DROP_TOP_PADDING - RAIN_DROP_SIDE);
	glVertex2f(rand_x + RAIN_DROP_SIDE, TOP - RAIN_DROP_TOP_PADDING - RAIN_DROP_SIDE);
	glVertex2f(rand_x + RAIN_DROP_SIDE, TOP - RAIN_DROP_TOP_PADDING);
	glVertex2f(rand_x, TOP - RAIN_DROP_TOP_PADDING);
	glEnd();
	glutPostRedisplay();
}

void displayRain(int value) {
	if (game_started) {
		int i = 0;

		while (i < 5) {
			displayRainDrop();
			i++;
		}
	}
	Sleep(time_between_waves);
}

void displayAll() {
	glClear(GL_COLOR_BUFFER_BIT);
	displayCatcher();

	if (game_started) {
		displayRain(0);
	}
	glFlush();
	glutSwapBuffers();
}

void keyPressHandler(unsigned char key, int x, int y) {
	#if DEBUG
	printf(DEBUG_MSG "Key %c pressed!\n", key);
	#endif

	if (!game_started) {
		game_started = true;
		glutSetWindowTitle(GAME_START);
	} else {
		switch (key) {

			case LEFT_KEY:
				if (offset_x - CATCHER_WIDTH / 2 < LEFT) {
					return;
				}

				offset_x -= MOVE_STEP;

				#if DEBUG
				printf(DEBUG_MSG "CATCHER MOVED TO %d\n", offset_x);
				#endif
				break;
			case RIGHT_KEY:
				if (CATCHER_WIDTH / 2 + offset_x > RIGHT) {
					return;
				}

				offset_x += MOVE_STEP;

				#if DEBUG
				printf(DEBUG_MSG "CATCHER MOVED TO %d\n", offset_x);
				#endif
				break;
			default:
				printf("Invalid key %c\n", key);
		}

		glutPostRedisplay();
	}
}

int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIN_WIDTH, WIN_WIDTH);
	glutInitWindowPosition(WIN_X_POS, WIN_Y_POS);
	glutCreateWindow(GAME_TITLE);

	init();

	glutDisplayFunc(displayAll);
	glutKeyboardFunc(keyPressHandler);
	glutTimerFunc(10000, displayRain, 0);

	glutMainLoop();

	return 0;
}
