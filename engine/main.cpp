#include<iostream>
#include<functional>
#include"./functionLayer/FunctionLayer.h"
#include"./ResourceLayer/ResourceLayer.h"

int main()
{
	Initialization initializer;
	initializer.GLFWInitialization();
	auto* window = initializer.window;
	VertexShader vert("./shaderLib/BPVertex.glsl");
	FragmentShader frag("./shaderLib/BPFrag.glsl");
	Shader shader(vert, frag);
	Model model;
	Light light(glm::vec3(-5, 10, 0), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255));
	Camera camera(glm::vec3(0.0f, 5, 25), glm::vec3(0.0, 5, -1), glm::vec3(0, 1, 0));
	Camera::SetMainCamera(&camera);
	glfwSetKeyCallback(window, Camera::CameraKeyDetection);//接收一个函数指针
	glfwSetCursorPosCallback(window, Camera::CameraMouseDetection);
	//还有一个鼠标的待定
	model.loadModel("./Resource/shenhe/shenhe.pmx");
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Bind();
		shader.UpLoadUniformMat4("MVP", camera.GetMVP());
		shader.UpLoadUniformMat4("model", glm::mat4(1.0));
		shader.UpLoadUniformFloat3("lightPos", light.GetPos());
		shader.UpLoadUniformFloat3("viewPos", camera.GetPosition());
		model.Draw(shader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}