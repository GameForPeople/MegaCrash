#pragma once

class CMesh;
struct RoomInfo;
struct Ragdoll;

using namespace std;

typedef enum {
	DYNAMIC,
	STATIC,
	CONTROLLER
} ActorType;

class ObjContainer {
public:
	ActorType			actorType;
	ObjectTag			objType;
	UINT				ID = INVALID_OBJECT_ID;
	int					Owner = INVALID_OBJECT_ID;
	void*				data;
};

class CGameObject
{
private:
	UINT			m_ID = 0;
	bool			m_bActive = false;

protected:
	ObjectTag		m_Tag;
	
	PxRigidActor	*m_pActor = NULL;

	int				m_LastHitId = INVALID_OBJECT_ID;

public:
	PxVec3			m_Pos	{ 0,0,0 };
	PxQuat			m_Quat	{ 0,0,0,1 };
	WORD			m_HP	= MAX_HP;
	WORD			m_SP	= MAX_SP;
	short			m_CP	= MAX_CP;

	CGameObject();
	virtual ~CGameObject() { delete m_pActor->userData; };

	void CreateActor(PxPhysics* Physics, PxScene* Scene, const PxTransform & t, const PxGeometry& geometry, PxMaterial* mat, float mass) {}
	
	virtual int Animate(float fTimeElapsed) { return 0; };

	ObjectTag	GetObjectTag() const { return m_Tag; }
	void SetObjectType(ObjectTag tag) { m_Tag = tag; }

	PxRigidActor* GetActor() { return m_pActor; }

	UINT GetID() const { return m_ID; }
	void SetID(UINT id) { m_ID = id; }

	void SetActivation(bool isActive) { m_bActive = isActive; }
	bool IsActive() const { return m_bActive; }

	void SetHP(UINT HP) { m_HP = HP; }
	void Damaged(WORD damage, int id) { 
		if (m_SP > 0 && m_SP <= MAX_SP) { 
			m_SP -= damage; 
			if (m_SP > MAX_SP) m_SP = 0; 
		}                     
		else {
			m_SP = 0;
			m_HP -= damage;
		}
		m_LastHitId = id;
	}
	WORD GetHP() const { return m_HP; }

	int GetLastHitId() { return m_LastHitId; }
};

class CStaticObject : public CGameObject
{
	PxRigidStatic		*m_pStaticActor = NULL;

public :
	CStaticObject() :CGameObject() {}
	virtual ~CStaticObject() {};

	PxRigidStatic * GetActor() { return m_pStaticActor; }
	void CreateActor(PxPhysics* Physics, PxScene* Scene, const PxTransform & t, const PxGeometry& geometry, PxMaterial* mat)
	{
		m_pStaticActor = Physics->createRigidStatic(t);
		PxShape* triangleMeshShape = PxRigidActorExt::createExclusiveShape(*m_pStaticActor, geometry, *mat);
		m_pActor = m_pStaticActor;
		Scene->addActor(*m_pStaticActor);

		ObjContainer* userdata = new ObjContainer();
		userdata->actorType = ActorType::STATIC;
		userdata->objType = ObjectTag::None;
		userdata->ID = 0;
		userdata->data = this;
		m_pStaticActor->userData = userdata;
	}
};

class CDynamicObject : public CGameObject
{
protected:
	PxRigidDynamic		*m_pDynamicActor = NULL;
	bool				m_isCollide = false;
	float				m_RespawnTimeCounter = 0.f;
	PxVec3				m_OriginPos{};

public:
	CDynamicObject() :CGameObject() {}
	virtual ~CDynamicObject() {};

	void IncreaseRespawnTimer(float timeElapsed) { m_RespawnTimeCounter += timeElapsed; }
	void ResetRespawnTimer() { m_RespawnTimeCounter = 0.f; }
	float GetRespawnTimer() { return m_RespawnTimeCounter; }

	void SetCollisionFlag(bool flag) { m_isCollide = flag; }
	bool IsCollide() { return m_isCollide; }

	void SetOriginPos(PxVec3 p) { m_OriginPos = p; }
	void ResetToOriginPos() { m_pDynamicActor->setGlobalPose(PxTransform(m_OriginPos)); }
	PxVec3 GetOriginPos() { return m_OriginPos; }

	PxRigidDynamic * GetActor() { return m_pDynamicActor; }

	void CreateActor(PxPhysics* Physics, PxScene* Scene, const PxTransform & t, const PxGeometry& geometry, PxMaterial* mat, float mass)
	{
		PxRigidDynamic* dynamic = PxCreateDynamic(*Physics, t, geometry, *mat, mass);
		
		m_pDynamicActor = dynamic;
		m_pActor = m_pDynamicActor;
		Scene->addActor(*m_pDynamicActor);
		m_pDynamicActor->putToSleep();
		
		ObjContainer* userdata = new ObjContainer();
		userdata->actorType = ActorType::DYNAMIC;
		userdata->objType = ObjectTag::None;
		userdata->data = this;
		m_pDynamicActor->userData = userdata;
	}
};

