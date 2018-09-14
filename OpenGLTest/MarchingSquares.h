#ifndef MARCHINGSQUARES_H
#define MARCHINGSQUARES_H

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "Map.h"

namespace MarchingSquares {
	std::vector<glm::vec3> GenerateMesh(Map m);
}

#endif