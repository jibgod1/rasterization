#include "Plane.h"

Plane::Plane(glm::vec3 normal, glm::vec3 point, glm::vec3 color)
	: normal(normal), point(point), color(color)
{
}

bool Plane::intersect(Ray& ray, float& t) const
{
    float denom = glm::dot(normal, ray.getDirection());
    if (glm::abs(denom) > 1e-6) { // denom이 0이 아니면
        t = glm::dot(point - ray.getOrigin(), normal) / denom;
        return (t >= 0);
    }
    return false; // 평면과 광선이 평행함
}

