#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include"shader.h"
#define MAX_BONE_INFLUENCE 4

using namespace std;

struct Vertex {
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

struct MaterialPBR
{
    //所有的material 用唯一的id标识
    unsigned int id;
    string path;
    bool operator==(const MaterialPBR& other) const 
    {
        return id == other.id;
    }
    //pbr部分
    float Ns;//反射参数 控制镜面高光的锐利程度，值越大高光越集中。
    float roughness;//粗糙度，0-1
    float Ni;//折射率 1.0表示无折射，透明材质
    int ill_method;//光照模型
    //1 环境光+漫反射
    //2环境光+漫反射+高光
    //3 反射+光线追踪
    glm::vec3 Ka;//ambient color 材质在环境光下的颜色
    glm::vec3 Kd;//diffuse color 材质在直接光照下的颜色
    glm::vec3 Ks;//specular color 高光颜色 
    glm::vec3 Ke;//自发光颜色
};


struct Texture 
{
    unsigned int id;
    string type;
    string path;    // 我们储存纹理的路径用于与其它纹理进行比较
    bool operator==(const Texture& other) const 
    {
        return id == other.id && type == other.type && path == other.path;
    }
    //void LoadFromFile(std::string path);

};

class Mesh 
{
    public:
        /*  网格数据  */
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        MaterialPBR  material;
        unsigned int VAO;
        /*  函数  */
        //容器内的类必须有默认构造函数
        Mesh() = default;
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
             vector<Texture> textures, MaterialPBR material);
        
        void Draw(Shader& shader);
        void Draw(GLuint shader);
        void DrawPBR(Shader& shader);
    private:
        /*  渲染数据  */
        unsigned int VBO, EBO;
        /*  函数  */
        void setupMesh();
        void NsToRoughness();
};  

#endif