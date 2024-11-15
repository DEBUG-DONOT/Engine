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

	void CameraMouseDetection();
	static void CameraKeyDetection(GLFWwindow* window,int key,int scancode,int action,int mods);
	static void SetMainCamera(Camera* cam) { MainCamera = cam; }
private:
	static Camera* MainCamera;
	float aspect = 16.0 / 9.0, fov = glm::radians(45.0), near = 0.1, far = 100;
	static float speed;
	glm::vec3 _position;
	glm::vec3 _lookAt;
	glm::vec3 _up;
	glm::mat4 model, view, projection;
	glm::vec3 cameraFront, cameraUp, cameraRight;
};


