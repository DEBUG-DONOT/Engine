#version 460 core
in vec3 nNormal;
in vec2 nTexCoord;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;
uniform sampler2D texture_albedo1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_metallic1;
uniform sampler2D texture_roughness1;
uniform sampler2D texture_ao1;


//uniform sampler2D 
float NormalDF(float alpha,vec3 n,vec3 h);
vec3 FresnelFuntion(vec3 h,vec3 v,vec3 f0);
float Gsub(vec3 n,vec3 v,float k);
float GeometryFunction(vec3 n,vec3 v,vec3 l,float alpha);
float pi=3.14159;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(texture_normal1, nTexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(nTexCoord);
    vec2 st2 = dFdy(nTexCoord);

    vec3 N   = normalize(nNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{

    vec3 albedo=pow(texture(texture_albedo1,nTexCoord).rgb,vec3(2.2));
    float metallic=texture(texture_metallic1,nTexCoord).r;
    float roughness=texture(texture_roughness1,nTexCoord).r;
    float ao=texture(texture_ao1,nTexCoord).r;
    vec3 n= getNormalFromMap();
    vec3 v=normalize(viewPos-FragPos);
    vec3 l=normalize(lightPos-FragPos);
    vec3 h=normalize(v+l);
    vec3 F0=vec3(0.04);
    F0=mix(F0,albedo,metallic);
    vec3 Lo=vec3(0.0);
    //calculate radiance
    float dist=length(lightPos-FragPos);
    float attenuation=1.0/(dist*dist);
    //float attenuation=1.0;
    vec3 radiance=lightColor*attenuation;
    //cook-torrance brdf
    float N=NormalDF(roughness,n,h);
    float G=GeometryFunction(n,v,l,roughness);
    vec3 F=FresnelFuntion(h,v,F0);
    vec3 ks=F;
    vec3 kd=vec3(1.0)-ks;
    kd*=(1.0-metallic);
    float NdotL=max(dot(n,l),0.0);

    vec3 nominator=N*G*F;
    float denominator=4*max(dot(n,v),0.0)*max(dot(n,l),0.0);
    vec3 specular=nominator/denominator;
    vec3 diffuse=kd*albedo/pi;
    Lo+=(diffuse+specular)*radiance*NdotL;
    //ambient
    vec3 ambient=vec3(0.03)*albedo*ao;
    //HDR
    vec3 color=Lo+ambient;
    color= color/(color+vec3(1.0));
    color=pow(color,vec3(1.0/2.2));
    FragColor=vec4(color,1.0);
}
float NormalDF(float alpha,vec3 n,vec3 h)
{
    //法线分布函数

    float alpha2=alpha*alpha;
    float NdotH=dot(n,h);
    float NdotH2=NdotH*NdotH;
    float nom=alpha2;
    float denom=NdotH2*(alpha2-1)+1;
    denom=pi*denom*denom;
    return nom/denom;
}

vec3 FresnelFuntion(vec3 h,vec3 v,vec3 f0)
{
    //frenel 函数 基础反射率是一个向量
    float hdotv=dot(h,v); 
    float x=pow(1.0 - hdotv,5);
    return f0 + (vec3(1.0) - f0)*x  ;

}

float Gsub(vec3 n,vec3 v,float k)
{
    //计算几何函数的一部分
    float NdotV=max(dot(n,v),0.0);//上半球
    float nom=NdotV*(1-k)+k;
    return NdotV/nom;
}

float GeometryFunction(vec3 n,vec3 v,vec3 l,float alpha)//几何函数
{
    float k=(alpha+1)*(alpha+1);
    k=k/8;
    float Gv=Gsub(n,v,k);
    float Gl=Gsub(n,l,k);
    return Gv*Gl;
}
