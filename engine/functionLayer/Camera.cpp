#include"Camera.h"
#include<iostream>
#include<glm/gtc/matrix_transform.hpp>
Camera::Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up)
	:_position(position),_lookAt(lookAt),_up(up)
{
	cameraFront = glm::normalize(_lookAt - _position);
	cameraUp = glm::normalize(up);
	cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
	this->model = glm::mat4(1.0);
	this->view = glm::lookAt(_position, cameraFront, cameraUp);
	this->projection = glm::perspective(fov, aspect, near, far);
}
Camera* Camera::MainCamera = nullptr;
float Camera::speed = 1.0;
bool Camera::firstTimeMouse = true;
double Camera::mouseX = 0; double Camera::mouseY = 0; double Camera::sensitivity = 0.05;
double Camera::pitch = 0; double Camera::yaw = 0;
void Camera::CameraKeyDetection(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//view矩阵应用到物体上。View=Rotate*Translate,我们这里只修改其中的translate部分。对于translate来说，就是把模型移动到相机位置，所以这里方向都取负
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
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		//x方向
		MainCamera->SetView(glm::rotate(MainCamera->view, glm::radians(45.0f), glm::vec3(0, 1, 0)));
		return;
	}
	else
	{
		//do nothing
		transDir = glm::vec3(0, 0, 0) * Camera::speed;
	}
	MainCamera->SetView(glm::translate(MainCamera->view, transDir));
}

inline void Camera::SetView(glm::mat4 new_view)
{
	this->view = new_view;
}



