# 计划

- [x] 一个box的类
- [x] 实现一个Sphere的类 
- [x] 实现球的PBR
- [ ] 导入ImGUI实现有UI控制的球的PBR
- [ ] Shadow Map
- [ ] 实现defered shading
- [ ] 待IBL的PBR

# 结构



# FunctionLayer

## gameObject和component

按照piccolo的做法和教程中的做法，可以想到的一点是，保证component的派生类一定使用默认的构造函数。



#### MeshComponent

用来渲染的组件。

调用ResourceLayer中的Model来加载和渲染。



#### 相机



##### 遇到的问题

这里有一个问题就是不能做相机的平移的时候需要考虑glm::lookAt函数的参数定义。

参数是（位置，目标位置，up），我们更新摄像机位置的之后，需要重新计算view矩阵，这个时候需要写成这样的形式：
glm::lookAt(pos,front+pos,up),我们的想法是实现按a和d的时候实现平移，这个时候我们应该就需要让目标位置和相机位置保持front的方向不变，如果我们只写glm::lookAt(pos,front,up),因为这里代表的不是front方向而是目标位置，就会导致我们的相机造成一个旋转的效果。

## PBR

texture_other
texture_roughness
texture_diffuse
texture_metallic
texture_albedo
texture_specular
texture_height
texture_normal
texture_ao

现在可以确定的是，assimp没有只加载了diffuse的纹理，其他的都没有加载。

纹理加载问题对于不同格式的模型差距比较大，能够稳定加载的是glb，assimp对fbx的支持不是很到位。



模型的颜色不对主要还是纹理对应的绑定有问题，不能在for循环中执行shader.UpLoadUniformInt("aoMap", ao.texture_id);这样的操作。



# ResourceLayer

## Model

理想情况下肯定是直接对model调用draw操作就可以让它在屏幕上画出来。

使用Assimp来加载模型。

Assimp默认使用OpenGL的右手系。

+X向右，+Y向上，+Z指向屏幕外面。

UV坐标中，原点在左下角，在read file时可以使用aiProcess_FlipUVs flag来把坐标系的原点放到左上角。

![img](https://learnopengl-cn.github.io/img/03/01/assimp_structure.png)

### Mesh

Assimp中的Mesh存储在一个数组中。一个aiMesh由一系列的数据组成，其中保证存在的只有mVertices和mFaces.

所有的mesh实体保存在Scene的mesh中，而所有的node中的mMeshes数组仅仅只保留对应的mesh在Scene的Mesh中的index。

在代码中构造一个类来对应这个aiMesh.

在read file的时候通过指定flag也可以计算出其中的一些值。

#### Vertex

mesh中的值存储在vertex类中，包含位置、法线、纹理坐标等。

### Texture



## shader

对shader的一个优化：如果shader上传失败的话，会不停的重新尝试，我们加上一个变量控制这个问题。

# 碰到的问题

开启深度测试之后没有清理 GL_DEPTH_BUFFER_BIT,导致画面上只有clear-color 的颜色。

如果不清理GL_Color_BUffer_BIt, 那么屏幕就会一直保持第一帧的颜色，如果不清理GL_Depth_Bit,那么物体就会无法显示。

它们就是默认的framebuffer的attachment。

