#include<iostream>
#include<functional>
#include"./functionLayer/FunctionLayer.h"
#include"./ResourceLayer/ResourceLayer.h"
#include<glm/gtc/matrix_transform.hpp>
#include"./ResourceLayer/SingleTexture.h"
#include <fstream>
int main()
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
	Light light(glm::vec3(-5, 10, 0), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255));
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
	glActiveTexture(GL_TEXTURE0 + 5);
	shader.UpLoadUniformInt("albedoMap", albedo.texture_id);
	albedo.bind();
	glActiveTexture(GL_TEXTURE0 + 6);
	shader.UpLoadUniformInt("normalMap", normal.texture_id);
	normal.bind();
	glActiveTexture(GL_TEXTURE0 + 7);
	shader.UpLoadUniformInt("metallicMap", metallic.texture_id);
	metallic.bind();
	glActiveTexture(GL_TEXTURE0 + 8);
	shader.UpLoadUniformInt("roughnessMap", roughness.texture_id);
	roughness.bind();
	glActiveTexture(GL_TEXTURE0 + 9);
	shader.UpLoadUniformInt("aoMap", ao.texture_id);
	ao.bind();

	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
	camera.SetModel(modelMatrix);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Bind();
		shader.UpLoadUniformMat4("MVP", camera.GetMVP());
		shader.UpLoadUniformMat4("model", modelMatrix);
		//shader.UpLoadUniformFloat3("lightPos", light.GetPos());
		shader.UpLoadUniformFloat3("lightPositions", light.GetPos());//sample
		//shader.UpLoadUniformFloat3("viewPos", camera.GetPosition());
		shader.UpLoadUniformFloat3("camPos", camera.GetPosition());//sample
		shader.UpLoadUniformFloat3("lightColors", glm::vec3(255,255,255));
		model.DrawPBR(shader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}