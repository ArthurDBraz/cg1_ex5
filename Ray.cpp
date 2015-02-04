#include "Ray.h"


Ray::Ray()
{
	origin = glm::vec3(0, 0, 0);
	direction = glm::vec3(1, 0, 0);
}

Ray::Ray(glm::vec3 inOrigin, glm::vec3 inDirection)
{
	origin = inOrigin;
	direction = inDirection;
}

Ray::~Ray()
{
}
