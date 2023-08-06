#ifndef KEYS_H
#define KEYS_H

#define STATE_FORWARD 0
#define STATE_BACKWARDS 1
#define STATE_TURN_LEFT 2
#define STATE_TURN_RIGHT 3
#define STATE_MOVING 4
#define STATE_RESET 5
#define STATE_EXIT 6

uint8_t getKeyState(int state);

#endif // KEYS_H