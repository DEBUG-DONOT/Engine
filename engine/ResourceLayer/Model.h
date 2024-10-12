#pragma once
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

/*
* 使用assimp读取
*/
namespace runtime
{
	//读取并保存模型，控制资源的声明周期
	class Model
	{
	public:
		void LoadModel(const std::string path);


	private:
		


	};

	class Mesh
	{

	};

	class Texture
	{


	};
}