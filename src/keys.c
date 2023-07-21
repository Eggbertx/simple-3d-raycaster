#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include "keys.h"

keyStates states;

void onKeyDown(unsigned char key, int x, int y) {
	switch(key) {
	case KEY_MOVE_FORWARD:
		states.forward = 1;
		break;
	case KEY_MOVE_BACKWARDS:
		states.backwards = 1;
		break;
	case KEY_TURN_LEFT:
		states.turnLeft = 1;
		break;
	case KEY_TURN_RIGHT:
		states.turnRight = 1;
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
		states.forward = 0;
		break;
	case KEY_MOVE_BACKWARDS:
		states.backwards = 0;
		break;
	case KEY_TURN_LEFT:
		states.turnLeft = 0;
		break;
	case KEY_TURN_RIGHT:
		states.turnRight = 0;
		break;
	default:
		break;
	}
}

int getKeyState(unsigned int state) {
	switch (state) {
	case STATE_FORWARD:
		return states.forward;
	case STATE_BACKWARDS:
		return states.backwards;
	case STATE_TURN_LEFT:
		return states.turnLeft;
	case STATE_TURN_RIGHT:
		return states.turnRight;
	}
	return 0;
}

void initKeyboard() {
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);
	glutIgnoreKeyRepeat(GL_TRUE);
}