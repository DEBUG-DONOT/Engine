#ifndef MODEL_H
#define MODEL_H
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include<unordered_map>
#include<unordered_set>
#include"shader.h"
class Model
{
    public:
        /*  模型数据  */
        vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        vector<Mesh> meshes;//所有节点，包括子节点的mesh都放在这个vector中
        std::string directory;
        /*  函数   */
        Model(string path)
        {
            loadModel(path);
        }
        Model();
        virtual ~Model()=default;
        void Draw(Shader& shader);   
        void Draw(GLuint shader);
        void DrawPBR(Shader& shader);
        void loadModel(std::string path);
        void loadModel(std::string path,bool loadMaterial);
        void checkAllTypeTexture();
		void showAllLoadedTexture();
        void checkMesh();
    private:
        /*  函数   */
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        //void processMaterial_NoTexture(aiScene* scene, aiMaterial* mat, Mesh& mesh);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
            string typeName, vector<bool> *bRepeats = nullptr);
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, 
            string typeName,const aiScene* scene, vector<bool>* bRepeats);
        unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
        unsigned int TextureEembed(const aiTexture* aitex);
        //用来查看有多少种类的texture
        std::unordered_map<std::string,int> allTypeTexture;
};

class MyOBJLoader
{
public:
    MyOBJLoader();
    void loadModel(const std::string& filename);
    void loadMaterial(const std::string& filename);
    void draw(Shader& shader);
    void drawPBR(Shader& shader);
private:
    string directory;
    string obj_filename,mtl_filename;
    vector<Mesh> meshes;
    vector<MaterialPBR> materials;
};

#endif