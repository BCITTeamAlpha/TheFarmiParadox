#include "Planetoid.h"

Planetoid::Planetoid(float x, float y, float r) {
	_x = x;
	_y = y;
	_r = r;
}

float Planetoid::xPos() {
	return _x;
}

float Planetoid::yPos(){
	return _y;
}

float Planetoid::radius(){
	return _r;
}

float Planetoid::distanceFunction(int x, int y) {
	return sqrt((x - _x) * (x - _x) + (y - _y) * (y - _y)) - _r;
}
