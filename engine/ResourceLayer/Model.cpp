#include "Model.h"
#include<iostream>
#include<cassert>
void runtime::Model::LoadModel(const std::string path)
{
	Assimp::Importer importer;
	//Ĭ������ϵ
	auto scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	assert(scene != nullptr);
	if (scene == nullptr)
	{
		std::cerr << "Model::LoadModel find a nullptr scene" << std::endl;
	}
	//�������������ȡ����
	//node


	//mesh

}
