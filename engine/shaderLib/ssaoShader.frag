#version 460 core
out float FragColor;
in vec2 TexCoords;
uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D noiseTexture;
uniform vec3 samples[64];
uniform mat4 projection;
uniform mat4 view;
// 屏幕的平铺噪声纹理会根据屏幕分辨率除以噪声大小的值来决定
const vec2 noiseScale=vec2(1920.0/4.0,1080.0/4.0);
float radius=10;
void main()
{
    vec3 m_fragPosition=texture(texture1, TexCoords).rgb;//worldSpce坐标
    m_fragPosition=(view*vec4(m_fragPosition,1.0)).xyz; //转换到viewspace
    vec3 normal=texture(texture3, TexCoords).rgb;
    vec3 randomVec = texture(noiseTexture, TexCoords * noiseScale).xyz;
    
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    float occlusion=0.0;
    int kernelSize=64;
    float bias=0.025;
    for(int i=0;i<kernelSize;i++)
    {
        vec3 curr_sample = TBN*samples[i];//tangent to view space
        curr_sample=m_fragPosition+curr_sample*radius;//view space
        
        vec4 offset = vec4(curr_sample, 1.0);
        offset = projection * offset; // 观察->裁剪空间
        offset.xyz /= offset.w; // 透视划分
        offset.xyz = offset.xyz * 0.5 + 0.5; // 变换到0.0 - 1.0的值域

        //float depthValue=texture(texture1,offset.xy).z;
        //经过render doc的检验，这里的w值确实存储的是深度值
        float depthValue= texture(texture3, offset.xy).a;//texture3的w对应的是[near,far]的线性深度
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(m_fragPosition.z - depthValue));
        occlusion+= (depthValue>curr_sample.z+bias? 1.0:0.0)*rangeCheck;//如果当前Sample被

    }
    occlusion=1.0-(occlusion/kernelSize);
    FragColor=occlusion;//0表示完全遮蔽，1表示没有遮蔽。
    //FragColor = vec4(1.0);
}