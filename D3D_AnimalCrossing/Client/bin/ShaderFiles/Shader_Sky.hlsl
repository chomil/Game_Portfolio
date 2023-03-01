
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlAmbient = vector(0.5f, 0.5f, 0.5f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);;

vector		g_vCamPosition;
float		g_fPower = 30.f;

texture		g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	mipfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;			
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};


VS_OUT VS_MAIN(/* 정점 */VS_IN In)
{
	VS_OUT			Out;	

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}



struct PS_IN
{
	float4		vPosition : POSITION;	
	float3		vTexUV : TEXCOORD0;		
};

struct PS_OUT
{
	float4		vColor : COLOR0;	
};



PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	vector	vDiffuse = texCUBE(DiffuseSampler, In.vTexUV);
	
	/* 지형의 디퓨즈머테리얼이다. */

	Out.vColor = g_vLightDiffuse*vDiffuse;

	return Out;	
}

technique DefaultTechnique
{	
	pass DefaultRendering
	{
		fillmode = solid;	//m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	//m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		lighting = false;
		CULLMODE = none;
		zwriteenable = false;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

}

