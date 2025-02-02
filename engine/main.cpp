#include<iostream>
#include<functional>
#include"./functionLayer/FunctionLayer.h"
#include"./ResourceLayer/ResourceLayer.h"
#include<glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <cmath>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include"GeneralData.h"
#include"glm/gtc/matrix_transform.hpp"

void gun()
{
	Initialization initializer;
	initializer.GLFWInitialization(1920,1080);
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
	SingleTexure normal("./Resource/pbr/Textures/Cerberus_N.tga");
	SingleTexure metallic("./Resource/pbr/Textures/Cerberus_M.tga");
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
	initializer.GLFWInitialization(1920,1080);//默认开启深度测试
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
	initializer.GLFWInitialization(1920,1080);//默认开启深度测试
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
	initializer.GLFWInitialization(1920,1080);
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
	//FragmentShader frag("./shaderLib/BoxPBR.frag");
	FragmentShader frag("./shaderLib/BoxPBRtex.frag");
	//FragmentShader frag("./shaderLib/sampleBoxPBRtex.frag");
	//FragmentShader frag("./shaderLib/sampleBox.frag");
	Shader shader(vert, frag);
	Model model;
	Light light(glm::vec3(15, 15, 0), glm::vec3(0, 0, 0), glm::vec3(150, 150, 150));//light dir color
	Camera camera(glm::vec3(0.0f, 0, 10), glm::vec3(0.0, 0, -1), glm::vec3(0, 1, 0));
	Camera::SetMainCamera(&camera);
	glfwSetKeyCallback(window, Camera::CameraKeyDetection);//接收一个函数指针
	glfwSetCursorPosCallback(window, Camera::CameraMouseDetection);
	Sphere sp;
	glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.3, 0.3, 0.3));
	camera.SetModel(modelMatrix);
	float roughness = 0, metallic = 0;
	SingleTexure albedo_map("./Resource/sphereTexture/rustediron2_basecolor.png");
	SingleTexure normal_map("./Resource/sphereTexture/rustediron2_normal.png");
	SingleTexure metallic_map("./Resource/sphereTexture/rustediron2_metallic.png");
	SingleTexure roughness_map("./Resource/sphereTexture/rustediron2_roughness.png");
	shader.Bind();
	
	shader.UpLoadUniformInt("albedoMap", albedo_map.texture_id);//告诉OpenGL每个着色器采样器属于哪个纹理单元
	shader.UpLoadUniformInt("normalMap", normal_map.texture_id);
	shader.UpLoadUniformInt("metallicMap", metallic_map.texture_id);
	shader.UpLoadUniformInt("roughnessMap",roughness_map.texture_id);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow(); // Show demo window! :)
		//ImGui::SliderFloat("roughtness", &roughness, 0, 1);
		//ImGui::SliderFloat("metallic", &metallic, 0, 1);
		// Rendering
		// (Your code clears your framebuffer, renders your other stuff etc.)
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Bind();
		shader.UpLoadUniformMat4("MVP", camera.GetMVP());
		shader.UpLoadUniformMat4("model", modelMatrix);
		shader.UpLoadUniformFloat3("lightColor", glm::vec3(300, 300, 300));
		shader.UpLoadUniformFloat3("lightPosition", light.GetPos());
		shader.UpLoadUniformFloat3("eyePosition", camera.GetPosition());
		//shader.UpLoadUniformFloat("metallic", metallic);
		//shader.UpLoadUniformFloat("roughness", roughness);
		//shader.UpLoadUniformFloat3("albedo", glm::vec3(100, 0, 0));
		
		//using texture
		glActiveTexture(GL_TEXTURE0+albedo_map.texture_id);
		albedo_map.bind();
		glActiveTexture(GL_TEXTURE0 + normal_map.texture_id);
		normal_map.bind();
		glActiveTexture(GL_TEXTURE0 + metallic_map.texture_id);
		metallic_map.bind();
		glActiveTexture(GL_TEXTURE0 + roughness_map.texture_id);
		roughness_map.bind();
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

