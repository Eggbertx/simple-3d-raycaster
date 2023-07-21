#include <GL/gl.h>

#include "map.h"

int _map[] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};

void drawMap2D() {
	int xo, yo;
	for(int y = 0; y < MAP_HEIGHT; y++) {
		for(int x = 0; x < MAP_WIDTH; x++) {
			if(_map[y * MAP_HEIGHT + x] == 1) {
				glColor3f(1, 1, 1);
			} else {
				glColor3f(0, 0, 0);
			}
			xo = x*TILE_SIZE;
			yo = y*TILE_SIZE;
			glBegin(GL_QUADS);
			glVertex2i(xo + 1, yo + 1);
			glVertex2i(xo + 1, yo + TILE_SIZE - 1);
			glVertex2i(xo + TILE_SIZE - 1, yo + TILE_SIZE - 1);
			glVertex2i(xo + TILE_SIZE - 1, yo + 1);
			glEnd();
		}
	}
}

int hasCollision(int x, int y) {
	if(x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_WIDTH) {
		return 1;
	}
	return _map[y * MAP_WIDTH + x];
}

int* getCurrentMap() {
	return _map;
}