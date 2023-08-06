#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <simple3D_util.h>
#include "keys.h"
#include "actors.h"
#include "map.h"
#include "graphics.h"

actor* player;
int* map;
float defaultDistance;
int draw2D;

void initPlayer() {
	player->angle = ONE_RAD*90;
	setPlayerPos(1, 1);
	player->dx = cos(player->angle) * PLAYER_MOVE_SPEED;
	player->dy = sin(player->angle) * PLAYER_MOVE_SPEED;
}

void updatePlayer() {
	int xOffset = 0, yOffset = 0;
	if(player->dx < 0 || (getKeyState(STATE_MOVING) == 1 && getKeyState(STATE_TURN_LEFT) == 1)) {
		xOffset = -PLAYER_OFFSET;
	} else if(player->dx >= 0 || (getKeyState(STATE_MOVING) == 1 && getKeyState(STATE_TURN_RIGHT) == 1)) {
		xOffset = PLAYER_OFFSET;
	}
	if(player->dy < 0) {
		yOffset = -PLAYER_OFFSET;
	} else {
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
	if(getKeyState(STATE_RESET) == 1) {
		initPlayer();
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
		if(getKeyState(STATE_MOVING) == 1) {
			if(map[ipySub * MAP_WIDTH + ipx] == 0) {
				player->x += sin(player->angle) * PLAYER_MOVE_SPEED;
			}
			if(map[ipySub * MAP_WIDTH + ipxSub] == 0) {
				player->y -= cos(player->angle) * PLAYER_MOVE_SPEED;
			}
		} else {
			player->angle = fixAngle(player->angle - PLAYER_TURN_SPEED);
			player->dx = cos(player->angle) * PLAYER_MOVE_SPEED;
			player->dy = sin(player->angle) * PLAYER_MOVE_SPEED;
		}
	}
	if(getKeyState(STATE_TURN_RIGHT) == 1) {
		if(getKeyState(STATE_MOVING) == 1) {
			if(map[ipyAdd * MAP_WIDTH + ipxAdd] == 0) {
				player->x -= sin(player->angle) * PLAYER_MOVE_SPEED;
			}
			if(map[ipyAdd * MAP_WIDTH + ipxAdd] == 0) {
				player->y += cos(player->angle) * PLAYER_MOVE_SPEED;
			}
		} else {
			player->angle = fixAngle(player->angle + PLAYER_TURN_SPEED);
			player->dx = cos(player->angle) * PLAYER_MOVE_SPEED;
			player->dy = sin(player->angle) * PLAYER_MOVE_SPEED;
		}
	}
}

void initGame() {
	glClearColor(0.3, 0.3, 0.3, 1.0);
	setDrawColor(76, 76, 76, 255);
	fillScreen();
	flipScreen();
	defaultDistance = sqrt(pow(WINDOW_WIDTH, 2) + pow(WINDOW_HEIGHT, 2));
	player = getPlayer();
	map = getCurrentMap();
	draw2D = 1;
	initPlayer();
}

void drawStuff() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawSkyAndFloor();
	drawRays(player, 3);
	if(draw2D == 1) {
		drawMap2D(map);
		drawRays(player, 2);
		drawPlayer(player);
	}
	flipScreen();
}

int main(int argc, char *argv[]) {
	if(initSDL() != 0) {
		return 1;
	}
	int gError = initGL();
	if(gError != GL_NO_ERROR) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Error initializing OpenGL: %s\n", gluErrorString(gError));
		return 1;
	}

	initGame();
	SDL_Event event;
	Uint64 a = 0, b = 0, delta = 0;
	while(1) {
		a = SDL_GetTicks64();
		delta = a - b;
		if(getKeyState(STATE_EXIT)) {
			break;
		}
		if(delta <= 1000.0/FPS) continue;
		b = a;

		updatePlayer();

		if(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_KEYDOWN:
				if(event.key.keysym.scancode == SDL_SCANCODE_TAB && !event.key.repeat) {
					draw2D = !draw2D;
				}
				break;
			case SDL_QUIT:
				cleanupGraphics();
				return 0;
			}
		}
		drawStuff();
	}
	cleanupGraphics();
	return 0;
}
