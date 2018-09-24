//-----------------------------------------------------------------------------
// File: MainScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Framework\Framework.h"
#include "Object/Player/Player.h"
#include "Object/Particle/Particle.h"
#include "Object/Font/Font.h"
#include "Shader/Shader.h"
#include "Light/Light.h"
#include "Material/Material.h"
#include "UI/UI.h"
#include "MainScene.h"

CMainScene::CMainScene()
	: CScene()
	, m_pPlayer			{ nullptr }
	, m_bPlayerDie		{ false }
	, m_fPlayerDieTimer	{ 0.0f }
	, m_bGameover		{ false }
{
}
CMainScene::~CMainScene()
{
}

void CMainScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCmdLst)
{
	m_pLights = make_shared<CLight>();
	m_pMaterials = make_shared<CGlobalMaterial>();

	// SkyBox
	m_pSkyBox = make_shared<CSkyBox>(pd3dDevice, pd3dCmdLst);

	// Font
	m_pKillCounter = make_shared<CFont>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Font::Default
		, CFont::DefaultFontPath
		, NUM_FONT_CHAR
		, CLIENT_WIDTH
		, CLIENT_HEIGHT);
	
	// UI
	m_pHUDUIShader = CResMgr::Instance()->GetShader(ePSO::Graphic::HUD_UI);
	m_pRaderUIShader = CResMgr::Instance()->GetShader(ePSO::Graphic::RADER_UI);
	m_pCrossHair = make_shared<CCrosshair>();
	m_pDefaultFont = make_shared<CFont>(
		  pd3dDevice
		, pd3dCmdLst
		, eTex::Font::Default
		, CFont::DefaultFontPath
		, 1024
		, CLIENT_WIDTH
		, CLIENT_HEIGHT);

	// Billboard
	{
		m_szUIDamageBillboard = XMFLOAT2(40, 40);
		m_pUIDamageBillboard = make_shared<CParticleBillboards>(
			  pd3dDevice
			, pd3dCmdLst
			, eTex::Billboard::Digit
			, XMFLOAT2(10, 1)
			, 30
			, m_szUIDamageBillboard);
	}

	m_pExitButton = make_shared<CButton>();
	m_pExitButton->SetMaterial(eMat::Object::ExitButton);
	m_pExitButton->SetInfoCenterPos(XMFLOAT2{ 0.9f, 0.9f });
	m_pExitButton->SetInfoButtonSize(XMFLOAT2{ 150.f / 1600.f, 75.f / 900.f });

	m_pGameEnd = make_shared<CGameEndUI>();
	m_pGameEnd->SetMaterial(eMat::Object::GameEnd);
	m_pGameEnd->SetInfoCenterPos(XMFLOAT2{ 0.0f, 0.0f });
	m_pGameEnd->SetInfoWindowSize(XMFLOAT2{ 1200.f / 1600.f, 750.f / 900.f });
	m_pGameEnd->SetInfoColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDeathUI = make_shared<CDeathUI>();
	m_pDeathUI->SetMaterial(eMat::Object::DeathUI);
	m_pDeathUI->SetInfoColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	m_pKillCounter->ResetTextData();
	CreateShaderVariables(pd3dDevice, pd3dCmdLst);
}

void CMainScene::ReleaseObjects()
{
	ReleaseShaderVariables();
}

