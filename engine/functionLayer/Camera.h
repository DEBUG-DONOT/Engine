#pragma once
#include<glm/glm.hpp>
class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up);
	inline glm::mat4 GetView() { return view; }
	inline glm::mat4 GetProjection() { return projection; }
	inline glm::mat4 GetMVP() { return projection* view * model; }
	inline glm::vec3 GetPosition() { return _position; }
private:
	float aspect=16.0/9.0, fov=glm::radians(45.0), near = 0.1, far = 100, speed = 1.0f;
	glm::vec3 _position;
	glm::vec3 _lookAt;
	glm::vec3 _up;
	glm::mat4 model, view, projection;
	glm::vec3 cameraFront, cameraUp, cameraRight;
};
