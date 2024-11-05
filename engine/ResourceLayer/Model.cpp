#include "Model.h"
#include<iostream>
#include<cassert>
void Model::LoadModel(const std::string path)
{
	Assimp::Importer importer;
	//Ĭ������ϵ
	auto scene = importer.ReadFile(path, aiProcess_CalcTangentSpace |
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	assert(scene != nullptr);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Model::LoadModel find a nullptr scene" << std::endl;
		return;
	}
	//��¼λ��
	this->directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::Draw()
{
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	//����ڵ��mesh
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* currMesh = scene->mMeshes[node->mMeshes[i]];
		this->_meshes.push_back(ProcessMesh(currMesh, scene));
	}
	//�ݹ�Ĵ����ӽڵ�
	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i],scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* m, const aiScene* scene)
{
	//���ݵ�ǰmesh����һ��mesh
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
			//warn: ʵ��������һ���ܴ�����飬����ֻ�����˵�һ��
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