void CMainScene::ResetObjects()
{
	m_bGameover = false;
	m_bPlayerDie = false;
	m_fPlayerDieTimer = 0.0f;
	m_pPlayer = m_pFramework->GetPlayer();
	XMFLOAT4X4 mtx{ Matrix4x4::Identity() };
	for (auto p : m_pObjectManager->GetVecStages()) {
		p->SetWorldMtx(mtx);
		p->SetPosition(XMFLOAT3(16262.f, 16766.f, 26967.f));
		p->UpdateTransform();
	}
	m_pExitButton->SetInfoCenterPos(XMFLOAT2{ -0.95f, 0.9f });
	m_pExitButton->SetInfoButtonSize(XMFLOAT2{ 150.f / 1600.f, 75.f / 900.f });
	m_PlayerKillCount = 0;
	m_GlobalKillCount = 0;
	wstring str = L"P" + to_wstring(m_pPlayer->GetID()) + L" : " + to_wstring(m_PlayerKillCount) + L" / Top : " + to_wstring(m_GlobalKillCount);
	m_pKillCounter->ResetTextData();
	m_pKillCounter->AppendTextData(
		str
		, XMFLOAT2(0.8f, 0.01f)
		, XMFLOAT2(1.5f, 1.5f)
		, XMFLOAT2(0.5f, 0.0f)
		, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}

void CMainScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCmdLst)
{
	m_pLights->CreateShaderVariables();
	m_pMaterials->CreateShaderVariables();
}
void CMainScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst)
{
	m_pLights->UpdateShaderVariables(pd3dCmdLst);
	m_pMaterials->UpdateShaderVariables(pd3dCmdLst);
	m_pShaderHelper->UpdateShaderVariables(pd3dCmdLst);
}
void CMainScene::ReleaseShaderVariables()
{
}

bool CMainScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		if (m_pExitButton->IsCursorOn()) {
			SendMsgs(MSGTYPE::MSGROOM::DISCONNECTROOM);
			m_pFramework->ChangeCurrScene(CScene::Tag::Lobby);
		}
		break;
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		::GetCursorPos(&m_ptCursorPos);
		::ScreenToClient(hWnd, &m_ptCursorPos);
		m_pExitButton->CollisionCheck(m_ptCursorPos);
		break;

	default:
		break;
	}
	return false;
}

bool CMainScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			SendMsgs(MSGTYPE::MSGROOM::DISCONNECTROOM);
			break;
		case VK_CONTROL:
			::SetCapture(hWnd);
			::GetCursorPos(&m_ptOldCursorPos);
			break;
#if USE_SERVER_CONNECT
		case VK_NUMPAD1: SendMsgs(MSGTYPE::MSGROOM::CREATEROOM); break;
		case VK_NUMPAD2: SendMsgs(MSGTYPE::MSGROOM::ACCEPTROOM); break;
		case VK_NUMPAD3: SendMsgs(MSGTYPE::MSGROOM::STARTROOM); break;
		case VK_NUMPAD4: SendMsgs(MSGTYPE::MSGROOM::DISCONNECTROOM); break;
		case VK_NUMPAD5: SendMsgs(MSGTYPE::MSGROOM::CLIENT_TYPECHANGE); break;
		
#endif
		case VK_NUMPAD6:
		{
			m_pParticleManager->PopRubyBulletExplosion(m_pPlayer->GetPosition());
			break;
		}
		case VK_NUMPAD7:
		{
			//m_pParticleManager->PopMuzzleFlashParticle(m_pPlayer->GetPosition(),XMFLOAT4(1.f,0.0f,0.0f,1.f));
			m_pParticleManager->PopLegionBulletExplosion(m_pPlayer->GetPosition());
			break;
		}
		case VK_NUMPAD8:
		{
			m_pParticleManager->PopEpsilonBulletExplosion(m_pPlayer->GetPosition());
			break;
		}
		case VK_NUMPAD9:
		{
			m_pParticleManager->PopGravisBulletExplosion(m_pPlayer->GetPosition());

			break;
		}


		}
		//m_pPlayer->GetCamera()->ShakeCamera(0.2f, 3.0f);
		//CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::EFFECT_HIT, m_pPlayer->GetPosition());
		//RenderDamage(m_pPlayer->GetPosition(), m_pPlayer->GetRightVector(), rand()%1000);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_CONTROL:
			::ReleaseCapture();
			break;
		}
		break;
	}
	
	return false;
}

bool CMainScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{
		//if (LOWORD(wParam) == WA_INACTIVE)
		//{
		//	::ReleaseCapture();
		//}
		//else
		//{
		//	::SetCapture(hWnd);
		//	::GetCursorPos(&m_ptOldCursorPos);
		//}
		break;
	}
	case WM_SIZE:
	{
		//m_nWndClientWidth = LOWORD(lParam);
		//m_nWndClientHeight = HIWORD(lParam);

		break;
	}
	}
	return false;
}

