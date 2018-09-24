//-----------------------------------------------------------------------------
// File: TitleScene.h
//-----------------------------------------------------------------------------
#pragma once

#include "Scene/Scene.h"

class CFont;
class CButton;
class CTitleScene : public CScene
{
public:
	CTitleScene();
	virtual ~CTitleScene();

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
	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera=NULL);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = NULL);
	virtual void RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = NULL);
	virtual void RenderShadowMap(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = NULL);

#if USE_SERVER_CONNECT
	virtual void ProcessMsgs();
	virtual void SendMsgs(BYTE MsgType);
#endif

protected:
	shared_ptr<CLight>					m_pLights;
	shared_ptr<CGlobalMaterial>			m_pMaterials;

	shared_ptr<CSkyBox>					m_pSkyBox;

	CPlayer*							m_pPlayer;

	shared_ptr<CFont>					m_pFont;
	shared_ptr<CButton>					m_pLogoImage;
	shared_ptr<CButton>					m_pConnectButton;
	shared_ptr<CButton>					m_pExitButton;

	POINT								m_ptCursorPos;
};
