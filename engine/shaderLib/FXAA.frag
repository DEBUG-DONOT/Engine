#version 460 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D inputTexture;

float RGB_To_Luminance(vec3 color)
{
    return dot(color,vec3(0.299,0.587,0.114));//标准亮度计算
    //有优化计算，因为人眼对蓝色不敏感
    //return color.g*(0.587/0.299)+color.r;
}
float EDGE_THRESHOLD_MIN = 0.0312;
float EDGE_THRESHOLD_MAX = 0.125;
void main()
{

    vec3 color=texture(inputTexture,TexCoords).rgb;
    FragColor=vec4(color,1.0);
    vec2 texelSize = 1.0/textureSize(inputTexture,0);
    //计算四个方向的亮度
    vec3 N=texture(inputTexture,TexCoords+vec2(0.0,-texelSize.y)).rgb;
    vec3 S=texture(inputTexture,TexCoords+vec2(0.0,texelSize.y)).rgb;
    vec3 W=texture(inputTexture,TexCoords+vec2(-texelSize.x,0.0)).rgb;
    vec3 E=texture(inputTexture,TexCoords+vec2(texelSize.x,0.0)).rgb;
    float L=RGB_To_Luminance(color);
    float L_N=RGB_To_Luminance(N);
    float L_S=RGB_To_Luminance(S);
    float L_W=RGB_To_Luminance(W);
    float L_E=RGB_To_Luminance(E);
    float L_min=min(L,min(min(L_N,L_S),min(L_W,L_E)));
    float L_max=max(L,max(max(L_N,L_S),max(L_W,L_E)));
    float Lumn_Range=L_max-L_min;
    //  如果差值比阈值要小，这里不是边缘
    if(Lumn_Range<min(EDGE_THRESHOLD_MIN,L_max*EDGE_THRESHOLD_MAX))
    {
        //跳出
        FragColor=vec4(color,1.0);
        return;
    }
    //计算梯度和方向
    float NW=texture(inputTexture,TexCoords+vec2(-texelSize.x,-texelSize.y)).rgb;
    float NE=texture(inputTexture,TexCoords+vec2(texelSize.x,-texelSize.y)).rgb;
    float SW=texture(inputTexture,TexCoords+vec2(-texelSize.x,texelSize.y)).rgb;
    float SE=texture(inputTexture,TexCoords+vec2(texelSize.x,texelSize.y)).rgb;
    float L_NW=RGB_To_Luminance(NW);
    float L_NE=RGB_To_Luminance(NE);
    float L_SW=RGB_To_Luminance(SW);
    float L_SE=RGB_To_Luminance(SE);
    //horizontal 计算竖直方向上的亮度差值
    float H_grident=abs(L_NW-L_W-(L_W-L_SW))+2.0*abs(L_N-L-(L-L_S))+abs(L_NE-L_E-(L_E-L_SE));
    //vertical 计算水平方向上的亮度差值
    float V_grident=abs(L_NE-L_N-(L_N-L_NW))+2.0*abs(L_E-L-(L-L_W))+abs(L_SE-L_S-(L_S-L_SW));
    bool isHorizontal=H_grident>V_grident;
    //决定具体是在哪个方向，我们现在知道是竖直或者水平，但是是在上还是下，左还是右还需要决定
    float grident_1= isHorizontal? L_N-L:L_W-L;
    float grident_2= isHorizontal? L_S-L:L_E-L;
    bool isForward=abs(grident_1)>abs(grident_2);//true: North or West false: South or East
    float gradientScale=0.25*max(abs(grident_1),abs(grident_2));
    //决定如何移动
    float stepDirection=isHorizontal?texelSize.y:texelSize.x;
    
} 