#pragma once
#include<glm/glm.hpp>
#include<GLFW/glfw3.h>
class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up);
	inline glm::mat4 GetView() { return view; }
	inline glm::mat4 GetProjection() { return projection; }
	inline glm::mat4 GetMVP() { return projection* view * model; }
	inline glm::vec3 GetPosition() { return _position; }
	inline void SetView(glm::mat4 new_view) { this->view = new_view; }
	inline void SetModel(glm::mat4 new_model) { this->model = new_model; }

	static void CameraKeyDetection(GLFWwindow* window,int key,int scancode,int action,int mods);
	static void CameraMouseDetection(GLFWwindow* window,double xpos,double ypos);
	static void SetMainCamera(Camera* cam) { MainCamera = cam; }

private:
	static Camera* MainCamera;
	float aspect = 16.0 / 9.0, fov = glm::radians(45.0), near = 0.1, far = 100.0f;
	static float speed;
	glm::vec3 _position, _lookAt, _up, cameraFront, cameraUp, cameraRight;
	glm::mat4 model, view, projection;
	static double mouseX, mouseY,sensitivity,pitch,yaw;
	static bool firstTimeMouse;
};