class CBullet : public CDynamicObject
{
private:
	bool				m_isReady = false;
	AtkType				m_AtkType;
	PxVec3				m_shootpos = { 0,0,0 };
	PxQuat				m_angle = { 0,0,0,0 };
	PxVec3				m_shootdir = { 0,0,0 };

public:
	virtual ~CBullet() {};
	void ResetBullet();

	void SetReadyToShoot(bool flag) { m_isReady = flag; }
	bool IsReadyToShoot() { return m_isReady; }

	void SetShootPos(PxVec3& pos) { m_Pos = m_shootpos = pos; }
	void SetShootAngle(PxQuat& angle) { m_Quat = m_angle = angle; }
	void SetShootDir(PxVec3& dir) { m_shootdir = dir; }
	void SetAtkType(AtkType type) { m_AtkType = type; }

	PxVec3	GetShootPos()		{ return m_shootpos; }
	PxVec3	GetShootDir()		{ return m_shootdir; }
	PxQuat	GetShootAngle()		{ return m_angle; }
	AtkType	GetAtkType()		{ return m_AtkType; }

	void Shoot() {
		m_pDynamicActor->setGlobalPose(PxTransform(m_shootpos, m_angle));
		m_pDynamicActor->putToSleep();
		m_pDynamicActor->clearForce();
		m_pDynamicActor->setLinearVelocity(m_shootdir * 10000);
		if (m_AtkType == AtkType::Skill_Shoot) {
			m_pDynamicActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
		}
		else
			m_pDynamicActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
	}
};

class CPlayer : public CDynamicObject
{
private:
	RoomInfo				*m_pRoominfo = nullptr;

	PxController			*m_pController = NULL;
	PxControllerFilters		m_filter;
	Ragdoll					*m_pRagdoll;

	UINT			m_AnimationState = 0;
	PxVec3			m_Dir = { 0,0,0 };
	PlayerType		m_Type = PlayerType::Ruby;
	MoveType		m_MoveType;
	int				m_KillCount = 0;

	int				Ani_Frame = 0;
	float			Ani_Time = 0;

	double	PrevHeight = 0.f;
	float	SPRegenerateTimer = 0.f;
	float	GravityVelocity = 0.f;
	float	FloatingTime = 0.f;
	float	SkillCoolDownTime[SkillType::TypeCount] = { 0 };
	float	StopCounter = 0.f; 
	float	jumpduration = 0;

	bool	IsKnockbacked = false;
	PxVec3	Dir_KnockBack;

public:
	bool			IsLanded = false;

	CPlayer() {}
	virtual ~CPlayer() { delete m_pRagdoll; }

	void SetRoominfo(RoomInfo* room) { m_pRoominfo = room; }
	RoomInfo* GetRoominfo() { return m_pRoominfo; }
	
	void SetType(PlayerType type) { m_Type = type; }
	PlayerType GetType() const { return m_Type; }

	void SetRagdoll(Ragdoll* ragdoll) { m_pRagdoll = ragdoll; }
	Ragdoll* GetRagdoll() { return m_pRagdoll; }
	
	void ResetGravityVelocity() { GravityVelocity = 0; }
	void OnMove() { StopCounter = 0.5f; }
	bool IsStop() { return StopCounter <= 0.0f; }

	int AttackedByObj(ObjContainer* Attacker, PxVec3 HitPos = {0,0,0});

	void SetDirection(const PxVec3& dir) { m_Dir = dir; }
	PxVec3 GetDirection() const { return m_Dir; }

	void SetMoveType(MoveType type);
	MoveType GetMoveType() const { return m_MoveType; }

	void SetKnockBack(PxVec3 atkpos);

	UINT GetAnimationState() const { return m_AnimationState; }
	void SetAnimationState(UINT state) { m_AnimationState = state; }

	PxController * GetController() { return m_pController; }

	int UpdateFrame(float fTimeElapsed);
	void AnimationFrameAdvance(float fTimeElapsed);
	int AttackProcess(int Attacktype);
	int MoveProcess(float fTimeElapsed);

	void IncreaseKillcount() { 	m_KillCount++;	}
	int GetKillCount() const { return m_KillCount; }

	void SetController(PxControllerManager* manager, const PxBoxControllerDesc& desc, PxScene* Scene)
	{
		m_pController = manager->createController(desc);
		m_pActor = m_pDynamicActor = m_pController->getActor();

		ObjContainer* userdata = new ObjContainer();
		userdata->data = this;
		userdata->actorType = ActorType::CONTROLLER;
		userdata->objType = ObjectTag::Players;
		userdata->ID = GetID();
		m_pController->setUserData((void*)userdata);
		m_pDynamicActor->userData = userdata;
	}
	void SetController(PxControllerManager* manager, const PxCapsuleControllerDesc& desc, PxScene* Scene) 
	{
		m_pController = manager->createController(desc);
		m_pActor = m_pDynamicActor = m_pController->getActor();

		ObjContainer* userdata = new ObjContainer();
		userdata->data = this;
		userdata->actorType = ActorType::CONTROLLER;
		userdata->objType = ObjectTag::Players;
		userdata->ID = GetID();
		m_pController->setUserData((void*)userdata);
		m_pDynamicActor->userData = userdata;
	}
};