#include <GL/glut.h>

#include "keys.h"

unsigned char keyStates[256];
unsigned char specialKeyStates[21];

void onKeyDown(unsigned char key, int x, int y) {
	keyStates[key] = 1;
}

void onKeyUp(unsigned char key, int x, int y) {
	keyStates[key] = 0;
}

void onSpecialDown(int special, int x, int y) {
	specialKeyStates[special] = 1;
}

void onSpecialUp(int special, int x, int y) {
	specialKeyStates[special] = 0;
}

int isKeyPressed(unsigned char key) {
	return keyStates[key] == 1;
}

int isSpecialKeyPressed(unsigned char key) {
	return specialKeyStates[key] == 0;
}

void initKeyboard() {
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);
	glutSpecialFunc(onSpecialDown);
	glutIgnoreKeyRepeat(GL_TRUE);
}