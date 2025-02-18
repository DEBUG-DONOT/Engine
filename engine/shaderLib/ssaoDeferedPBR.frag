#version 460 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D shadowMap;
uniform sampler2D gDepth;
uniform sampler2D aoMap;
//这三个不随着fragment的位置变化而变化
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform mat4 lightSpaceMatrix;
uniform int usingSSAO;

float CalcuShadowFactor(vec4 lightSpacePos)//lightSpacePos是从光源视角下的片元位置
{
	vec3 projCoords = lightSpacePos.xyz/lightSpacePos.w;//进行透视除法
	projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;
	if(currentDepth>1.0)//超出远平面就认为在阴影中
		return 0.0;
	float shadow = 0.0;
	float bias = 0.002;
//////////////////////PCF&ShadowMapping//////////////////////
// 	float closeDepth = texture(shadowMap, projCoords.xy).r;
// //PCF
// 	vec2 texelSize = 1.0/textureSize(shadowMap,0);
// 	int pcfSearchRigoin=2;
// 	for(int x=-pcfSearchRigoin;x<=pcfSearchRigoin;++x)
// 	{
// 		for(int y=-pcfSearchRigoin;y<=pcfSearchRigoin;++y)
// 		{
// 			float pcfDepth = texture(shadowMap, projCoords.xy+vec2(x,y)*texelSize).r;
// 			shadow += currentDepth-bias>pcfDepth?1.0:0.0;
// 		}
// 	}
// 	shadow/=(2*pcfSearchRigoin+1)*(2*pcfSearchRigoin+1);
// 	return 1.0-shadow;
//original shadow mapping
	// if(currentDepth > closeDepth+bias)
	// {
	// 	shadow = 0.0;
	// }
	// else shadow = 1.0;
	// return shadow;
///////////////PCF&ShadowMapping//////////////////////

//PCSS
	//search reigion
	//在3*3的区域内搜索
	float w_light=50.0;
	vec2 texelSize = 1.0/textureSize(shadowMap,0);//得到纹理的一个像素大小
	float d_Blocker=0.0;
	int searchRegion=8;
	for(int x=-searchRegion;x<=searchRegion;++x)
	{
		for(int y=-searchRegion;y<=searchRegion;++y)
		{
			float nearDepth = texture(shadowMap, projCoords.xy+vec2(x,y)*texelSize).r;//0到1之内到深度值
			if(currentDepth>nearDepth+bias)//如果当前位置在这个点后面，也就是在阴影中
			{
				shadow+=1.0;
				d_Blocker+=nearDepth;
			}
		}
	}
	if(shadow==0.0)//没有阴影，
		return 1.0;
	d_Blocker/=shadow;
	shadow=0.0;
	//float d_Receiver=distance(lightPosition,fragPosition);//world space distance
	float d_Receiver=projCoords.z;
	//calculate penumbra
	int w_penumra=int((max(d_Receiver-d_Blocker,0.0)/d_Blocker)*w_light);
	w_penumra=max(0,min(w_penumra,10)); //可能就是这里导致我的阴影不够软
	//search PCF
	for(int x=-w_penumra;x<=w_penumra;++x)
	{
		for(int y=-w_penumra;y<=w_penumra;++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy+vec2(x,y)*texelSize).r;
			shadow += currentDepth-bias>pcfDepth?1.0:0.0;
		}
	}
	float result = max(0.0, (shadow / float((2 * w_penumra + 1) * (2 * w_penumra + 1))));
	return 1.0-result;
}
//PBR function
float PI = 3.14159265359;
vec3 Frenel(vec3 h,vec3 v,vec3 F0)
{
	return F0+(vec3(1.0)-F0)*pow(1.0-max(dot(h,v),0.0),5.0);
}

//Normal Distribution Function
float NormalDistribution(vec3 n,vec3 h,float roughness)
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
	//从纹理中获取数据
	// 检查深度值是否为背景（例如深度值 1.0）
	float depth = texture(gDepth, TexCoords).r;
	if (depth == 1.0) 
	{
    // 直接使用清除颜色或跳过光照计算
    FragColor = vec4(0.2f, 0.3f, 0.5f, 1.0);
    return;
	}
	//get data
	vec3 fragPosition=texture(texture1, TexCoords).rgb;
	float roughness=texture(texture1, TexCoords).a;

	vec3 albedo=texture(texture2, TexCoords).rgb;
	float metallic=texture(texture2, TexCoords).a;

	vec3 fragNormal=texture(texture3, TexCoords).rgb;
	float depthValue=texture(texture3, TexCoords).a;

	vec4 lightSpacePos=lightSpaceMatrix*vec4(fragPosition,1.0);

	float ao=texture(aoMap,TexCoords).r;

	//计算
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
	float NDF=NormalDistribution(n,h,roughness);
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
	
	//ambient
	if(usingSSAO==0)
	{
		ao=1.0;
	}
	//else ao=texture(aoMap,TexCoords).r;
	vec3 ambient=albedo*0.03*ao;
	color+=ambient;
	//gamma correction
	color=color/(color+vec3(1.0));
	color= pow(color,vec3(1.0/2.2));
	FragColor=vec4(color,1.0);
	//FragColor=vec4(1.0,0.5,0.5,1.0);

}