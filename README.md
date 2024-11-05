# FunctionLayer

## gameObject和component

按照piccolo的做法和教程中的做法，可以想到的一点是，保证component的派生类一定使用默认的构造函数。



#### MeshComponent

用来渲染的组件。

调用ResourceLayer中的Model来加载和渲染。

# ResourceLayer

## Model

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

