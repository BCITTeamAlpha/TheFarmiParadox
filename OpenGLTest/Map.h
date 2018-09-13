#ifndef MAP_H
#define MAP_H

#include <algorithm>
#include <vector>
#include "Planetoid.h"

class Map {
public:
	Map(unsigned int type);
	bool isSolid(int x, int y);
	int height();
	int width();
private:
	int _index(int x, int y);
	float* _mapArray;
	int _height;
	int _width;
};

#endif