#include "MarchingSquares.h"

#include <glm/vec3.hpp>

using glm::vec3;

namespace MarchingSquares {
	const static float HALF_THICKNESS = 5.0f;

	// each int[] represents one of marching square's 16 cases
	// each positive int represents one of the 16 possible vertex positions
	// vertices 4, 5, 6, 7, 12, 13, 14, and 15 have their positions interpolated
	// -1 signals the end of the list of vertex positions
	//
	//   11_14_10
	//   |     |
	//   15    13
	//   |_____|
	//   8  12 9
	//
	//   3__6__2
	//   |     |
	//   7     5
	//   |_____|
	//   0  4  1
	const static int SQUARES[16][25] = {
		{ -1 },
		{ 8, 12, 15, 15, 12, 7, 12, 4, 7, -1 },
		{ 12, 9, 13, 12, 13, 4, 13, 5, 4, -1 },
		{ 8, 9, 15, 9, 13, 15, 15, 13, 7, 13, 5, 7, -1 },
		{ 13, 10, 14, 13, 14, 6, 13, 6, 5, -1 },
		{ 8, 10, 15, 15, 10, 14, 8, 12, 13, 8, 13, 10,		15, 14, 6, 15, 6, 7,		13, 12, 4, 13, 4, 5,		 -1 },
		{ 12, 9, 10, 12, 10, 14, 12, 14, 6, 12, 6, 4, -1 },
		{ 8, 9, 10, 8, 10, 15, 15, 10, 14, 15, 14, 6, 15, 6, 7, -1 },
		{ 15, 14, 11, 14, 15, 7, 14, 7, 6, -1 },
		{ 8, 14, 11, 8, 12, 14,	14, 12, 4, 14, 4, 6, -1 },
		{ 12, 9, 15, 9, 11, 15,	9, 13, 11, 13, 14, 11, 12, 15, 7, 12, 7, 4, 14, 13, 5, 14, 5, 6, -1 },
		{ 8, 9, 11, 9, 13, 14, 9, 14, 11, 14, 13, 5, 14, 5, 6, -1 },
		{ 15, 10, 11, 15, 13, 10, 13, 15, 7, 13, 7, 5, -1 },
		{ 8, 10, 11, 8, 12, 13, 13, 10, 8, 13, 12, 4, 13, 4, 5, -1 },
		{ 9, 10, 11, 12, 11, 15, 12, 9, 11, 12, 15, 7, 12, 7, 4, -1 },
		{ 10, 11, 8, 9, 10, 8, -1 }
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

				for (int i = 0; SQUARES[index][i] != -1; i++) {
					vec3 v;
					switch (SQUARES[index][i]) {
						case 0:
							v = vec3(0, 0, -HALF_THICKNESS);
							break;
						case 1:
							v = vec3(1, 0, -HALF_THICKNESS);
							break;
						case 2:
							v = vec3(1, 1, -HALF_THICKNESS);
							break;
						case 3:
							v = vec3(0, 1, -HALF_THICKNESS);
							break;
						case 4:
							v = vec3(zeroCrossing(bottomLeft, bottomRight), 0, -HALF_THICKNESS);
							break;
						case 5:
							v = vec3(1, zeroCrossing(bottomRight, topRight), -HALF_THICKNESS);
							break;
						case 6:
							v = vec3(zeroCrossing(topLeft, topRight), 1, -HALF_THICKNESS);
							break;
						case 7:
							v = vec3(0, zeroCrossing(bottomLeft, topLeft), -HALF_THICKNESS);
							break;
						case 8:
							v = vec3(0, 0, HALF_THICKNESS);
							break;
						case 9:
							v = vec3(1, 0, HALF_THICKNESS);
							break;
						case 10:
							v = vec3(1, 1, HALF_THICKNESS);
							break;
						case 11:
							v = vec3(0, 1, HALF_THICKNESS);
							break;
						case 12:
							v = vec3(zeroCrossing(bottomLeft, bottomRight), 0, HALF_THICKNESS);
							break;
						case 13:
							v = vec3(1, zeroCrossing(bottomRight, topRight), HALF_THICKNESS);
							break;
						case 14:
							v = vec3(zeroCrossing(topLeft, topRight), 1, HALF_THICKNESS);
							break;
						case 15:
							v = vec3(0, zeroCrossing(bottomLeft, topLeft), HALF_THICKNESS);
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
			ret.elements.push_back(i);
		}

		for (int i = 0; i < ret.positions.size(); i += 3) {
			glm::vec3 U = ret.positions[i + 1] - ret.positions[i];
			glm::vec3 V = ret.positions[i + 2] - ret.positions[i];
			glm::vec3 normal;
			normal.x = U.y * V.z - U.z * V.y;
			normal.y = U.z * V.x - U.x * V.z;
			normal.z = U.x * V.y - U.y * V.x;
			ret.normals.push_back(normal);
			ret.normals.push_back(normal);
			ret.normals.push_back(normal);
		}

		return ret;
	}

}