bool CMainScene::ProcessInput(UCHAR *pKeysBuffer, float fTimeElapsed)
{
	if (m_bGameover || m_bPlayerDie) return false;
	if (pKeysBuffer['0'] & 0xF0) {
		CHEAT* p = reinterpret_cast<CHEAT*>(CClient::Instance()->GetSendBuf());
		p->size = sizeof(CHEAT);
		p->type = MSGTYPE::CHEAT::RESTART;
		CClient::Instance()->SendMsg(p->size);
	}

	float cxDelta = 0.0f, cyDelta = 0.0f;
	POINT ptCursorPos;
	if (GetCapture() == m_hWnd) {
		SetCursor(NULL);
		GetCursorPos(&ptCursorPos);
		cxDelta = static_cast<float>(ptCursorPos.x - m_ptOldCursorPos.x) * 0.3f;
		cyDelta = static_cast<float>(ptCursorPos.y - m_ptOldCursorPos.y) * 0.3f;
		SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
	}

	if ((cxDelta || cyDelta)) {
		static auto&	vecStages	{ m_pObjectManager->GetVecStages() };
		static auto&	vecPlayers	{ m_pObjectManager->GetVecPlayers() };
		CCamera*		pCamera		{ m_pPlayer->GetCamera() };

		pCamera->Rotate(cyDelta, cxDelta, 0.0f);

		XMFLOAT3	camera_pos		{ pCamera->GetPosition() };
		XMFLOAT3	camera_look		{ pCamera->GetLookVector() };
		float		result_t		{ FLT_MAX };
		for (auto p : vecStages) {
			if (p->IsActive()) {
				XMFLOAT3 ray_origin{ Vector3::TransformCoord(camera_pos, Matrix4x4::Inverse(p->GetWorldMtx())) };
				float t{ 0.0f };
				p->CollisionCheck(ray_origin, camera_look, &t);
				if (result_t > t) result_t = t;
			}
		}
		for (auto p : vecPlayers) {
			if (p->IsActive()) {
				if (p->GetID() == m_pPlayer->GetID()) continue;
				float t{ 0.0f };
				p->CollisionCheck(camera_pos, camera_look, &t);
				if (result_t > t) result_t = t;
			}
		}
		m_pPlayer->SetShootDir(Vector3::Normalize(Vector3::Subtract(
			  Vector3::Add(camera_pos, camera_look, result_t)
			, m_pPlayer->GetPosition())));
	}
	m_pPlayer->ProcessInput(pKeysBuffer, fTimeElapsed);

	return false;
}

void CMainScene::AnimateObjects(float fTimeElapsed)
{
	if (m_bPlayerDie && !m_bGameover) {
		m_fPlayerDieTimer += fTimeElapsed;
		if (m_fPlayerDieTimer > 3.0f) {
			m_fPlayerDieTimer = 0.0f;
			m_bPlayerDie = false;
		}
	}

	m_pShaderHelper->UpdateTimer(fTimeElapsed);

	m_pObjectManager->Animate(fTimeElapsed);
	m_pParticleManager->Animate(fTimeElapsed);
	auto sound_mgr = CSoundMgr::Instance();
	sound_mgr->GetListenerHandle().SetParameter(m_pPlayer->GetPosition(), m_pPlayer->GetLookVector(), m_pPlayer->GetUpVector() );
	sound_mgr->Update(fTimeElapsed);

	if (m_pUIDamageBillboard) m_pUIDamageBillboard->Animate(fTimeElapsed);
}

void CMainScene::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	static ID3D12RootSignature* pRootSignature = CResMgr::Instance()->GetRootSignature();
	pd3dCmdLst->SetGraphicsRootSignature(pRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCmdLst);
	pCamera->UpdateShaderVariables(pd3dCmdLst);

	UpdateShaderVariables(pd3dCmdLst);
}

void CMainScene::Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	static auto pShadowMap = CResMgr::Instance()->GetShadowMap();
	OnPrepareRender(pd3dCmdLst, pCamera);
	pShadowMap->Render(pd3dCmdLst);

	if (m_pSkyBox) m_pSkyBox->Render(pd3dCmdLst, pCamera);
	if (m_pObjectManager) m_pObjectManager->Render(pd3dCmdLst, pCamera);
}

