#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include "keys.h"

unsigned char keyStates[4];

void onKeyDown(unsigned char key, int x, int y) {
	switch(key) {
	case KEY_MOVE_FORWARD:
		keyStates[STATE_FORWARD] = 1;
		break;
	case KEY_MOVE_BACKWARDS:
		keyStates[STATE_BACKWARDS] = 1;
		break;
	case KEY_TURN_LEFT:
		keyStates[STATE_TURN_LEFT] = 1;
		break;
	case KEY_TURN_RIGHT:
		keyStates[STATE_TURN_RIGHT] = 1;
		break;
	case KEY_ESCAPE:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}

}

void onKeyUp(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_MOVE_FORWARD:
		keyStates[STATE_FORWARD] = 0;
		break;
	case KEY_MOVE_BACKWARDS:
		keyStates[STATE_BACKWARDS] = 0;
		break;
	case KEY_TURN_LEFT:
		keyStates[STATE_TURN_LEFT] = 0;
		break;
	case KEY_TURN_RIGHT:
		keyStates[STATE_TURN_RIGHT] = 0;
		break;
	default:
		break;
	}
}

int getKeyState(unsigned int state) {
	if(state > 3) return 0;
	return keyStates[state];
}

void initKeyboard() {
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);
	glutIgnoreKeyRepeat(GL_TRUE);
}