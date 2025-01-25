#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Initialization
{
public:

	void GLFWInitialization(unsigned int width , unsigned int hight, const char* name = "OpenGlWindow");
	void GammaCorrectionControl(bool flga);
	GLFWwindow* window=nullptr;
private:
	int m_width, m_heigh;
};

