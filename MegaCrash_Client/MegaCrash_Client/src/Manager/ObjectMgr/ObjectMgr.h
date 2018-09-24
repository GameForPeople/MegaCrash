#pragma once
#include "Object\Object.h"
#include "Object\Player\Player.h"
#include "Manager\ParticleMgr\ParticleMgr.h"

/* ================================================================================== //
// ================================ ObjectID setting ================================ //
// ================================================================================== */
enum {
	  ID_TAG_PLAYER = 0
	, ID_TAG_CUBE
	, ID_TAG_BULLET
	, ID_TAG_STAGE
	, ID_TAG_INVALID_OBJECT = 9
};
constexpr UINT ID_FACTOR				{ 1'000'000 };
constexpr UINT ID_PLAYER_START			{ ID_TAG_PLAYER * ID_FACTOR };
constexpr UINT ID_CUBE_START			{ ID_TAG_CUBE * ID_FACTOR };
constexpr UINT ID_BULLET_START			{ ID_TAG_BULLET * ID_FACTOR };
constexpr UINT ID_STAGE_START			{ ID_TAG_STAGE * ID_FACTOR };
constexpr UINT ID_INVALID				{ ID_TAG_INVALID_OBJECT * ID_FACTOR };

/* ================================================================================== //
// ============================== class CObjectMgr ============================== //
// ================================================================================== */

class CObjectMgr
{
public:
	CObjectMgr(const shared_ptr<CParticleMgr>& pParticleMgr);
	~CObjectMgr();

	void Initialize(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst);
	void Release();

	void Animate(float fTimeElapsed);
	void Render(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera);
	void RenderShadowMap(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera);

	CPlayer* PlayerSetting(UINT id);
	CGameObject* Search(UINT id);
	CGameObject* ActiveObject(UINT id);
	CGameObject* CreatePlayerObject(UINT client_id, CPlayer::Tag tag, const XMFLOAT3& xmf3Pos);
	CGameObject* CreateCubeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst, const XMFLOAT3& xmf3Pos);
	CGameObject* CreateBulletObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst, const XMFLOAT3& xmf3Pos);
	vector<CStage*>& CreateStageObjects(const XMFLOAT3& xmf3Pos);

	void DeletePlayerObject(UINT id);
	void ResetPlayerLst();

	vector<CPlayer*>&		GetVecPlayers() { return m_vecPlayers; }
	vector<CStage*>&		GetVecStages() { return m_vecStages; }
	vector<CGameObject*>&	GetVecCubes() { return m_vecCubes; }

private:
	shared_ptr<CParticleMgr>					m_pParticleMgr;

	CPlayer*									m_pPlayer{ nullptr };
	UINT										m_nPlayerID{ ID_PLAYER_START };
	vector<CPlayer*>							m_vecPlayers;
	unordered_map<UINT, CPlayer*>				m_PlayerLst;

	UINT										m_nCubeID{ ID_CUBE_START };
	vector<CGameObject*>						m_vecCubes;

	UINT										m_nBulletID{ ID_BULLET_START };
	vector<CBullet*>							m_vecBullets;

	UINT										m_nStageID{ ID_STAGE_START };
	vector<CStage*>								m_vecStages;
};