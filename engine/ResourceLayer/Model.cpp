#include "model.h"
#include <iostream>
#define STBI_WINDOWS_UTF8
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Model::Model()
    :directory("")
{
}

void Model::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader);
    }
}

void Model::Draw(GLuint shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader);
    }
}

void Model::DrawPBR(Shader& shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].DrawPBR(shader);  
    }
}

void Model::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | 
        aiProcess_FlipUVs | aiProcess_CalcTangentSpace);    

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    std::string temp= path.substr(0, path.find_last_of('/'));
    this->directory = temp;
    processNode(scene->mRootNode, scene);//load model调用process node

}

void Model::loadModel(string path,bool loadMaterial)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | 
        aiProcess_FlipUVs | aiProcess_CalcTangentSpace);    

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    std::string temp= path.substr(0, path.find_last_of('/'));
    this->directory = temp;
    processNode(scene->mRootNode, scene);//load model调用process node
    if(loadMaterial)
    {

    }
    
}

void Model::checkAllTypeTexture()
{
    std::cout << "*********check all type texture************" << std::endl;
    if(allTypeTexture.size() == 0)
        std::cout << "No texture loaded!" << std::endl;
    else
        std::cout << "All texture type loaded!" << std::endl;
    for (auto it = allTypeTexture.begin(); it != allTypeTexture.end(); it++)
        cout << (*it).first<<" "<<(*it).second << endl;
    std::cout << "***********All texture type showed********" << std::endl;
}

void Model::showAllLoadedTexture()
{
    std::cout << "------show all loaded texture----------" << std::endl;
    for (auto& curr : textures_loaded)
    {
        std::cout << curr.path << " " << curr.type << std::endl;
    }
    std::cout << "-----------------end---------------------" << std::endl;
}

