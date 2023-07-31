#include <GL/gl.h>
#include <SDL.h>

#include <simple3D_util.h>
#include "graphics.h"
#include "actors.h"
#include "map.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
SDL_GLContext glContext;

int initSDL() {
	int success = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS);
	if(success < 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Could not initialize SDL: %s\n", SDL_GetError());
		return success;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow(WINDOW_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	if(window == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC,
		WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	glContext = SDL_GL_CreateContext(window);
	if(glContext == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Could not create OpenGL context: %s\n", SDL_GetError());
		return 1;
	}

	if(SDL_GL_SetSwapInterval(1) < 0) {
		SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Could not set VSync: %s\n", SDL_GetError());
	}
}

GLenum initGL() {
	GLenum error = GL_NO_ERROR;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	error = glGetError();
	if(error != GL_NO_ERROR) {
		return error;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	error = glGetError();
	
	return error;
}

void cleanupGraphics() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void drawSkyAndFloor() {
	SDL_Rect rect = {
		x: 0,
		y: 0,
		w: WINDOW_WIDTH,
		h: WINDOW_HEIGHT
	};
	glColor3f(0.0, 0.59, 1.0); // sky color
	glRecti(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT/2);
	glColor3f(0.35, 0.18, 0.0); // ground color
	glRecti(0, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void setDrawColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void fillScreen() {
	SDL_RenderClear(renderer);
}

void flipScreen() {
	// SDL_RenderPresent(renderer);
	SDL_GL_SwapWindow(window);
}

void drawRays(actor* player, int whichD) {
	int r, mapX, mapY, mapIndex, depthOfField;
	float rayX, rayY, rayAngle, rayDistance, xOffset, yOffset;

	rayAngle = fixAngle(player->angle - ONE_RAD*30);
	for(int r = 0; r < NUM_RAYS; r++) {
		// check horizontal lines
		depthOfField = 0;
		float distH = WINDOW_WIDTH * WINDOW_HEIGHT, hx = player->x, hy = player->y;
		float aTan = -1/tan(rayAngle);
		if(rayAngle > PI) {
			// looking down
			rayY = (((int)player->y >> 6) << 6) - 0.0001;
			rayX = (player->y - rayY) * aTan + player->x;
			yOffset = -64;
			xOffset = -yOffset * aTan;
		} else if(rayAngle < PI) {
			// looking up
			rayY = (((int)player->y >> 6) << 6) + 64;
			rayX = (player->y - rayY) * aTan + player->x;
			yOffset = 64;
			xOffset = -yOffset * aTan;
		}
		if(rayAngle == 0 || rayAngle == PI) {
			// looking straight left or right
			rayX = player->x;
			rayY = player->y;
			depthOfField = 8;
		}
		while(depthOfField < 8) {
			mapX = (int)(rayX) >> 6;
			mapY = (int)(rayY) >> 6;
			mapIndex = mapY * MAP_WIDTH + mapX;
			if(hasCollision(mapX, mapY)) {
				hx = rayX;
				hy = rayY;
				distH = distance(player->x, player->y, hx, hy);
				depthOfField = 8;
			} else {
				rayX += xOffset;
				rayY += yOffset;
				depthOfField++;
			}
		}

		// check vertical lines
		depthOfField = 0;
		float nTan = -tan(rayAngle);
		float distV, vx = player->x, vy = player->y;
		if(rayAngle > PI_2 && rayAngle < PI_3) {
			// looking right
			rayX = (((int)player->x >> 6) << 6) - 0.0001;
			rayY = (player->x - rayX) * nTan + player->y;
			xOffset = -64;
			yOffset = -xOffset * nTan;
		} else if(rayAngle < PI_2 || rayAngle > PI_3) {
			// looking left
			rayX = (((int)player->x >> 6) << 6) + 64;
			rayY = (player->x - rayX) * nTan + player->y;
			xOffset = 64;
			yOffset = -xOffset * nTan;
		}
		if(rayAngle == 0 || rayAngle == PI) {
			// looking straight up or down
			rayX = player->x;
			rayY = player->y;
			depthOfField = 8;
		}
		while(depthOfField < 8) {
			mapX = (int)(rayX) >> 6;
			mapY = (int)(rayY) >> 6;
			mapIndex = mapY * MAP_WIDTH + mapX;
			if(hasCollision(mapX, mapY)) {
				vx = rayX;
				vy = rayY;
				distV = distance(player->x, player->y, vx, vy);
				depthOfField = 8;
			} else {
				rayX += xOffset;
				rayY += yOffset;
				depthOfField++;
			}
		}
		if(distV < distH) {
			// ray hit a vertical line/wall
			rayX = vx;
			rayY = vy;
			rayDistance = distV;
			glColor3f(WALL_COLOR_V, 0, 0);
		}
		if(distH < distV) {
			// ray hit a horizontal line/wall
			rayX = hx;
			rayY = hy;
			rayDistance = distH;
			glColor3f(WALL_COLOR_H, 0, 0);
		}

		switch(whichD) {
		case 2:
			// draw ray(s) to the nearest horizontal or vertical wall
			glLineWidth(2);
			glBegin(GL_LINES);
			glVertex2i(player->x/4 + MAP2D_X, player->y/4 + MAP2D_Y);
			glVertex2i(rayX/4 + MAP2D_X, rayY/4 + MAP2D_Y);
			glEnd();
			break;
		case 3:
			// start drawing 3D walls
			float angleDiff = fixAngle(player->angle - rayAngle);
			rayDistance *= cos(angleDiff);
			float lineHeight = min((TILE_SIZE * 320)/rayDistance, 320);
			float lineOffset = 160.0 - lineHeight/2;
			glLineWidth(RAY_WALL_SIZE);
			glBegin(GL_LINES);
			glVertex2i(r*RAY_WALL_SIZE+RAY_WALL_SIZE/2.0, lineOffset);
			glVertex2i(r*RAY_WALL_SIZE+RAY_WALL_SIZE/2.0, lineHeight + lineOffset);
			glEnd();
			break;
		default:
			break;
		}
		rayAngle = fixAngle(rayAngle + ONE_RAD/2.0);
	}
}

void drawPlayer(actor* player) {
	// draw player as dot
	glColor3f(1, 1, 0);
	glPointSize(4);
	glBegin(GL_POINTS);

	glVertex2i(player->x/4 + MAP2D_X, player->y/4 + MAP2D_Y);
	glEnd();

	// draw view direction as segment
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2i(player->x/4 + MAP2D_X, player->y/4 + MAP2D_Y);
	glVertex2i(
		player->x/4 + player->dx * DELTA_MODIFIER + MAP2D_X,
		player->y/4 + player->dy * DELTA_MODIFIER + MAP2D_Y
	);
	glEnd();
}


void drawMap2D(int* map) {
	int xo, yo;
	glColor3f(0.3, 0.3, 0.3);
	glRectf(MAP2D_X, MAP2D_Y, WINDOW_WIDTH, WINDOW_HEIGHT);
	for(int y = 0; y < MAP_HEIGHT; y++) {
		for(int x = 0; x < MAP_WIDTH; x++) {
			if(map[y * MAP_HEIGHT + x] == 1) {
				glColor3f(1, 1, 1);
			} else {
				glColor3f(0, 0, 0);
			}
			xo = MAP2D_X + x*16;
			yo = MAP2D_Y + y*16;
			glBegin(GL_QUADS);
			glVertex2i(xo + 0.5, yo + 0.5);
			glVertex2i(xo + 0.5, yo + 16 - 0.5);
			glVertex2i(xo + 16 - 0.5, yo + 16 - 0.5);
			glVertex2i(xo + 16 - 0.5, yo + 0.5);
			glEnd();
		}
	}
}