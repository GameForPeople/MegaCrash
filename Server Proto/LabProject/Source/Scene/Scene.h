#pragma once

#define GroundHeight 1600.f

class CGameFramework;
class CModelMesh;
struct RoomInfo;

class CGameObject;
class CDynamicObject;
class CStaticObject;
class CBullet;
class CPlayer;
class ContactReportCallback;
struct Ragdoll;

class CScene
{
public:
	CScene();
	~CScene();

	void SetFramework(CGameFramework* pFramework) { m_pFramework = pFramework; }
	void RegisterFoundation(PxFoundation* PxF) { m_pFoundation = PxF; }

	void BuildObjects();
	void ReleaseObjects();
	
	void InitPhysX();
	void ReleasePhysX();

	void AnimateObjects(float fTimeElapsed);
	void CreateRagDoll(CPlayer* Player, PlayerType type);

	void ProcessMsg(const MSGWRAPPER& msg);
	void SendMsgs(UINT MsgType, LPVOID buf = nullptr, UINT id = INVALID_OBJECT_ID);

	void ShootBullet(const XMFLOAT3& xmf3Pos, XMFLOAT3& xmf3Dir, int id, AtkType type = AtkType::Normal2);
	void RegisterRoomInfo(RoomInfo* room) { m_pRoomInfo = room; }

	void ReadGroundMesh();
	void BlockPosition(bool restart = false);
	PxExtendedVec3 GetRespawnPos(int idx);

	PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0));
	PxCapsuleControllerDesc GetControllerDesc(PlayerType type);

protected:
	CGameFramework						*m_pFramework = NULL;
	RoomInfo							*m_pRoomInfo = NULL;

	// Physx
	PxDefaultErrorCallback				m_ErrorCallback;
	PxFoundation						*m_pFoundation = NULL;
	PxPhysics							*m_pPhysics = NULL;
	PxScene								*m_pScene = NULL;
	PxMaterial							*m_pMaterial = NULL;
	PxPvd								*m_pPvd = NULL;
	PxCooking							*m_pCooking = NULL;

	ContactReportCallback				*m_ContactReportCallback;
	PxControllerManager					*m_pControllerManager = NULL;
	PxControllerFilters					m_filter;
	
	std::vector<CModelMesh*>			m_pVecGroundMesh;

	UINT								m_pnObjects[ObjectTag::Count];
	std::unordered_map<UINT, CPlayer*>	m_pPlayers;
	CDynamicObject						**m_pBlocks = NULL;
	CBullet								**m_pBullets = NULL;
	CGameObject							**m_pBoard = NULL;

	UINT								m_ObjIDCounter = 0;

	int									m_nCurrentBulletIdx = 0;
	int									m_GlobalKillCount = 0;

	float								m_fRestartCounter;
	float								m_fBulletTimer;

	std::chrono::time_point<std::chrono::system_clock>	start_time = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock>	now = std::chrono::system_clock::now();
	std::chrono::duration<float>			timeElapsed;

	bool								bRestart = false;
};
