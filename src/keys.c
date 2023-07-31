#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include "keys.h"


uint8_t getKeyState(int state) {
	const uint8_t* kbState = SDL_GetKeyboardState(NULL);

	switch(state) {
	case STATE_FORWARD:
		return kbState[SDL_SCANCODE_W];
	case STATE_BACKWARDS:
		return kbState[SDL_SCANCODE_S];
	case STATE_TURN_LEFT:
		return kbState[SDL_SCANCODE_A];
	case STATE_TURN_RIGHT:
		return kbState[SDL_SCANCODE_D];
	case STATE_MOVING:
		return kbState[SDL_SCANCODE_LALT];
	case STATE_RESET:
		return kbState[SDL_SCANCODE_R];
	case STATE_EXIT:
		return kbState[SDL_SCANCODE_ESCAPE];
	}
	return 0;
}
