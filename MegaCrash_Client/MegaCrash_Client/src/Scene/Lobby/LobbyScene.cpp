//-----------------------------------------------------------------------------
// File: LobbyScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Framework\Framework.h"
#include "Object/Font/Font.h"
#include "UI/UI.h"
#include "LobbyScene.h"

CLobbyScene::CLobbyScene()
	: CScene()
	, m_pPlayer				{ nullptr }
	, m_RoomRefreshCounter	{ 0.0f }
	, m_bOwner				{ false }
{
}
CLobbyScene::~CLobbyScene()
{
}

void CLobbyScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCmdLst)
{
	m_pLights = make_shared<CLight>();
	m_pMaterials = make_shared<CGlobalMaterial>();

	// SkyBox
	m_pSkyBox = make_shared<CSkyBox>(pd3dDevice, pd3dCmdLst);

	// UI
	m_pFont = make_shared<CFont>(
		  pd3dDevice
		, pd3dCmdLst
		, eTex::Font::Default
		, CFont::DefaultFontPath
		, 1024
		, CLIENT_WIDTH
		, CLIENT_HEIGHT);

	m_pLobbyBaseFrame = make_shared<CWindowUI>(pd3dDevice, pd3dCmdLst);
	m_pLobbyBaseFrame->SetInfoCenterPos(XMFLOAT2{ 0.62f, 0.0f });
	m_pLobbyBaseFrame->SetInfoWindowSize(XMFLOAT2{ 1000.f / 1600.f, 1600.f / 900.f });

	const int numSlot{ 13 };
	m_vecLobbyRoomLst.reserve(numSlot);
	for (int i = 0; i < numSlot; ++i) {
		shared_ptr<CTextBoxUI> p{ make_shared<CTextBoxUI>(pd3dDevice, pd3dCmdLst) };
		p->SetInfoCenterPos(XMFLOAT2{ 0.623f, 0.75f - (0.125f * static_cast<float>(i)) });
		p->SetInfoWindowSize(XMFLOAT2{ 900.f / 1600.f, 100.f / 900.f });
		m_vecLobbyRoomLst.push_back(p);
	}

	m_pLogoImage = make_shared<CButton>();
	m_pLogoImage->SetMaterial(eMat::Object::LobbyLogo);
	m_pLogoImage->SetInfoCenterPos(XMFLOAT2{ -0.8f, 0.9f });
	m_pLogoImage->SetInfoButtonSize(XMFLOAT2{ 600.f / 1600.f, 450.f / 900.f });

	m_pCreateRoomButton = make_shared<CButton>();
	m_pCreateRoomButton->SetMaterial(eMat::Object::CreateButton);
	m_pCreateRoomButton->SetInfoCenterPos(XMFLOAT2{ -0.83f, -0.8f });
	m_pCreateRoomButton->SetInfoButtonSize(XMFLOAT2{ 384.f / 1600.f, 192.f / 900.f });

	m_pEnterRoomButton = make_shared<CButton>();
	m_pEnterRoomButton->SetMaterial(eMat::Object::EnterButton);
	m_pEnterRoomButton->SetInfoCenterPos(XMFLOAT2{ -0.83f, -0.58f });
	m_pEnterRoomButton->SetInfoButtonSize(XMFLOAT2{ 384.f / 1600.f, 192.f / 900.f });

	m_pExitButton = make_shared<CButton>();
	m_pExitButton->SetMaterial(eMat::Object::ExitButton);
	m_pExitButton->SetInfoCenterPos(XMFLOAT2{ -0.54f, -0.83f });
	m_pExitButton->SetInfoButtonSize(XMFLOAT2{ 256.f / 1600.f, 128.f / 900.f });

	m_pRecycleButton = make_shared<CButton>();
	m_pRecycleButton->SetMaterial(eMat::Object::RecycleButton);
	m_pRecycleButton->SetInfoCenterPos(XMFLOAT2{ -0.675f, -0.845f });
	m_pRecycleButton->SetInfoButtonSize(XMFLOAT2{ 100.f / 1600.f, 100.f / 900.f });

	m_pLeaveRoomButton = make_shared<CButton>();
	m_pLeaveRoomButton->SetMaterial(eMat::Object::LeaveButton);
	m_pLeaveRoomButton->SetInfoCenterPos(XMFLOAT2{ -0.83f, -0.8f });
	m_pLeaveRoomButton->SetInfoButtonSize(XMFLOAT2{ 384.f / 1600.f, 192.f / 900.f });

	m_pStartGameButton = make_shared<CButton>();
	m_pStartGameButton->SetMaterial(eMat::Object::StartButton);
	m_pStartGameButton->SetInfoCenterPos(XMFLOAT2{ 0.0f, -0.8f });
	m_pStartGameButton->SetInfoButtonSize(XMFLOAT2{ 512.f / 1600.f, 256.f / 900.f });

	m_pRubyButton = make_shared<CButton>();
	m_pRubyButton->SetMaterial(eMat::Object::RubyButton);
	m_pRubyButton->SetInfoCenterPos(XMFLOAT2{ -0.36f, -0.7f });
	m_pRubyButton->SetInfoButtonSize(XMFLOAT2{ 200.f / 1600.f, 200.f / 900.f });

	m_pLegionButton = make_shared<CButton>();
	m_pLegionButton->SetMaterial(eMat::Object::LegionButton);
	m_pLegionButton->SetInfoCenterPos(XMFLOAT2{ -0.23f, -0.75f });
	m_pLegionButton->SetInfoButtonSize(XMFLOAT2{ 200.f / 1600.f, 200.f / 900.f });

	m_pEpsilonButton = make_shared<CButton>();
	m_pEpsilonButton->SetMaterial(eMat::Object::EpsilonButton);
	m_pEpsilonButton->SetInfoCenterPos(XMFLOAT2{ 0.23f, -0.75f });
	m_pEpsilonButton->SetInfoButtonSize(XMFLOAT2{ 200.f / 1600.f, 200.f / 900.f });

	m_pGravisButton = make_shared<CButton>();
	m_pGravisButton->SetMaterial(eMat::Object::GravisButton);
	m_pGravisButton->SetInfoCenterPos(XMFLOAT2{ 0.36f, -0.7f });
	m_pGravisButton->SetInfoButtonSize(XMFLOAT2{ 200.f / 1600.f, 200.f / 900.f });
	
	CreateShaderVariables(pd3dDevice, pd3dCmdLst);
}

