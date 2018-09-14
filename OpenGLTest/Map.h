#ifndef MAP_H
#define MAP_H

#include <algorithm>
#include <vector>
#include <limits>
#include "Planetoid.h"

class Map {
public:
	Map(unsigned int type);
	bool isSolid(int x, int y);
	int height();
	void explosion(Planetoid p);
	int width();
	float value(int x, int y);
private:
	int _index(int x, int y);
	float* _mapArray;
	int _height;
	int _width;
};

#endif