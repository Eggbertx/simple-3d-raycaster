#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "actors.h"

int initSDL();

int initGL();

void cleanupGraphics();

void setDrawColor(int r, int g, int b, int a);

void fillScreen();

void flipScreen();

void drawSkyAndFloor();

void drawRays(actor* player, int whichD);

void drawPlayer(actor* player);

void drawMap2D();

#endif // GRAPHICS_H