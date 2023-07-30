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
	case KEY_RESET:
		states.reset = 1;
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
	case KEY_RESET:
		states.reset = 0;
		break;
	default:
		break;
	}
}

void onSpecialKeyDown(int key, int x, int y) {
	switch(key) {
	case GLUT_KEY_ALT_L:
	case GLUT_KEY_ALT_R:
		states.moving = 1;
		break;
	default:
		break;
	}
}

void onSpecialKeyUp(int key, int x, int y) {
	switch(key) {
	case GLUT_KEY_ALT_L:
	case GLUT_KEY_ALT_R:
		states.moving = 0;
		break;
	default:
		break;
	}
}

int getKeyState(unsigned int state) {
	switch(state) {
	case STATE_FORWARD:
		return states.forward;
	case STATE_BACKWARDS:
		return states.backwards;
	case STATE_TURN_LEFT:
		return states.turnLeft;
	case STATE_TURN_RIGHT:
		return states.turnRight;
	case STATE_MOVING:
		return states.moving;
	case STATE_RESET:
		return states.reset;
	}
	return 0;
}

void initKeyboard() {
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);
	glutSpecialFunc(onSpecialKeyDown);
	glutSpecialUpFunc(onSpecialKeyUp);
	glutIgnoreKeyRepeat(GL_TRUE);
}