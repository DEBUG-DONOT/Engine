#include"Init.h"
#include<iostream>
void Initialization::GLFWInitialization(unsigned int width, unsigned int high, const char* name)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//设置版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//macos只支持core profile
    glfwWindowHint(GLFW_SAMPLES, 4);
    this->window = glfwCreateWindow(width, high, name, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        //return -1;
        throw "Failed to create GLFW window";
    }
    glfwMakeContextCurrent(window);//设置上下文
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//初始化OpenGL函数指针
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glViewport(0, 0, 1920, 1080);//设置vidwport
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);//深度测试
    glDepthFunc(GL_LESS);

}

void Initialization::GammaCorrectionControl(bool flag)
{
    if (flag)
    {
        glEnable(GL_FRAMEBUFFER_SRGB);
    }
    else
    {
        glDisable(GL_FRAMEBUFFER_SRGB);
    }
}


