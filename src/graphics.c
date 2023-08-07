#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_image.h>

#include <simple3D_util.h>
#include "graphics.h"
#include "actors.h"
#include "map.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_GLContext glContext;
SDL_Surface** textures;

int initSDL() {
	int success = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_TIMER);
	if(success < 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO,
			"Could not initialize SDL: %s\n", SDL_GetError());
		return success;
	}

	if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"Could not initialize the image submodule: %s\n", IMG_GetError());
		return 1;
	}

	if(loadTextures() != 0) {
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow(WINDOW_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	if(window == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_RENDER,
			"Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	glContext = SDL_GL_CreateContext(window);
	if(glContext == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO,
			"Could not create OpenGL context: %s\n", SDL_GetError());
		return 1;
	}

	if(SDL_GL_SetSwapInterval(1) < 0) {
		SDL_LogWarn(SDL_LOG_CATEGORY_RENDER,
			"Could not set VSync: %s\n", SDL_GetError());
	}
	return 0;
}

int initGL() {
	int error = GL_NO_ERROR;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	error = glGetError();
	if(error != GL_NO_ERROR) {
		return error;
	}
	gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	error = glGetError();

	return error;
}

SDL_Surface* loadTexture(const char* file) {
	SDL_Surface* surface = IMG_Load(file);
	if(surface == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, IMG_GetError());
		return NULL;
	}
	if(surface->w != 32 || surface->h != 32) {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
			"Invalid resolution for %s (expected 32x32, got %dx%d)",
			file, surface->w, surface->h);
	}
	return surface;
}

int loadTextures() {
	if(textures != NULL) return 0;
	textures = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * TEXTURE_END);
	textures[TEXTURE_EMPTY] = SDL_CreateRGBSurfaceWithFormat(0, 32, 32, 32, SDL_PIXELFORMAT_RGBA8888);
	textures[TEXTURE_BRICKS] = IMG_Load("textures/bricks.png");
	textures[TEXTURE_DOOR] = IMG_Load("textures/door.png");
	textures[TEXTURE_CHECKERBOARD] = IMG_Load("textures/checkerboard.png");
	textures[TEXTURE_END] = IMG_Load("textures/end.png");
	for(int i = 0; i <= TEXTURE_END; i++) {
		if(textures[i] == NULL)
			return 1;
	}
	return 0;
}

void cleanupGraphics() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	for(int s = 0; s <= TEXTURE_END; s++) {
		SDL_FreeSurface(textures[s]);
	}
	IMG_Quit();
	SDL_Quit();
}

void drawSkyAndFloor() {
	glColor3f(0.0, 0.59, 1.0); // sky color
	glRectf(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT/2);
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
	SDL_GL_SwapWindow(window);
	// SDL_RenderPresent(renderer);
}

int colorAt(SDL_Surface* surface, SDL_Color* color, int x, int y) {
	if(surface == NULL || x > surface->w || y > surface->h || x < 0 || y < 0) {
		SDL_SetError("Invalid position on surface (%d,%d)", x, y);
		return 1;
	}
	Uint8* pPixel = (Uint8*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
	Uint32 pixelData = *(Uint32*)pPixel;
	SDL_GetRGBA(pixelData, surface->format, &color->r, &color->g, &color->b, &color->a);
	return 0;
}
