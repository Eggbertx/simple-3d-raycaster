#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

#include <simple3D_config.h>
#include "keys.h"
#include "actors.h"
#include "map.h"

actor* player;
int* map;
float defaultDistance;

float distance(float ax, float ay, float bx, float by) {
	return sqrt(pow(bx - ax, 2) + pow(by - ay, 2));
}

float fixAngle(float angle) {
	if(angle < 0) {
		angle += 2*PI;
	} else if(angle > 2*PI) {
		angle -= 2*PI;
	}
	return angle;
}

void drawRays3D() {
	int r, mapX, mapY, mapIndex, depthOfField;
	float rayX, rayY, rayAngle, rayDistance, xOffset, yOffset;

	rayAngle = fixAngle(player->angle - ONE_RAD*30);
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
			// ray hit a vertical line/wall
			rayX = vx;
			rayY = vy;
			rayDistance = distV;
			glColor3f(WALL_COLOR_V, 0, 0);
		}
		if(distH < distV) {
			// ray hit a horizontal line/wall
			rayX = hx;
			rayY = hy;
			rayDistance = distH;
			glColor3f(WALL_COLOR_H, 0, 0);
		}

		// draw ray(s) to the nearest horizontal or vertical wall
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex2i(player->x, player->y);
		glVertex2i(rayX, rayY);
		glEnd();

		// start drawing 3D walls
		float angleDiff = fixAngle(player->angle - rayAngle);
		rayDistance *= cos(angleDiff);
		float lineHeight = min((TILE_SIZE * 320)/rayDistance, 320);
		float lineOffset = 160 - lineHeight/2;
		glLineWidth(RAY_WALL_SIZE);
		glBegin(GL_LINES);
		glVertex2i(r*RAY_WALL_SIZE+MAP_WIDTH*TILE_SIZE+8, lineOffset);
		glVertex2i(r*RAY_WALL_SIZE+MAP_WIDTH*TILE_SIZE+8, lineHeight + lineOffset);
		glEnd();

		rayAngle = fixAngle(rayAngle + ONE_RAD);
	}
}

void updatePlayer() {
	int xOffset = 0, yOffset = 0;
	if(player->dx < 0) {
		xOffset = -PLAYER_OFFSET;
	} else {
		xOffset = PLAYER_OFFSET;
	}
	if(player->dy < 0) {
		yOffset = -PLAYER_OFFSET;
	} else  {
		yOffset = PLAYER_OFFSET;
	}
	int ipx = player->x/TILE_SIZE;
	int ipy = player->y/TILE_SIZE;
	int ipxAdd = (player->x + xOffset)/TILE_SIZE;
	int ipxSub = (player->x - xOffset)/TILE_SIZE;
	int ipyAdd = (player->y + yOffset)/TILE_SIZE;
	int ipySub = (player->y - yOffset)/TILE_SIZE;

	if(getKeyState(STATE_FORWARD) == 1) {
		if(map[ipy * MAP_WIDTH + ipxAdd] == 0) {
			player->x += player->dx;
		}
		if(map[ipyAdd * MAP_WIDTH + ipx] == 0) {
			player->y += player->dy;
		}
	}
	if(getKeyState(STATE_BACKWARDS) == 1) {
		if(map[ipy * MAP_WIDTH + ipxSub] == 0) {
			player->x -= player->dx;
		}
		if(map[ipySub * MAP_WIDTH + ipx] == 0) {
			player->y -= player->dy;
		}
	}
	if(getKeyState(STATE_TURN_LEFT) == 1) {
		player->angle = fixAngle(player->angle - PLAYER_TURN_SPEED);
		player->dx = cos(player->angle) * PLAYER_MOVE_SPEED;
		player->dy = sin(player->angle) * PLAYER_MOVE_SPEED;
	}
	if(getKeyState(STATE_TURN_RIGHT) == 1) {
		player->angle = fixAngle(player->angle + PLAYER_TURN_SPEED);
		player->dx = cos(player->angle) * PLAYER_MOVE_SPEED;
		player->dy = sin(player->angle) * PLAYER_MOVE_SPEED;
	}
	glutPostRedisplay();
}

void drawPlayer() {
	// draw player as dot
	glColor3f(1, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);

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
	map = getCurrentMap();
	player->angle = ONE_RAD*90;
	setPlayerPos(1, 1);
	player->dx = cos(player->angle) * PLAYER_MOVE_SPEED;
	player->dy = sin(player->angle) * PLAYER_MOVE_SPEED;
}

void reshapeWindow(int w, int h) {
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_TITLE);
	init();
	initKeyboard();
	glutDisplayFunc(display);
	glutReshapeFunc(reshapeWindow);
	glutMainLoop();
	return 0;
}
