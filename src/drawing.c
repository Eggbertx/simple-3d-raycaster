#include <math.h>
#include <GL/gl.h>
#include <simple3D_util.h>

#include "drawing.h"
#include "map.h"
#include "graphics.h"

void drawRays(actor* player, int whichD) {
	int r, mapX, mapY, mapIndex, depthOfField;
	float rayX, rayY, rayAngle, rayDistance, xOffset, yOffset;

	int* map = getCurrentMap();
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
		float shade = 1.0;
		if(distV < distH) {
			// ray hit a vertical line/wall
			rayX = vx;
			rayY = vy;
			rayDistance = distV;
			glColor4f(1, 0, 0, (whichD == 2)?MAP2D_ALPHA/2:1.0);
			shade = SHADED_WALL;
		}
		if(distH < distV) {
			// ray hit a horizontal line/wall
			rayX = hx;
			rayY = hy;
			rayDistance = distH;
			glColor4f(1, 0, 0, (whichD == 2)?MAP2D_ALPHA/2:1.0);
		}

		SDL_Color color = {0, 0, 0, SDL_ALPHA_OPAQUE};
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
			float lineHeight = (TILE_SIZE * 320)/rayDistance;
			float yStep = 32.0/(float)lineHeight;
			float texYOffset = 0;
			if(lineHeight > 320) {
				texYOffset = (lineHeight - 320)/2.0;
				lineHeight = 320;
			}
			float lineOffset = 160.0 - lineHeight/2;
			glPointSize(RAY_WALL_SIZE);
			glBegin(GL_POINTS);
			float texY = texYOffset * yStep;
			float texX;
			if(shade == 1.0) {
				texX = (int)(rayX/2.0) % textures[TEXTURE_BRICKS]->w;
				if(rayAngle <= PI)
					texX = 31 - texX;
			} else {
				texX = (int)(rayY/2.0) % textures[TEXTURE_BRICKS]->w;
				if(rayAngle < PI_3 && rayAngle > PI_2)
					texX = 31 - texX;
			}
			
			for(int y = 0; y < lineHeight; y++) {
				colorAt(textures[TEXTURE_BRICKS], &color, (int)texX, (int)texY);
				glColor3ub(color.r * shade, color.g * shade, color.b * shade);
				glVertex2i(r*RAY_WALL_SIZE+RAY_WALL_SIZE/2.0, y+lineOffset);
				texY += yStep;
			}
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
	glColor4f(1, 1, 0, MAP2D_ALPHA);
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
	glColor4f(0.3, 0.3, 0.3, MAP2D_ALPHA);
	glRectf(MAP2D_X, MAP2D_Y, WINDOW_WIDTH, WINDOW_HEIGHT);
	for(int y = 0; y < MAP_HEIGHT; y++) {
		for(int x = 0; x < MAP_WIDTH; x++) {
			if(map[y * MAP_HEIGHT + x] > 0) {
				glColor4f(1, 1, 1, MAP2D_ALPHA);
			} else {
				glColor4f(0, 0, 0, MAP2D_ALPHA);
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