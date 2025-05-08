#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	Camera(glm::vec3 e);
	~Camera();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::vec3 getEye();
	glm::vec3 getU();
	glm::vec3 getV();
	glm::vec3 getW();
	glm::vec3 getUp();
	float getnx();
	float getny();
private:
	glm::vec3 e;
	glm::vec3 u;
	glm::vec3 v;
	glm::vec3 w;
	glm::vec3 up;
	float l;
	float r;
	float b;
	float t;
	float n;
	float f;
	float nx;
	float ny;
};
