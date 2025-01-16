#include<iostream>
#include<functional>
#include"./functionLayer/FunctionLayer.h"
#include"./ResourceLayer/ResourceLayer.h"
#include<glm/gtc/matrix_transform.hpp>
#include"./ResourceLayer/SingleTexture.h"
#include <fstream>
#include <cmath>
void gun()
{
	Initialization initializer;
	initializer.GLFWInitialization();
	auto* window = initializer.window;
	//VertexShader vert("./shaderLib/BPVertex.vert");
	//FragmentShader frag("./shaderLib/BPFrag.frag");
	//VertexShader vert("./shaderLib/PBRV.vert");
	//FragmentShader frag("./shaderLib/PBRF.frag");
	VertexShader vert("./shaderLib/samplePBR.vert");
	FragmentShader frag("./shaderLib/samplePBR.frag");
	Shader shader(vert, frag);
	Model model;
	Light light(glm::vec3(-5, 10, 0), glm::vec3(0, 0, 0), glm::vec3(100, 150, 200));//light dir color
	Camera camera(glm::vec3(0.0f, 5, 25), glm::vec3(0.0, 5, -1), glm::vec3(0, 1, 0));
	Camera::SetMainCamera(&camera);
	glfwSetKeyCallback(window, Camera::CameraKeyDetection);//接收一个函数指针
	glfwSetCursorPosCallback(window, Camera::CameraMouseDetection);
	model.loadModel("./Resource/pbr/Cerberus_LP.fbx");
	model.checkAllTypeTexture();
	model.showAllLoadedTexture();

	SingleTexure albedo("./Resource/pbr/Textures/Cerberus_A.tga");
	SingleTexure metallic("./Resource/pbr/Textures/Cerberus_M.tga");
	SingleTexure normal("./Resource/pbr/Textures/Cerberus_N.tga");
	SingleTexure roughness("./Resource/pbr/Textures/Cerberus_R.tga");
	SingleTexure ao("./Resource/pbr/Textures/Raw/Cerberus_AO.tga");

	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
	camera.SetModel(modelMatrix);
	//这些不能放在for循环中
	//shader.Bind();
	shader.UpLoadUniformInt("albedoMap", albedo.texture_id);
	shader.UpLoadUniformInt("normalMap", normal.texture_id);
	shader.UpLoadUniformInt("metallicMap", metallic.texture_id);
	shader.UpLoadUniformInt("roughnessMap", roughness.texture_id);
	shader.UpLoadUniformInt("aoMap", ao.texture_id);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Bind();
		shader.UpLoadUniformMat4("MVP", camera.GetMVP());
		shader.UpLoadUniformMat4("model", modelMatrix);

		//shader.UpLoadUniformFloat3("lightPos", light.GetPos());
		//shader.UpLoadUniformFloat3("viewPos", camera.GetPosition());
		//shader.UpLoadUniformFloat3("lightColor", light.GetColor());
		shader.UpLoadUniformFloat3("lightPositions", light.GetPos());//sample
		shader.UpLoadUniformFloat3("camPos", camera.GetPosition());//sample
		shader.UpLoadUniformFloat3("lightColors", light.GetColor());//sample
		//*******************************************************
		glActiveTexture(GL_TEXTURE0);//纹理一定要从0开始绑定
		albedo.bind();
		glActiveTexture(GL_TEXTURE0 + 1);
		normal.bind();
		glActiveTexture(GL_TEXTURE0 + 2);
		metallic.bind();
		glActiveTexture(GL_TEXTURE0 + 3);
		roughness.bind();
		glActiveTexture(GL_TEXTURE0 + 4);
		ao.bind();
		//***********************************************
		model.DrawPBR(shader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

void drawCircle()
{
	Initialization initializer;
	initializer.GLFWInitialization();//默认开启深度测试
	auto* window = initializer.window;
	while (!glfwWindowShouldClose(window))
	{
		


		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

int main()
{

}