#include"example.h"
void GI()
{
    /*
    *这个函数用来实现Global Illumination
    * 使用康奈尔box来验证效果
    */
   #pragma region Init
   Initialization initializer;
   initializer.GLFWInitialization(1920, 1080);//默认开启深度测试
   auto* window = initializer.window;
   // 禁用 OpenGL 自动 sRGB 转换
   //glDisable(GL_FRAMEBUFFER_SRGB);
   //glEnable(GL_FRAMEBUFFER_SRGB);
   #pragma endregion
   #pragma region Scene
   //场景设置
   Camera camera(glm::vec3(5.0f, 5, 50), glm::vec3(0.0, 0, -1), glm::vec3(0, 1, 0));
   Camera::SetMainCamera(&camera);
   glfwSetKeyCallback(window, Camera::CameraKeyDetection);//接收一个函数指针
   //glfwSetCursorPosCallback(window, Camera::CameraMouseDetection);
   Light light(glm::vec3(2, 12, 40), glm::vec3(0, 0, -10), glm::vec3(100, 150, 200));//light dir color
   float near_plane = 0.1f, far_plane = 100.0f;
   glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
   glm::mat4 lightView = glm::lookAt(light.GetPos(), light.GetDirection(), glm::vec3(0.0, 1.0, 0.0));
   glm::mat4 lightSpaceMatrix = lightProjection * lightView;
   #pragma endregion

   //加载模型
   /*
    *加载obj和mtl文件
   */
    Model cornellBox;
    cornellBox.loadModel("./Resource/cornellBox/CornellBox-Original.obj");
    cornellBox.checkMesh();
    
    #pragma region Shder

    #pragma endregion



}