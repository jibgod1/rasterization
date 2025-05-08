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

        // ��� ��ü���� ���� �˻�
        for (const auto& object : objects) {
            float t;
            if (object->intersect(ray, t) && t < closest_t) {
                closest_t = t;
                closest_object = object;
            }
        }

        // ���� ����� ��ü�� ������ �ش� ��ü�� ������ ��ȯ
        if (closest_object) {
            return closest_object->getColor();
        }

        // �����ϴ� ��ü�� ������ ����(��: ������)�� ��ȯ
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
};