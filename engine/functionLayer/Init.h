#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Initialization
{
public:
	void GLFWInitialization(int width = 1600, int high = 900, const char* name = "OpenGlWindow");
	void GammaCorrectionControl(bool flga);
	GLFWwindow* window=nullptr;
};

