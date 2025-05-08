#include "Sphere.h"

Sphere::Sphere(glm::vec3 center, float radius, glm::vec3 color)
	:center(center), radius(radius), color(color)
{
}

bool Sphere::intersect(Ray& ray, float& t) const
{
    glm::vec3 oc = ray.getOrigin() - center;
    float a = glm::dot(ray.getDirection(), ray.getDirection());
    float b = 2.0f * glm::dot(oc, ray.getDirection());
    float c = glm::dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;
    }
    else {
        float sqrt_discriminant = std::sqrt(discriminant);
        float t1 = (-b - sqrt_discriminant) / (2.0f * a);
        float t2 = (-b + sqrt_discriminant) / (2.0f * a);

        // t1과 t2 중 더 작은 양의 값을 선택
        if (t1 > 0 && t1 < t2) {
            t = t1;
        }
        else {
            t = t2;
        }

        return true;
    }
}