void CLobbyScene::ReleaseObjects()
{
	ReleaseShaderVariables();
}

void CLobbyScene::ResetObjects()
{
	::ReleaseCapture();

	XMFLOAT4X4 mtx{ Matrix4x4::Identity() };
	for (auto p : m_pObjectManager->GetVecStages()) {
		p->SetWorldMtx(mtx);
		p->DelayTransformation(XMFLOAT3(16262.f, 16766.f, 26967.f), Vector4::QuatIdentity(), 0.01f);
		p->UpdateTransform();
	}

	m_pPlayer = m_pFramework->GetPlayer();
	m_pPlayer->ResetCameraOffset();
	m_pPlayer->DelayTransformation(XMFLOAT3(21824.2f, 26230.f, 35971.6f), Vector4::QuatIdentity(), 0.01f);
	m_pPlayer->SyncMemberVariableToMatrix();
	m_pPlayer->UpdateTransform();
	m_pPlayer->GetCamera()->ResetRotate();
	m_pPlayer->GetCamera()->Rotate(-15.f, -160.f, 0.f);

	m_PlayerPosLstIdx = 0;
	if (m_vecPlayerPosLst.size() == 0) {
		for (int i = 0; i < 5; ++i) {
			m_vecPlayerPosLst.push_back(Vector3::ScalarProduct(Vector3::Add(
				Vector3::ScalarProduct(m_pPlayer->GetCamera()->GetLookVector(), -1.0f)
				, m_pPlayer->GetCamera()->GetRightVector())
				, static_cast<float>(i + 1) * 100.f));
			m_vecPlayerPosLst.push_back(Vector3::ScalarProduct(Vector3::Add(
				Vector3::ScalarProduct(m_pPlayer->GetCamera()->GetLookVector(), -1.0f)
				, m_pPlayer->GetCamera()->GetRightVector(), -1.0f)
				, static_cast<float>(i + 1) * 100.f));
		}
	}
	SendMsgs(MSGTYPE::MSGROOM::REFRESHROOMINFO);
}

void CLobbyScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCmdLst)
{
	m_pLights->CreateShaderVariables();
	m_pMaterials->CreateShaderVariables();
}
void CLobbyScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst)
{
	m_pLights->UpdateShaderVariables(pd3dCmdLst);
	m_pMaterials->UpdateShaderVariables(pd3dCmdLst);
	m_pShaderHelper->UpdateShaderVariables(pd3dCmdLst);
}
void CLobbyScene::ReleaseShaderVariables()
{
}

bool CLobbyScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
	{
		static auto pClient = CClient::Instance();
		if (pClient->IsEnteredRoom()) {
			if (m_pLeaveRoomButton->IsCursorOn()) {
				SendMsgs(MSGTYPE::MSGROOM::DISCONNECTROOM);
				SendMsgs(MSGTYPE::MSGROOM::REFRESHROOMINFO);
			}
			else if (m_bOwner && m_pStartGameButton->IsCursorOn()) {
				SendMsgs(MSGTYPE::MSGROOM::STARTROOM);
			}
			else if (m_pRubyButton->IsCursorOn()) {
				PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(pClient->GetSendBuf());
				p->size = sizeof(PK_ROOM_INFO);
				p->type = MSGTYPE::MSGROOM::CLIENT_TYPECHANGE;
				p->RoomID = pClient->GetRoomID();
				p->ClientID = m_pPlayer->GetID();
				p->ClientType = static_cast<BYTE>(CPlayer::Tag::Ruby);
				pClient->SendMsg(p->size);
			}
			else if (m_pLegionButton->IsCursorOn()) {
				PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(pClient->GetSendBuf());
				p->size = sizeof(PK_ROOM_INFO);
				p->type = MSGTYPE::MSGROOM::CLIENT_TYPECHANGE;
				p->RoomID = pClient->GetRoomID();
				p->ClientID = m_pPlayer->GetID();
				p->ClientType = static_cast<BYTE>(CPlayer::Tag::Legion);
				pClient->SendMsg(p->size);
			}
			else if (m_pEpsilonButton->IsCursorOn()) {
				PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(pClient->GetSendBuf());
				p->size = sizeof(PK_ROOM_INFO);
				p->type = MSGTYPE::MSGROOM::CLIENT_TYPECHANGE;
				p->RoomID = pClient->GetRoomID();
				p->ClientID = m_pPlayer->GetID();
				p->ClientType = static_cast<BYTE>(CPlayer::Tag::Epsilon);
				pClient->SendMsg(p->size);
			}
			else if (m_pGravisButton->IsCursorOn()) {
				PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(pClient->GetSendBuf());
				p->size = sizeof(PK_ROOM_INFO);
				p->type = MSGTYPE::MSGROOM::CLIENT_TYPECHANGE;
				p->RoomID = pClient->GetRoomID();
				p->ClientID = m_pPlayer->GetID();
				p->ClientType = static_cast<BYTE>(CPlayer::Tag::Gravis);
				pClient->SendMsg(p->size);
			}
		}
		else {
			if (m_pCreateRoomButton->IsCursorOn()) {
				SendMsgs(MSGTYPE::MSGROOM::CREATEROOM);
				m_bOwner = true;
			}
			else if (m_pEnterRoomButton->IsCursorOn()) {
				for (auto room_slot : m_vecLobbyRoomLst) {
					if (room_slot->IsSelected()) {
						PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(pClient->GetSendBuf());
						p->size = sizeof(PK_ROOM_INFO);
						p->type = MSGTYPE::MSGROOM::ACCEPTROOM;
						p->RoomID = room_slot->GetRoomID();
						p->ClientID = m_pPlayer->GetID();
						pClient->SendMsg(p->size);
						SendMsgs(MSGTYPE::MSGROOM::REFRESHROOMINFO);
						m_bOwner = false;
						break;
					}
				}
			}
			else if (m_pExitButton->IsCursorOn()) {
				::PostQuitMessage(0);
			}
			else if (m_pRecycleButton->IsCursorOn()) {
				SendMsgs(MSGTYPE::MSGROOM::REFRESHROOMINFO);
			}
			else {
				for (auto p : m_vecLobbyRoomLst) {
					if (p->IsCursorOn()) {
						p->Select();
					}
					else {
						p->ResetSelect();
					}
				}
			}
		}
		break;
	}
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		::GetCursorPos(&m_ptCursorPos);
		::ScreenToClient(hWnd, &m_ptCursorPos);
		if (CClient::Instance()->IsEnteredRoom()) {
			m_pLeaveRoomButton->CollisionCheck(m_ptCursorPos);
			if (m_bOwner) {
				m_pStartGameButton->CollisionCheck(m_ptCursorPos);
			}

			m_pRubyButton->CollisionCheck(m_ptCursorPos);
			m_pLegionButton->CollisionCheck(m_ptCursorPos);
			m_pEpsilonButton->CollisionCheck(m_ptCursorPos);
			m_pGravisButton->CollisionCheck(m_ptCursorPos);
		}
		else {
			for (auto p : m_vecLobbyRoomLst) {
				p->CollisionCheck(m_ptCursorPos);
			}
			m_pCreateRoomButton->CollisionCheck(m_ptCursorPos);
			m_pEnterRoomButton->CollisionCheck(m_ptCursorPos);
			m_pExitButton->CollisionCheck(m_ptCursorPos);
			m_pRecycleButton->CollisionCheck(m_ptCursorPos);
		}
		break;
	default:
		break;
	}
	return false;
}

