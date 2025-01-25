#include "Shadow.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"glm/gtc/matrix_transform.hpp"
Shadow::Shadow(unsigned int width, unsigned int height,  Light& _light)
:light(_light)
{
	glGenFramebuffers(1, &shadowMapFBO);//生成FBO
	//生成depth buffer attachment
	glGenTextures(1, &shadowMapTextureID);
	glBindTexture(GL_TEXTURE_2D, shadowMapTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
	//保存参数
	this->width = width;
	this->height = height;
}


void Shadow::DrawPrepare(Shader& shadowShader)
{
	//绘制shadow map
	//shadowShader.Bind();
	//glViewport(0, 0, width, height);
	//glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	//glClear(GL_DEPTH_BUFFER_BIT);
	////set matrix
	//glm::mat4 lightProjection = glm::ortho(GeneralData::left, GeneralData::right, GeneralData::bottom,
	// GeneralData::top, GeneralData::near, GeneralData::far);
	//glm::mat4 lightView = glm::lookAt(light.GetPos(), 
	//light.GetPos() + light.GetDirection(), glm::vec3(0, 1, 0));
	//glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	//shadowShader.UpLoadUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
}



