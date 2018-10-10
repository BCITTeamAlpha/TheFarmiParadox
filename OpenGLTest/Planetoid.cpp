#include "Planetoid.h"

//represents a constant planet density multiplied with the gravitational
//constant to create reasonable and consistent gravity values
const float Planetoid::C_GP = 0.0000003677;
const float Planetoid::C_SCALE = 100;

Planetoid::Planetoid(float x, float y, float r) : 
	_x(x),
	_y(y),
	_r(r),
	//Calculates the mass of a planetoid with similar density to earth based
	//on the radius. Scale factor makes the radius more reasonable for a planet.
	//Multiplied by the gravitational constant for efficiency reasons.
	_m(4.0f*M_PI*r*C_SCALE*r*C_SCALE*r*C_SCALE*C_GP/3.0f),
	_pos(x, y)
{ }

// signed distance of a point from the surface of the planetoid
float Planetoid::distanceFunction(int x, int y) {
	return sqrt((x - _x) * (x - _x) + (y - _y) * (y - _y)) - _r;
}