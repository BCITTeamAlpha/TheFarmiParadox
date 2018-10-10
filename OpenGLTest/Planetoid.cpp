#include "Planetoid.h"

//represents a constant planet density multiplied with the gravitational
//constant to create reasonable and consistent gravity values
const float Planetoid::C_GP = 0.0000003677;
const float Planetoid::C_DIST = 100;

Planetoid::Planetoid(float x, float y, float r) : 
	_x(x),
	_y(y),
	_r(r),
	_m(4.0f/3.0f*M_PI*r*r*r*C_GP*C_DIST*C_DIST*C_DIST),
	_pos(x, y)
{ }

// signed distance of a point from the surface of the planetoid
float Planetoid::distanceFunction(int x, int y) {
	return sqrt((x - _x) * (x - _x) + (y - _y) * (y - _y)) - _r;
}