#pragma once
#include "Surface.h"
class Plane : public Surface
{
public:
	Plane(glm::vec3 normal, glm::vec3 point, glm::vec3 color);
	bool intersect(Ray& ray, float& t) const override;
	glm::vec3 getNormal(glm::vec3& point) const { return normal; }
	glm::vec3 getColor() const override { return color; }
	glm::vec3 getPoint() const { return point; }
private:
	glm::vec3 normal;
	glm::vec3 point;
	glm::vec3 color;
};