bool CLobbyScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_CONTROL:
			break;
#if USE_SERVER_CONNECT
		case VK_NUMPAD1: SendMsgs(MSGTYPE::MSGROOM::CREATEROOM); break;
		case VK_NUMPAD2: SendMsgs(MSGTYPE::MSGROOM::ACCEPTROOM); break;
		case VK_NUMPAD3: SendMsgs(MSGTYPE::MSGROOM::STARTROOM); break;
		case VK_NUMPAD4: SendMsgs(MSGTYPE::MSGROOM::DISCONNECTROOM); break;
		case VK_NUMPAD5: SendMsgs(MSGTYPE::MSGROOM::CLIENT_TYPECHANGE); break;
#endif
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_CONTROL:
			break;
		}
		break;
	}
	
	return false;
}

bool CLobbyScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{
		break;
	}
	case WM_SIZE:
	{
		break;
	}
	}
	return false;
}

bool CLobbyScene::ProcessInput(UCHAR *pKeysBuffer, float fTimeElapsed)
{

	return false;
}

void CLobbyScene::AnimateObjects(float fTimeElapsed)
{
	m_pShaderHelper->UpdateTimer(fTimeElapsed);
	for (auto p : m_pObjectManager->GetVecPlayers()) {
		if (p->IsActive()) {
			p->SetLookDir(Vector3::Subtract(m_pPlayer->GetCamera()->GetPosition(), p->GetPosition()));
		}
	}
	m_pObjectManager->Animate(fTimeElapsed);

	if (!CClient::Instance()->IsEnteredRoom()) {
		m_RoomRefreshCounter += fTimeElapsed;
		if (m_RoomRefreshCounter >= 5.0f) {
			SendMsgs(MSGTYPE::MSGROOM::REFRESHROOMINFO);
			m_RoomRefreshCounter = 0.f;
		}
	}

	auto sound_mgr = CSoundMgr::Instance();
	sound_mgr->GetListenerHandle().SetParameter(m_pPlayer->GetPosition(), m_pPlayer->GetLookVector(), m_pPlayer->GetUpVector() );
	sound_mgr->Update(fTimeElapsed);
}

void CLobbyScene::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	static ID3D12RootSignature* pRootSignature = CResMgr::Instance()->GetRootSignature();
	pd3dCmdLst->SetGraphicsRootSignature(pRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCmdLst);
	pCamera->UpdateShaderVariables(pd3dCmdLst);

	UpdateShaderVariables(pd3dCmdLst);
}

void CLobbyScene::Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	static auto pShadowMap = CResMgr::Instance()->GetShadowMap();
	OnPrepareRender(pd3dCmdLst, pCamera);
	pShadowMap->Render(pd3dCmdLst);

	if (m_pSkyBox) m_pSkyBox->Render(pd3dCmdLst, pCamera);
	if (m_pObjectManager) m_pObjectManager->Render(pd3dCmdLst, pCamera);
}

