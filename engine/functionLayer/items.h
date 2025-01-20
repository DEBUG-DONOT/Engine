#pragma once
#include"../ResourceLayer/shader.h"
#include<glm/glm.hpp>
class Sphere
{
	//一个球的类
	//实际上只需要是单位球，因为位置和大小可以通过model矩阵调节
	Sphere();




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