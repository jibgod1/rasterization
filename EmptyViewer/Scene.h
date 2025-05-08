#include <vector>
#include <glm/glm.hpp>
#include <limits>
#include "Surface.h"
class Scene {
private:
    std::vector<Surface*> objects;

public:
    void addObject(Surface* object) {
        objects.push_back(object);
    }

    glm::vec3 trace(Ray& ray) const {
        float closest_t = std::numeric_limits<float>::max();
        const Surface* closest_object = nullptr;

        // 모든 객체와의 교차 검사
        for (const auto& object : objects) {
            float t;
            if (object->intersect(ray, t) && t < closest_t) {
                closest_t = t;
                closest_object = object;
            }
        }

        // 가장 가까운 객체가 있으면 해당 객체의 색상을 반환
        if (closest_object) {
            return closest_object->getColor();
        }

        // 교차하는 객체가 없으면 배경색(예: 검정색)을 반환
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
};