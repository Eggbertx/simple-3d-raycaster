#include <math.h>

#include <simple3D_config.h>

float distance(float ax, float ay, float bx, float by) {
	return sqrt(pow(bx - ax, 2) + pow(by - ay, 2));
}

float fixAngle(float angle) {
	if(angle < 0) {
		angle += 2*PI;
	} else if(angle > 2*PI) {
		angle -= 2*PI;
	}
	return angle;
}