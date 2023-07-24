#ifndef KEYS_H
#define KEYS_H

#define KEY_MOVE_FORWARD 'w'
#define KEY_MOVE_BACKWARDS 's'
#define KEY_TURN_LEFT 'a'
#define KEY_TURN_RIGHT 'd'
#define KEY_TOGGLE_2D_MAP '\t'
#define KEY_ESCAPE 27

typedef struct {
	int forward;
	int backwards;
	int turnLeft;
	int turnRight;
	int moving;
} keyStates;

#define STATE_FORWARD 0
#define STATE_BACKWARDS 1
#define STATE_TURN_LEFT 2
#define STATE_TURN_RIGHT 3
#define STATE_MOVING 4

void initKeyboard();

int getKeyState(unsigned int state);

#endif // KEYS_H