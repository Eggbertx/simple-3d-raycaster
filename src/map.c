#include "map.h"

int _map[] = {
	1, 3, 1, 1, 1, 1, 1, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};

int hasCollision(int x, int y) {
	if(x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_WIDTH) {
		return 1;
	}
	return _map[y * MAP_WIDTH + x] > 0;
}

int tileAt(int x, int y) {
	if(x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_WIDTH) {
		return 0;
	}
	return _map[y * MAP_WIDTH + x];
}

int* getCurrentMap() {
	return _map;
}