void CLobbyScene::RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	static ID3D12RootSignature* pRootSignature = CResMgr::Instance()->GetRootSignature();

	pd3dCmdLst->SetGraphicsRootSignature(pRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCmdLst);
	pCamera->UpdateShaderVariables(pd3dCmdLst);

	vector<CPlayer*>& vecPlayers = m_pObjectManager->GetVecPlayers();
	int i{ 0 };
	for (auto& p : vecPlayers) {
		if (p->IsActive() && p->GetID() != m_pPlayer->GetID()) {
			m_pShaderHelper->SetOtherPlayerPosition(p->GetPosition(), i);
			++i;
		}
	}
	m_pShaderHelper->SetPlayerPosition(m_pPlayer->GetPosition());
	m_pShaderHelper->SetNumOfPlayers(i);
	m_pShaderHelper->SetPlayerLookVector(m_pPlayer->GetCamera()->GetLookVector());

	UpdateShaderVariables(pd3dCmdLst);
	
	m_pLogoImage->Render(pd3dCmdLst, pCamera);

	if (CClient::Instance()->IsEnteredRoom()) {
		m_pLeaveRoomButton->Render(pd3dCmdLst, pCamera);
		if (m_bOwner) {
			m_pStartGameButton->Render(pd3dCmdLst, pCamera);
		}
		m_pRubyButton->Render(pd3dCmdLst, pCamera);
		m_pLegionButton->Render(pd3dCmdLst, pCamera);
		m_pEpsilonButton->Render(pd3dCmdLst, pCamera);
		m_pGravisButton->Render(pd3dCmdLst, pCamera);
	}
	else {
		m_pLobbyBaseFrame->Render(pd3dCmdLst, pCamera);
		for (auto p : m_vecLobbyRoomLst) {
			p->Render(pd3dCmdLst, pCamera);
		}

		m_pCreateRoomButton->Render(pd3dCmdLst, pCamera);
		m_pEnterRoomButton->Render(pd3dCmdLst, pCamera);
		m_pExitButton->Render(pd3dCmdLst, pCamera);
		m_pRecycleButton->Render(pd3dCmdLst, pCamera);
	}
	
	//if (m_pFont) {
	//	m_pFont->ResetTextData();
	//	//m_pFont->Render(pd3dCmdLst, pCamera);
	//}
}

void CLobbyScene::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	static auto BufMgr = CD3DBufMgr::Instance();
	static auto pShadowMap = CResMgr::Instance()->GetShadowMap();
	static auto rootsignature = CResMgr::Instance()->GetRootSignature();

	BufMgr->SynchronizeResourceTransition(
		  pd3dCmdLst
		, pShadowMap->GetDSBuf()
		, D3D12_RESOURCE_STATE_GENERIC_READ
		, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	pd3dCmdLst->ClearDepthStencilView(
		pShadowMap->GetDsv()
		, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL
		, 1.0f, 0, 0, NULL);
	pd3dCmdLst->OMSetRenderTargets(0, nullptr, false, &pShadowMap->GetDsv());
	pd3dCmdLst->SetGraphicsRootSignature(rootsignature);

	pShadowMap->RenderShadowMap(pd3dCmdLst);

	if (m_pObjectManager) m_pObjectManager->RenderShadowMap(pd3dCmdLst, pShadowMap->GetLightCamera());

	BufMgr->SynchronizeResourceTransition(
		  pd3dCmdLst
		, pShadowMap->GetDSBuf()
		, D3D12_RESOURCE_STATE_DEPTH_WRITE
		, D3D12_RESOURCE_STATE_GENERIC_READ);
}

