#include<iostream>
#include<functional>
#include"./functionLayer/FunctionLayer.h"
#include"./ResourceLayer/ResourceLayer.h"
#include<glm/gtc/matrix_transform.hpp>
#include"./ResourceLayer/SingleTexture.h"
#include <fstream>
#include <cmath>
#include"./functionLayer/items.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
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

void drawTriangle()
{
	Initialization initializer;
	initializer.GLFWInitialization();//默认开启深度测试
	glDisable(GL_DEPTH_TEST);
	auto* window = initializer.window;
	Triangle tri;
	VertexShader t_vert("./shaderLib/triangle.vert");
	FragmentShader t_frag("./shaderLib/triangle.frag");
	Shader t_shader(t_vert, t_frag);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		t_shader.Bind();
		tri.Draw(t_shader);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

void DrawBox()
{
	Initialization initializer;
	initializer.GLFWInitialization();//默认开启深度测试
	auto* window = initializer.window;
	glEnable(GL_DEPTH_TEST);
	Box box;
	VertexShader vert("./shaderLib/box.vert");
	FragmentShader frag("./shaderLib/box.frag");
	Shader shader(vert, frag);
	Camera camera(glm::vec3(0.0f, 0, 0), glm::vec3(0.0, 0, -1), glm::vec3(0, 1, 0));
	//camera.SetModel(glm::mat4(glm::scale(glm::mat4(1.0), glm::vec3(5.0, 5.0, 5.0))));
	Camera::SetMainCamera(&camera);
	glfwSetKeyCallback(window, Camera::CameraKeyDetection);//接收一个函数指针
	glfwSetCursorPosCallback(window, Camera::CameraMouseDetection);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear( GL_DEPTH_BUFFER_BIT);
		shader.Bind();
		glm::mat4 projection =camera.GetProjection();
		glm::mat4 view = camera.GetView();
		shader.UpLoadUniformMat4("model", glm::mat4(1.0));
		shader.UpLoadUniformMat4("view", view);
		shader.UpLoadUniformMat4("projection", projection);
		box.Draw(shader);
		//glBindVertexArray(cubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

void DrawSphere()
{
	Initialization initializer;
	initializer.GLFWInitialization();
	auto* window = initializer.window;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

	VertexShader vert("./shaderLib/BoxPBR.vert");
	FragmentShader frag("./shaderLib/BoxPBR.frag");
	//FragmentShader frag("./shaderLib/sampleBox.frag");
	Shader shader(vert, frag);
	Model model;
	Light light(glm::vec3(-10, 10, 0), glm::vec3(0, 0, 0), glm::vec3(300, 300, 300));//light dir color
	Camera camera(glm::vec3(0.0f, 40, 25), glm::vec3(0.0, 5, -1), glm::vec3(0, 1, 0));
	Camera::SetMainCamera(&camera);
	//glfwSetKeyCallback(window, Camera::CameraKeyDetection);//接收一个函数指针
	//glfwSetCursorPosCallback(window, Camera::CameraMouseDetection);
	Sphere sp;
	glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.3, 0.3, 0.3));
	camera.SetModel(modelMatrix);
	float roughness = 0, metallic = 0;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow(); // Show demo window! :)
		ImGui::SliderFloat("roughtness", &roughness, 0, 1);
		ImGui::SliderFloat("metallic", &metallic, 0, 1);
		// Rendering
		// (Your code clears your framebuffer, renders your other stuff etc.)
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Bind();
		shader.UpLoadUniformMat4("MVP", camera.GetMVP());
		shader.UpLoadUniformMat4("model", modelMatrix);
		shader.UpLoadUniformFloat3("lightColor", glm::vec3(300, 300, 300));
		shader.UpLoadUniformFloat3("lightPosition", light.GetPos());
		shader.UpLoadUniformFloat3("eyePosition", camera.GetPosition());
		shader.UpLoadUniformFloat("metallic", metallic);
		shader.UpLoadUniformFloat("roughness", roughness);
		shader.UpLoadUniformFloat3("albedo", glm::vec3(255, 0, 0));
		sp.DrawPBR(shader);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// (Your code calls glfwSwapBuffers() etc.)
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

int main()
{
	DrawSphere();
	//ImGui::ShowDemoWindow();
}