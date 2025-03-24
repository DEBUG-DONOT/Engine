#include"shader.h"
#include<vector>
#include<iostream>	
#include<fstream>
#include<sstream>
#include<glm/gtc/type_ptr.hpp>
Shader::Shader(VertexShader vs, FragmentShader fs)
{
	// Read our shaders into the appropriate buffers
	std::string vertexSource = vs.GetCode(); // Get source code for vertex shader.
	std::string fragmentSource = fs.GetCode(); // Get source code for fragment shader.
	vsCode = vertexSource; fsCode = fragmentSource;
	// Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = (const GLchar*)vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, 0);
	// Compile the vertex shader
	glCompileShader(vertexShader);
	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
		// We don't need the shader anymore.
		glDeleteShader(vertexShader);
		// Use the infoLog as you see fit.
		//std::cout << "vertex shader compile failed!" << std::endl;
		std::cout << infoLog.data() << std::endl;
		throw "vertex Shader Error";
		// In this simple program, we'll just leave
		return;
	}
//////////////////////////////////////////////////////////////////////////////////////////
	// Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = (const GLchar*)fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);
	// Compile the fragment shader
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;  
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
		// We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);
		// Use the infoLog as you see fit.
		std::cout << "fragment shader compile failed!" << std::endl;
		std::cout << infoLog.data() << std::endl;
		throw "Fragment Shader Error";
		// In this simple program, we'll just leave
		return;
	}
	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	this->mShaderID = glCreateProgram();
	// Attach our shaders to our program
	glAttachShader(this->mShaderID, vertexShader);
	glAttachShader(this->mShaderID, fragmentShader);
	// Link our program
	glLinkProgram(this->mShaderID);
	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(this->mShaderID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(this->mShaderID, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(this->mShaderID, maxLength, &maxLength, &infoLog[0]);
		// We don't need the program anymore.
		glDeleteProgram(this->mShaderID);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		// Use the infoLog as you see fit.
		std::cout << "shader link failed!" << std::endl;
		std::cout << infoLog.data() << std::endl;
		std::cout << "end info" << std::endl;
		throw "Shader Link Error";
		// In this simple program, we'll just leave
		return;
	}
	std::cout<<"shader ID is"<<this->mShaderID<<std::endl;
	// Always detach shaders after a successful link.
	glDetachShader(this->mShaderID, vertexShader);
	glDetachShader(this->mShaderID, fragmentShader);
}

Shader::Shader(std::string vs, std::string fs)
	:Shader(VertexShader(vs),FragmentShader(fs))
{
	
}
 
Shader::~Shader()
{
	glDeleteProgram(this->mShaderID);
}

void Shader::Bind()
{
	glUseProgram(this->mShaderID);
}
void Shader::UnBind()
{
	glUseProgram(0);
}

BasicShader::BasicShader( std::string path):path(path)
{
	ReadCodeFromFile(path);
}

void BasicShader::ReadCodeFromFile(std::string path)
{
	std::string SrcCode;
	std::ifstream fileReader;
	fileReader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//std::ofstream out("wk.txt");
		fileReader.open(path.c_str());
		std::cout << path << "open sucess" << std::endl;
		std::stringstream codeStream;
		codeStream << fileReader.rdbuf();
		fileReader.close();
		SrcCode = codeStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << path << "fail to transfer the glsl src file,fail to read file" << std::endl;
	}
	 code=SrcCode;
}

/*
* 很多时候upLoad失败是因为根本没有用到对应的值，opengl在运行时就不会生成
*/

void Shader::UpLoadUniformMat4(const std::string& name, const glm::mat4& m_matrix)
{

	int location = glGetUniformLocation(mShaderID, name.c_str());
	if (location == -1)
	{
		//assert(location != -1);
		std::cout << "up load mat4 fail!" << name << std::endl;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_matrix));
}

void Shader::UpLoadUniformMat3(const std::string& name, const glm::mat3& m_matrix)
{

	int location = glGetUniformLocation(mShaderID, name.c_str());
	if (location == -1)
	{
		std::cout << "up load mat3 fail!" << name << std::endl;
	}
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(m_matrix));
}

void Shader::UpLoadUniformInt(const std::string& name, const int num)
{
	int location = glGetUniformLocation(mShaderID, name.c_str());
	if (location == -1)
	{
		if (errorName.find(name) == errorName.end())
		{
			std::cout <<this->name<< ": up load int fail!" << name << std::endl;
			errorName.insert(name);
		}
	}
	glUniform1i(location, num);
}

void Shader::UpLoadUniformFloat(const std::string& name, const float num)
{
	int location = glGetUniformLocation(mShaderID, name.c_str());
	if (location == -1)
	{
		std::cout << "up load float fail!" << name << std::endl;
	}
	glUniform1f(location, num);
}


void Shader::UPLoadUniformFloat2(const std::string& name, const glm::vec2& m_vec2)
{
	int location = glGetUniformLocation(mShaderID, name.c_str());
	if (location == -1)
	{
		std::cout << "up load float2 fail!" << name << std::endl;
	}
	glUniform2f(location, m_vec2.x, m_vec2.y);
}

void Shader::UpLoadUniformFloat3(const std::string& name, const glm::vec3& m_vec3)
{
	int location = glGetUniformLocation(mShaderID, name.c_str());
	if (location == -1)
	{
		std::cout << "up load float3 fail!" << name << std::endl;
	}
	glUniform3f(location, m_vec3.x, m_vec3.y, m_vec3.z);
}

void Shader::UpLoadUniformFloatArray (const std::string& name, const float* m_float, int num)
{
	int location = glGetUniformLocation(mShaderID, name.c_str());
	if (location == -1)
	{
		std::cout << "up load float array fail!" << name << std::endl;
	}
	glUniform1fv(location, num, m_float);
}

void Shader:: UpLoadUniformFloat3Array(const std::string& name, const glm::vec3* m_vec3, int num)
{
	int location = glGetUniformLocation(mShaderID, name.c_str());
	if (location == -1)
	{
		std::cout << "up load float3 array fail!" << name << std::endl;
	}
	glUniform3fv(location, num, glm::value_ptr(m_vec3[0]));
}


void Shader::UpLoadUniformFloat4(const std::string& name, const glm::vec4& m_vec3)
{
	int location = glGetUniformLocation(mShaderID, name.c_str());
	if (location == -1)
	{
		std::cout << "up load float4 fail!" << name << std::endl;
	}
	glUniform4f(location, m_vec3.x, m_vec3.y, m_vec3.z, m_vec3.w);
}

void Shader::PrintAllSourceCode()
{
	std::cout << "------------vertex source code----------" << std::endl;
	std::cout << vsCode << std::endl;
	std::cout << "------------fragment source code----------" << std::endl;
	std::cout << fsCode << std::endl;
}

