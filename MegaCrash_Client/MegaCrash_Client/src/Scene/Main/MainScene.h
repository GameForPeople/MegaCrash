//-----------------------------------------------------------------------------
// File: MainScene.h
//-----------------------------------------------------------------------------
#pragma once

#include "Scene/Scene.h"

class CParticleBillboards;
class CCrosshair;
class CFont;
class CButton;
class CGameEndUI;
class CDeathUI;
class CTextBoxUI;
class CMainScene : public CScene
{
public:
	CMainScene();
	virtual ~CMainScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual void ReleaseObjects();
	virtual void ResetObjects();

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual void ReleaseShaderVariables();

	virtual bool ProcessInput(UCHAR* pKeysBuffer, float fTimeElapsed);
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr);
	virtual void RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr);
	virtual void RenderShadowMap(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr);

#if USE_SERVER_CONNECT
	virtual void ProcessMsgs();
	virtual void SendMsgs(BYTE MsgType);
#endif

	void RenderDamage(const XMFLOAT3& pos, const XMFLOAT3& right, UINT nDamage);

private:
	shared_ptr<CLight>					m_pLights;
	shared_ptr<CGlobalMaterial>			m_pMaterials;

	CPlayer*							m_pPlayer;

	shared_ptr<CSkyBox>					m_pSkyBox;

	shared_ptr<CShader>					m_pHUDUIShader;
	shared_ptr<CShader>					m_pRaderUIShader;
	shared_ptr<CCrosshair>				m_pCrossHair;

	XMFLOAT2							m_szUIDamageBillboard;
	shared_ptr<CParticleBillboards>		m_pUIDamageBillboard;

	shared_ptr<CFont>					m_pDefaultFont;
	shared_ptr<CButton>					m_pExitButton;

	shared_ptr<CFont>					m_pKillCounter;

	POINT								m_ptOldCursorPos;
	POINT								m_ptCursorPos;

	shared_ptr<CGameEndUI>				m_pGameEnd;
	shared_ptr<CDeathUI>				m_pDeathUI;

	bool								m_bPlayerDie;
	float								m_fPlayerDieTimer;

	bool								m_bGameover;

	int									m_GlobalKillCount = 0;
	int									m_PlayerKillCount = 0;
};
