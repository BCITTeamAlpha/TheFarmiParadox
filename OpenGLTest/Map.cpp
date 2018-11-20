#include "Map.h"

#include "AssetLoader.h"
#include <algorithm>
#include <limits>
#include "MarchingSquares.h"

using std::numeric_limits;
using std::vector;
using std::min;
using std::max;


Map::Map(vector<Planetoid> planets, int width, int height) {
	_height = height;
	_width = width;

	_mapArray = new float[_height * _width];

	// calculate the union of all planetoid distance fields
	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			_mapArray[_index(x, y)] = numeric_limits<float>::max();
			for (unsigned int i = 0; i < planets.size(); i++) {
				_mapArray[_index(x, y)] = min(
					_mapArray[_index(x, y)], 
					planets[i].distanceFunction(x, y)
				);
			}
		}
	}

	renderable.reset(new Renderable());
	renderable->z = 0;
	renderable->position = new glm::vec2(0, 0);
	renderable->rotation = new glm::vec3(0, 0, 0);
	renderable->texture = AssetLoader::loadTexture("checkerboard.png");
	renderable->color = glm::vec4(0.5, 1, 0, 1);
	renderable->model = MarchingSquares::GenerateModel(*this);
}

// calculate array index so it can be accessed as it were 2d
inline int Map::_index(int x, int y) {
	return x + y * _width;
}

bool Map::isSolid(int x, int y) {
	// if outside of array bounds return that point is outside of terrain
	if (x < 0 || x >= _width || y < 0 || y >= _height) {
		return false;
	}

	return _mapArray[_index(x, y)] <= 0.0f;
}

float Map::value(int x, int y) {
	// if outside of array bounds return that point is outside of terrain
	if (x < 0 || x >= _width || y < 0 || y >= _height) {
		return 1.0f; // 1.0f is an arbitrary value, can return any positive float
	}

	return _mapArray[_index(x, y)];
}

int Map::width() {
	return _width;
}

int Map::height() {
	return _height;
}

void Map::explosion(Planetoid p) {
	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			_mapArray[_index(x, y)] = max(
				_mapArray[_index(x, y)],
				-p.distanceFunction(x, y)
			);
		}
	}

	Model m = MarchingSquares::GenerateModel(*this);
	renderable->model.positions = m.positions;
	renderable->model.normals = m.normals;
	renderable->model.UVs = m.UVs;
	renderable->model.elements = m.elements;
	renderable->invalidated = true;
}