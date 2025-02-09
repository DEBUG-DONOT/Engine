#pragma once
#include<glm/glm.hpp>
#include"../ResourceLayer/shader.h"
#include<memory>
#include<functional>
#include"./Light.h"
#include"../GeneralData.h"
class Shadow
{
	//输入光源位置，生成一个光源角度的z-buffer的texture
public:
	Shadow(unsigned int width,unsigned int height,Light& _light);
	//void DrawPrepare(Shader& shadowShader);
	//std::shared_ptr<Shader> shadowShader;
	//static Shader shadowShader;
	unsigned int width, height;
	unsigned int shadowMapTextureID,shadowMapFBO;
private:
	Light& light;
};