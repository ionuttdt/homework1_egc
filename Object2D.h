// Duta Viorel-Ionut, 331CB
#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateSquare2(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* Wings(std::string name, glm::vec3 leftBottomCorner, float side, glm::vec3 color);
	Mesh* Circle(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color);
	Mesh* Bird(std::string name, glm::vec3 leftBottomCorner, float radius);

}
