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
	Camera camera(glm::vec3(0.0f, 2, 10), glm::vec3(0, 2, -1), glm::vec3(0, 1, 0));
	Camera::SetMainCamera(&camera);
	glfwSetKeyCallback(window, Camera::CameraKeyDetection);//接收一个函数指针
	//glfwSetCursorPosCallback(window, Camera::CameraMouseDetection);
	model.loadModel("./Resource/shenhe/shenhe.pmx");
	while (!glfwWindowShouldClose(window))
	{
		//fixed time physical loop
		//render loop	
		//背面的东西穿透到正面来了,结果是忘记开深度测试了
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