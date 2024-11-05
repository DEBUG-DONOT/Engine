#pragma once
#include <glm/glm.hpp>
#include<vector>
#define MAX_BONE_INFLUENCE 4

/*
* 一个vertex是一个per vertex 的data的集合
*/
class Vertex
{
public:
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

/*
* Mesh收录了一个mesh上的所有vertex数据
*/

class Mesh
{
public:
	std::vector<Vertex> _vertexs;
	std::vector<unsigned int> _indexs;


};


class Texture
{


};