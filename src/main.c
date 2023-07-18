#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

#include <simple3D_config.h>
#include "keys.h"


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
	// draw player as dot
	glColor3f(1, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	printf("Player info:\n"
		"Position: (%f,%f)\n"
		"Angle: %f\n"
		"(dx, dy): (%f,%f\n)",
		playerX, playerY, playerAngle, playerDX, playerDY);
	glVertex2i(playerX, playerY);
	glEnd();

	// draw view direction as segment
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(playerX, playerY);
	glVertex2i(playerX + playerDX*5, playerY + playerDY*5);
	glEnd();
}

void fixAngle() {
	if(playerAngle < 0) {
		playerAngle += 2 * PI;
	} else if(playerAngle > 2 * PI) {
		playerAngle -= 2 * PI;
	}
}

void buttons(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_TURN_LEFT:
		playerAngle -= PLAYER_TURN_SPEED;
		fixAngle();
		playerDX = cos(playerAngle) * 5;
		playerDY = sin(playerAngle) * 5;
		break;
	case KEY_TURN_RIGHT:
		playerAngle += PLAYER_TURN_SPEED;
		fixAngle();
		playerDX = cos(playerAngle) * 5;
		playerDY = sin(playerAngle) * 5;
		break;
	case KEY_MOVE_FORWARD:
		playerX += playerDX;
		playerY += playerDY;
		break;
	case KEY_MOVE_BACKWARDS:
		playerX -= playerDX;
		playerY -= playerDY;
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
	playerDX = cos(playerAngle) * 5;
	playerDY = sin(playerDY) * 5;
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
