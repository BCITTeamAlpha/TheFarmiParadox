#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Map.h"

namespace MarchingSquares {
	std::vector<glm::vec3> GenerateMesh(Map m);
}