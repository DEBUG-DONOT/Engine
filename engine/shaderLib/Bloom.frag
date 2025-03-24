#version 460 core
out vec4 fragColor;
in vec2 TexCoords;
uniform sampler2D screenTexture;

void main()
{
    //判断亮度，当亮度大于一定级别的时候就输出
    vec3 col = texture(screenTexture, TexCoords).rgb;
    float brightness = dot(col, vec3(0.2126, 0.7152, 0.0722));
    if(brightness>0.9)
    fragColor = vec4(col, 1.0);
    //else
    //fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}