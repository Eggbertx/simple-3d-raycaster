#ifndef ACTORS_H
#define ACTORS_H

typedef struct {
	float x;
	float y;
	float dx;
	float dy;
	float angle;
} actor;

actor* getPlayer();

void setPlayerPos(int x, int y);

#endif // ACTORS_H