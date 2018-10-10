#pragma once
//created by Map
#define _USE_MATH_DEFINES
#include <Math.h>
#include <glm/glm.hpp>

class Planetoid {
	public:
		Planetoid(float x, float y, float r);
		const float _x;
		const float _y;
		const glm::vec2 _pos;
		const float _r;
		const float _m;
		float distanceFunction(int x, int y);
		static const float C_GP;
		static const float C_DIST;
	private:
};