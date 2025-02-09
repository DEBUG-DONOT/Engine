#version 460 core
in vec3 fragPosition;
in vec3 fragNormal;
in vec4 lightSpacePos;
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform float metallic;
uniform float roughness;
uniform vec3 albedo;
uniform sampler2D shadowMap;

float CalcuShadowFactor(vec4 lightSpacePos)
{
	vec3 projCoords = lightSpacePos.xyz/lightSpacePos.w;//进行透视除法
	projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;
	if(currentDepth>1.0)//超出远平面就认为在阴影中
		return 0.0;
	float shadow = 0.0;
	float closeDepth = texture(shadowMap, projCoords.xy).r;
	float bias = 0.005;
//PCF
	vec2 texelSize = 1.0/textureSize(shadowMap,0);
	for(int x=-1;x<=1;++x)
	{
		for(int y=-1;y<=1;++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy+vec2(x,y)*texelSize).r;
			shadow += currentDepth-bias>pcfDepth?1.0:0.0;
		}
	}
	shadow/=9.0;
	return 1.0-shadow;

//original shadow mapping
	// if(currentDepth > closeDepth+bias)
	// {
	// 	shadow = 0.0;
	// }
	// else shadow = 1.0;
	// return shadow;
}

float PI = 3.14159265359;
vec3 Frenel(vec3 h,vec3 v,vec3 F0)
{
	return F0+(vec3(1.0)-F0)*pow(1.0-max(dot(h,v),0.0),5.0);
}

//Normal Distribution Function
float NormalDistribution(vec3 n,vec3 h)
{
	float alpha2=roughness*roughness;
	float a2=alpha2*alpha2;
	//float under=pow(max(dot(n,h),0.0),2.0)*(alpha2-1.0)+1.0;
	float under=pow(max(dot(n,h),0.0),2.0)*(a2-1.0)+1.0;
	//return alpha2/(PI*pow(under,2.0));
	return a2/(PI*pow(under,2.0));
}

float GSub(float ndotv,	float k)
{
	return ndotv/(ndotv*(1.0-k)+k);
}

float Geometry(vec3 n,vec3 v,vec3 l,float roughness)
{
	float ndotv =max(dot(n,v),0.0) ;
	float ndotl =max(dot(n,l),0.0) ;
	float k= (roughness+1.0)*(roughness+1.0)/8.0;
	return GSub(ndotv,k)*GSub(ndotl,k);
}


void main()
{
	vec3 v=normalize(eyePosition-fragPosition);
	vec3 l=normalize(lightPosition-fragPosition);
	vec3 n=normalize(fragNormal);
	vec3 h=normalize(v+l);
	//radiance
	//float dis=distance(fragPosition, lightPosition);
	float dis=length(fragPosition-lightPosition);
	float attenuation=1.0/(dis*dis);
	vec3 radiance=lightColor*attenuation;
	//Cook-Torrance BRDF
	vec3 F0=vec3(0.04);
	F0=mix(F0,albedo.rgb,metallic);
	vec3 F=Frenel(h,v,F0);
	float NDF=NormalDistribution(n,h);
	float G=Geometry(n,v,l,roughness);
	//calculate
	vec3 kS=F;
	vec3 kD=vec3(1.0)-kS;
	kD*=(1.0-metallic);//如果是金属，kD=0，没有漫反射
	float ndotl=max(dot(n,l),0.0);//上半球
	//direct light
	vec3 kDColor=(albedo/(PI));
	vec3 KSColor=F*G*NDF/(4.0*max(dot(n,v),0.0)*max(dot(n,l),0.0)+0.000001);
	//KD+KS表示BRDF，也就是对于radiance有多少光出射，所以这里可以用对应相乘
	vec3 color=(kD*kDColor+KSColor)*ndotl*radiance;
	//shadow
	color*=CalcuShadowFactor(lightSpacePos);
	//if(CalcuShadowFactor(lightSpacePos)==0.0)
	//{
	//	color=vec3(1.0,0.0,0.0);
	//}
	//ambient
	vec3 ambient=albedo*0.03;
	color+=ambient;
	//gamma correction
	color=color/(color+vec3(1.0));
	color= pow(color,vec3(1.0/2.2));
	FragColor=vec4(color,1.0);
	//FragColor=vec4(1.0,0.5,0.5,1.0);

}