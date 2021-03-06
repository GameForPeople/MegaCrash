#pragma once

class CLight
{
public:
	static constexpr UINT MAX_LIGHTS{ 8 };

	typedef enum Type {
		  POINT_LIGHT = 1
		, SPOT_LIGHT
		, DIRECTIONAL_LIGHT
	} Type;
	struct LIGHT_PARAMETER
	{
		XMFLOAT4				xmf4Ambient;
		XMFLOAT4				xmf4Diffuse;
		XMFLOAT4				xmf4Specular;
		XMFLOAT3				xmf3Position;
		float 					fFalloff;
		XMFLOAT3				xmf3Direction;
		float 					fTheta; //cos(m_fTheta)
		XMFLOAT3				xmf3Attenuation;
		float					fPhi; //cos(m_fPhi)
		bool					bEnable;
		UINT					nType;
		float					fRange;
		float					padding;
	};
	struct LIGHTS
	{
		LIGHT_PARAMETER			params[MAX_LIGHTS];
		XMFLOAT4				xmf4GlobalAmbient;
	};

private:
	LIGHTS						m_Lights;

	LIGHTS*						m_pcbMapped;
	CB_DESC						m_cbLightsDesc;

public:
	CLight();

	void CreateShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCmdLst);

	LIGHT_PARAMETER* GetLight(UINT light_idx) { return &m_Lights.params[light_idx]; }

	void SetPos(UINT light_idx, const XMFLOAT3& pos) { m_Lights.params[light_idx].xmf3Position = pos; }
	void SetDir(UINT light_idx, const XMFLOAT3& dir) { m_Lights.params[light_idx].xmf3Direction = dir; }
};
