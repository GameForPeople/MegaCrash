#pragma once

class CTexture;
class CShader;
class CGlobalMaterial
{
public:
	static constexpr UINT MAX_MATERIALS{ 64 };

	struct MATERIAL
	{
		XMFLOAT4				m_xmf4Ambient;
		XMFLOAT4				m_xmf4Diffuse;
		XMFLOAT4				m_xmf4Specular; //(r,g,b,a=glossiness_factor)
		XMFLOAT4				m_xmf4Emissive;
	};

private:
	MATERIAL					m_pReflections[MAX_MATERIALS];

	MATERIAL*					m_pcbMapped = nullptr;
	CB_DESC						m_cbMaterialsDesc;

public:
	CGlobalMaterial();
	
	void CreateShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCmdLst);
	
	void SetMaterial(int nIndex, MATERIAL *pMaterial) { m_pReflections[nIndex] = *pMaterial; }
};

class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();

public:
	XMFLOAT4						m_xmf4Albedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	UINT							m_GlobalMaterialIdx = 0;
	shared_ptr<CTexture>			m_pTexture;
	shared_ptr<CShader>				m_pShader;
	shared_ptr<CShader>				m_pShadowMapShader;

	void SetAlbedo(XMFLOAT4 xmf4Albedo) { m_xmf4Albedo = xmf4Albedo; }
	void SetGlobalMaterialIdx(UINT idx) { m_GlobalMaterialIdx = idx; }
	void SetTexture(const shared_ptr<CTexture>& pTexture);
	void SetShader(const shared_ptr<CShader>& pShader);
	void SetShadowMapShader(const shared_ptr<CShader>& pShader);

	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst);
	void ReleaseShaderVariables();
};