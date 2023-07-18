#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

#include <simple3D_config.h>
#include "keys.h"
#include "actors.h"
#include "map.h"

actor* player;

void updatePlayer() {
	if(getKeyState(STATE_FORWARD) == 1) {
		player->x += player->dx;
		player->y += player->dy;
	} else if(getKeyState(STATE_BACKWARDS) == 1) {
		player->x -= player->dx;
		player->y -= player->dy;
	}
	if(getKeyState(STATE_TURN_LEFT) == 1) {
		player->angle -= PLAYER_TURN_SPEED;
		player->dx = cos(player->angle) * DELTA_MODIFIER;
		player->dy = sin(player->angle) * DELTA_MODIFIER;
		fixAngle();
	} else if(getKeyState(STATE_TURN_RIGHT) == 1) {
		player->angle += PLAYER_TURN_SPEED;
		player->dx = cos(player->angle) * DELTA_MODIFIER;
		player->dy = sin(player->angle) * DELTA_MODIFIER;
		fixAngle();
	}
	glutPostRedisplay();
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
		player->x, player->y, player->angle, player->dx, player->dy);
	glVertex2i(player->x, player->y);
	glEnd();

	// draw view direction as segment
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(player->x, player->y);
	glVertex2i(player->x + player->dx * DELTA_MODIFIER, player->y + player->dy * DELTA_MODIFIER);
	glEnd();
	updatePlayer();
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
	player = getPlayer();
	setPlayerPos(1, 1);
	player->dx = cos(player->angle) * DELTA_MODIFIER;
	player->dy = sin(player->dy) * DELTA_MODIFIER;
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_TITLE);
	init();
	initKeyboard();
	glutDisplayFunc(display);
	// glutKeyboardFunc(buttons);
	glutMainLoop();
	return 0;
}
