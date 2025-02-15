#version 460 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture1;
//uniform float near_plane;
//uniform float far_plane;
// float LinearizeDepth(float depth)
// {
//     float z = depth * 2.0 - 1.0; // Back to NDC 
//     return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
// }
void main()
{
    //float depthValue = texture(shadowMap, TexCoords).r;
    //depthValue = LinearizeDepth(depthValue);
    vec3 color = texture(texture1, TexCoords).rgb;
    FragColor = vec4(color, 1.0f);
    //FragColor = vec4(vec3(depthValue), 1.0f);
} 