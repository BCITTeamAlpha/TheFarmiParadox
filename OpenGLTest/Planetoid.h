#ifndef PLANETOID_H
#define PLANETOID_H

#include <math.h>

class Planetoid {
public:
	Planetoid(float x, float y, float r);
	float xPos();
	float yPos();
	float radius();
	float distanceFunction(int x, int y);
private:
	float _x;
	float _y;
	float _r;
};

#endif