void DrawSphereShadow()
{
	Initialization initializer;
	initializer.GLFWInitialization(1920,1080);
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
	//shader
	VertexShader vert("./shaderLib/BoxPBR.vert");
	FragmentShader frag("./shaderLib/BoxPBR.frag");
	//Shader shader1(vert, frag);
	Shader shader1("./shaderLib/BoxPBR.vert", "./shaderLib/BoxPBR.frag");
	Shader shader2(vert, frag);
	Shader shadowShader1("./shaderLib/shadow.vert", "./shaderLib/shadow.frag");
	//light& camera
	Light light(glm::vec3(-15, 15, 0), glm::vec3(0, 0, 0), glm::vec3(200, 150, 150));//light dir color
	Camera camera(glm::vec3(0.0f, 0, 25), glm::vec3(0.0, 0, -1), glm::vec3(0, 1, 0));
	Camera::SetMainCamera(&camera);
	glfwSetKeyCallback(window, Camera::CameraKeyDetection);//接收一个函数指针
	//glfwSetCursorPosCallback(window, Camera::CameraMouseDetection);
	//model
	Model model;
	Sphere sp1;
	Sphere sp2;
	Box box;
	//Shadow shadow1(GeneralData::width,GeneralData::height,light);
	glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.3, 0.3, 0.3));
	glm::mat4 modelMatrix2 = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
	 modelMatrix2=glm::translate(glm::mat4(1.0),glm::vec3(5,-15,0));
	float roughness = 0, metallic = 0;
	glm::mat4 lightProjection = glm::ortho(GeneralData::left, GeneralData::right, GeneralData::bottom,
		GeneralData::top, GeneralData::near, GeneralData::far);
	glm::mat4 lightView = glm::lookAt(light.GetPos(),
		light.GetPos() + light.GetDirection(), glm::vec3(0, 1, 0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SliderFloat("roughtness", &roughness, 0, 1);
		ImGui::SliderFloat("metallic", &metallic, 0, 1);
		// Rendering
		// shadow map
		//shadow1.DrawPrepare(shadowShader1);

		shadowShader1.Bind();
		shadowShader1.UpLoadUniformMat4("MVP", lightSpaceMatrix);
		//shadowShader1.UpLoadUniformMat4("Model", glm::mat4(1.0));
		//sp1.DrawPBR(shadowShader1);
		box.Draw(shadowShader1);
		shadowShader1.UnBind();

		//first sphere
		//{
		//	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//	camera.SetModel(modelMatrix);
		//	shader1.Bind();
		//	shader1.UpLoadUniformMat4("MVP", camera.GetMVP());
		//	shader1.UpLoadUniformMat4("model", modelMatrix);
		//	shader1.UpLoadUniformFloat3("lightColor", glm::vec3(300, 300, 300));
		//	shader1.UpLoadUniformFloat3("lightPosition", light.GetPos());
		//	shader1.UpLoadUniformFloat3("eyePosition", camera.GetPosition());
		//	shader1.UpLoadUniformFloat("metallic", metallic);
		//	shader1.UpLoadUniformFloat("roughness", roughness);
		//	shader1.UpLoadUniformFloat3("albedo", glm::vec3(100, 0, 0));
		//	sp1.DrawPBR(shader1);
		//	//second sphere
		//	camera.SetModel(modelMatrix2);
		//	shader2.Bind();
		//	shader2.UpLoadUniformMat4("MVP", camera.GetMVP());
		//	shader2.UpLoadUniformMat4("model", modelMatrix2);
		//	shader2.UpLoadUniformFloat3("lightColor", glm::vec3(300, 300, 300));
		//	shader2.UpLoadUniformFloat3("lightPosition", light.GetPos());
		//	shader2.UpLoadUniformFloat3("eyePosition", camera.GetPosition());
		//	shader2.UpLoadUniformFloat("metallic", metallic);
		//	shader2.UpLoadUniformFloat("roughness", roughness);
		//	shader2.UpLoadUniformFloat3("albedo", glm::vec3(50, 100, 50));
		//	sp2.DrawPBR(shader2);
		//}

		//ImGUI
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

void ShadowPass()
{
	Initialization initializer;
	initializer.GLFWInitialization(1920, 1080);//默认开启深度测试
	//渲染一个和屏幕一样的quad
	//测试一下普通的shadow shader的作用
	auto* window = initializer.window;
	Triangle tri;
	Quad quad;
	Sphere sp,sp1;
	Shader quadShader("./shaderLib/quadTex.vert", "./shaderLib/quadTex.frag");
	Shader simpleShader("./shaderLib/simple.vert", "./shaderLib/simple.frag");
	Light light(glm::vec3(2, 1, 50), glm::vec3(0, 0, -1), glm::vec3(100, 150, 200));//light dir color
	Camera camera(glm::vec3(0.0f, 0, 50), glm::vec3(0.0, 0, -1), glm::vec3(0, 1, 0));
	Camera::SetMainCamera(&camera);
	glfwSetKeyCallback(window, Camera::CameraKeyDetection);//接收一个函数指针
	glfwSetCursorPosCallback(window, Camera::CameraMouseDetection);
	Shader shadowShader("./shaderLib/shadow.vert", "./shaderLib/shadow.frag");
	Shader shader1("./shaderLib/BoxPBR.vert", "./shaderLib/BoxPBR.frag");
	//create FBO& texture
	{

	}
	unsigned int shadowMapTextureID, shadowMapFBO;
	glGenFramebuffers(1, &shadowMapFBO);//生成FBO
	//生成depth buffer attachment
	glGenTextures(1, &shadowMapTextureID);
	glBindTexture(GL_TEXTURE_2D, shadowMapTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		GeneralData::width, GeneralData::height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//把生成的纹理作为frambuffer的depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTextureID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);//释放
	//render loop
	quadShader.UpLoadUniformInt("shadowMap", shadowMapTextureID);

	glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.3, 0.3, 0.3));
	glm::mat4 modelMatrix2 = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
	modelMatrix2 = glm::translate(glm::mat4(1.0), glm::vec3(3, -10, 0));
	float metallic = 0.0f, roughness = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		//shadow pass
		{
			glEnable(GL_DEPTH_TEST);
			//shadow pass
			shadowShader.Bind();
			glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			////set matrix
			glm::mat4 lightProjection = glm::ortho(GeneralData::left, GeneralData::right, GeneralData::bottom,
			 GeneralData::top, GeneralData::near, GeneralData::far);
			glm::mat4 lightView = glm::lookAt(light.GetPos(), 
			light.GetPos() + light.GetDirection(), glm::vec3(0, 1, 0));
			glm::mat4 lightSpaceMatrix = lightProjection * lightView;
			shadowShader.UpLoadUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
			shadowShader.UpLoadUniformMat4("model", modelMatrix);
			sp.DrawPBR(shadowShader);
			shadowShader.UpLoadUniformMat4("model", modelMatrix2);
			sp1.DrawPBR(shadowShader);
		}
		
		//render pass
		{
			//use default frame buffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//render
			camera.SetModel(modelMatrix);
			shader1.Bind();
			shader1.UpLoadUniformMat4("MVP", camera.GetMVP());
			shader1.UpLoadUniformMat4("model", modelMatrix);
			shader1.UpLoadUniformFloat3("lightColor", glm::vec3(300, 300, 300));
			shader1.UpLoadUniformFloat3("lightPosition", light.GetPos());
			shader1.UpLoadUniformFloat3("eyePosition", camera.GetPosition());
			shader1.UpLoadUniformFloat("metallic", metallic);
			shader1.UpLoadUniformFloat("roughness", roughness);
			shader1.UpLoadUniformFloat3("albedo", glm::vec3(100, 50, 5));
			sp.DrawPBR(shader1);
			camera.SetModel(modelMatrix2);
			shader1.UpLoadUniformMat4("MVP", camera.GetMVP());
			shader1.UpLoadUniformMat4("model", modelMatrix2);
			sp1.DrawPBR(shader1);
		}

		//Draw quad
		{
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//glDisable(GL_DEPTH_TEST);
			//glClearColor(1.0, 0.0, 0.0, 1.0);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//quadShader.Bind();
			//quadShader.UpLoadUniformFloat("near_plane", GeneralData::near);
			//quadShader.UpLoadUniformFloat("far_plane", GeneralData::far);
			//quad.Draw(quadShader);
		}
		//others
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

int main()
{
	ShadowPass();
	//drawTriangle();
}



