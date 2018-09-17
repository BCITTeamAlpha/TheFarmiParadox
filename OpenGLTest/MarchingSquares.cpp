#include "MarchingSquares.h"

using std::vector;
using glm::vec3;

// each int[] represents one of marching square's 16 cases
// each positive int represents one of the 8 possible vertex positions
// vertices 4, 5, 6, and 7 have their positions interpolated
// -1 signals the end of the list of vertex positions
//
//   3__6__2
//   |     |
//   7     5
//   |_____|
//   0  4  1
const static int _squares[16][10] = {
	{ -1 },
	{ 7, 4, 0, -1 },
	{ 4, 5, 1, -1 },
	{ 7, 5, 0, 5, 1, 0, -1 },
	{ 6, 2, 5, -1 },
	{ 7, 4, 0, 6, 2, 5, -1 },
	{ 6, 2, 4, 2, 1, 4, -1 },
	{ 7, 1, 0, 7, 6, 1, 6, 2, 1, -1 },
	{ 3, 6, 7, -1 },
	{ 3, 6, 0, 6, 4, 0, -1 },
	{ 3, 6, 7, 5, 1, 4, -1 },
	{ 3, 6, 0, 6, 5, 0, 5, 1, 0, -1 },
	{ 3, 2, 7, 2, 5, 7, -1 },
	{ 3, 4, 0, 3, 5, 4, 3, 2, 5, -1 },
	{ 3, 2, 7, 7, 2, 4, 4, 2, 1, -1 },
	{ 3, 2, 0, 2, 1, 0, -1 },
};

// reciprocal of lerp where output value is 0
inline float zeroCrossing(float a, float b) {
	return -a / (b - a);
}

vector<vec3> MarchingSquares::GenerateMesh(Map m) {
	vector<vec3> vertexVector;
	// reserve enough memory for worst case number of vertices
	vertexVector.reserve(m.height() * m.width() * 9);
	for (int x = 0; x < m.width() - 1; x++) {
		for (int y = 0; y < m.height() - 1; y++) {
			vec3 offset = vec3(x, y, 0);

			// grab values from map distance field
			float bottomLeft = m.value(x, y);
			float bottomRight = m.value(x + 1, y);
			float topRight = m.value(x + 1, y + 1);
			float topLeft = m.value(x, y + 1);

			// check which square case we're dealing with
			int index = 0;
			if (bottomLeft <= 0) { index += 1; }
			if (bottomRight <= 0) { index += 2; }
			if (topRight <= 0) { index += 4; }
			if (topLeft <= 0) { index += 8; }

			for (int i = 0; _squares[index][i] != -1; i++) {
				vec3 v;
				switch (_squares[index][i]) {
					case 0:
						v = vec3(0, 0, 0);
						break;
					case 1:
						v = vec3(1, 0, 0);
						break;
					case 2:
						v = vec3(1, 1, 0);
						break;
					case 3:
						v = vec3(0, 1, 0);
						break;
					case 4:
						v = vec3(zeroCrossing(bottomLeft, bottomRight), 0, 0);
						break;
					case 5:
						v = vec3(1, zeroCrossing(bottomRight, topRight), 0);
						break;
					case 6:
						v = vec3(zeroCrossing(topLeft, topRight), 1, 0);
						break;
					case 7:
						v = vec3(0, zeroCrossing(bottomLeft, topLeft), 0);
						break;
				}
				vertexVector.push_back(v + offset);
			}
		}
	}
	return vertexVector;
}