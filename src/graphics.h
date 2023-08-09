#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL_surface.h>
#include "actors.h"

enum {
	TEXTURE_EMPTY, // not loaded/rendered
	TEXTURE_BRICKS,
	TEXTURE_DOOR,
	TEXTURE_CHECKERBOARD,
	TEXTURE_END
};

// extern SDL_Surface* checkerboard;
extern SDL_Surface** textures;

int initSDL(char* texturesDir);

int initGL();

int loadTextures(char* texturesDir);

void cleanupGraphics();

void setDrawColor(int r, int g, int b, int a);

void fillScreen();

void flipScreen();

int colorAt(SDL_Surface* surface, SDL_Color* color, int x, int y);

#endif // GRAPHICS_H