void CMainScene::RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
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
	m_pShaderHelper->SetPlayerHP((float)m_pPlayer->GetPlayerUI()->GetPlayerStatInfo().hp);
	
	UpdateShaderVariables(pd3dCmdLst);
	
	if (m_pParticleManager) m_pParticleManager->Render(pd3dCmdLst, pCamera);
	if (m_pHUDUIShader) m_pHUDUIShader->Render(pd3dCmdLst, pCamera);
	if (m_pRaderUIShader && !m_bPlayerDie) m_pRaderUIShader->Render(pd3dCmdLst, pCamera);
	if (m_pPlayer && !m_bPlayerDie) m_pPlayer->RenderUI(pd3dCmdLst, pCamera);
	if (m_pUIDamageBillboard) m_pUIDamageBillboard->Render(pd3dCmdLst, pCamera);

	if (m_pCrossHair) m_pCrossHair->Render(pd3dCmdLst, pCamera);

	m_pExitButton->Render(pd3dCmdLst, pCamera);

	m_pKillCounter->Render(pd3dCmdLst, pCamera);

	if (m_bPlayerDie) {
		m_pDeathUI->Render(pd3dCmdLst, pCamera);
	}
	if (m_bGameover) {
		m_pGameEnd->Render(pd3dCmdLst, pCamera);
	}
}

