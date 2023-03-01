
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector		g_vCamPosition;


texture		g_DiffuseTexture;

texture		g_DiffuseTexture_Sand;

texture		g_FilterTexture;


int			g_iTerrain; //0:ground, 1:else


sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	mipfilter = linear;
	magfilter = linear;
};

sampler DiffuseSandSampler = sampler_state
{
	texture = g_DiffuseTexture_Sand;
	mipfilter = linear;
	magfilter = linear;
};

sampler FilterSampler = sampler_state
{
	texture = g_FilterTexture;
	mipfilter = linear;
	magfilter = linear;
};


struct VS_IN
{
	float3		vPosition : POSITION;	
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;			
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};


VS_OUT VS_MAIN(/* 정점 */VS_IN In)
{
	VS_OUT			Out;	

	matrix			matWV, matWVP;


	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	
	float4	vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

	vector	vWorldNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);

	//글로브뷰 시작
	Out.vPosition.y -= pow(((vWorldPos.z - g_vCamPosition.z) / 30.f), 4);
	//여기까지 글로브뷰
	Out.vNormal = vWorldNormal;
	Out.vProjPos = Out.vPosition;
	return Out;
}



struct PS_IN
{
	float4		vPosition : POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	float4		vDiffuse : COLOR0;
	float4		vNormal : COLOR1;
	float4		vDepth : COLOR2;
};



PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	vector	vSourDiffuse = tex2D(DiffuseSampler, In.vTexUV * 40);
	vector	vDestDiffuse = tex2D(DiffuseSandSampler, In.vTexUV * 40);
	vector	vFilter = tex2D(FilterSampler, In.vTexUV);
	
	/* 지형의 디퓨즈머테리얼이다. */

	vector	vDiffuse = vFilter * vSourDiffuse + (1.f - vFilter) * vDestDiffuse;

	Out.vDiffuse = vDiffuse;
	Out.vDiffuse.a = 1.f;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.w / 300.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;	
}

technique DefaultTechnique
{	
	pass DefaultRendering
	{
		fillmode = solid;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Default2Rendering
	{
		fillmode = wireframe;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}

