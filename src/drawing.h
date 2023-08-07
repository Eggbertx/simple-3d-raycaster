#ifndef DRAWING_H
#define DRAWING_H

#include "actors.h"

void drawSkyAndFloor();

void drawRays(actor* player, int whichD);

void drawPlayer(actor* player);

void drawMap2D(int* map);

#endif // DRAWING_H