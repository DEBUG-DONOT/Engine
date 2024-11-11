#include"Camera.h"
#include<glm/gtc/matrix_transform.hpp>
Camera::Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up)
	:_position(position),_lookAt(lookAt),_up(up)
{
	this->view = glm::lookAt(_position, _lookAt, _up);
	this->model = glm::mat4(1.0);
	this->projection = glm::perspective(fov, aspect, near, far);
	cameraFront = glm::normalize(_lookAt - _position);
	cameraUp = glm::normalize(up);
	cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}
