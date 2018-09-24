//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------
#pragma once

class CFramework;
class CCamera;
class CObjectMgr;
class CParticleMgr;
class CShaderHelper;
class CScene abstract
{
public:
	enum class Tag { Logo, Title, Lobby, Main };

public:
    CScene();
	virtual ~CScene();

	void SetHInstanceAndHWnd(HINSTANCE hInstance, HWND hWnd) { m_hInstance = hInstance; m_hWnd = hWnd; }
	void SetFramework(CFramework* pFramework) { m_pFramework = pFramework; }
	void SetObjectMgr(const shared_ptr<CObjectMgr>& pObjectMgr) { m_pObjectManager = pObjectMgr; }
	void SetShaderHelper(const shared_ptr<CShaderHelper>& pShaderHelper) { m_pShaderHelper = pShaderHelper; }
	void SetParticleMgr(const shared_ptr<CParticleMgr>& pParticleMgr) { m_pParticleManager = pParticleMgr; }

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual bool OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst) = 0;
	virtual void ReleaseObjects() = 0;
	virtual void ResetObjects() = 0;

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst) = 0;
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCmdLst) = 0;
	virtual void ReleaseShaderVariables() = 0;

	virtual bool ProcessInput(UCHAR* pKeysBuffer, float fTimeElapsed) = 0;
	virtual void AnimateObjects(float fTimeElapsed) = 0;
	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr) = 0;
	virtual void Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr) = 0;
	virtual void RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr) = 0;
	virtual void RenderShadowMap(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr) = 0;

#if USE_SERVER_CONNECT
	virtual void ProcessMsgs() = 0;
	virtual void SendMsgs(BYTE MsgType) = 0;
#endif

protected:
	HINSTANCE								m_hInstance;
	HWND									m_hWnd;

	CFramework*								m_pFramework;

	shared_ptr<CShaderHelper>				m_pShaderHelper;
	shared_ptr<CObjectMgr>					m_pObjectManager;
	shared_ptr<CParticleMgr>				m_pParticleManager;
};