#if USE_SERVER_CONNECT
void CLobbyScene::ProcessMsgs()
{
	static BYTE* msg{ nullptr };
	static CClient* pClient = CClient::Instance();

	for(;;) {
		msg = pClient->MsgDequeue();
		if (msg == nullptr) break;

		switch (msg[1])
		{
		case MSGTYPE::MSGROOM::STARTROOM: {
			PK_ROOM_START* p = reinterpret_cast<PK_ROOM_START*>(msg);
			cout << "Room " << p->RoomID << " Game Start.\n";
			m_pFramework->ChangeCurrScene(CScene::Tag::Main);
			break;
		}
		case MSGTYPE::MSGROOM::ROOMINFO: {
			PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(msg);
			cout << "New Room : " << p->RoomID << "\n";
			m_vecLobbyRoomLst[m_nRoomCounter++ % m_vecLobbyRoomLst.size()]->SetRoomID(p->RoomID);
			break;
		}
		case MSGTYPE::MSGROOM::ROOM_CLIENTINFO: {
			PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(msg);
			cout << "New Client " << p->ClientID << " Enter Room ID : " << p->RoomID << "\n";
			break;
		}
		case MSGTYPE::MSGROOM::ACCEPTROOM: {
			PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(msg);
			cout << "Enter Room " << p->RoomID << " " << p->ClientID << "\n";
			pClient->SetRoomID(p->RoomID);
			break;
		}
		case MSGTYPE::MSGROOM::DISCONNECTROOM: {
			PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(msg);
			cout << "Leave Room(lobby) " << p->RoomID << " " << p->ClientID << "\n";
			if (p->ClientID == pClient->GetClientID()) {
				pClient->LeaveRoom();
				m_pObjectManager->ResetPlayerLst();
				//SendMsgs(MSGTYPE::MSGROOM::REFRESHROOMINFO);
			}
			else {
				m_pObjectManager->DeletePlayerObject(p->ClientID);
			}
			break;
		}
		case MSGTYPE::MSGROOM::CLOSEROOM: {
			PK_ROOM_END* p = reinterpret_cast<PK_ROOM_END*>(msg);
			cout << "Room " << p->RoomID << " Game End.\n";
			break;
		}
		case MSGTYPE::MSGUPDATE::CREATE_OBJECT:
		{
			PK_MSG_ADD_PLAYER* p = reinterpret_cast<PK_MSG_ADD_PLAYER*>(msg);
			cout << "Create Client " << p->id << "\n";
			if (p->id != pClient->GetClientID()) {
				CPlayer* player{ reinterpret_cast<CPlayer*>(m_pObjectManager->CreatePlayerObject(
					  p->id
					, CPlayer::Tag(p->ObjType)
					, Vector3::Add(
						  XMFLOAT3(21824.2f, 26230.f, 35971.6f)
						, m_vecPlayerPosLst[m_PlayerPosLstIdx++ % m_vecPlayerPosLst.size()])
				)) };
			}
			break;
		}
		case MSGTYPE::MSGUPDATE::UPDATE_OBJECT:
		{
			PK_MSG_MOVE* p = reinterpret_cast<PK_MSG_MOVE*>(msg);
			//if (p->id != m_pPlayer->GetID())
			{
				static XMFLOAT4X4 m;
				CGameObject* pObject = m_pObjectManager->Search(p->id);
				if (pObject == nullptr) break;
				switch (p->id / ID_FACTOR)
				{
				case ID_TAG_PLAYER:
				{
					CPlayer* pPlayer = static_cast<CPlayer*>(pObject);
					if (p->id != m_pPlayer->GetID()) {
						pPlayer->DelayTransformation(p->p, p->q, MAX_UPDATE_FPS);
					}
					else {
						pPlayer->DelayTransformation(p->p, pPlayer->GetQuat(), MAX_UPDATE_FPS);
					}
					switch (p->movetype) {
					case MOVE_TYPE_NORMAL:
						if (p->isLanded) pPlayer->Landing();
						else pPlayer->TakingOff();
						break;
					case MOVE_TYPE_DASH:
						pPlayer->ChangeCurrAnim(CAnimCtrlr::Tag::Dash);
						break;
					case MOVE_TYPE_JUMP:
						pPlayer->ChangeCurrAnim(CAnimCtrlr::Tag::Jump);
						pPlayer->Jumping();
						pPlayer->TakingOff();
						m_pParticleManager->PopSparkParticle(Vector3::Subtract(pPlayer->GetPosition(), XMFLOAT3(0, 180, 0)));
						break;
					}
					break;
				}
				case ID_TAG_BULLET:
				{
					if (pObject->IsActive()) {
						pObject->DelayTransformation(p->p, p->q, MAX_UPDATE_FPS);

					}
					else {
						pObject->DelayTransformation(p->p, p->q, 0.0f);
						pObject->Awake();
						m_pParticleManager->PopMuzzleFlashParticle(Vector3::Subtract(pObject->GetPosition(), pObject->GetLook()), XMFLOAT4(1.f, 0.f, 0.f, 1.f));
					}
					break;
				}
				default:
				{
					if (pObject->IsActive()) {
						pObject->DelayTransformation(p->p, p->q, MAX_UPDATE_FPS);
					}
					else {
						pObject->DelayTransformation(p->p, p->q, 0.0f);
						pObject->Awake();
					}
					break;
				}
				}
			}
			break;
		}
		case MSGTYPE::MSGROOM::CLIENT_TYPECHANGE: 
		{
			PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(msg);
			cout << "Change Tag " << p->ClientID << "\n";
			CPlayer* pPlayer = static_cast<CPlayer*>(m_pObjectManager->Search(p->ClientID));
			if (pPlayer) {
				pPlayer->ChangeTag(static_cast<CPlayer::Tag>(p->ClientType));
			}
			break;
		}
		}
		delete[] msg;
		msg = nullptr;
	}
}

