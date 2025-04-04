#pragma once
#include<string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<unordered_set>
class BasicShader
{
public:
	BasicShader(std::string path);
	inline const std::string GetCode() { return code; }
private:
	void ReadCodeFromFile(std::string path);
	std::string path;
	std::string code;
};

class VertexShader:public BasicShader
{
public:
	//顶点着色器
	VertexShader(const std::string path)
		:BasicShader(path)
	{}
};

class FragmentShader:public BasicShader
{
	//片段着色器
public:
	FragmentShader(const std::string path)
		:BasicShader(path)
	{}
};

class Shader
{
public:
	Shader(VertexShader vs,FragmentShader fs);
	Shader(std::string vs,std::string fs);
	inline void setName(const std::string& _name){name=_name;}
	~Shader();
	inline GLuint GetID() { return mShaderID; }
	void Bind();
	void UnBind();
	void UpLoadUniformMat4(const std::string& name, const glm::mat4& m_matrix);
	void UpLoadUniformMat3(const std::string& name, const glm::mat3& m_matrix);
	void UpLoadUniformInt(const std::string& name, const int num);
	
	void UpLoadUniformFloat(const std::string& name, const float num);
	void UPLoadUniformFloat2(const std::string& name, const glm::vec2& m_vec2);
	void UpLoadUniformFloat3(const std::string& name, const glm::vec3& m_vec3);
	void UpLoadUniformFloat4(const std::string& name, const glm::vec4& m_vec3);

	void UpLoadUniformFloatArray(const std::string& name, const float* m_float, int num);
	void UpLoadUniformFloat3Array(const std::string& name, const glm::vec3* m_vec3, int num);
	void PrintAllSourceCode();
private:
	std::string name;
	std::unordered_set<std::string> errorName;
	GLuint mShaderID;
	std::string vsCode, fsCode;

};