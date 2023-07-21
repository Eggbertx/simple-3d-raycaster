#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 8
#define MAP_HEIGHT 8
#define TILE_SIZE 64.0
// int mapWidth = 8, mapHeight = 8, mapSize = 64;

void drawMap2D();

int hasCollision(int x, int y);

int* getCurrentMap();

#endif // MAP_H
