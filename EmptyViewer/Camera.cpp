#include "Camera.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 e)
	: e(e), u(glm::vec3(1.0f, 0.0f, 0.0f)), v(glm::vec3(0.0f, 1.0f, 0.0f)), w(glm::vec3(0.0f, 0.0f, 1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)),
	l(-0.1f), r(0.1f), b(-0.1f), t(0.1f), n(0.1f), f(1000.0f), nx(512), ny(512)
{
     
}

Camera::~Camera()
{
    // Destructor implementation
}


glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(e, e - w, up);
}

glm::mat4 Camera::getProjectionMatrix()
{
    glm::mat4 P(0.0f);

    float A = 2.0f * n / (r - l);
    float B = 2.0f * n / (t - b);
    float C = (r + l) / (r - l);
    float D = (t + b) / (t - b);
    float E = -(f + n) / (f - n);
    float F = -(2.0f * f * n) / (f - n);

    P[0][0] = A;
    P[1][1] = B;
    P[2][0] = C;
    P[2][1] = D;
    P[2][2] = E;
    P[2][3] = -1.0f;
    P[3][2] = F;

    return P;
}


glm::vec3 Camera::getEye() { return e; }
glm::vec3 Camera::getU() { return u; }
glm::vec3 Camera::getV() { return v; }
glm::vec3 Camera::getW() { return w; }
glm::vec3 Camera::getUp() { return up; }
float Camera::getnx() { return nx; }
float Camera::getny() { return ny; }
