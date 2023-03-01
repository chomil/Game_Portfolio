
vector			g_vLightDir;
vector			g_vLightPos;
float			g_fRange;
vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;


matrix		g_matLightProj;
matrix		g_matLightView;

vector			g_vCamPosition;
matrix			g_ProjMatrixInv;
matrix			g_ViewMatrixInv;


vector			g_vMtrlAmbient = (vector)0.5f;
vector			g_vMtrlSpecular = (vector)0.f;



texture			g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;

	texture = g_DiffuseTexture;
};

texture			g_ShadeTexture;

sampler ShadeSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;

	texture = g_ShadeTexture;
};

texture			g_DepthTexture;

sampler DepthSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;

	texture = g_DepthTexture;
};


texture		g_ShadowDepthTexture;

sampler ShadowDepthSampler = sampler_state
{
	texture = g_ShadowDepthTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = none;
	AddressU = clamp;
	AddressV = clamp;
};


texture			g_NormalTexture;

sampler NormalSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;

	texture = g_NormalTexture;
};

texture			g_SpecularTexture;

sampler SpecularSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;

	texture = g_SpecularTexture;
};



texture			g_DebugTexture;

sampler DebugSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;

	texture = g_DebugTexture;
};


struct PS_IN
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(DebugSampler, In.vTexUV);

	return Out;
}

struct PS_OUT_LIGHTACC
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
};

PS_OUT_LIGHTACC PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHTACC		Out = (PS_OUT_LIGHTACC)0;
	
	vector		vNormalDesc = tex2D(NormalSampler, In.vTexUV);
	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector		vDepthDesc = tex2D(DepthSampler, In.vTexUV);
	float		fViewZ = vDepthDesc.r * 300.f;


	

	vector		vWorldPos = (vector)0.f;

	/* ���������̽� ���� ��ġ�� ���ϱ����ؼ�. */
	/* uv:0,0 -> p.xy:-1,1*/
	/* uv:1,1 -> p.xy:1,-1*/
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;

	/* �佺���̽� ���Ǽ��� �þ߹��� ���� : near ~ far */
	/* �佺���̽� ��(������ı��� ����)�Ǽ��� �þ߹��� ���� : 0 ~ far */
	/* ����������(w(�佺���̽����� ����)�� ������.) �� ����. 0 ~ 1������ ����. */
	vWorldPos.z = vDepthDesc.g;
	vWorldPos.w = 1.f;

	/* �佺���̽��� ���� ���̸� �����ϳ�.  */
	/* �佺���̽� ��(������ı��� ����) */
	vWorldPos = vWorldPos * fViewZ;

	/* �佺���̽� ���� ��ġ. */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	/* ���彺���̽� ���� ��ġ. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	float fFogAtt = saturate(1 - (vWorldPos.z - g_vCamPosition.z - 45) / 50);

	vector		vReflect = reflect(normalize(g_vLightDir), vNormal);
	vector		vLook = normalize(vWorldPos - g_vCamPosition);

	vector vShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(vNormal)), 0.f);
	if (1.f == vNormalDesc.a)
	{
		Out.vShade = g_vLightDiffuse * saturate(vShade + (g_vLightAmbient * g_vMtrlAmbient)) * fFogAtt;
		Out.vShade.a = 1.f;

		Out.vSpecular.rgb = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, vLook), 0.f), 30.f) * fFogAtt;
		Out.vSpecular.a = 1.f;
	}
	else
		discard;

	return Out;	
}

PS_OUT_LIGHTACC PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHTACC		Out = (PS_OUT_LIGHTACC)0;

	vector		vNormalDesc = tex2D(NormalSampler, In.vTexUV);
	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector		vDepthDesc = tex2D(DepthSampler, In.vTexUV);
	float		fViewZ = vDepthDesc.r * 300.f;

	vector		vWorldPos = (vector)0.f;

	/* ���������̽� ���� ��ġ�� ���ϱ����ؼ�. */
	/* uv:0,0 -> p.xy:-1,1*/
	/* uv:1,1 -> p.xy:1,-1*/
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;

	/* �佺���̽� ���Ǽ��� �þ߹��� ���� : near ~ far */
	/* �佺���̽� ��(������ı��� ����)�Ǽ��� �þ߹��� ���� : 0 ~ far */
	/* ����������(w(�佺���̽����� ����)�� ������.) �� ����. 0 ~ 1������ ����. */
	vWorldPos.z = vDepthDesc.g;
	vWorldPos.w = 1.f;

	/* �佺���̽��� ���� ���̸� �����ϳ�.  */
	/* �佺���̽� ��(������ı��� ����) */
	vWorldPos = vWorldPos * fViewZ;

	/* �佺���̽� ���� ��ġ. */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	/* ���彺���̽� ���� ��ġ. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector		vLightDir = vWorldPos - g_vLightPos;
	float		fDistance = length(vLightDir);

	float		fAtt = saturate((g_fRange - fDistance) / g_fRange);
	

	vector		vReflect = reflect(normalize(vLightDir), vNormal);
	vector		vLook = normalize(vWorldPos - g_vCamPosition);

	if (1.f == vNormalDesc.a)
	{
		Out.vShade = (g_vLightDiffuse * max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
		Out.vShade.a = 1.f;
		

		Out.vSpecular.rgb = ((g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, vLook), 0.f), 30.f)) * fAtt;
		Out.vSpecular.a = 1.f;
	}
	else
		discard;

	return Out;
}

struct PS_OUT_BLEND
{
	vector		vColor : COLOR0; /* ����ۿ� �׸���. */
};

PS_OUT_BLEND PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT_BLEND		Out = (PS_OUT_BLEND)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);
	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);

	//�⺻ ��� ����
	Out.vColor = vDiffuse * vShade + vSpecular;
	
	//���� Ÿ�� ������
	vector		vDepthInfo = tex2D(DepthSampler, In.vTexUV);
	float		fViewZ = vDepthInfo.x * 300.0f;
	vector		vPosition;
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepthInfo.y * fViewZ;
	vPosition.w = fViewZ;

	//���� Ÿ���� ���� �� �����̽��� ��ȯ
	vPosition = mul(vPosition, g_ProjMatrixInv);
	vPosition = mul(vPosition, g_ViewMatrixInv);
	vPosition = mul(vPosition, g_matLightView);
	//���� ��ġ�� ���� ����
	vector		vUVPos = mul(vPosition, g_matLightProj);
	float2		vNewUV;
	vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
	vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;

	vector		vShadowDepthInfo = tex2D(ShadowDepthSampler, vNewUV);

	//��ȯ�� ���� Ÿ�ٰ� �׸��� ���� Ÿ���� ��
	if (vPosition.z - 0.5f  > vShadowDepthInfo.r * 300.f)
		Out.vColor.rgb = Out.vColor*0.7f; //�׸��� ó��

	return Out;
}


technique DefaultTechnique
{
	pass Render_Debug
	{
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_DEBUG();
	}

	pass Render_LightAcc_Directional
	{
		AlphaTestEnable = false;

		AlphaBlendenable = true;
		BlendOp = Add;
		SrcBlend = One;
		DestBlend = One;

		ZWriteEnable = false;
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Render_LightAcc_Point
	{
		AlphaTestEnable = false;

		AlphaBlendenable = true;
		BlendOp = Add;
		SrcBlend = One;
		DestBlend = One;

		ZWriteEnable = false;
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_LIGHT_POINT();
	}


	pass Render_Blend
	{
		ZWriteEnable = false;
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 0;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_BLEND();
	}
}