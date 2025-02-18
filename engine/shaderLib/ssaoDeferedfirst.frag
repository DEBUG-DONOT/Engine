#version 460 core
//out
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gAlbedoSpec;
layout (location = 2) out vec4 gNormal;
//in
in vec3 fragPosition;
in vec3 fragNormal;
in vec2 TexCoord;

//这里也可以替换成texture，但是我们首先用float来代替
uniform float metallic;
uniform float roughness;
uniform vec3 albedo;

uniform float NEAR = 0.1; // 投影矩阵的近平面
uniform float FAR = 50.0f; // 投影矩阵的远平面
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // 回到NDC
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));    
}


void main()
{
    gPosition = vec4(fragPosition,roughness);//世界坐标
    gAlbedoSpec=vec4(albedo,metallic);
    gNormal = vec4(normalize(fragNormal),LinearizeDepth(gl_FragCoord.z));
}