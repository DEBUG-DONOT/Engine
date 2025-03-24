#version 460 core
out vec4 fragColor;
in vec2 TexCoords;
uniform sampler2D bloomTexture;
uniform float weight[25];
void main()
{
    //判断亮度，当亮度大于一定级别的时候就输出
    vec3 temp=vec3(0.0,0.0,0.0);
    vec2 texSize=1.0/textureSize(bloomTexture,0).xy;
    for(int i=-2;i<=2;i++)
    {
        for(int j=-2;j<=2;j++)
        {
            int index=(i+1)*5+(j+1);
            vec2 offset=vec2(i,j);
            vec3 sampleColor=texture(bloomTexture,TexCoords+offset*texSize).rgb;
            temp+=sampleColor*weight[index];
        }
    }
    fragColor=vec4(temp,1.0);
}