void CLobbyScene::SendMsgs(BYTE MsgType)
{
	static CClient* pClient = CClient::Instance();
	if (!pClient->IsConnect()) return;

	switch (MsgType)
	{
	case MSGTYPE::MSGROOM::CREATEROOM:
	{
		PK_ROOM_CREATE* p = reinterpret_cast<PK_ROOM_CREATE*>(pClient->GetSendBuf());
		p->size = sizeof(PK_ROOM_CREATE);
		p->type = MsgType;
		pClient->SendMsg(p->size);
		break;
	}
	case MSGTYPE::MSGROOM::ACCEPTROOM:
	{
		int roomnum = -1;
		cout << "roomnumber : ";
		cin >> roomnum;

		PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(pClient->GetSendBuf());
		p->size = sizeof(PK_ROOM_INFO);
		p->type = MsgType;
		p->RoomID = roomnum;
		p->ClientID = m_pPlayer->GetID();
		pClient->SendMsg(p->size);
		break;
	}
	case MSGTYPE::MSGROOM::STARTROOM:
	{
		PK_ROOM_START* p = reinterpret_cast<PK_ROOM_START*>(pClient->GetSendBuf());
		p->size = sizeof(PK_ROOM_START);
		p->type = MsgType;
		p->RoomID = pClient->GetRoomID();
		p->ClientID = pClient->GetClientID();
		pClient->SendMsg(p->size);
		m_pFramework->ChangeCurrScene(CScene::Tag::Main);
		break;
	}
	case MSGTYPE::MSGROOM::DISCONNECTROOM:
	{
		if (pClient->IsEnteredRoom()) {
			PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(pClient->GetSendBuf());
			p->size = sizeof(PK_ROOM_INFO);
			p->type = MsgType;
			p->RoomID = pClient->GetRoomID();
			p->ClientID = m_pPlayer->GetID();
			pClient->SendMsg(p->size);
		}
		break;
	}
	case MSGTYPE::MSGROOM::CLIENT_TYPECHANGE:
	{
		if (pClient->IsEnteredRoom()) {
			PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(pClient->GetSendBuf());
			p->size = sizeof(PK_ROOM_INFO);
			p->type = MsgType;
			p->RoomID = pClient->GetRoomID();
			p->ClientID = m_pPlayer->GetID();
			p->ClientType = (static_cast<BYTE>(m_pPlayer->GetPlayerTag()) + 1) % 4;
			pClient->SendMsg(p->size);
		}
		break;
	}
	case MSGTYPE::MSGROOM::REFRESHROOMINFO:
	{
		PK_ROOM_REQUEST* p = reinterpret_cast<PK_ROOM_REQUEST*>(pClient->GetSendBuf());
		p->size = sizeof(PK_ROOM_REQUEST);
		p->type = MSGTYPE::MSGROOM::REFRESHROOMINFO;
		pClient->SendMsg(p->size);
		m_nRoomCounter = 0;
		for (auto p : m_vecLobbyRoomLst) {
			p->ResetFont();
			p->ResetSelect();
		}
		break;
	}
	}
}
#endif