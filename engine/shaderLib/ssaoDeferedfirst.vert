#version 460 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexCoord;
uniform mat4 model;//用来转换到世界坐标
uniform mat4 MVP;
//uniform mat4 lightSpaceMatrix;
out vec3 fragPosition;
out vec3 fragNormal;
out vec2 TexCoord;
//out vec4 lightSpacePos;
void main()
{
	gl_Position=MVP*vec4(aPos,1.0);
	fragPosition=vec3(model*vec4(aPos,1.0)).xyz;//在view space
	TexCoord=aTexCoord;
	fragNormal=mat3(transpose(inverse(model)))*aNormal;
	//lightSpacePos=(lightSpaceMatrix*vec4(fragPosition,1.0));
}