void Model::checkMesh()
{
    if(this->meshes.empty())
        std::cout << "No mesh loaded!" << std::endl;
    else
        std::cout << "All mesh loaded!" <<"Mesh Num:"<< meshes.size()<< std::endl;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // 处理节点所有的网格（如果有的话）
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; //得到一个mesh对象
        meshes.push_back(processMesh(mesh, scene));         //对于节点树中的每一个节点，调用process mesh,process mesh会返回一个mesh对象
    }
    // 接下来对它的子节点重复这一过程
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);//递归的处理
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class 
        //so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            if (mesh->mTangents != NULL)
            {
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
            }
            if (mesh->mBitangents != NULL)
            {
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);        
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN
    //albedo : texture_albedoN

    // 1. diffuse maps
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse",scene,nullptr);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene, nullptr);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", scene, nullptr);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    bool heightNormalFlag = false;
    bool normalFlag = false;
    if (normalMaps.size() > 0)
        heightNormalFlag = true;
    std::vector<Texture> normalMaps1 = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal",  scene, nullptr);
    if (normalMaps1.size() > 0)
        normalFlag = true;
    if (normalMaps.size() > 0 && heightNormalFlag && normalFlag)
        std::cout << "[Model::processMesh] warning: aiTextureType_HEIGHT and aiTextureType_NORMALS both are recognize as normalMaps!" << std::endl;
    if (normalMaps.size() > 0 && normalMaps1 == normalMaps)
        std::cout << "[Model::processMesh] warning: aiTextureType_NORMALS has been loaded repeatly as " << normalMaps[0].type << std::endl;
    else
        textures.insert(textures.end(), normalMaps1.begin(), normalMaps1.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", scene, nullptr);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    // 5. PBR albedo
    vector<bool> bRepeats;
    std::vector<Texture> albedoMaps = loadMaterialTextures(material, aiTextureType_BASE_COLOR, "texture_albedo", scene,&bRepeats);
    for (size_t i = 0; i < albedoMaps.size(); i++)
    {
        if (bRepeats[i])//避免重复添加
        {
            std::cout << "[Model::processMesh] warning: aiTextureType_BASE_COLOR has been loaded repeatly as " << albedoMaps[i].type << std::endl;
            albedoMaps.erase(albedoMaps.begin() + i);
            bRepeats.erase(bRepeats.begin() + i);
            --i;
        }
    }
    textures.insert(textures.end(), albedoMaps.begin(), albedoMaps.end());
    // 6. PBR metallicMap
    std::vector<Texture> metallicMaps = loadMaterialTextures(material, aiTextureType_METALNESS, "texture_metallic", scene, nullptr);
    textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());
    // 7. PBR roughnessMap
    std::vector<Texture> roughnessMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness", scene,&bRepeats);
    for (size_t i = 0; i < roughnessMaps.size(); i++)
    {
        if (bRepeats[i])//避免重复添加
        {
            std::cout << "[Model::processMesh] warning: aiTextureType_DIFFUSE_ROUGHNESS has been loaded repeatly as " << roughnessMaps[i].type << std::endl;
            roughnessMaps.erase(roughnessMaps.begin() + i);
            bRepeats.erase(bRepeats.begin() + i);
            --i;
        }
    }
    textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
    // 8. PBR aoMap
    std::vector<Texture> aoMaps = loadMaterialTextures(material, aiTextureType_UNKNOWN, "texture_ao", scene,&bRepeats);
    for (size_t i = 0; i < aoMaps.size(); i++)
    {
        if (bRepeats[i])//避免重复添加
        {
            std::cout << "[Model::processMesh] warning: aiTextureType_UNKNOWN has been loaded repeatly as " << aoMaps[i].type << std::endl;
            aoMaps.erase(aoMaps.begin() + i);
            bRepeats.erase(bRepeats.begin() + i);
            --i;
        }
    }
    textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());
    // 9. other maps
    for (int j = aiTextureType_EMISSIVE; j < AI_TEXTURE_TYPE_MAX; j++)
    {
        if (j == aiTextureType_HEIGHT)
            continue;
        std::vector<Texture> otherMaps = loadMaterialTextures(material, aiTextureType(j), "texture_other", scene, nullptr);
        if (otherMaps.size() > 0)
            std::cout << "[Model::processMesh] Unknown Map: " << j << std::endl;
    }
    //mtl
    //对每一个mesh能够取得对应的材质
    std::cout<< "material name: " << scene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str() << std::endl;
    MaterialPBR m_MaterialPBR;
    //vector<MaterialPBR> m_materials;
    auto* mat = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D tempColor;
    if(mat->Get(AI_MATKEY_SHININESS, m_MaterialPBR.Ns) == aiReturn_SUCCESS)
    {
        std::cout << "Ns: " << m_MaterialPBR.Ns << std::endl;
    }
    if (mat->Get(AI_MATKEY_REFLECTIVITY, m_MaterialPBR.Ni) == aiReturn_SUCCESS)
    {
        std::cout << "Ni: " << m_MaterialPBR.Ni << std::endl;
    }
    if (mat->Get(AI_MATKEY_REFRACTI, m_MaterialPBR.ill_method) == aiReturn_SUCCESS)
    {
        std::cout << "ill_method: " << m_MaterialPBR.ill_method << std::endl;
    }
    if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, tempColor) == aiReturn_SUCCESS)
    {
        m_MaterialPBR.Kd = glm::vec3(tempColor.r, tempColor.g, tempColor.b);
        std::cout << "Kd: " << m_MaterialPBR.Kd.x << " " << m_MaterialPBR.Kd.y << " " << m_MaterialPBR.Kd.z << std::endl;
    }
    if (mat->Get(AI_MATKEY_COLOR_AMBIENT,tempColor ) == aiReturn_SUCCESS)
    {
        m_MaterialPBR.Ka= glm::vec3(tempColor.r, tempColor.g, tempColor.b);
        std::cout << "Ka: " << m_MaterialPBR.Ka.x << " " << m_MaterialPBR.Ka.y << " " << m_MaterialPBR.Ka.z << std::endl;
    }
    if (mat->Get(AI_MATKEY_COLOR_SPECULAR, tempColor) == aiReturn_SUCCESS)
    {
        m_MaterialPBR.Ks= glm::vec3(tempColor.r, tempColor.g, tempColor.b);
        std::cout << "Ks: " << m_MaterialPBR.Ks.x << " " << m_MaterialPBR.Ks.y << " " << m_MaterialPBR.Ks.z << std::endl;
    }
    if (mat->Get(AI_MATKEY_COLOR_EMISSIVE, tempColor) == aiReturn_SUCCESS)
    {
        m_MaterialPBR.Ke= glm::vec3(tempColor.r, tempColor.g, tempColor.b);
        std::cout << "Ke: " << m_MaterialPBR.Ke.x << " " << m_MaterialPBR.Ke.y << " " << m_MaterialPBR.Ke.z << std::endl;
    }
    //m_materials.push_back(m_MaterialPBR);
    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures, m_MaterialPBR);
    //return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
    string typeName, vector<bool> *bRepeats)
{
    if (bRepeats != nullptr)//如果不为空就清除
        bRepeats->clear();
    vector<Texture> textures;
    std::cout << "In LoadMaterialTextures " << typeName << std::endl;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        //string strUtf8(str.C_Str());
        //string strFromUtf8 = UTF8ToANSI(strUtf8);
        
        std::cout <<"   Load " << str.C_Str() <<"type "<<typeName << std::endl;
        // check if texture was loaded before and if so, 
        // continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        //遍历所有已经加载的texture查找是否已经加载了这张texture
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded,
                //continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
            //记录已经加载的类型
            allTypeTexture[typeName]++;
        }
        if (allTypeTexture.count(typeName) == 0) allTypeTexture[typeName]++;
        if (bRepeats != nullptr)
           bRepeats->push_back(skip);
    }
    
    return textures;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName,
   const aiScene* scene,vector<bool>* bRepeats)
{
    if (bRepeats != nullptr)//如果不为空就清除
        bRepeats->clear();
    vector<Texture> textures;
    //std::cout << "In LoadMaterialTextures " << typeName << std::endl;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        //string strUtf8(str.C_Str());
        //string strFromUtf8 = UTF8ToANSI(strUtf8);
        //std::cout << "   Load " << str.C_Str() << "type " << typeName << std::endl;
        // check if texture was loaded before and if so, 
        // continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
            //遍历所有已经加载的texture查找是否已经加载了这张texture
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded,
                //continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            auto aitex = scene->GetEmbeddedTexture(str.C_Str());//因为是FBX，嵌入的纹理，要从场景中加载纹理
            if (aitex != nullptr)
            {
                //std::cout << "use embedded texture" << std::endl;
                texture.id = TextureEembed(aitex);
            }
            else texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
            //记录已经加载的类型
            allTypeTexture[typeName]++;
        }
        if (allTypeTexture.count(typeName) == 0) allTypeTexture[typeName]++;
        if (bRepeats != nullptr)
            bRepeats->push_back(skip);
    }

    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int Model::TextureEembed(const aiTexture* aitex)
{
    //加载embedded纹理
    std::cout << "use embed texture" << std::endl;
    if (aitex == nullptr) return 0;
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    width = aitex->mWidth;
    height = aitex->mHeight;
    unsigned char* imageData = nullptr;
    if (aitex->mHeight == 0)
    {
        imageData = stbi_load_from_memory(reinterpret_cast<unsigned char*>(aitex->pcData), aitex->mWidth,
            &width, &height, &nrComponents, 0);
    }
    else
    {
        imageData = stbi_load_from_memory(reinterpret_cast<unsigned char*>(aitex->pcData),
            aitex->mWidth * aitex->mHeight, &width, &height, &nrComponents, 0);
    }
    if (imageData)
    {
        GLenum format;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    return textureID;
}

MyOBJLoader::MyOBJLoader()
{
    //分别读取，然后
}

void MyOBJLoader::loadModel(const std::string &filename)
{
    this->obj_filename=filename;

}

void MyOBJLoader::loadMaterial(const std::string &filename)
{
    this->mtl_filename=filename;
}

void MyOBJLoader::draw(Shader &shader)
{
}

void MyOBJLoader::drawPBR(Shader &shader)
{
}
