#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

#include <simple3D_config.h>
#include "keys.h"
#include "actors.h"
#include "map.h"

actor* player;
float defaultDistance;

float distance(float ax, float ay, float bx, float by) {
	return sqrt(pow(bx - ax, 2) + pow(by - ay, 2));
}

void drawRays3D() {
	int r, mapX, mapY, mapIndex, depthOfField;
	float rayX, rayY, rayAngle, xOffset, yOffset;
	rayAngle = player->angle;
	for(int r = 0; r < NUM_RAYS; r++) {
		// check horizontal lines
		depthOfField = 0;
		float distH = defaultDistance, hx = player->x, hy = player->y;
		float aTan = -1/tan(rayAngle);
		if(rayAngle > PI) {
			// looking down
			rayY = (((int)player->y >> 6) << 6) - 0.0001;
			rayX = (player->y - rayY) * aTan + player->x;
			yOffset = -64;
			xOffset = -yOffset * aTan;
		} else if(rayAngle < PI) {
			// looking up
			rayY = (((int)player->y >> 6) << 6) + 64;
			rayX = (player->y - rayY) * aTan + player->x;
			yOffset = 64;
			xOffset = -yOffset * aTan;
		}
		if(rayAngle == 0 || rayAngle == PI) {
			// looking straight left or right
			rayX = player->x;
			rayY = player->y;
			depthOfField = 8;
		}
		while(depthOfField < 8) {
			mapX = (int)(rayX) >> 6;
			mapY = (int)(rayY) >> 6;
			mapIndex = mapY * MAP_WIDTH + mapX;
			if(hasCollision(mapX, mapY)) {
				hx = rayX;
				hy = rayY;
				distH = distance(player->x, player->y, hx, hy);
				depthOfField = 8;
			} else {
				rayX += xOffset;
				rayY += yOffset;
				depthOfField++;
			}
		}

		// check vertical lines
		depthOfField = 0;
		float nTan = -tan(rayAngle);
		float distV, vx = player->x, vy = player->y;
		if(rayAngle > PI_2 && rayAngle < PI_3) {
			// looking right
			rayX = (((int)player->x >> 6) << 6) - 0.0001;
			rayY = (player->x - rayX) * nTan + player->y;
			xOffset = -64;
			yOffset = -xOffset * nTan;
		} else if(rayAngle < PI_2 || rayAngle > PI_3) {
			// looking left
			rayX = (((int)player->x >> 6) << 6) + 64;
			rayY = (player->x - rayX) * nTan + player->y;
			xOffset = 64;
			yOffset = -xOffset * nTan;
		}
		if(rayAngle == 0 || rayAngle == PI) {
			// looking straight up or down
			rayX = player->x;
			rayY = player->y;
			depthOfField = 8;
		}
		while(depthOfField < 8) {
			mapX = (int)(rayX) >> 6;
			mapY = (int)(rayY) >> 6;
			mapIndex = mapY * MAP_WIDTH + mapX;
			if(hasCollision(mapX, mapY)) {
				vx = rayX;
				vy = rayY;
				distV = distance(player->x, player->y, vx, vy);
				depthOfField = 8;
			} else {
				rayX += xOffset;
				rayY += yOffset;
				depthOfField++;
			}
		}
		if(distV < distH) {
			rayX = vx;
			rayY = vy;
		}
		if(distH < distV) {
			rayX = hx;
			rayY = hy;
		}
		glColor3f(1, 0, 0);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex2i(player->x, player->y);
		glVertex2i(rayX, rayY);
		glEnd();
	}
}

void updatePlayer() {
	if(getKeyState(STATE_FORWARD) == 1) {
		player->x += player->dx;
		player->y += player->dy;
	}
	if(getKeyState(STATE_BACKWARDS) == 1) {
		player->x -= player->dx;
		player->y -= player->dy;
	}
	if(getKeyState(STATE_TURN_LEFT) == 1) {
		player->angle -= PLAYER_TURN_SPEED;
		player->dx = cos(player->angle) * PLAYER_MOVE_SPEED;
		player->dy = sin(player->angle) * PLAYER_MOVE_SPEED;
		fixAngle();
	}
	if(getKeyState(STATE_TURN_RIGHT) == 1) {
		player->angle += PLAYER_TURN_SPEED;
		player->dx = cos(player->angle) * PLAYER_MOVE_SPEED;
		player->dy = sin(player->angle) * PLAYER_MOVE_SPEED;
		fixAngle();
	}
	glutPostRedisplay();
}

void drawPlayer() {
	// draw player as dot
	glColor3f(1, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);

	// printf("Player info:\n"
	// 	"Position: (%f,%f)\n"
	// 	"Angle: %f\n"
	// 	"(dx, dy): (%f,%f)\n",
	// 	player->x, player->y, player->angle, player->dx, player->dy);
	glVertex2i(player->x, player->y);
	glEnd();

	// draw view direction as segment
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(player->x, player->y);
	glVertex2i(
		player->x + player->dx * DELTA_MODIFIER,
		player->y + player->dy * DELTA_MODIFIER
	);
	glEnd();
	updatePlayer();
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap2D();
	drawRays3D();
	drawPlayer();
	glutSwapBuffers();
}

void init() {
	glClearColor(0.3, 0.3, 0.3, 1.0);
	gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	defaultDistance = sqrt(pow(WINDOW_WIDTH, 2) + pow(WINDOW_HEIGHT, 2));
	player = getPlayer();
	setPlayerPos(1, 1);
	player->dx = cos(player->angle) * PLAYER_MOVE_SPEED;
	player->dy = sin(player->dy) * PLAYER_MOVE_SPEED;
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
