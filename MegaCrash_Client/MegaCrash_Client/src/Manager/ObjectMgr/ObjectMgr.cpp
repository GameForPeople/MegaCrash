#include "stdafx.h"
#include "ObjectMgr.h"

CObjectMgr::CObjectMgr(const shared_ptr<CParticleMgr>& pParticleMgr)
	: m_pParticleMgr{ pParticleMgr }
{
}
CObjectMgr::~CObjectMgr()
{
	Release();
}

void CObjectMgr::Initialize(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst)
{
	CResMgr* pRes = CResMgr::Instance();

	for (int i = 0; i < MAX_PLAYER; ++i) {
		CPlayer* pPlayer = new CPlayer(pd3dDevice, pd3dCmdLst);
		pPlayer->SetID(m_nPlayerID++); 
		pPlayer->SetParticleMgr(m_pParticleMgr);
		pPlayer->CreateShaderVariables();
		pPlayer->Sleep();
		m_vecPlayers.push_back(pPlayer);
	}
}

void CObjectMgr::Release()
{
	for (auto& p : m_vecPlayers) delete p;
	for (auto& p : m_vecCubes) delete p;
	for (auto& p : m_vecBullets) delete p;
	for (auto& p : m_vecStages) delete p;
	m_vecPlayers.clear();
	m_PlayerLst.clear();
	m_vecCubes.clear();
	m_vecBullets.clear();
	m_vecStages.clear();
}

void CObjectMgr::Animate(float fTimeElapsed)
{
	for (int i = 0; i < m_vecStages.size(); ++i) {
		if (m_vecStages[i]->IsActive()) {
			m_vecStages[i]->Animate(fTimeElapsed);
			m_vecStages[i]->Update(fTimeElapsed);
			m_vecStages[i]->UpdateTransform(NULL);
		}
	}
	for (auto& p : m_PlayerLst) {
		//if (p.second->IsActive()) {
			p.second->Update(fTimeElapsed);
			p.second->Animate(fTimeElapsed);
			p.second->UpdateTransform(NULL);
		//}
	}
	for (auto& p : m_vecCubes) {
		if (p->IsActive()) {
			p->Update(fTimeElapsed);
			p->Animate(fTimeElapsed);
			p->UpdateTransform(NULL);
		}
	}
	for (auto& p : m_vecBullets) {
		if (p->IsActive()) {
			p->Update(fTimeElapsed);
			p->Animate(fTimeElapsed);
			p->UpdateTransform(NULL);

			// 투사체 트레일 파티클 생성부
			CPlayer* pPlayer = static_cast<CPlayer*>(Search(p->GetShooterID()));
			CBullet* pBullet = static_cast<CBullet*>(p);
			if(pPlayer)
			switch (pPlayer->GetPlayerTag())
			{
			case CPlayer::Tag::Ruby:
				m_pParticleMgr->PopRubyBulletTrail(p->GetPosition());
				break;
			case CPlayer::Tag::Legion:
				m_pParticleMgr->PopLegionBulletTrail(p->GetPosition());
				break;
			case CPlayer::Tag::Epsilon:
				if (pBullet)
				{
					if (pBullet->IsSkill())
					{
						m_pParticleMgr->PopEpsilonSpecialBulletTrail(p->GetPosition());
					}
					else
					{
						m_pParticleMgr->PopEpsilonBulletTrail(p->GetPosition());
					}
				}
				
				break;
			case CPlayer::Tag::Gravis:
				m_pParticleMgr->PopGravisBulletTrail(p->GetPosition());
				break;
			}
		}
	}
}

void CObjectMgr::Render(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	for (auto& p : m_vecStages) {
		if (p->IsActive())
			p->Render(pd3dCmdLst, pCamera);
	}
	for (auto& p : m_vecCubes) {
		if (p->IsActive())
			p->Render(pd3dCmdLst, pCamera);
	}
	for (auto& p : m_PlayerLst) {
		if (p.second->IsActive())
			p.second->Render(pd3dCmdLst, pCamera);
	}
	//for (auto& p : m_vecBullets) {
	//	if (p->IsActive())
	//		p->Render(pd3dCmdLst, pCamera);
	//}
}

void CObjectMgr::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	for (auto& p : m_vecStages) {
		if (p->IsActive())
			p->RenderShadowMap(pd3dCmdLst, pCamera);
	}
	for (auto& p : m_vecCubes) {
		if (p->IsActive())
			p->RenderShadowMap(pd3dCmdLst, pCamera);
	}
	for (auto& p : m_vecBullets) {
		if (p->IsActive())
			p->RenderShadowMap(pd3dCmdLst, pCamera);
	}
	for (auto& p : m_PlayerLst) {
		if (p.second->IsActive())
			p.second->RenderShadowMap(pd3dCmdLst, pCamera);
	}
}

CPlayer* CObjectMgr::PlayerSetting(UINT id)
{
	if (m_PlayerLst.count(id - ID_PLAYER_START) != 0) {
		m_pPlayer = m_PlayerLst[id - ID_PLAYER_START];
		return m_pPlayer;
	}
	return nullptr;
}

