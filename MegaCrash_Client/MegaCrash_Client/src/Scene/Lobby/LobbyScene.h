//-----------------------------------------------------------------------------
// File: LobbyScene.h
//-----------------------------------------------------------------------------
#pragma once

#include "Scene/Scene.h"
#include "Shader/Shader.h"
#include "Manager/ObjectMgr/ObjectMgr.h"
#include "Object/Player/Player.h"
#include "Light/Light.h"
#include "Material/Material.h"
#include "Shader/Helper/ShaderHelper.h"

class CCrosshair;
class CFont;
class CWindowUI;
class CTextBoxUI;
class CLobbyScene : public CScene
{
public:
	CLobbyScene();
	virtual ~CLobbyScene();

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

	CPlayer*							m_pPlayer;
	vector<XMFLOAT3>					m_vecPlayerPosLst;
	UINT								m_PlayerPosLstIdx;

	shared_ptr<CSkyBox>					m_pSkyBox;

	shared_ptr<CFont>					m_pFont;

	shared_ptr<CWindowUI>				m_pLobbyBaseFrame;
	vector<shared_ptr<CTextBoxUI>>		m_vecLobbyRoomLst;

	shared_ptr<CButton>					m_pLogoImage;

	shared_ptr<CButton>					m_pCreateRoomButton;
	shared_ptr<CButton>					m_pEnterRoomButton;
	shared_ptr<CButton>					m_pExitButton;
	shared_ptr<CButton>					m_pRecycleButton;

	shared_ptr<CButton>					m_pLeaveRoomButton;
	shared_ptr<CButton>					m_pStartGameButton;

	shared_ptr<CButton>					m_pRubyButton;
	shared_ptr<CButton>					m_pLegionButton;
	shared_ptr<CButton>					m_pEpsilonButton;
	shared_ptr<CButton>					m_pGravisButton;

	POINT								m_ptCursorPos;
	UINT								m_nRoomCounter;
	
	float								m_RoomRefreshCounter;
	bool								m_bOwner;
};
