#version 460 core
out float FragColor;
in vec2 TexCoords;
uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D noiseTexture;
uniform vec3 samples[64];
uniform mat4 projection;
// 屏幕的平铺噪声纹理会根据屏幕分辨率除以噪声大小的值来决定
const vec2 noiseScale=vec2(1920.0/4.0,1080.0/4.0);
float radius=10.0;
void main()
{
    vec3 m_fragPosition=texture(texture1, TexCoords).rgb;
    vec3 normal=texture(texture3, TexCoords).rgb;
    vec3 randomVec = texture(noiseTexture, TexCoords * noiseScale).xyz;
    
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    float occlusion=0.0;
    int kernelSize=64;
    for(int i=0;i<kernelSize;i++)
    {
        vec3 curr_sample = TBN*samples[i];
        vec4 offset = vec4(curr_sample, 1.0);
        offset = projection * offset; // 观察->裁剪空间
        offset.xyz /= offset.w; // 透视划分
        offset.xyz = offset.xyz * 0.5 + 0.5; // 变换到0.0 - 1.0的值域
        float depthValue= texture(texture3, offset.xy).w;//??????? 为什么取负号？
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(m_fragPosition.z - depthValue));
        occlusion+= (depthValue>curr_sample.z? 1.0:0.0)*rangeCheck;
    }
    occlusion=1.0-(occlusion/kernelSize);
    FragColor=occlusion;
    //FragColor = vec4(1.0);
}