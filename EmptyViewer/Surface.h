#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
class Surface
{
public:
    virtual bool intersect(Ray& ray, float& t) const = 0;
    virtual glm::vec3 getColor() const = 0;
    virtual ~Surface() = default;
};

