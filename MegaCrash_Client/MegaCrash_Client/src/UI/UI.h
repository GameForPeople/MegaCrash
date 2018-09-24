//------------------------------------------------------- ----------------------
// File: UI.h
//-----------------------------------------------------------------------------
#pragma once
#include "Texture\Texture.h"
#include "Material\Material.h"
#include "Object/Font/Font.h"

class CCamera;
class CShader;
class CFont;

class CLogoUI
{
public:
	struct CB_INFO
	{
	};

public:
	CLogoUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual ~CLogoUI();

protected:
	shared_ptr<CMaterial>				m_pMaterial;

	CB_INFO								m_cbInfo;
	CB_INFO*							m_pcbMapped;
	CB_DESC								m_cbDesc;

public:
	void SetShader(const shared_ptr<CShader>& pShader);
	void SetTextureSet(const shared_ptr<CTexture>& pTexture);
	void SetMaterial(const shared_ptr<CMaterial>& pMaterial);

	void CreateShaderVariables();
	void ReleaseShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst);

	void Animate(float fTimeElapsed);

	void RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr);
};

class CWindowUI
{
public:
	struct CB_INFO
	{
		XMFLOAT2 Center;
		XMFLOAT2 Size;
	};

public:
	CWindowUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual ~CWindowUI();

protected:
	shared_ptr<CMaterial>				m_pMaterial;

	CB_INFO								m_cbInfo;
	CB_INFO*							m_pcbMapped;
	CB_DESC								m_cbDesc;

public:
	void SetShader(const shared_ptr<CShader>& pShader);
	void SetTextureSet(const shared_ptr<CTexture>& pTexture);
	void SetMaterial(const shared_ptr<CMaterial>& pMaterial);

	void CreateShaderVariables();
	void ReleaseShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst);

	void Animate(float fTimeElapsed);

	void Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr);

	void SetInfo(const CB_INFO& info) { m_cbInfo = info; }
	void SetInfoCenterPos(const XMFLOAT2& xmf2Center) { m_cbInfo.Center = xmf2Center; }
	void SetInfoWindowSize(const XMFLOAT2& xmf2Size) { m_cbInfo.Size = xmf2Size; }
};

class CTextBoxUI
{
public:
	struct CB_INFO
	{
		XMFLOAT2 Center;
		XMFLOAT2 Size;
		UINT bCursorOn;
	};

public:
	CTextBoxUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual ~CTextBoxUI();

protected:
	shared_ptr<CMaterial>				m_pMaterial;

	CB_INFO								m_cbInfo;
	CB_INFO*							m_pcbMapped;
	CB_DESC								m_cbDesc;

	UINT								m_RoomID;
	bool								m_bSelected;
	wstring								m_str;
	shared_ptr<CFont>					m_pFont;

public:
	void SetShader(const shared_ptr<CShader>& pShader);
	void SetTextureSet(const shared_ptr<CTexture>& pTexture);
	void SetMaterial(const shared_ptr<CMaterial>& pMaterial);

	void CreateShaderVariables();
	void ReleaseShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst);

	void Animate(float fTimeElapsed);

	void Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr);

	void SetText(const wstring& str);
	void SetInfo(const CB_INFO& info) { m_cbInfo = info; }
	void SetInfoCenterPos(const XMFLOAT2& xmf2Center) { m_cbInfo.Center = xmf2Center; }
	void SetInfoWindowSize(const XMFLOAT2& xmf2Size) { m_cbInfo.Size = xmf2Size; }
	void SetInfoCursorOn(bool bCursorOn) { m_cbInfo.bCursorOn = bCursorOn; }
	void SetRoomID(UINT id) { m_RoomID = id; SetText(L"A room name of MegaCrash " + std::to_wstring(id)); }
	UINT GetRoomID() const { return m_RoomID; }

	void CollisionCheck(const POINT& pos);
	void Select() { m_bSelected = true; }
	void ResetSelect() { m_bSelected = false; }
	void ResetFont() { m_str.clear(); m_pFont->ResetTextData(); }
	bool IsCursorOn() const { return m_cbInfo.bCursorOn; }
	bool IsSelected() const { return m_bSelected; }
};

class CSkillIconUI
{
public:
	struct CB_INFO
	{
		XMFLOAT2 LeftTop;
		XMFLOAT2 IconSize;
		float cool_time;
		XMFLOAT3 padding;
	};

public:
	CSkillIconUI();
	~CSkillIconUI();
	void CreateShaderVariables();
	void ReleaseShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst);

	void Render(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera = nullptr);

	void SetSkillIconRenderInfo(const CB_INFO& info) { m_cbInfo = info; }
	void SetSkillIconRenderInfoCoolTime(float time) { m_cbInfo.cool_time = time; }
	void SetSkillIconRenderInfoLeftTop(XMFLOAT2 LeftTop) { m_cbInfo.LeftTop = LeftTop; }
	void SetSkillIconRenderInfoIconSize(XMFLOAT2 IconSize) { m_cbInfo.IconSize = IconSize; }
	void SetSkillIconImage(eMat::Object obj);

