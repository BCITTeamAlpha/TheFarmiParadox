#pragma once
//created by Map
#define _USE_MATH_DEFINES
#include <Math.h>

class Planetoid {
	public:
		Planetoid(float x, float y, float r);
		const float _x;
		const float _y;
		const float _r;
		const float _m;
		float distanceFunction(int x, int y);
		const float G = 15000000000;
		const float DENSITY = 5500000000;
	private:
};