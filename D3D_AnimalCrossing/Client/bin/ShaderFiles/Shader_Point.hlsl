
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float		g_fAlpha;
float		g_fSize;
vector		g_vCamPosition;
texture		g_DiffuseTexture;
vector		g_vLightDiffuse;

sampler DiffuseSampler = sampler_state 
{  
	texture = g_DiffuseTexture;

	/* 현재 텍스쳐가 픽셀들에게 샘플링 될때의 상태를 설정한다. */
	mipfilter = linear;
	magfilter = linear;

	//AddressU = clamp;
	//AddressV = clamp;

};

struct VS_IN
{
	float3		vPosition : POSITION;	
	float		fSize : PSIZE;
	float2		vTexUV : TEXCOORD0;		
	
};

struct VS_OUT
{

	float4		vPosition : POSITION;
	float		fSize : PSIZE;
	float2		vTexUV : TEXCOORD0;	
};


VS_OUT VS_MAIN(/* 정점 */VS_IN In)
{
	VS_OUT			Out;	


	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	In.fSize = g_fSize;
	Out.fSize = (720 * In.fSize) * sqrt(1.f / pow(Out.vPosition.w, 2.f));

	//글로브뷰 시작
	float4 vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vPosition.y -= pow(((vWorldPos.z - g_vCamPosition.z) / 30.f), 4);
	//여기까지 글로브뷰
	Out.vTexUV = In.vTexUV;
	

	return Out;
}



struct PS_IN
{
	float4		vPosition : POSITION;	
	float		fSize : PSIZE;	
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : COLOR0;	
};



PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV) * g_vLightDiffuse;
	Out.vColor.a *= g_fAlpha;
	return Out;	
}

technique DefaultTechnique
{	
	pass DefaultRendering
	{	
		PointSpriteEnable = true;
		AlphablendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		CullMode = none;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}	
}

