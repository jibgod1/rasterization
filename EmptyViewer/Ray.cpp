#include "Ray.h"
#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
    : origin(origin), direction(direction)
{
    // Constructor implementation
}

Ray::~Ray()
{
    // Destructor implementation
}

glm::vec3 Ray::getDirection()
{
    return this->direction;
}

glm::vec3 Ray::getOrigin()
{
    return this->origin;
}
