#pragma once
#include"../ResourceLayer/shader.h"
#include<glm/glm.hpp>
#include<memory>
#include"../ResourceLayer/Model.h"
class Sphere
{
	//一个球的类
	//实际上只需要是单位球，因为位置和大小可以通过model矩阵调节
public:
	Sphere();
	void DrawPBR(Shader& shader);
private:
	unsigned int vao, vbo;
	std::shared_ptr<Model> modle_ptr;
};

class Triangle
{
public:
	Triangle();
	void Draw(Shader& shader);
	unsigned int vbo,vao;
};

class Box
{
	//一个box的类
public:
	Box();
	void Draw(Shader& shader);
	unsigned int vbo,vao;
private:
	
};

class Circle
{
	//一个圆
public:
	Circle();
	~Circle();
};