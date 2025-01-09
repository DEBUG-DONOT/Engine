#include<iostream>
#include<functional>
#include"./functionLayer/FunctionLayer.h"
#include"./ResourceLayer/ResourceLayer.h"
#include<glm/gtc/matrix_transform.hpp>

int main()
{
	Initialization initializer;
	initializer.GLFWInitialization();
	auto* window = initializer.window;
	//VertexShader vert("./shaderLib/BPVertex.vert");
	//FragmentShader frag("./shaderLib/BPFrag.frag");
	VertexShader vert("./shaderLib/PBRV.vert");
	FragmentShader frag("./shaderLib/PBRF.frag");
	Shader shader(vert, frag);
	Model model;
	Light light(glm::vec3(-5, 10, 0), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255));
	Camera camera(glm::vec3(0.0f, 5, 25), glm::vec3(0.0, 5, -1), glm::vec3(0, 1, 0));
	Camera::SetMainCamera(&camera);
	glfwSetKeyCallback(window, Camera::CameraKeyDetection);//接收一个函数指针
	glfwSetCursorPosCallback(window, Camera::CameraMouseDetection);
	//model.loadModel("./Resource/shenhe/shenhe.pmx");
	//model.loadModel("./Resource/pbr/Cerberus_LP.fbx");
	//model.loadModel("./Resource/Paladin J Nordstrom.fbx");
	//model.loadModel("./Resource/source/SKETCHFAB_Grizzly_MKV.fbx");
	//model.loadModel("./Resource/source/tea/model/base.obj");
	model.loadModel("./Resource/antique_stool_pbrgr.glb");
	model.checkAllTypeTexture();
	model.showAllLoadedTexture();
	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
	camera.SetModel(modelMatrix);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Bind();
		shader.UpLoadUniformMat4("MVP", camera.GetMVP());
		shader.UpLoadUniformMat4("model", modelMatrix);
		shader.UpLoadUniformFloat3("lightPos", light.GetPos());
		shader.UpLoadUniformFloat3("viewPos", camera.GetPosition());
		shader.UpLoadUniformFloat3("lightColor", glm::vec3(255,255,255));
		model.Draw(shader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}