#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 8
#define MAP_HEIGHT 8
#define TILE_SIZE 64.0

int hasCollision(int x, int y);

int tileAt(int x, int y);

int* getCurrentMap();

#endif // MAP_H
