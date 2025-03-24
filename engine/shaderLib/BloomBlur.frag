#version 460 core
out vec4 fragColor;
in vec2 TexCoords;
uniform sampler2D bloomTexture;
//float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
uniform float weight[65];
int vertical;
// void main()
// {
//     //判断亮度，当亮度大于一定级别的时候就输出
//     vec3 temp=vec3(0.0,0.0,0.0);
//     vec2 texSize=1.0/textureSize(bloomTexture,0).xy;
//     int kernelSize=15;
//     int columns=(kernelSize*2+1)*(kernelSize*2+1);
//     for(int i=-kernelSize;i<=kernelSize;i++)
//     {
//         for(int j=-kernelSize;j<=kernelSize;j++)
//         {
//             int index=(i+1)*columns+(j+1);
//             vec2 offset=vec2(i,j);
//             vec3 sampleColor=texture(bloomTexture,TexCoords+offset*texSize).rgb;
//             temp+=sampleColor*weight[index];
//         }
//     }
//     fragColor=vec4(temp,1.0);
// }
void main()
{
    //判断亮度，当亮度大于一定级别的时候就输出
    vec3 temp=vec3(0.0,0.0,0.0);
    vec2 texSize=1.0/textureSize(bloomTexture,0).xy;
    int kernelSize=32;
    if(vertical==0)
    {
        for(int i=-kernelSize;i<=kernelSize;i++)
        {
            vec3 sampleColor=texture(bloomTexture,TexCoords+vec2(i,0)*texSize).rgb;
            temp+=sampleColor*weight[i+kernelSize];
        }
    }
    if(vertical==1)
    {
        for(int i=-kernelSize;i<=kernelSize;i++)
        {
            vec3 sampleColor=texture(bloomTexture,TexCoords+vec2(0,i)*texSize).rgb;
            temp+=sampleColor*weight[i+kernelSize];
        }
    }

    fragColor=vec4(temp,1.0);
}