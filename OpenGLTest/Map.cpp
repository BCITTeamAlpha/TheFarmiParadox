#include "Map.h"

Map::Map(unsigned int level) {
	std::vector<Planetoid> planets;

	switch (level) {
		default:
			_height = 128;
			_width = 128;
			planets.push_back(Planetoid(63.5f, 63.5f, 32.0f));
	}

	_mapArray = new float[_height * _width];

	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			_mapArray[_index(x, y)] = 1.0;
			for (int i = 0; i < planets.size(); i++) {
				_mapArray[_index(x, y)] = std::min(
					_mapArray[_index(x, y)], 
					planets[i].distanceFunction(x, y)
				);
			}
		}
	}
}

// array access
int Map::_index(int x, int y) {
	return x + y * _width;
}

bool Map::isSolid(int x, int y) {
	return _mapArray[_index(x, y)] <= 0.0f;
}

int Map::width() {
	return _width;
}

int Map::height() {
	return _height;
}
