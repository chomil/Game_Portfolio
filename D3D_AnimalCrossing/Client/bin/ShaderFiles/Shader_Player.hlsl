
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;


vector		g_vCustomColor;

vector		g_vCamPosition;

int			g_iSkin; //0:face, 1:skin, 2:else

texture		g_DiffuseTexture;
texture		g_NormalTexture;
texture		g_MixTexture;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;/*
	mipfilter = linear;
	magfilter = linear;*/
};



sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
	mipfilter = linear;
	magfilter = linear;
};

sampler MixSampler = sampler_state
{
	texture = g_MixTexture;/*
	mipfilter = linear;
	magfilter = linear;*/
};

struct VS_IN
{
	float3		vPosition : POSITION;	
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
};


struct VS_OUT
{
	float4		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(/* 정점 */VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix)).xyz;
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent)).xyz;




	//글로브뷰 시작
	float4 vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vPosition.y -= pow(((vWorldPos.z - g_vCamPosition.z) / 30.f), 4);
	//여기까지 글로브뷰

	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};

struct PS_OUT
{
	float4		vDiffuse : COLOR0;
	float4		vNormal : COLOR1;
	float4		vDepth : COLOR2;
};



PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	vector		vMtrlDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector		vMixDiffuse = tex2D(MixSampler, In.vTexUV);
	
	if (g_iSkin==0)
	{
		vMtrlDiffuse += vMixDiffuse.r * g_vCustomColor;
	}
	else
	{
		vMtrlDiffuse *= g_vCustomColor;
	}


	Out.vDiffuse = vMtrlDiffuse;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.0f);
	Out.vDepth = vector(In.vProjPos.w / 300.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	

	return Out;	
}


PS_OUT PS_MAIN_NORMALMAP(PS_IN In)
{
	PS_OUT			Out;

	vector		vMtrlDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector		vNormalDesc = tex2D(NormalSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	vector		vMixDiffuse = tex2D(MixSampler, In.vTexUV);

	if (g_iSkin == 0)
	{
		vMtrlDiffuse += vMixDiffuse.r * g_vCustomColor;
	}
	else
	{
		vMtrlDiffuse *= g_vCustomColor;
	}


	Out.vDiffuse = vMtrlDiffuse;

	float3x3		TBN = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, TBN);
	/* -1.f ~ 1.f */
	/* 0.f ~ 1.f */
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.0f);
	Out.vDepth = vector(In.vProjPos.w / 300.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);


	return Out;
}

// 여기부터 그림자패스용
matrix		g_matLightView;
matrix		g_matLightProj;

struct VS_IN_SHADOW
{
	float3			vPosition : POSITION;
};

struct VS_OUT_SHADOW
{
	float4			vPosition : POSITION;
	float4			vProjPos : TEXCOORD0;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN_SHADOW In)
{
	VS_OUT_SHADOW		Out = (VS_OUT_SHADOW)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

	//글로브뷰 시작
	float4 vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vPosition.y -= pow(((vWorldPos.z - g_vCamPosition.z) / 30.f), 4);
	//여기까지 글로브뷰

	Out.vProjPos = Out.vPosition;
	return Out;
}

struct PS_IN_SHADOW
{
	float4			vPosition : POSITION;
	float4			vProjPos : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
	vector			vLightDepth : COLOR0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW		Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjPos.w / 300.f;

	Out.vLightDepth.a = 1.f;

	return Out;
}

// 여기까지 그림자패스용

technique DefaultTechnique
{	
	pass DefaultRendering
	{	/*
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 30;*/
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	pass NormalRendering //필터
	{
		/*
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 30;*/

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_NORMALMAP();
	}

	pass Shadow_Depth
	{
		ZWriteEnable = false;

		CullMode = none;
		VertexShader = compile vs_3_0 VS_MAIN_SHADOW();
		PixelShader = compile ps_3_0 PS_MAIN_SHADOW();
	}
}