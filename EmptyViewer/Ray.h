#pragma once
#include <glm/glm.hpp>

class Ray
{
public:
	Ray(glm::vec3 origin, glm::vec3 direction);
	~Ray();
	glm:: vec3 getDirection();
	glm::vec3 getOrigin();
private:
	glm::vec3 origin;
	glm::vec3 direction;
};

