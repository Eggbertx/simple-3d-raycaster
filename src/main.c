#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

#include <simple3D_config.h>
#include "keys.h"

#define PI 3.1415926535

float playerX, playerY, playerDX, playerDY, playerAngle;
int mapWidth = 8, mapHeight = 8, mapSize = 64;
int map[] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};

void drawMap2D() {
	int xo, yo;
	for(int y = 0; y < mapHeight; y++) {
		for(int x = 0; x < mapWidth; x++) {
			if(map[y * mapHeight + x] == 1) {
				glColor3f(1, 1, 1);
			} else {
				glColor3f(0, 0, 0);
			}
			xo = x*mapSize;
			yo = y*mapSize;
			glBegin(GL_QUADS);
			glVertex2i(xo + 1, yo + 1);
			glVertex2i(xo + 1, yo + mapSize - 1);
			glVertex2i(xo + mapSize - 1, yo + mapSize - 1);
			glVertex2i(xo + mapSize - 1, yo + 1);
			glEnd();
		}
	}
}

void drawPlayer() {
	glColor3f(1, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	printf("Drawing player at position (%f,%f)\n", playerX, playerY);
	glVertex2i(playerX, playerY);
	glEnd();
}

void buttons(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		playerX -= PLAYER_SPEED;
		break;
	case 'd':
		playerX += PLAYER_SPEED;
		break;
	case 'w':
		playerY -= PLAYER_SPEED;
		break;
	case 's':
		playerY += PLAYER_SPEED;
		break;
	case KEY_ESCAPE:
		printf("Escape key pressed, exiting\n");
		glutLeaveMainLoop();
	default:
		break;
	}
	glutPostRedisplay();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap2D();
	drawPlayer();
	glutSwapBuffers();
}

void init() {
	glClearColor(0.3, 0.3, 0.3, 1.0);
	gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	playerX = 300;
	playerY = 300;
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_TITLE);
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(buttons);
	glutMainLoop();
	return 0;
}
