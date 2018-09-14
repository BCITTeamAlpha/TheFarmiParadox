#include "Planetoid.h"

Planetoid::Planetoid(float x, float y, float r) : 
	_x(x),
	_y(y),
	_r(r),
	_m(r*r)
{ }

// signed distance of a point from the surface of the planetoid
float Planetoid::distanceFunction(int x, int y) {
	return sqrt((x - _x) * (x - _x) + (y - _y) * (y - _y)) - _r;
}