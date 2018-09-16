#include "MarchingSquares.h"

using std::vector;
using glm::vec3;

// each number represents a vertex
// vertices on square edges have their positions interpolated
//
//   3__6__2
//   |     |
//   7     5
//   |_____|
//   0  4  1
const static int _squares[16][10] = {
	{ -1 }, //0
	{ 7, 4, 0, -1 }, //1
	{ 4, 5, 1, -1 }, //2
	{ 7, 5, 0, 5, 1, 0, -1 }, //3
	{ 6, 2, 5, -1 }, //4
	{ 7, 4, 0, 6, 2, 5, -1 }, //5
	{ 6, 2, 4, 2, 1, 4, -1 }, //6
	{ 7, 1, 0, 7, 6, 1, 6, 2, 1, -1 }, //7
	{ 3, 6, 7, -1 }, //8
	{ 3, 6, 0, 6, 4, 0, -1 }, //9
	{ 3, 6, 7, 5, 1, 4, -1 }, //10
	{ 3, 6, 0, 6, 5, 0, 5, 1, 0, -1 }, //11
	{ 3, 2, 7, 2, 5, 7, -1 }, //12
	{ 3, 4, 0, 3, 5, 4, 3, 2, 5, -1 }, //13
	{ 3, 2, 7, 7, 2, 4, 4, 2, 1, -1 }, //14
	{ 3, 2, 0, 2, 1, 0, -1 }, //15
};

float zeroCrossing(float a, float b) {
	return -a / (b - a);
}

vector<vec3> MarchingSquares::GenerateMesh(Map m) {
	vector<vec3> vertexVector;
	for (int x = 0; x < m.width() - 1; x++) {
		for (int y = 0; y < m.height() - 1; y++) {
			int index = 0;
			vec3 offset = vec3(x, y, 0);
			float bL = m.value(x, y);
			float bR = m.value(x + 1, y);
			float tR = m.value(x + 1, y + 1);
			float tL = m.value(x, y + 1);
			if (bL <= 0) { index += 1; }
			if (bR <= 0) { index += 2; }
			if (tR <= 0) { index += 4; }
			if (tL <= 0) { index += 8; }
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
					v = vec3(zeroCrossing(bL, bR), 0, 0);
					break;
				case 5:
					v = vec3(1, zeroCrossing(bR, tR), 0);
					break;
				case 6:
					v = vec3(zeroCrossing(tL, tR), 1, 0);
					break;
				case 7:
					v = vec3(0, zeroCrossing(bL, tL), 0);
					break;
				}
				vertexVector.push_back(v + offset);
			}
		}
	}
	return vertexVector;
}