void CMainScene::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
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
void CMainScene::ProcessMsgs()
{
	static BYTE* msg{ nullptr };
	static CClient* pClient = CClient::Instance();

	for(;;) {
		msg = pClient->MsgDequeue();
		if (msg == nullptr) break;

		switch (msg[1])
		{
		case MSGTYPE::MSGROOM::ROOMINFO: {
			PK_ROOM_INFO* p = reinterpret_cast<PK_ROOM_INFO*>(msg);
			cout << "New Room : " << p->RoomID << "\n";
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
			cout << "Leave Room " << p->RoomID << " " << p->ClientID << "\n";
			if (p->ClientID == pClient->GetClientID()) {
				pClient->LeaveRoom();
				m_pObjectManager->ResetPlayerLst();
			}
			else {
				m_pObjectManager->DeletePlayerObject(p->ClientID);
			}
			break;
		}
		case MSGTYPE::MSGROOM::ENDGAME: {
			PK_ROOM_END* p = reinterpret_cast<PK_ROOM_END*>(msg);
			cout << "Room " << p->RoomID << " Game End.\n";
			if (p->WinnerID == pClient->GetClientID()) {
				m_pGameEnd->SetIsVictory(true);
			}
			else {
				m_pGameEnd->SetIsVictory(false);
				m_bPlayerDie = true;
			}
			m_pGameEnd->SetInfoStartTime(m_pShaderHelper->GetTimer());
			m_bGameover = true;
			m_pExitButton->SetInfoCenterPos(XMFLOAT2{ 0.0f, -0.4f });
			m_pExitButton->SetInfoButtonSize(XMFLOAT2{ 256.f / 1600.f, 128.f / 900.f });
			::ReleaseCapture();
			break;
		}
		case MSGTYPE::MSGROOM::CLOSEROOM: {
			PK_ROOM_END* p = reinterpret_cast<PK_ROOM_END*>(msg);
			cout << "Room " << p->RoomID << " closed.\n";
			break;
		}
		case MSGTYPE::MSGROOM::STARTROOM: {
			PK_ROOM_START* p = reinterpret_cast<PK_ROOM_START*>(msg);
			cout << "Room " << p->RoomID << " Game Start.\n";
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
		case MSGTYPE::MSGACTION::ATTACK:
		{
			PK_MSG_ATTACK* p = reinterpret_cast<PK_MSG_ATTACK*>(msg);
			switch (p->atktype)
			{
			case ATK_TYPE_ATTACK1:
			{
				break;
			}
			case ATK_TYPE_ATTACK2:
			{
				
				break;
			}
			case ATK_TYPE_SKILL:
			{
				CPlayer* pPlayer = static_cast<CPlayer*>(m_pObjectManager->Search(p->id));
				if (pPlayer) {
					pPlayer->ChangeCurrAnim(CAnimCtrlr::Tag::Skill);
					pPlayer->UseSkiil();

					switch (pPlayer->GetPlayerTag())
					{
					case CPlayer::Tag::Ruby:
						m_pParticleManager->PopRubySpecial(pPlayer->GetPosition());
						break;
					case CPlayer::Tag::Legion:
						m_pParticleManager->PopLegionSpecial(pPlayer->GetPosition());
						break;
					case CPlayer::Tag::Epsilon:
						CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::SHOT_BASE, p->p);
						CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::SHOT_ES, p->p);
						m_pParticleManager->PopEpsilonSpecial(pPlayer->GetPosition());
						break;
					case CPlayer::Tag::Gravis:
						m_pParticleManager->PopGravisSpecial(pPlayer->GetPosition());
						break;
					}

				}
				break;
			}
			}
			break;
		}
		case MSGTYPE::MSGACTION::SKILLCOOLDOWN:
		{
			PK_MSG_SKILL* p = reinterpret_cast<PK_MSG_SKILL*>(msg);
			m_pPlayer->ChangeCurrAnim(CAnimCtrlr::Tag::Jump);
			m_pPlayer->Jumping();
			m_pPlayer->TakingOff();
			m_pParticleManager->PopSparkParticle(Vector3::Subtract(m_pPlayer->GetPosition(), XMFLOAT3(0, 180, 0)));
			break;
		}
		case MSGTYPE::MSGACTION::ANIMATE:
		{
			PK_MSG_STATECHANGE* p = reinterpret_cast<PK_MSG_STATECHANGE*>(msg);
			if (p->id != m_pPlayer->GetID()) {
				CPlayer* pPlayer = static_cast<CPlayer*>(m_pObjectManager->Search(p->id));
				if (pPlayer) {
					pPlayer->ChangeCurrAnim(static_cast<CAnimCtrlr::Tag>(p->data1));
				}
			}
			break;
		}
		case MSGTYPE::MSGUPDATE::DELETE_OBJECT:
		{
			PK_MSG_DELETE_PLAYER* p = reinterpret_cast<PK_MSG_DELETE_PLAYER*>(msg);
			CGameObject* pObject = m_pObjectManager->Search(p->id);
			if (pObject) {
				pObject->Sleep();
			}
			//m_pObjectManager->DeletePlayerObject(p->id);
			break;
		}
		case MSGTYPE::MSGUPDATE::UPDATE_PLAYER:
		{
			PK_MSG_STATECHANGE* p = reinterpret_cast<PK_MSG_STATECHANGE*>(msg);
			if (p->id == m_pPlayer->GetID()) {
				CPlayer* pPlayer = static_cast<CPlayer*>(m_pObjectManager->Search(p->id));
				if (pPlayer) {
					pPlayer->GetPlayerUI()->SetPlayerInfo({
						(float)p->data1 / MAX_HP,
						(float)p->data2 / MAX_SP,
						(float)p->data3 / MAX_CP });
				}
			}
			break;
		}
		case MSGTYPE::MSGUPDATE::PLAYER_DIED:
		{
			PK_MSG_PLAYERDIED* p = reinterpret_cast<PK_MSG_PLAYERDIED*>(msg);
			CPlayer* pPlayer = static_cast<CPlayer*>(m_pObjectManager->Search(p->id));
			if (pPlayer) {
				if (p->id == m_pPlayer->GetID()) {
					CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::DEAD_1, p->p);
					m_pDeathUI->SetInfoStartTime(m_pShaderHelper->GetTimer());
					m_bPlayerDie = true;
					pPlayer->GetCamera()->ShakeCamera(2.0f, 50.0f);
				}
				CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::DEAD_2, p->p);
				switch (pPlayer->GetPlayerTag())
				{
				case CPlayer::Tag::Ruby:
					m_pParticleManager->PopRubyExplode(pPlayer->GetPosition());
					break;
				case CPlayer::Tag::Legion:
					m_pParticleManager->PopLegionExplode(pPlayer->GetPosition());
					break;
				case CPlayer::Tag::Epsilon:
					m_pParticleManager->PopEpsilonExplode(pPlayer->GetPosition());
					break;
				case CPlayer::Tag::Gravis:
					m_pParticleManager->PopGravisExplode(pPlayer->GetPosition());
					break;
				}
			}
			break;
		}
		case MSGTYPE::MSGUPDATE::UPDATE_KILLCOUNT:
		{
			PK_MSG_KILLCOUNT* p = reinterpret_cast<PK_MSG_KILLCOUNT*>(msg);
			m_GlobalKillCount = (int)p->GlobalKill;
			if (pClient->GetClientID() == p->attackerid) {
				m_PlayerKillCount = (int)p->PlayerKill;
			}
			
			wstring str = L"P" + to_wstring(m_pPlayer->GetID()) + L" : " + to_wstring(m_PlayerKillCount) + L" / Top : " + to_wstring(m_GlobalKillCount);
			m_pKillCounter->ResetTextData();
			m_pKillCounter->AppendTextData(
				str
				, XMFLOAT2(0.8f, 0.01f)
				, XMFLOAT2(1.5f, 1.5f)
				, XMFLOAT2(0.5f, 0.0f)
				, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			//cout << "attacker : " << p->attackerid << endl;
			//cout << "PlayerKill : " << (int)p->PlayerKill << endl;
			//cout << "GlobalKill : " << (int)p->GlobalKill << endl;
			break;
		}
		case MSGTYPE::MSGUPDATE::RESET_OBJECT:
		{
			PK_MSG_STATECHANGE* p = reinterpret_cast<PK_MSG_STATECHANGE*>(msg);
			CGameObject* pObject = m_pObjectManager->Search(p->id);
			if (p->id == m_pPlayer->GetID())
				m_pPlayer->GetCamera()->ShakeCamera(2.0f, 15.0f);

			if (pObject) {
				CPlayer* pPlayer = static_cast<CPlayer*>(pObject);
				pPlayer->GetPlayerUI()->SetPlayerInfo({ 1, 1, 1 });
				pPlayer->Awake();
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
						m_pParticleManager->PopSparkParticle(Vector3::Subtract( pPlayer->GetPosition() ,XMFLOAT3(0,180,0) ) );
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
						CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::SHOT_BASE, p->p);
						CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::SHOT_N, p->p);
						
						pObject->DelayTransformation(p->p, p->q, 0.0f);
						pObject->Awake();
						m_pParticleManager->PopMuzzleFlashParticle(Vector3::Subtract(pObject->GetPosition(), pObject->GetLook()),XMFLOAT4(1.f,0.f,0.f,1.f));
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
		case MSGTYPE::MSGACTION::DAMAGE:
		{
			PK_MSG_DAMAGE* p = reinterpret_cast<PK_MSG_DAMAGE*>(msg);
			CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::EFFECT_HIT, p->p);
			m_pPlayer->GetCamera()->ShakeCamera(0.2f, 3.0f);
			m_pCrossHair->SetCrosshairInfoDamagedTime(m_pShaderHelper->GetTimer());
			if (m_pPlayer->GetID() == p->victimid) {
				m_pPlayer->GetPlayerUI()->SetPlayerInfoHP(
					static_cast<float>(p->HP) / static_cast<float>(MAX_HP));
			}

			// °ø°ÝÀÚ
			if (p->victimid / ID_FACTOR == ID_TAG_PLAYER) {
				CGameObject* pVictimObject = m_pObjectManager->Search(p->victimid);
				if (pVictimObject) {
					pVictimObject->Stun(0.05f);
				}
			}
			CGameObject* pAttackerObject = m_pObjectManager->Search(p->attackerid);
			if (pAttackerObject) {
				CPlayer* pAttacker = static_cast<CPlayer*>(pAttackerObject);
				if (p->attackerid / ID_FACTOR == ID_TAG_PLAYER && p->atktype == ATK_TYPE_ATTACK1) {
					pAttacker->Stun(0.05f);
				}
				switch (pAttacker->GetPlayerTag()) {
				case CPlayer::Tag::Ruby:
					if (p->atktype == 2)
					{
						CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::SHOT_BASE, p->p);
						CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::EXPLOSION, p->p);
						m_pParticleManager->PopRubySpecialExplosion(XMFLOAT3(p->p.x, p->p.y, p->p.z));
						m_pParticleManager->PopRubyBulletExplosion(XMFLOAT3(p->p.x, p->p.y, p->p.z));
					}
					else {
						m_pParticleManager->PopRubyBulletExplosion(XMFLOAT3(p->p.x, p->p.y, p->p.z));
					}

					break;
				case CPlayer::Tag::Legion:
					if (p->atktype == 2)
					{
						CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::SHOT_BASE, p->p);
						CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::EXPLOSION, p->p);
						m_pParticleManager->PopLegionSpecialExplosion(XMFLOAT3(p->p.x, p->p.y, p->p.z), pAttacker->GetPosition());
						m_pParticleManager->PopLegionBulletExplosion(XMFLOAT3(p->p.x, p->p.y, p->p.z));
					}
					else {
						m_pParticleManager->PopLegionBulletExplosion(XMFLOAT3(p->p.x, p->p.y, p->p.z));
					}

					break;
				case CPlayer::Tag::Epsilon:
					if (p->atktype == 2)
					{
						m_pParticleManager->PopEpsilonBulletExplosion(XMFLOAT3(p->p.x, p->p.y, p->p.z));
						m_pParticleManager->PopEpsilonSpecialExplosion(XMFLOAT3(p->p.x, p->p.y, p->p.z));
					}
					else {
						m_pParticleManager->PopEpsilonBulletExplosion(XMFLOAT3(p->p.x, p->p.y, p->p.z));
					}
					break;
				case CPlayer::Tag::Gravis:
					if (p->atktype == 2)
					{
						CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::SHOT_BASE, p->p);
						CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::EXPLOSION, p->p);
						m_pParticleManager->PopGravisSpecialExplosion(XMFLOAT3(p->p.x, p->p.y, p->p.z), pAttacker->GetPosition());
						m_pParticleManager->PopGravisBulletExplosion(XMFLOAT3(p->p.x, p->p.y, p->p.z));
					}
					else {
						m_pParticleManager->PopGravisBulletExplosion(XMFLOAT3(p->p.x, p->p.y, p->p.z));
					}

					break;
				}
			}
			m_pParticleManager->PopSparkParticle(XMFLOAT3(p->p.x, p->p.y + 60.0f, p->p.z));
			break;
		}
		case MSGTYPE::MSGACTION::SHOOT:
		{
			PK_MSG_SHOOT* p = reinterpret_cast<PK_MSG_SHOOT*>(msg);
			CGameObject* pObject = m_pObjectManager->Search(p->bulletid);
			if (pObject) {
				CBullet* pBullet = static_cast<CBullet*>(pObject);
				pBullet->SetAtkTypeIsSkill(static_cast<bool>(p->isSkill));
				pBullet->SetShooterID(p->shooterid);
			}
		}
		}
		delete[] msg;
		msg = nullptr;
	}
}

