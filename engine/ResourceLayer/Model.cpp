#include "Model.h"
#include<iostream>
#include<cassert>
void Model::LoadModel(const std::string path)
{
	Assimp::Importer importer;
	//默认右手系
	auto scene = importer.ReadFile(path, aiProcess_CalcTangentSpace |
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	assert(scene != nullptr);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Model::LoadModel find a nullptr scene" << std::endl;
		return;
	}
	//记录位置
	this->directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::Draw()
{
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	//处理节点的mesh
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* currMesh = scene->mMeshes[node->mMeshes[i]];
		this->_meshes.push_back(ProcessMesh(currMesh, scene));
	}
	//递归的处理子节点
	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i],scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* m, const aiScene* scene)
{
	//根据当前mesh给出一个mesh
	glm::vec3 tempVec;
	Vertex vetx;
	for (int i = 0; i < m->mNumVertices; i++)
	{
		//process per vertex data
		//pos
		tempVec.x = m->mVertices[i].x;
		tempVec.y = m->mVertices[i].y;
		tempVec.z = m->mVertices[i].z;
		vetx.Position = tempVec;
		//normal
		if (m->HasNormals())
		{
			tempVec.x = m->mNormals[i].x;
			tempVec.y = m->mNormals[i].y;
			tempVec.z = m->mNormals[i].z;
			vetx.Normal = tempVec;
		}
		if (m->HasTextureCoords(0))
		{
			//warn: 实际上这是一个很大的数组，我们只考虑了第一个
			glm::vec2 texC;
			texC.x = m->mTextureCoords[0][i].x;
			texC.y = m->mTextureCoords[0][i].y;
			vetx.TexCoords = texC;
			if (m->mTangents)
			{
				tempVec.x = m->mTangents[i].x;
				tempVec.y = m->mTangents[i].y;
				tempVec.z = m->mTangents[i].z;
				vetx.Tangent = tempVec;
			}
			if (m->mBitangents)
			{
				tempVec.x = m->mBitangents[i].x;
				tempVec.y = m->mBitangents[i].y;
				tempVec.z = m->mBitangents[i].z;
				vetx.Bitangent = tempVec;
			}
		}
		else vetx.TexCoords = glm::vec2(0.0, 0.0);
	}
	//texture




	return Mesh();
}





