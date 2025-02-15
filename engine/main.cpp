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
	//prepare imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
	////////////////////////////////////////////
	Quad quad;
	Sphere sp,sp1;
	Model model;
	model.loadModel("./Resource/shenhe/shenhe.pmx");
	Shader quadShader("./shaderLib/quadTex.vert", "./shaderLib/quadTex.frag");
	Shader simpleShader("./shaderLib/simple.vert", "./shaderLib/simple.frag");
	
	Light light(glm::vec3(2, 12, 40), glm::vec3(0, 0, -10), glm::vec3(100, 150, 200));//light dir color
	float near_plane = 0.1f, far_plane = 100.0f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(light.GetPos(), light.GetDirection(), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;


	Camera camera(glm::vec3(5.0f, 5, 50), glm::vec3(0.0, 0, -1), glm::vec3(0, 1, 0));
	Camera::SetMainCamera(&camera);
	
	glfwSetKeyCallback(window, Camera::CameraKeyDetection);//接收一个函数指针
	//glfwSetCursorPosCallback(window, Camera::CameraMouseDetection);
	
	Shader shadowShader("./shaderLib/shadow.vert", "./shaderLib/shadow.frag");
	Shader shader1("./shaderLib/BoxPBR.vert", "./shaderLib/BoxPBR.frag");
	Shadow shadow(GeneralData::width,GeneralData::height,light);
	
	//render loop
	quadShader.UpLoadUniformInt("shadowMap", shadow.shadowMapTextureID);
	shader1.Bind();
	shader1.UpLoadUniformInt("shadowMap", shadow.shadowMapTextureID);
	shader1.UnBind();


	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, -2, 30));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));

	glm::mat4 modelMatrix2 = glm::scale(glm::mat4(1.0), glm::vec3(5, 0.2, 3));
	modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(3, -3, -3));
	float metallic = 0.0f, roughness = 0.0f;
	bool showQuad = false;


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		//shadow pass
		{
			glEnable(GL_DEPTH_TEST);
			//shadow pass
			shadowShader.Bind();
			glViewport(0, 0, GeneralData::width, GeneralData::height);
			glBindFramebuffer(GL_FRAMEBUFFER, shadow.shadowMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			shadowShader.UpLoadUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
			shadowShader.UpLoadUniformMat4("model", modelMatrix);
			//sp.DrawPBR(shadowShader);
			model.DrawPBR(shadowShader);
			shadowShader.UpLoadUniformMat4("model", modelMatrix2);
			sp1.DrawPBR(shadowShader);
			
		}
		
		//render pass
		{
			//use default frame buffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::Checkbox("quad", &showQuad);
			glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//render
			camera.SetModel(modelMatrix);
			shader1.Bind();
			shader1.UpLoadUniformMat4("MVP", camera.GetMVP());
			shader1.UpLoadUniformMat4("model", modelMatrix);
			shader1.UpLoadUniformFloat3("lightColor", glm::vec3(200, 200, 200));
			shader1.UpLoadUniformFloat3("lightPosition", light.GetPos());
			shader1.UpLoadUniformFloat3("eyePosition", camera.GetPosition());
			shader1.UpLoadUniformFloat("metallic", metallic);
			shader1.UpLoadUniformFloat("roughness", roughness);
			shader1.UpLoadUniformFloat3("albedo", glm::vec3(100, 50, 5));
			shader1.UpLoadUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
			glActiveTexture(GL_TEXTURE0 + shadow.shadowMapTextureID);
			glBindTexture(GL_TEXTURE_2D, shadow.shadowMapTextureID);
			//sp.DrawPBR(shader1);
			model.DrawPBR(shader1);
			camera.SetModel(modelMatrix2);
			shader1.UpLoadUniformFloat3("albedo", glm::vec3(50, 50, 50));
			shader1.UpLoadUniformMat4("MVP", camera.GetMVP());
			shader1.UpLoadUniformMat4("model", modelMatrix2);
			sp1.DrawPBR(shader1);
			
		}

		//Draw quad
		if(showQuad)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			glClearColor(1.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			quadShader.Bind();
			quadShader.UpLoadUniformFloat("near_plane", GeneralData::near);
			quadShader.UpLoadUniformFloat("far_plane", GeneralData::far);
			quad.Draw(quadShader);
		}
		//others
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

float srgbToLinear(float c) 
{
    return c <= 0.04045 ? c / 12.92 : pow((c + 0.055) / 1.055, 2.4);
}
void DeferedShading()
{
	Initialization initializer;
	initializer.GLFWInitialization(1920, 1080);//默认开启深度测试
	auto* window = initializer.window;
	// 禁用 OpenGL 自动 sRGB 转换
	glDisable(GL_FRAMEBUFFER_SRGB);
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
	//加载模型
	Quad quad;//第二个pass用于渲染的quad
	Model model;model.loadModel("./Resource/shenhe/shenhe.pmx");
	Sphere sp;
	//模型modle matrix
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, -2, 30));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
	glm::mat4 modelMatrix2 = glm::scale(glm::mat4(1.0), glm::vec3(5, 0.2, 3));
	modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(3, -3, -3));
	//设置G-Buffer
	GLuint gBuffer,gPosition,gNormal,gAlbedoSpec,gDepth;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//position color buffer pos+roughness vec4
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GeneralData::width, GeneralData::height, 
		0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	//albedo color buffer albedo+metallic vec4
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GeneralData::width, GeneralData::height, 
		0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gAlbedoSpec, 0);

	//normal color buffer vec3
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GeneralData::width, GeneralData::height, 
		0, GL_RGB, GL_FLOAT, NULL);//HDR，存储线性空间的颜色，opengl不会进行自动的srgb转换
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gNormal, 0);

	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	//depth buffer
	glGenTextures(1, &gDepth);
	glBindTexture(GL_TEXTURE_2D, gDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GeneralData::width, GeneralData::height, 
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);

	//设置shader
	Shader gBufferShader("./shaderLib/Deferedfirst.vert", "./shaderLib/Deferedfirst.frag");
	Shader quadShader("./shaderLib/quadTex.vert","./shaderLib/quadTex.frag");	
	Shader quadDeferedPBR("./shaderLib/DeferedPBR.vert", "./shaderLib/DeferedPBR.frag");
	Shader shadowShader("./shaderLib/shadow.vert", "./shaderLib/shadow.frag");
	//设置shadow
	Shadow shadow(GeneralData::width,GeneralData::height,light);
	//设置纹理
	quadShader.Bind();
	quadShader.UpLoadUniformInt("texture1", GL_TEXTURE0);

	quadDeferedPBR.Bind();
	quadDeferedPBR.UpLoadUniformInt("texture1", 0);//sampler2D绑定到纹理单元
	quadDeferedPBR.UpLoadUniformInt("texture2", 1);
	quadDeferedPBR.UpLoadUniformInt("texture3", 2);
	quadDeferedPBR.UpLoadUniformInt("shadowMap", 3);
	quadDeferedPBR.UnBind();
	//render loop
	bool testQuad = false;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		//shadow pass
		{
			glEnable(GL_DEPTH_TEST);
			shadowShader.Bind();
			glBindFramebuffer(GL_FRAMEBUFFER, shadow.shadowMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			shadowShader.UpLoadUniformMat4("lightSpaceMatrix", lightSpaceMatrix);	
			shadowShader.UpLoadUniformMat4("model", modelMatrix);	
			model.DrawPBR(shadowShader);
			shadowShader.UpLoadUniformMat4("model", modelMatrix2);
			sp.DrawPBR(shadowShader);
		}
		{
			//G-Buffer pass
			glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
			//glViewport(0, 0, GeneralData::width, GeneralData::height);
			//GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			//glDrawBuffers(3, attachments);
		
			glEnable(GL_DEPTH_TEST);
			glClearColor(srgbToLinear(0.2f), srgbToLinear(0.3f), srgbToLinear(0.5f), 1.0);
			//glClearColor(0.2f, 0.3f, 0.5f, 1.0);
			//glClearColor(pow(0.2f,1.0/2.2), pow(0.3f,1.0/2.2), pow(0.5f,1.0/2.2), 1.0);
			//glClearColor(51, 76, 127, 255);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			gBufferShader.Bind();
			camera.SetModel(modelMatrix);
			gBufferShader.UpLoadUniformMat4("MVP",camera.GetMVP());
			gBufferShader.UpLoadUniformMat4("model", modelMatrix);
			gBufferShader.UpLoadUniformFloat("metallic", 0.0);
			gBufferShader.UpLoadUniformFloat("roughness", 0.0);
			gBufferShader.UpLoadUniformFloat3("albedo", glm::vec3(100, 50, 5));
			model.DrawPBR(gBufferShader);

			camera.SetModel(modelMatrix2);
			gBufferShader.UpLoadUniformMat4("model", modelMatrix2);
			gBufferShader.UpLoadUniformMat4("MVP",camera.GetMVP());
			gBufferShader.UpLoadUniformFloat3("albedo", glm::vec3(50, 50, 50));
			sp.DrawPBR(gBufferShader);
		}
		//draw quad
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			//glClearColor(0.2f, 0.3f, 0.5f, 1.0);
			//glClearColor(51, 76.5, 127.5, 1.0);
			//glClearColor(pow(0.2f,1.0/2.2), pow(0.3f,1.0/2.2), pow(0.5f,1.0/2.2), 1.0);
			glClearColor(srgbToLinear(0.2f), srgbToLinear(0.3f), srgbToLinear(0.5f), 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glViewport(0, 0, GeneralData::width, GeneralData::height);
			if(testQuad)
			{
				quadShader.Bind();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gNormal);
				quad.Draw(quadShader);
			}
			else
			{
				quadDeferedPBR.Bind();
				glActiveTexture(GL_TEXTURE0);//在绑定纹理之前先激活纹理单元 纹理单位就是GL_TEXTUREx这样的
				glBindTexture(GL_TEXTURE_2D, gPosition);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, gNormal);
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D,shadow.shadowMapTextureID);
				quadDeferedPBR.UpLoadUniformFloat3("lightColor", glm::vec3(200, 200, 200));
				quadDeferedPBR.UpLoadUniformFloat3("lightPosition", light.GetPos());
				quadDeferedPBR.UpLoadUniformFloat3("eyePosition", camera.GetPosition());
				quadDeferedPBR.UpLoadUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
				quad.Draw(quadDeferedPBR);
			}

		}
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

int main()
{
	//DeferedShading();
	ShadowPass();
	//drawTriangle();
}