private:
	shared_ptr<CMaterial>			m_pMaterial;

	CB_INFO							m_cbInfo;
	CB_INFO*						m_pcbMapped;
	CB_DESC							m_cbDesc;
};

class CCrosshair
{
public:
	struct CB_INFO
	{
		XMFLOAT2 Center{ 0,0 };
		XMFLOAT2 BaseRadius{ 100.0f / static_cast<float>(CLIENT_WIDTH), 100.0f / static_cast<float>(CLIENT_HEIGHT) };
		float DamagedTime{ 0 };
		XMFLOAT3 padding{ 0,0,0 };
	};

	CCrosshair();
	~CCrosshair();
	void CreateShaderVariables();
	void ReleaseShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst);

	void Render(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera = nullptr);

	void SetCrosshairInfoDamagedTime(float time) { m_cbInfo.DamagedTime = time; }

private:
	shared_ptr<CMaterial>			m_pMaterial;

	CB_INFO							m_cbInfo;
	CB_INFO*						m_pcbMapped;
	CB_DESC							m_cbDesc;
};

class CButton
{
public:
	struct CB_INFO
	{
		XMFLOAT2 Center;
		XMFLOAT2 Size;
		UINT bCursorOn;
	};

public:
	CButton();
	~CButton();
	void CreateShaderVariables();
	void ReleaseShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst);

	void Render(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera = nullptr);

	void SetMaterial(eMat::Object obj);
	void SetInfo(const CB_INFO& info) { m_cbInfo = info; }
	void SetInfoCenterPos(const XMFLOAT2& xmf2Center) { m_cbInfo.Center = xmf2Center; }
	void SetInfoButtonSize(const XMFLOAT2& xmf2Size) { m_cbInfo.Size = xmf2Size; }
	void SetInfoCursorOn(bool bCursorOn) { m_cbInfo.bCursorOn = bCursorOn; }

	void CollisionCheck(const POINT& pos);
	bool IsCursorOn() const { 
		if (m_cbInfo.bCursorOn == true)
			CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::UI_ON);
		return m_cbInfo.bCursorOn; 
	}

private:
	shared_ptr<CMaterial>			m_pMaterial;

	CB_INFO							m_cbInfo;
	CB_INFO*						m_pcbMapped;
	CB_DESC							m_cbDesc;
};

class CGameEndUI
{
public:
	struct CB_INFO
	{
		XMFLOAT4	Color;
		XMFLOAT2	Center;
		XMFLOAT2	Size;
		float		StartTime{0};
		float		isVictory{0};
		XMFLOAT2    padding;
	};

public:
	CGameEndUI();
	~CGameEndUI();
	void CreateShaderVariables();
	void ReleaseShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst);

	void Render(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera = nullptr);

	void SetMaterial(eMat::Object obj);
	void SetInfo(const CB_INFO& info) { m_cbInfo = info; }
	void SetInfoCenterPos(const XMFLOAT2& xmf2Center) { m_cbInfo.Center = xmf2Center; }
	void SetInfoWindowSize(const XMFLOAT2& xmf2Size) { m_cbInfo.Size = xmf2Size; }
	void SetInfoStartTime(float fStartTime) { m_cbInfo.StartTime = fStartTime; }
	void SetInfoColor(const XMFLOAT4& xmf4Color) { m_cbInfo.Color = xmf4Color; }
	void SetIsVictory(float _isVictory) { m_cbInfo.isVictory = _isVictory; }

private:
	shared_ptr<CMaterial>			m_pMaterial;

	CB_INFO							m_cbInfo;
	CB_INFO*						m_pcbMapped;
	CB_DESC							m_cbDesc;
};

class CDeathUI
{
public:
	struct CB_INFO
	{
		XMFLOAT4	Color;
		float		StartTime;
	};

public:
	CDeathUI();
	~CDeathUI();
	void CreateShaderVariables();
	void ReleaseShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst);

	void Render(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera = nullptr);

	void SetMaterial(eMat::Object obj);
	void SetInfo(const CB_INFO& info) { m_cbInfo = info; }
	void SetInfoStartTime(float fStartTime) { m_cbInfo.StartTime = fStartTime; }
	void SetInfoColor(const XMFLOAT4& xmf4Color) { m_cbInfo.Color = xmf4Color; }


private:
	shared_ptr<CMaterial>			m_pMaterial;

	CB_INFO							m_cbInfo;
	CB_INFO*						m_pcbMapped;
	CB_DESC							m_cbDesc;
};