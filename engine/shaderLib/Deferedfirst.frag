#version 460 core
//out
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gAlbedoSpec;
layout (location = 2) out vec3 gNormal;
//in
in vec3 fragPosition;
in vec3 fragNormal;
in vec2 TexCoord;

//这里也可以替换成texture，但是我们首先用float来代替
uniform float metallic;
uniform float roughness;
uniform vec3 albedo;

void main()
{
    gPosition = vec4(fragPosition,roughness);//世界坐标
    gAlbedoSpec=vec4(albedo,metallic);
    gNormal = normalize(fragNormal);
}