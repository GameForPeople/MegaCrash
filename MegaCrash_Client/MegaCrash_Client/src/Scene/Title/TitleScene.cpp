//-----------------------------------------------------------------------------
// File: TitleScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Framework\Framework.h"
#include "Object/Font/Font.h"
#include "UI/UI.h"
#include "TitleScene.h"


CTitleScene::CTitleScene()
	: CScene()
{
}
CTitleScene::~CTitleScene()
{
}

void CTitleScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCmdLst)
{
	m_pLights = make_shared<CLight>();
	m_pMaterials = make_shared<CGlobalMaterial>();

	m_pSkyBox = make_shared<CSkyBox>(pd3dDevice, pd3dCmdLst);

	m_pPlayer = m_pFramework->GetPlayer();
	m_pPlayer->GetCamera()->Rotate(-15.0f, 15.0f, -15.0f);
	m_pPlayer->GetCamera()->SetPosition(XMFLOAT3(25000.0f, 25000.0f, 25000.0f));

	m_pFont = make_shared<CFont>(
		  pd3dDevice
		, pd3dCmdLst
		, eTex::Font::Default
		, CFont::DefaultFontPath
		, 128
		, CLIENT_WIDTH
		, CLIENT_HEIGHT);

	m_pLogoImage = make_shared<CButton>();
	m_pLogoImage->SetMaterial(eMat::Object::LobbyLogo);
	m_pLogoImage->SetInfoCenterPos(XMFLOAT2{ 0.0f, 0.5f });
	m_pLogoImage->SetInfoButtonSize(XMFLOAT2{ 2000.f / 1600.f, 1500.f / 900.f });

	m_pConnectButton = make_shared<CButton>();
	m_pConnectButton->SetMaterial(eMat::Object::ConnectButton);
	m_pConnectButton->SetInfoCenterPos(XMFLOAT2{ -0.5f, -0.5f });
	m_pConnectButton->SetInfoButtonSize(XMFLOAT2{ 512.f / 1600.f, 256.f / 900.f });

	m_pExitButton = make_shared<CButton>();
	m_pExitButton->SetMaterial(eMat::Object::ExitButton);
	m_pExitButton->SetInfoCenterPos(XMFLOAT2{ 0.5f, -0.5f });
	m_pExitButton->SetInfoButtonSize(XMFLOAT2{ 512.f / 1600.f, 256.f / 900.f });

	CreateShaderVariables(pd3dDevice, pd3dCmdLst);
}

void CTitleScene::ReleaseObjects()
{
	ReleaseShaderVariables();
}

void CTitleScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCmdLst)
{
	m_pLights->CreateShaderVariables();
	m_pMaterials->CreateShaderVariables();
}
void CTitleScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst)
{
	m_pLights->UpdateShaderVariables(pd3dCmdLst);
	m_pMaterials->UpdateShaderVariables(pd3dCmdLst);
	m_pShaderHelper->UpdateShaderVariables(pd3dCmdLst);
}
void CTitleScene::ReleaseShaderVariables()
{
}

bool CTitleScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		if (m_pConnectButton->IsCursorOn()) {
			CClient::Instance()->ConnectServer();
		}
		else if (m_pExitButton->IsCursorOn()) {
			::PostQuitMessage(0);
		}
		break;
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		::GetCursorPos(&m_ptCursorPos);
		::ScreenToClient(hWnd, &m_ptCursorPos);
		m_pConnectButton->CollisionCheck(m_ptCursorPos);
		m_pExitButton->CollisionCheck(m_ptCursorPos);
		break;
	default:
		break;
	}
	return false;
}

bool CTitleScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		break;
	case WM_KEYDOWN:
		break;
	}

	return false;
}

bool CTitleScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_ACTIVATE:
		break;
	case WM_SIZE:
		break;
	}
	return false;
}

bool CTitleScene::ProcessInput(UCHAR *pKeysBuffer, float fTimeElapsed)
{
	return false;
}

void CTitleScene::AnimateObjects(float fTimeElapsed)
{
	m_pShaderHelper->UpdateTimer(fTimeElapsed);
	m_pPlayer->GetCamera()->Rotate(0, 0.01f, 0);
	m_pPlayer->GetCamera()->Update(fTimeElapsed);
}

void CTitleScene::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
}

void CTitleScene::Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	static ID3D12RootSignature* pRootSignature = CResMgr::Instance()->GetRootSignature();
	pd3dCmdLst->SetGraphicsRootSignature(pRootSignature);
	pCamera->SetViewportsAndScissorRects(pd3dCmdLst);
	pCamera->UpdateShaderVariables(pd3dCmdLst);
	UpdateShaderVariables(pd3dCmdLst);

	m_pSkyBox->Render(pd3dCmdLst, pCamera);
	for (int i = 0; i < 500; ++i) {
		m_pObjectManager->GetVecCubes()[i]->Render(pd3dCmdLst, pCamera);
	}
}

void CTitleScene::RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	static ID3D12RootSignature* pRootSignature = CResMgr::Instance()->GetRootSignature();

	pd3dCmdLst->SetGraphicsRootSignature(pRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCmdLst);
	pCamera->UpdateShaderVariables(pd3dCmdLst);
	UpdateShaderVariables(pd3dCmdLst);

	m_pLogoImage->Render(pd3dCmdLst, pCamera);

	m_pConnectButton->Render(pd3dCmdLst, pCamera);
	m_pExitButton->Render(pd3dCmdLst, pCamera);
}

void CTitleScene::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
}

#if USE_SERVER_CONNECT
void CTitleScene::ProcessMsgs()
{
	static BYTE* msg{ nullptr };
	static CClient* pClient = CClient::Instance();

	for (;;) {
		msg = pClient->MsgDequeue();
		if (msg == nullptr) break;

		switch (msg[1])
		{
		case MSGTYPE::MSGSTATE::CONNECT:
		{
			PK_MSG_ADD_PLAYER* p = reinterpret_cast<PK_MSG_ADD_PLAYER*>(msg);
			pClient->SetClientID(p->id);
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjectManager->CreatePlayerObject(
				  pClient->GetClientID()
				, CPlayer::Tag::Ruby
				, XMFLOAT3(17407.0, 31263.0, 28868.0)));
			pPlayer->GetPlayerUI()->SetPlayerInfo({ 1, 1, 1 });
			m_pObjectManager->PlayerSetting(pClient->GetClientID());
			m_pObjectManager->ResetPlayerLst();
			m_pFramework->ChangePlayerObject(pPlayer);
			m_pFramework->ChangeCurrScene(CScene::Tag::Lobby);
			break;
		}
		}
		delete[] msg;
		msg = nullptr;
	}
}

void CTitleScene::SendMsgs(BYTE MsgType)
{
}
#endif