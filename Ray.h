#pragma once

#include "glm/glm.hpp"
#include <vector>

class Ray
{
public:
	Ray();
	Ray::Ray(glm::vec3 inOrigin, glm::vec3 inDirection);
	~Ray();

	glm::vec3 origin;
	glm::vec3 direction;
};

