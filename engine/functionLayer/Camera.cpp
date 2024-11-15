#include"Camera.h"
#include<iostream>
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
Camera* Camera::MainCamera = nullptr;
float Camera::speed = 1.0;
void Camera::CameraKeyDetection(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//记得取逆
	glm::vec3 transDir;
	if (key == GLFW_KEY_W&&action==GLFW_PRESS)
	{
		//朝-z方向
		transDir = glm::vec3(0, 0, 1) * Camera::speed;
		//std::cout << "W" << std::endl;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		//z方向
		transDir = glm::vec3(0, 0, -1) * Camera::speed;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		//-x方向
		transDir = glm::vec3(1, 0, 0) * Camera::speed;
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		//x方向
		transDir = glm::vec3(-1, 0, 0) * Camera::speed;
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		//x方向
		transDir = glm::vec3(0, -1, 0) * Camera::speed;
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		//x方向
		transDir = glm::vec3(0, 1, 0) * Camera::speed;
	}
	else
	{
		//do nothing
		transDir = glm::vec3(0, 0, 0) * Camera::speed;
	}
	MainCamera->SetView(glm::translate(MainCamera->view, transDir));
}

void Camera::CameraMouseDetection()
{
}