CGameObject* CObjectMgr::Search(UINT id)
{
	switch (id / ID_FACTOR)
	{
	case ID_TAG_PLAYER:
		if (m_PlayerLst.count(id - ID_PLAYER_START) != 0) {
			return m_PlayerLst[id - ID_PLAYER_START];
		}
		break;
	case ID_TAG_CUBE:
		return m_vecCubes[id - ID_CUBE_START];
	case ID_TAG_BULLET:
		return m_vecBullets[id - ID_BULLET_START]; 
	case ID_TAG_STAGE:
		return m_vecStages[id - ID_STAGE_START];
	default:
		cout << "[ERROR]: CObjectMgr::Search() failed. Invalid ID\n";
		break;
	}
	return nullptr;
}

CGameObject* CObjectMgr::ActiveObject(UINT id)
{
	CGameObject* pObject = Search(id);
	if(pObject) pObject->Awake();
	return pObject;
}

CGameObject* CObjectMgr::CreatePlayerObject(
	  UINT client_id
	, CPlayer::Tag tag
	, const XMFLOAT3& xmf3Pos
)
{
	CPlayer* pObject{ nullptr };
	for (auto& p : m_vecPlayers) {
		if (!p->IsActive()) {
			pObject = p;
			break;
		}
	}
	if (pObject == nullptr) return pObject;

	pObject->Awake();
	pObject->SetID(client_id);
	pObject->ChangeTag(tag);
	pObject->DelayTransformation(xmf3Pos, Vector4::QuatIdentity(), 0.01f);
	pObject->SyncMemberVariableToMatrix();
	pObject->UpdateTransform();

	m_PlayerLst[client_id] = pObject;
	return pObject;
}

CGameObject* CObjectMgr::CreateCubeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst, const XMFLOAT3& xmf3Pos)
{
	CResMgr* pRes = CResMgr::Instance();

	CGameObject* pObject = nullptr;
	pObject = new CGameObject();
	pObject->SetID(m_nCubeID++);
	pObject->SetMesh(0, pRes->GetMesh(eGeo::Mesh::Cube));
	pObject->CreateShaderVariables();

	shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>();
	pMaterial->SetGlobalMaterialIdx(1 + rand_int() % 2);
	//pMaterial->SetShader(pRes->GetShader(ePSO::Graphic::Cube));
	pMaterial->SetShader(pRes->GetShader(ePSO::Graphic::DeferredCube));
	pMaterial->SetShadowMapShader(pRes->GetShader(ePSO::Graphic::ShadowMapCube));
	pObject->SetMaterial(pMaterial);
	pObject->SetParticleMgr(m_pParticleMgr);
	pObject->SetPosition(xmf3Pos);
	pObject->UpdateTransform();
	pObject->Sleep();

		m_vecCubes.push_back(pObject);
	return pObject;
}

CGameObject* CObjectMgr::CreateBulletObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst, const XMFLOAT3& xmf3Pos)
{
	CResMgr* pRes = CResMgr::Instance();

	CBullet* pObject = nullptr;
	pObject = new CBullet();
	pObject->SetID(m_nBulletID++);
	pObject->SetMesh(0, pRes->GetMesh(eGeo::Mesh::Bullet));
	pObject->SetMaterial(pRes->GetMaterial(eMat::Object::Ruby_Bullet));
	pObject->SetParticleMgr(m_pParticleMgr);
	pObject->CreateShaderVariables();
	pObject->SetPosition(xmf3Pos);
	pObject->UpdateTransform();
	pObject->Sleep();

	m_vecBullets.push_back(pObject);
	return pObject;
}

vector<CStage*>& CObjectMgr::CreateStageObjects(const XMFLOAT3 & xmf3Pos)
{
	CResMgr* pRes = CResMgr::Instance();

	CStage* pObject{ nullptr };
	auto& meshes = pRes->GetStageMeshes();
	for (int i = 0; i < meshes.size(); ++i) {
		pObject = new CStage();
		pObject->SetID(m_nStageID++);
		pObject->SetMesh(0, meshes[i]);
		pObject->SetMaterial(pRes->GetMaterial(eMat::Object::Stage));
		pObject->CreateShaderVariables();
		pObject->SetPosition(xmf3Pos);
		pObject->UpdateTransform();
		pObject->Awake();
		m_vecStages.push_back(pObject);
	}
	return m_vecStages;
}

void CObjectMgr::DeletePlayerObject(UINT id)
{
	if (m_pPlayer->GetID() == id) return;
	if (m_PlayerLst.count(id) != 0) {
		m_PlayerLst[id]->Sleep();
		m_PlayerLst.erase(id);
	}
}

void CObjectMgr::ResetPlayerLst()
{
	for (auto p = m_PlayerLst.begin(); p != m_PlayerLst.end();) {
		if (p->first != m_pPlayer->GetID()) {
			p->second->Sleep();
			p = m_PlayerLst.erase(p);
		}
		else {
			++p;
		}
	}
}