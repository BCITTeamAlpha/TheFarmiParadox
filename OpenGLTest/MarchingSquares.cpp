#include "MarchingSquares.h"

#include <glm/vec3.hpp>

using glm::vec3;

namespace MarchingSquares {
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
		{ 4, 7, 0, -1 },
		{ 5, 4, 1, -1 },
		{ 5, 7, 0, 1, 5, 0, -1 },
		{ 2, 6, 5, -1 },
		{ 4, 7, 0, 2, 6, 5, -1 },
		{ 2, 6, 4, 1, 2, 4, -1 },
		{ 1, 7, 0, 6, 7, 1, 2, 6, 1, -1 },
		{ 6, 3, 7, -1 },
		{ 6, 3, 0, 4, 6, 0, -1 },
		{ 6, 3, 7, 1, 5, 4, -1 },
		{ 6, 3, 0, 5, 6, 0, 1, 5, 0, -1 },
		{ 2, 3, 7, 5, 2, 7, -1 },
		{ 4, 3, 0, 5, 3, 4, 2, 3, 5, -1 },
		{ 2, 3, 7, 2, 7, 4, 2, 4, 1, -1 },
		{ 2, 3, 0, 1, 2, 0, -1 },
	};

	// reciprocal of lerp where output value is 0
	inline float zeroCrossing(float a, float b) {
		return -a / (b - a);
	}

	Model MarchingSquares::GenerateModel(Map &m) {
		Model ret;
		// reserve enough memory for worst case number of vertices
		ret.positions.reserve(m.height() * m.width() * 9);
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
					ret.positions.push_back(v + offset);
				}
			}
		}

		ret.normals.reserve(ret.positions.size());
		ret.UVs.reserve(ret.positions.size() / 3 * 2);
		ret.elements.reserve(ret.positions.size() / 3);

		for (int i = 0; i < ret.positions.size(); i++) {
			glm::vec2 UV;
			UV.x = ret.positions[i].x / m.width();
			UV.y = ret.positions[i].y / m.height();
			ret.UVs.push_back(UV);
			ret.normals.push_back({ 0, 0, 1 });
			ret.elements.push_back(i);
		}

		return ret;
	}

}