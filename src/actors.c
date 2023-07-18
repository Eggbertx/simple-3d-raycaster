#include <simple3D_config.h>

#include "actors.h"
#include "map.h"

actor _player;

actor* getPlayer() {
	return &_player;
}

void setPlayerPos(int x, int y) {
	_player.x = x * TILE_SIZE + TILE_SIZE/2;
	_player.y = y * TILE_SIZE + TILE_SIZE/2;
}

void fixAngle() {
	if(_player.angle < 0) {
		_player.angle += 2 * PI;
	} else if(_player.angle > 2 * PI) {
		_player.angle -= 2 * PI;
	}
}