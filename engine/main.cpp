#include<iostream>
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
	Camera camera(glm::vec3(0.0f, 10, 100), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

	model.loadModel("./Resource/shenhe/shenhe.pmx");
	while (!glfwWindowShouldClose(window))
	{
		//fixed time physical loop
		//render loop	
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Bind();
		shader.UpLoadUniformMat4("MVP", camera.GetMVP());
		shader.UpLoadUniformMat4("model", glm::mat4(1.0));
		shader.UpLoadUniformFloat3("lightPos", glm::vec3(0, 10, 30));
		shader.UpLoadUniformFloat3("viewPos", camera.GetPosition());
		model.Draw(shader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}