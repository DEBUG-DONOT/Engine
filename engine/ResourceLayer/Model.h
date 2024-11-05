#pragma once
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"Mesh.h"

/*
* 使用assimp读取
*/
	
class Mesh;
	//读取并保存模型
class Model
{
public:
	std::vector<Mesh> _meshes;
	//texture

	void LoadModel(const std::string path);
	void Draw();

private:
	std::string directory;
	void ProcessNode(aiNode* node,const aiScene* scene);
	Mesh ProcessMesh(aiMesh* m,const aiScene* scene);

};




