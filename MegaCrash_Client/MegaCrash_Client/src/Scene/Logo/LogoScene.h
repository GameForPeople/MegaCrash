//-----------------------------------------------------------------------------
// File: LogoScene.h
//-----------------------------------------------------------------------------
#pragma once

#include "Scene/Scene.h"

constexpr float LOGO_WAIT_TIME	{ 3.0f };

class CFont;
class CLogoUI;
class CLogoScene : public CScene
{
public:
	CLogoScene();
	virtual ~CLogoScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual void ReleaseObjects();
	virtual void ResetObjects() {}

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
	virtual void ProcessMsgs() {}
	virtual void SendMsgs(BYTE MsgType) {}
#endif

private:
	shared_ptr<CLogoUI>					m_pUI;
	shared_ptr<CFont>					m_pFont;

	float								m_fLogoTimer;
};