void CMainScene::SendMsgs(BYTE MsgType)
{
	static CClient* pClient = CClient::Instance();
	if (!pClient->IsConnect()) return;

	static long long tNow{ 0 };
	tNow = GetCurrTime();

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
		pClient->SendMsg(p->size);
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
	case MSGTYPE::MSGUPDATE::CREATE_OBJECT:
	{
		break;
	}
	case MSGTYPE::MSGUPDATE::DELETE_OBJECT:
	{
		break;
	}
	case MSGTYPE::MSGUPDATE::UPDATE_OBJECT:
	{
		break;
	}
	}
}
#endif

void CMainScene::RenderDamage(const XMFLOAT3& pos, const XMFLOAT3& right, UINT nDamage)
{
	UINT num = 0;
	float texX = 1.0f;
	XMFLOAT3 offset = Vector3::ScalarProduct(right, -m_szUIDamageBillboard.x, false);
	XMFLOAT3 pivot = XMFLOAT3(pos.x - offset.x, pos.y, pos.z - offset.z);
	for (;;) {
		if (nDamage == 0) break;
		num = nDamage % 10;
		if (num == 0) texX = 9.f;
		else texX = static_cast<float>(num - 1);
		m_pUIDamageBillboard->PopParticle(
			  pivot
			, XMFLOAT2(texX, 0)
			, XMFLOAT3(0.0f, 1.0f, 0.0f)
			, 64.0f
			, 0.5f);
		nDamage /= 10;
		pivot = Vector3::Add(pivot, offset);
	}
}

