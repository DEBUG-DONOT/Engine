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
//void Camera::CalculateViewMatrix()
//{
//	this->view = glm::lookAt(_position, cameraFront + _position, cameraUp);
//}

Camera* Camera::MainCamera = nullptr;
float Camera::speed = 1.0;
bool Camera::firstTimeMouse = true;
double Camera::mouseX = 0; double Camera::mouseY = 0; double Camera::sensitivity = 0.05;
double Camera::pitch = 0; double Camera::yaw = 0;
void Camera::CameraKeyDetection(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//view矩阵应用到物体上。View=Rotate*Translate,我们这里只修改其中的translate部分。对于translate来说，就是把模型移动到相机位置，所以这里方向都取负
	//换成对model矩阵的修改，现在的修改是不合理的
	glm::vec3 transDir;
	if (key == GLFW_KEY_W&&(action==GLFW_PRESS||action==GLFW_REPEAT))//W
	{
		MainCamera->_position += MainCamera->cameraFront * Camera::speed;
	}
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))//S
	{
		//z方向
		MainCamera->_position -= MainCamera->cameraFront * Camera::speed;
	}
	else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))//A
	{
		//-x方向
		MainCamera->_position -= glm::normalize(glm::cross(MainCamera->cameraFront, MainCamera->cameraUp)) * Camera::speed;
	}
	else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))//D
	{
		//x方向
		MainCamera->_position += glm::normalize(glm::cross(MainCamera->cameraFront, MainCamera->cameraUp)) * Camera::speed;
	}
	else if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		MainCamera->_position += MainCamera->cameraUp * Camera::speed;
	}
	else if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		MainCamera->_position -= MainCamera->cameraUp * Camera::speed;
	}
	MainCamera->SetView( glm::lookAt(MainCamera-> _position, MainCamera->cameraFront + MainCamera->_position, MainCamera->cameraUp));
}

void Camera::CameraMouseDetection(GLFWwindow* window, double xpos, double ypos)
{
	if (firstTimeMouse)
	{
		mouseX = xpos;
		mouseY = ypos;
		firstTimeMouse = false;
	}
	float xoffset = xpos - Camera::mouseX;
	float yoffset = Camera::mouseY - ypos;
	Camera::mouseX = xpos;
	Camera::mouseY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	Camera::yaw += xoffset;
	Camera::pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	MainCamera->cameraFront = glm::normalize(front);
	MainCamera->SetView(glm::lookAt(MainCamera->_position, MainCamera->cameraFront + MainCamera->_position, MainCamera->cameraUp));
}





