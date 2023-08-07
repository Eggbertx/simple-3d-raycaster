#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL_surface.h>
#include "actors.h"

extern SDL_Surface* checkerboard;

int initSDL();

int initGL();

int loadTextures();

void cleanupGraphics();

void setDrawColor(int r, int g, int b, int a);

void fillScreen();

void flipScreen();

int colorAt(SDL_Surface* surface, SDL_Color* color, int x, int y);

#endif // GRAPHICS_H