#pragma once
#include "Surface.h"
class Sphere : public Surface
{
public:
	Sphere(glm::vec3 center, float radius, glm::vec3 color);
	bool intersect(Ray& ray, float& t) const override;
	glm::vec3 getColor() const override { return color; }
private:
	glm::vec3 center;
	float radius;
	glm::vec3 color;
};

