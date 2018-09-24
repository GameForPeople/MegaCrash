#include "stdafx.h"
#include "Server\MainServer.h"
#include "Framework\GameFramework.h"
#include "Objects\GameObject.h"
#include "Mesh\Mesh.h"
#include "PhysX\Animation.h"
#include "Scene.h"

CScene::CScene()
{
	m_nCurrentBulletIdx = 0;
	m_fBulletTimer = 0.0f;
	m_fRestartCounter = 0.0f;
}

CScene::~CScene()
{
	ReleaseObjects();
}

void CScene::ReleaseObjects()
{
	ReleasePhysX();
}

class ContactReportCallback : public PxSimulationEventCallback, public PxUserControllerHitReport
{
public:
	CScene * mainScene;

	void setScene(CScene* scene) { mainScene = scene; }

	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onTrigger(PxTriggerPair* pairs, PxU32 count) { PX_UNUSED(pairs); PX_UNUSED(count); }
	void onAdvance(const PxRigidBody*const* actor, const PxTransform* pos, const PxU32 count) {}
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
	{
		for (PxU32 i = 0; i < nbPairs; i++)
		{
			ObjContainer* p = (ObjContainer*)pairHeader.actors[0]->userData;
			ObjContainer* p2 = (ObjContainer*)pairHeader.actors[1]->userData;
			if (!p && !p2) continue;

			if (p->objType == ObjectTag::Players || p2->objType == ObjectTag::Players)
			{
				CPlayer* Player = (p->objType == ObjectTag::Players) ? (CPlayer*)p->data : (CPlayer*)p2->data;
				ObjContainer* OtherObj = (p->objType != ObjectTag::Players) ? p : p2;

				if (OtherObj->actorType == ActorType::STATIC || 
					Player->AttackedByObj(OtherObj) == -1) continue;
			
				PK_MSG_DAMAGE packet{};
			
				packet.size = sizeof(PK_MSG_DAMAGE);
				packet.type = MSGTYPE::MSGACTION::DAMAGE;
				packet.HP = Player->GetHP(); 
				packet.victimid = Player->GetID();
				packet.attackerid = OtherObj->Owner;
			
				if (OtherObj->objType == ObjectTag::Weapon || OtherObj->objType == ObjectTag::Weapon_Skill)	
				{
					auto ragdoll = reinterpret_cast<RagdollNode*>(OtherObj->data);
					if (OtherObj->objType == ObjectTag::Weapon) 
					{
						packet.p = toXmfloat3(ragdoll->actor[0]->getGlobalPose().p);
						packet.atktype = AtkType::Normal1;
					}
					else 
					{
						packet.p = toXmfloat3(ragdoll->actor[1]->getGlobalPose().p);
						packet.atktype = AtkType::Skill;
					}
				}
				else if (OtherObj->objType == ObjectTag::Bullet || OtherObj->objType == ObjectTag::Block) 
				{
					Assign_float3(packet.p, reinterpret_cast<CDynamicObject*>(OtherObj->data)->m_Pos);
					if (OtherObj->objType == ObjectTag::Bullet) {
						if (reinterpret_cast<CBullet*>(OtherObj->data)->GetAtkType() == AtkType::Skill_Shoot) {
							packet.atktype = AtkType::Skill;
						}
					}
					else
						packet.atktype = AtkType::Normal2;
				}
				mainScene->SendMsgs(MSGTYPE::MSGACTION::DAMAGE, &packet);
				mainScene->SendMsgs(MSGTYPE::MSGUPDATE::UPDATE_PLAYER, Player, Player->GetID());
			}
			else if (p->objType == ObjectTag::GroundBoard && 
				(p2->objType == ObjectTag::Weapon || p2->objType == ObjectTag::Weapon_Skill))
			{

				PK_MSG_DAMAGE packet{};
				packet.size = sizeof(PK_MSG_DAMAGE);
				packet.type = MSGTYPE::MSGACTION::DAMAGE;
				packet.HP = 0;
				packet.attackerid = p2->Owner;
				packet.victimid = INVALID_OBJECT_ID;

				auto ragdoll = reinterpret_cast<RagdollNode*>(p2->data);
				if (p2->objType == ObjectTag::Weapon)
				{
					packet.p = toXmfloat3(ragdoll->actor[0]->getGlobalPose().p);
					packet.atktype = AtkType::Normal1;
				}
				else
				{
					packet.p = toXmfloat3(ragdoll->actor[1]->getGlobalPose().p);
					packet.atktype = AtkType::Skill;
				}

				mainScene->SendMsgs(MSGTYPE::MSGACTION::DAMAGE, &packet);
			}
			else if (
				p->objType == ObjectTag::Bullet &&
				(p2->objType == ObjectTag::Block || p2->objType == ObjectTag::GroundBoard))
			{
				auto bullet = reinterpret_cast<CBullet*>(p->data);
				auto otherobj = reinterpret_cast<CDynamicObject*>(p2->data);

				PK_MSG_DAMAGE packet{};

				packet.size = sizeof(PK_MSG_DAMAGE);
				packet.type = MSGTYPE::MSGACTION::DAMAGE;
				packet.HP = 0;
				packet.attackerid = p->Owner;
				packet.victimid = INVALID_OBJECT_ID;
				packet.atktype = bullet->GetAtkType() == AtkType::Normal2 ? AtkType::Normal2 : AtkType::Skill;

				Assign_float3(packet.p, bullet->m_Pos);
				mainScene->SendMsgs(MSGTYPE::MSGACTION::DAMAGE, &packet);

				otherobj->m_HP -= 11;
				bullet->SetReadyToShoot(false);
				bullet->SetCollisionFlag(true);
				bullet->SetActivation(false);
			}
			else if (p->objType == ObjectTag::Block && 
				(p2->objType == ObjectTag::Weapon || p2->objType == ObjectTag::Weapon_Skill))
			{
				PK_MSG_DAMAGE packet{};

				packet.size = sizeof(PK_MSG_DAMAGE);
				packet.type = MSGTYPE::MSGACTION::DAMAGE;
				packet.HP = 0;
				packet.attackerid = p2->Owner;
				packet.victimid = INVALID_OBJECT_ID;

				auto ragdoll = reinterpret_cast<RagdollNode*>(p2->data);
				if (p2->objType == ObjectTag::Weapon)
				{
					packet.p = toXmfloat3(ragdoll->actor[0]->getGlobalPose().p);
					packet.atktype = AtkType::Normal1;
				}
				else
				{
					packet.p = toXmfloat3(ragdoll->actor[1]->getGlobalPose().p);
					packet.atktype = AtkType::Skill;
				}

				reinterpret_cast<CDynamicObject*>(p->data)->m_HP -= 11;
				mainScene->SendMsgs(MSGTYPE::MSGACTION::DAMAGE, &packet);
			}
		}
	}

	//CCT Collide Callback
	virtual void onShapeHit(const PxControllerShapeHit& hit)
	{
		auto TouchedObj = (ObjContainer*)hit.actor->userData;
		if (TouchedObj->objType == ObjectTag::GroundBoard) 
		{
			auto Player = (CPlayer*)((ObjContainer*)hit.controller->getActor()->userData)->data;

			auto vel = hit.controller->getActor()->getLinearVelocity();
			if (-1 < vel.y && vel.y < 1) {
				Player->IsLanded = true;
				Player->ResetGravityVelocity();
			}
			
			if (TouchedObj->actorType == ActorType::STATIC) return;
			if (((CGameObject*)TouchedObj->data)->IsActive() == false) return;
			if (hit.worldPos.y < hit.controller->getFootPosition().y + 5) return;
			if (Player->AttackedByObj(TouchedObj, toVec3(hit.worldPos)) == -1) return;

			PK_MSG_DAMAGE packet{};

			packet.size = sizeof(PK_MSG_DAMAGE);
			packet.type = MSGTYPE::MSGACTION::DAMAGE;
			packet.HP = Player->GetHP();
			packet.attackerid = Player->GetID();
			packet.victimid = TouchedObj->Owner;
			packet.atktype = AtkType::Normal1;
			
			mainScene->SendMsgs(MSGTYPE::MSGACTION::DAMAGE, &packet);
			mainScene->SendMsgs(MSGTYPE::MSGUPDATE::UPDATE_PLAYER, Player, Player->GetID());

		}
		//cout << str_objtag(TouchedObj->objType) <<  hit.worldPos.x << ", " << hit.worldPos.y << ", " << hit.worldPos.z << endl;
	};
	virtual void onControllerHit(const PxControllersHit& hit) {}
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) {}
};

PxFilterFlags contactReportFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	// Enable CCD(continuous collision detection) flag
	pairFlags =
		PxPairFlag::eCONTACT_DEFAULT
		| PxPairFlag::eDETECT_CCD_CONTACT
		| PxPairFlag::eNOTIFY_TOUCH_CCD
		| PxPairFlag::eNOTIFY_TOUCH_FOUND
		| PxPairFlag::eNOTIFY_CONTACT_POINTS
		| PxPairFlag::eCONTACT_EVENT_POSE;

	return PxFilterFlag::eDEFAULT;
}

void CScene::InitPhysX()
{
	// Physx Init

#if PVD_STANDBY
	m_pPvd = PxCreatePvd(*m_pFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	bool retval = m_pPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	if (retval)	printf("pvd Connection Successed\n");
	else printf("Fail to Connect pvd\n");
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPvd);
#else
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true);
#endif

	m_ContactReportCallback = new ContactReportCallback();
	m_ContactReportCallback->mainScene = this;

	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -750.f, 0.0f);
	//m_pPhysxDispatcher = PxDefaultCpuDispatcherCreate(4);
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(4);
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = m_ContactReportCallback;

	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
	sceneDesc.flags |= PxSceneFlag::eENABLE_KINEMATIC_PAIRS;
	sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;

	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, PxCookingParams(PxTolerancesScale()));
	m_pScene = m_pPhysics->createScene(sceneDesc);

#if PVD_STANDBY
	PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
#endif

	m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.3f);

	m_pControllerManager = PxCreateControllerManager(*m_pScene);

	cout << "PhysX Initiated\n";
}

void CScene::ReleasePhysX()
{
	for (UINT i = 0; i < m_pnObjects[ObjectTag::GroundBoard]; ++i)
		delete m_pBoard[i];
	for (UINT i = 0; i < m_pnObjects[ObjectTag::Block]; ++i)
		delete m_pBlocks[i];
	for (UINT i = 0; i < m_pnObjects[ObjectTag::Bullet]; ++i)
		delete m_pBullets[i];
	for (auto p : m_pPlayers)
		delete p.second;
	for (auto m : m_pVecGroundMesh)
		delete m;

	delete[] m_pBoard;
	delete[] m_pBlocks;
	delete[] m_pBullets;
	m_pPlayers.clear();
	m_pVecGroundMesh.clear();


	m_pScene->release();
	m_pPhysics->release();
#if _DEBUG
	if (m_pPvd) {
		PxPvdTransport* transport = m_pPvd->getTransport();
		m_pPvd->release();
		transport->release();
	}
#endif

	cout << "delete Scene\n";
}

void CScene::BuildObjects()
{
	InitPhysX();
	timeElapsed = now - start_time;

	// Init Mesh
	ReadGroundMesh();

	// Ground Board
	m_ObjIDCounter = START_STAGE_ID;
	m_pnObjects[ObjectTag::GroundBoard] = m_pVecGroundMesh.size();
	m_pBoard = new CGameObject*[m_pnObjects[ObjectTag::GroundBoard]];
	for (UINT i = 0; i < m_pnObjects[ObjectTag::GroundBoard]; ++i)
	{
		if (m_pVecGroundMesh[i]->m_isBreakable) {
			CDynamicObject *Ground = new CDynamicObject();
			Ground->SetObjectType(ObjectTag::GroundBoard);
			Ground->SetID(m_ObjIDCounter++);
			Ground->CreateActor(m_pPhysics, m_pScene, 
				PxTransform(PxVec3(16262.f, 16766.f, 26967.f)),
				PxConvexMeshGeometry(m_pVecGroundMesh[i]->m_pConvexmesh), 
				m_pMaterial, 50000.);
			Ground->SetActivation(false);
			((ObjContainer*)Ground->GetActor()->userData)->objType = ObjectTag::GroundBoard;
			((ObjContainer*)Ground->GetActor()->userData)->ID = Ground->GetID();
			Ground->GetActor()->setName("Ground");
			Ground->GetActor()->setMaxAngularVelocity(3.f);
			Ground->GetActor()->setSleepThreshold(8'000);
			Ground->GetActor()->setStabilizationThreshold(7'000);
			Ground->m_Pos = PxVec3(16262.f, 16766.f, 26967.f);

			m_pBoard[i] = Ground;
		}
		else {
			CStaticObject *Ground = new CStaticObject();
			Ground->SetActivation(false);
			Ground->CreateActor(
				m_pPhysics, m_pScene,
				PxTransform(PxVec3(16262.f, 16766.f, 26967.f)),
				PxConvexMeshGeometry(m_pVecGroundMesh[i]->m_pConvexmesh), m_pMaterial);
			Ground->SetID(m_ObjIDCounter++);
			((ObjContainer*)Ground->GetActor()->userData)->objType = ObjectTag::GroundBoard;
			Ground->SetObjectType(ObjectTag::GroundBoard);
			Ground->m_Pos = PxVec3(16262.f, 16766.f, 26967.f);

			m_pBoard[i] = Ground;
		}
	}

	// Player
	m_pnObjects[ObjectTag::Players] = m_pRoomInfo->MaxClientCount;
	int offset = 0;
	for (auto clientcontainer : m_pRoomInfo->ClientIDMatcher) {
		auto client = clientcontainer.second;

		PxCapsuleControllerDesc desc = GetControllerDesc(client->PlayerType);
		desc.position = GetRespawnPos(client->ID);

		CPlayer* Player = new CPlayer();
		Player->SetOriginPos(toVec3(GetRespawnPos(client->ID)));
		Player->SetID(client->ID);
		Player->SetObjectType(ObjectTag::Players);
		Player->SetController(m_pControllerManager, desc, m_pScene);
		Player->SetType(client->PlayerType);
		Player->SetRoominfo(m_pRoomInfo);

		m_pPlayers[client->ID] = Player;

		CreateRagDoll(Player, client->PlayerType);

		PK_MSG_MOVE packet{};
		packet.size = sizeof(PK_MSG_MOVE);
		packet.type = MSGTYPE::MSGUPDATE::UPDATE_OBJECT;
		packet.id = Player->GetID();
		Assign_float3(packet.p, Player->GetController()->getActor()->getGlobalPose().p);
		packet.q = { 0,0,0,1 };
		m_pRoomInfo->SendPacket(&packet);

		offset++;
	}

	// Block
	BlockPosition();

	//Bullets
	m_ObjIDCounter = START_BULLET_ID;
	m_pnObjects[ObjectTag::Bullet] = 50;
	m_pBullets = new CBullet*[m_pnObjects[ObjectTag::Bullet]];
	for (UINT i = 0; i < m_pnObjects[ObjectTag::Bullet]; ++i)
	{
		CBullet* Bullet = new CBullet();
		PxTransform pos = PxTransform(PxVec3(i * 100.f, -10000.f, i * 100));
		Bullet->SetOriginPos(pos.p);
		Bullet->SetID(m_ObjIDCounter++);
		Bullet->CreateActor(m_pPhysics, m_pScene, pos, PxSphereGeometry(25.f), m_pMaterial, 1000000);
		Bullet->SetObjectType(ObjectTag::Bullet);
		Bullet->GetActor()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		((ObjContainer*)Bullet->GetActor()->userData)->objType = ObjectTag::Bullet;
		((ObjContainer*)Bullet->GetActor()->userData)->ID = Bullet->GetID();
		Bullet->GetActor()->setName("Bullet");
		Bullet->GetActor()->putToSleep();
		Bullet->SetActivation(false);

		m_pBullets[i] = Bullet;

		SendMsgs(MSGTYPE::MSGUPDATE::DELETE_OBJECT, nullptr, m_pBullets[i]->GetID());
	}
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	static bool retval;
	if (m_pRoomInfo->Status != RoomState::Running) return;

	if (m_pRoomInfo->MaxClientCount >= 2 && m_pRoomInfo->AliveClientCounter <= 1)
	{
		if (m_pRoomInfo->ClientIDMatcher.size() <= 1) {
			for (auto clientcontainer : m_pRoomInfo->ClientIDMatcher) {
				auto client = clientcontainer.second;

				PK_ROOM_END p;
				p.size = sizeof(PK_ROOM_END);
				p.type = MSGTYPE::MSGROOM::ENDGAME;
				p.RoomID = m_pRoomInfo->RoomID;
				p.WinnerID = client->ID;

				m_pRoomInfo->SendPacket(&p);

				m_pRoomInfo->Status = RoomState::Finished;
			}
		}
		return;
	}

	if (fTimeElapsed > 0 && !bRestart)
	{
		m_pScene->simulate(fTimeElapsed);

		// Animate
		for (auto& player : m_pPlayers) {
			UINT idx = player.first;

			int retcode = m_pPlayers[idx]->UpdateFrame(fTimeElapsed);
			switch (retcode) 
			{
			case MSGTYPE::MSGUPDATE::RESET_OBJECT:
				// Respawn Player
				SendMsgs(retcode, nullptr, m_pPlayers[idx]->GetID());
				break;

			case MSGTYPE::MSGUPDATE::DELETE_OBJECT:
			{
				// Player Dead		
				PK_MSG_PLAYERDIED pk_died{};
				pk_died.id = idx;
				pk_died.p = toXmfloat3(m_pPlayers[idx]->m_Pos);
				m_pRoomInfo->SendPacket(&pk_died);

				if (player.second->GetLastHitId() != INVALID_OBJECT_ID &&
					player.second->GetLastHitId() != idx)
				{
					auto attacker = m_pPlayers[player.second->GetLastHitId()];
					attacker->IncreaseKillcount();
					if (m_GlobalKillCount < attacker->GetKillCount())
						m_GlobalKillCount = attacker->GetKillCount();

					PK_MSG_KILLCOUNT pk_killcount{};
					pk_killcount.attackerid = attacker->GetID();
					pk_killcount.PlayerKill = attacker->GetKillCount();
					pk_killcount.GlobalKill = m_GlobalKillCount;
					m_pRoomInfo->SendPacket(&pk_killcount);

					if (attacker->GetKillCount() >= GAMEEND_KILLCOUNT) {
						cout << "Finish\nWinner : " << attacker->GetID() << endl;
						m_pRoomInfo->Status = RoomState::Finished;

						PK_ROOM_END pk_end;
						pk_end.size = sizeof(PK_ROOM_END);
						pk_end.type = MSGTYPE::MSGROOM::ENDGAME;
						pk_end.RoomID = m_pRoomInfo->RoomID;
						pk_end.WinnerID = attacker->GetID();

						m_pRoomInfo->SendPacket(&pk_end);
						break;
					}
				}
				SendMsgs(retcode, nullptr, m_pPlayers[idx]->GetID());
				break;
			}
			case MSGTYPE::MSGUPDATE::UPDATE_PLAYER:
				SendMsgs(MSGTYPE::MSGUPDATE::UPDATE_PLAYER, m_pPlayers[idx], m_pPlayers[idx]->GetID());
				break;
			default:
				break;
			}

			m_pPlayers[idx]->MoveProcess(fTimeElapsed);
			SendMsgs(MSGTYPE::MSGUPDATE::UPDATE_PLAYER, m_pPlayers[idx], m_pPlayers[idx]->GetID());
		}

		for (UINT i = 0; i < m_pnObjects[ObjectTag::Block]; ++i) {
			if (m_pBlocks[i]->IsActive() &&
				(m_pBlocks[i]->GetActor()->getGlobalPose().p.y < 5000.f || m_pBlocks[i]->GetHP() > MAX_HP))
			{
				m_pBlocks[i]->GetActor()->setGlobalPose(PxTransform(PxVec3(-5000 - (i * 70))));
				m_pBlocks[i]->SetActivation(false);
				SendMsgs(MSGTYPE::MSGUPDATE::DELETE_OBJECT, nullptr, m_pBlocks[i]->GetID());
				continue;
			}

			if (m_pBlocks[i]->GetActor()->isSleeping())
				m_pBlocks[i]->SetActivation(false);
			else {
				m_pBlocks[i]->m_Pos = m_pBlocks[i]->GetActor()->getGlobalPose().p;
				m_pBlocks[i]->m_Quat = m_pBlocks[i]->GetActor()->getGlobalPose().q;
				m_pBlocks[i]->SetActivation(true);
			}
		}

		for (UINT i = 0; i < m_pnObjects[ObjectTag::Bullet]; i++)
		{
			if (m_pBullets[i]->IsCollide() == true)
			{
				m_pBullets[i]->ResetBullet();

				PK_MSG_MOVE packet{};
				packet.size = sizeof(PK_MSG_MOVE);
				packet.type = MSGTYPE::MSGUPDATE::UPDATE_OBJECT;
				packet.id = m_pBullets[i]->GetID();
				Assign_float3(packet.p, m_pBullets[i]->GetOriginPos());
				Assign_float4(packet.q, m_pBullets[i]->m_Quat);
				m_pRoomInfo->SendPacket(&packet);
				SendMsgs(MSGTYPE::MSGUPDATE::DELETE_OBJECT, nullptr, m_pBullets[i]->GetID());
			}

			if (m_pBullets[i]->GetActor()->isSleeping() == false)
				m_pBullets[i]->IncreaseRespawnTimer(fTimeElapsed);

			if (m_pBullets[i]->GetRespawnTimer() > 3.f) {
				m_pBullets[i]->SetCollisionFlag(true);
				m_pBullets[i]->SetActivation(false);
			}

			if (m_pBullets[i]->IsReadyToShoot()) {
				m_pBullets[i]->Shoot();
				m_pBullets[i]->SetReadyToShoot(false);
			}

			m_pBullets[i]->m_Pos = m_pBullets[i]->GetActor()->getGlobalPose().p;
			m_pBullets[i]->m_Quat = m_pBullets[i]->GetActor()->getGlobalPose().q;
		}

		for (int i = 0; i < m_pnObjects[ObjectTag::GroundBoard]; i++) 
		{
			if (((ObjContainer*)m_pBoard[i]->GetActor()->userData)->actorType == ActorType::DYNAMIC) 
			{
				auto board = (CDynamicObject*)m_pBoard[i];

				if (board->IsCollide()) {
					board->GetActor()->setLinearVelocity(PxVec3{ 0 });
					board->GetActor()->setGlobalPose(PxTransform(PxVec3(-100000), PxQuat(0, 0, 0, 1)));
					board->GetActor()->putToSleep();
					board->SetActivation(false);
					continue;
				}

				if (board->GetActor()->isSleeping()) {
					board->SetActivation(false);
				}
				else {
					board->SetActivation(true);
					if (board->GetActor()->getGlobalPose().p.y < -100000)
					{
						board->SetCollisionFlag(true);
					}
				}

				m_pBoard[i]->m_Pos = board->GetActor()->getGlobalPose().p;
				m_pBoard[i]->m_Quat = board->GetActor()->getGlobalPose().q;
			}
		}

		PxU32 errcode = 0;
		retval = m_pScene->fetchResults(true, &errcode);
		if (errcode != 0) {
			cout << "error : " << errcode << endl;
		}
	}
	else if (fTimeElapsed > 0 && bRestart) {
		BlockPosition(true);
		bRestart = false;
	}
}

void CScene::CreateRagDoll(CPlayer* Player, PlayerType type)
{
	Ragdoll* ragdoll;
	PxBoxGeometry ColliderBox;
	PxBoxGeometry SkillColliderBox;
	switch (type) {
	case PlayerType::Ruby:
	{
		ragdoll = ReadAnimationfile("Hammering");
		ragdoll->SetActiveFrame(5, 20, 0.8f);
		ColliderBox = PxBoxGeometry(50, 50, 50);
		SkillColliderBox = PxBoxGeometry(80, 80, 80);
		break;
	}
	case PlayerType::Legion:
	{
		ragdoll = ReadAnimationfile("SpearCharge");
		ragdoll->SetActiveFrame(25, 40, 0.2f);
		ColliderBox = PxBoxGeometry(20, 20, 20);
		SkillColliderBox = PxBoxGeometry(80, 80, 80);
		break;
	}
	case PlayerType::Epsilon:
	{
		ragdoll = ReadAnimationfile("SpearCharge");
		ragdoll->SetActiveFrame(25, 40, 0.5f);
		ColliderBox = PxBoxGeometry(1, 1, 1);
		SkillColliderBox = PxBoxGeometry(1, 1, 1);
		break;
	}
	case PlayerType::Gravis:
	{
		ragdoll = ReadAnimationfile("SpearCharge");
		ragdoll->SetActiveFrame(25, 40, 0.5f);
		ColliderBox = PxBoxGeometry(20, 20, 20);
		SkillColliderBox = PxBoxGeometry(80, 80, 80);
		break;
	}
	}

	for (int bone = 0; bone < ragdoll->nBones; ++bone) {
		if (ragdoll->pRagdollNode[bone].Valid) {
			PxRigidDynamic* actor = createDynamic(
				PxTransform(PxVec3(-5000 * Player->GetID() + 1)),
				ColliderBox);
			actor->setName(ragdoll->pRagdollNode[bone].name.c_str());
			actor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			ObjContainer* userdata = new ObjContainer();
			userdata->data = &ragdoll->pRagdollNode[bone];
			userdata->objType = ObjectTag::Weapon;
			userdata->actorType = ActorType::DYNAMIC; 
			userdata->Owner = Player->GetID();
			actor->userData = userdata;

			ragdoll->pRagdollNode[bone].actor[0] = actor;

			PxRigidDynamic* skillactor = createDynamic(
				PxTransform(PxVec3(-50000 * Player->GetID() + 1)),
				SkillColliderBox);
			skillactor->setName(ragdoll->pRagdollNode[bone].name.c_str());
			skillactor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			ObjContainer* userdata2 = new ObjContainer();
			userdata2->data = &ragdoll->pRagdollNode[bone];
			userdata2->objType = ObjectTag::Weapon_Skill;
			userdata2->actorType = ActorType::DYNAMIC;
			userdata2->Owner = Player->GetID();
			skillactor->userData = userdata2;

			ragdoll->pRagdollNode[bone].actor[1] = skillactor;
		}
	}
	Player->SetRagdoll(ragdoll);
}

PxCapsuleControllerDesc CScene::GetControllerDesc(PlayerType type)
{
	PxCapsuleControllerDesc desc;
	desc.density = 1;
	desc.material = m_pMaterial;
	desc.reportCallback = m_ContactReportCallback;

	switch (type) {
	case PlayerType::Ruby:
	{
		desc.height = 10;
		desc.radius = 60;
		break;
	}
	case PlayerType::Legion:
	{
		desc.height = 25;
		desc.radius = 60;
		break;
	}
	case PlayerType::Epsilon:
	{
		desc.height = 15;
		desc.radius = 60;
		break;
	}
	case PlayerType::Gravis:
	{
		desc.height = 85;
		desc.radius = 60;
		break;
	}
	}

	return desc;
}

void CScene::ProcessMsg(const MSGWRAPPER& msg)
{
	switch (((char*)msg)[1])
	{
	case MSGTYPE::MSGACTION::ATTACK:
	{
		PK_MSG_ATTACK* m = (PK_MSG_ATTACK*)msg;

		int retcode = m_pPlayers[m->id]->AttackProcess(m->atktype);
		switch (retcode) {
		case AtkType::Normal2:
			ShootBullet(m->p, m->d, m->id);
			break;
		case AtkType::Skill_Shoot:
			ShootBullet(m->p, m->d, m->id, AtkType::Skill_Shoot);
			break;
		}
		if (retcode == AtkType::InvalidAtkType) break;

		SendMsgs(MSGTYPE::MSGACTION::ATTACK, msg);
		break;
	}

	case MSGTYPE::MSGACTION::MOVE:
	{
		PK_MSG_MOVE* m = (PK_MSG_MOVE*)msg;
		m_pPlayers[m->id]->m_Quat = toPxQuat(m->q);
		m_pPlayers[m->id]->SetDirection(toPxVec3(m->p));
		m_pPlayers[m->id]->SetMoveType((MoveType)m->movetype);
		if(m->p.x != 0 && m->p.y != 0 && m->p.z != 0)
			m_pPlayers[m->id]->OnMove();
		break;
	}

	case MSGTYPE::MSGACTION::ANIMATE:
	{
		PK_MSG_STATECHANGE* m = (PK_MSG_STATECHANGE*)msg;
		//m_pPlayers[m->id]->SetAnimationState(m->data1);
		m->data2 = m->data3 = 0;
		SendMsgs(MSGTYPE::MSGACTION::ANIMATE, m);

		break;
	}

	case MSGTYPE::CHEAT::RESTART:
	{
		bRestart = true;
		break;
	}
	}
}

void CScene::SendMsgs(UINT MsgType, LPVOID buf, UINT id)
{
	now = std::chrono::system_clock::now();
	timeElapsed = now - start_time;

	switch (MsgType)
	{
	case MSGTYPE::MSGUPDATE::UPDATE_OBJECT:
	{
		start_time = std::chrono::system_clock::now();
		PK_MSG_MOVE packet{};
		packet.size = sizeof(PK_MSG_MOVE);
		packet.type = MSGTYPE::MSGUPDATE::UPDATE_OBJECT;
		for (auto& playerContainer : m_pPlayers) {
			auto player = playerContainer.second;
			packet.id = player->GetID();
			packet.p = toXmfloat3(player->m_Pos);
			packet.q = toXmfloat4(player->m_Quat);
			packet.isLanded = player->IsLanded;
			packet.movetype = player->GetMoveType();
			m_pRoomInfo->SendPacket(&packet);
		}

		for (int i = 0; i < m_pnObjects[ObjectTag::Block]; ++i) {
			if (m_pBlocks[i]->IsActive() == true)
			{
				packet.id = m_pBlocks[i]->GetID();
				Assign_float3(packet.p, m_pBlocks[i]->m_Pos);
				Assign_float4(packet.q, m_pBlocks[i]->m_Quat);
				m_pRoomInfo->SendPacket(&packet);
			}
		}

		for (int i = 0; i < m_pnObjects[ObjectTag::Bullet]; ++i) {
			if (m_pBullets[i]->IsActive() == true) {
				packet.id = m_pBullets[i]->GetID();
				Assign_float3(packet.p, m_pBullets[i]->m_Pos);
				Assign_float4(packet.q, m_pBullets[i]->m_Quat);
				m_pRoomInfo->SendPacket(&packet);
			}
		}

		for (int i = 0; i < m_pnObjects[ObjectTag::GroundBoard]; ++i) {
			if (m_pBoard[i]->IsActive() == true) {
				packet.id = m_pBoard[i]->GetID();
				Assign_float3(packet.p, m_pBoard[i]->m_Pos);
				Assign_float4(packet.q, m_pBoard[i]->m_Quat);
				m_pRoomInfo->SendPacket(&packet);
			}
		}
		break;
	}

	case MSGTYPE::MSGACTION::ANIMATE:
	case MSGTYPE::MSGACTION::ATTACK:
	{
		PK_MSG_STATECHANGE* packet = (PK_MSG_STATECHANGE*)buf;
		m_pRoomInfo->SendPacket(packet);
		break;
	}

	case MSGTYPE::MSGACTION::DAMAGE:
	{
		m_pRoomInfo->SendPacket(buf);
		break;
	}

	case MSGTYPE::MSGUPDATE::UPDATE_PLAYER:
	{
		CPlayer* Player = reinterpret_cast<CPlayer*>(buf);

		PK_MSG_STATECHANGE packet;
		packet.type = MSGTYPE::MSGUPDATE::UPDATE_PLAYER;
		packet.size = sizeof(PK_MSG_STATECHANGE);
		packet.id = id;
		packet.data1 = Player->m_HP;
		packet.data2 = Player->m_SP;
		packet.data3 = Player->m_CP;

		m_pRoomInfo->SendPacket(id, &packet);
		break;
	}

	case MSGTYPE::MSGUPDATE::RESET_OBJECT:
	{
		PK_MSG_STATECHANGE packet;
		packet.type = MSGTYPE::MSGUPDATE::RESET_OBJECT;
		packet.size = sizeof(PK_MSG_STATECHANGE);
		packet.id = id;
		packet.data1 = MAX_HP;
		packet.data2 = MAX_SP;
		packet.data3 = MAX_CP;

		m_pRoomInfo->SendPacket(&packet);
		break;
	}

	case MSGTYPE::MSGUPDATE::DELETE_OBJECT:
	{
		PK_MSG_DELETE_PLAYER packet;
		packet.id = id;
		packet.size = sizeof(PK_MSG_DELETE_PLAYER);
		packet.type = MSGTYPE::MSGUPDATE::DELETE_OBJECT;

		m_pRoomInfo->SendPacket(&packet);
		break;
	}
	}
}

void CScene::ShootBullet(const XMFLOAT3& xmf3Pos, XMFLOAT3& xmf3Dir, int id, AtkType type)
{
	if (m_nCurrentBulletIdx == m_pnObjects[ObjectTag::Bullet])
		m_nCurrentBulletIdx = 0;
	auto bullet = m_pBullets[m_nCurrentBulletIdx++];
	PxVec3 Velocity = toPxVec3(xmf3Dir);
	float offset_horizontal{ 100.f };
	float offset_vertical{ 100.f };
	
	if (m_pPlayers[id]->GetMoveType() == MoveType::Dash)
		offset_horizontal = 300;
	else if (m_pPlayers[id]->GetMoveType() == MoveType::Normal)
		offset_horizontal = 200;

	
	PxVec3 ShootPos{ 
		xmf3Pos.x + xmf3Dir.x * offset_horizontal, 
		xmf3Pos.y + xmf3Dir.y * offset_vertical, 
		xmf3Pos.z + xmf3Dir.z * offset_horizontal 
	};
	PxQuat angle = m_pPlayers[id]->m_Quat * PxQuat(3.141593f, PxVec3(0, 1, 0));

	if (m_pPlayers[id]->GetType() == PlayerType::Epsilon)
		Velocity *= 2;

	bullet->SetShootPos(ShootPos);
	bullet->SetShootAngle(angle);
	bullet->SetShootDir(Velocity);
	bullet->SetReadyToShoot(true);
	bullet->SetActivation(true);
	bullet->SetAtkType(type);
	((ObjContainer*)bullet->GetActor()->userData)->Owner = id;

	PK_MSG_SHOOT p{};
	p.size = sizeof(PK_MSG_SHOOT);
	p.type = MSGTYPE::MSGACTION::SHOOT;
	p.bulletid = bullet->GetID();
	p.shooterid = m_pPlayers[id]->GetID();
	p.isSkill = (type == AtkType::Skill_Shoot) ? true : false;
	
	m_pRoomInfo->SendPacket(&p);
	SendMsgs(MSGTYPE::MSGUPDATE::DELETE_OBJECT, nullptr, bullet->GetID());
}


void CScene::ReadGroundMesh()
{
	string root = "./ServerAssets/Meshes/";
	string name = "OutPostBreakable";
	string path = root + name + "/" + name + ".mbox";
	
	int nMesh = 563;
	
	FILE* fp = nullptr;
	fopen_s(&fp, path.c_str(), "rb");
	fread_s(&nMesh, sizeof(UINT), sizeof(UINT), 1, fp);
	
	for (int i = 0; i < nMesh; ++i) {
		m_pVecGroundMesh.push_back(new CModelMesh(m_pPhysics, m_pCooking, fp, 1.0f));
	}
	
	fclose(fp);
}

void CScene::BlockPosition(bool restart)
{
	int nStride = sizeof(PxVec3);
	int nVerts = 0;

	string Filepath = "./ServerAssets/BlockPosition.pos";

	FILE* fp = nullptr;
	fopen_s(&fp, Filepath.c_str(), "rb");
	fread_s(&nVerts, sizeof(UINT), sizeof(UINT), 1, fp);
	PxVec3* pVerts = new PxVec3[nVerts];
	fread_s(pVerts, nStride * nVerts, nStride, nVerts, fp);
	fclose(fp);

	if (!restart) {
		m_ObjIDCounter = START_CUBE_ID;

		m_pnObjects[ObjectTag::Block] = nVerts;
		m_pBlocks = new CDynamicObject*[m_pnObjects[ObjectTag::Block]];

		for (int i = 0; i < m_pnObjects[ObjectTag::Block]; ++i)
		{
			PxTransform pos = PxTransform(pVerts[i]);
			CDynamicObject *Block = new CDynamicObject();
			Block->SetOriginPos(pos.p);
			Block->SetID(m_ObjIDCounter++);
			Block->SetObjectType(ObjectTag::Block);
			Block->CreateActor(m_pPhysics, m_pScene, pos, PxBoxGeometry(PxVec3(96.f)), m_pMaterial, 100);
			Block->SetActivation(true);
			((ObjContainer*)Block->GetActor()->userData)->objType = ObjectTag::Block;
			((ObjContainer*)Block->GetActor()->userData)->ID = Block->GetID();
			Block->GetActor()->setName("Block");
			Block->GetActor()->setSleepThreshold(1'000);
			Block->GetActor()->setStabilizationThreshold(10'000);
			Block->m_Pos = pos.p;

			m_pBlocks[i] = Block;

			PK_MSG_MOVE packet{};
			packet.size = sizeof(PK_MSG_MOVE);
			packet.type = MSGTYPE::MSGUPDATE::UPDATE_OBJECT;
			packet.id = Block->GetID();
			Assign_float3(packet.p, pos.p);
			packet.q = { 0,0,0,0 };
			m_pRoomInfo->SendPacket(&packet);
		}
	}

	else {
		for (int i = 0; i < m_pnObjects[ObjectTag::Block]; ++i)
		{
			m_pBlocks[i]->GetActor()->setGlobalPose(PxTransform(pVerts[i]));
			m_pBlocks[i]->GetActor()->clearForce();
			m_pBlocks[i]->GetActor()->putToSleep();

			PK_MSG_MOVE packet{};
			packet.size = sizeof(PK_MSG_MOVE);
			packet.type = MSGTYPE::MSGUPDATE::UPDATE_OBJECT;
			packet.id = m_pBlocks[i]->GetID();
			Assign_float3(packet.p, pVerts[i]);
			packet.q = { 0,0,0,1 };
			m_pRoomInfo->SendPacket(&packet);
		}
		for (int i = 0; i < m_pnObjects[ObjectTag::GroundBoard]; ++i) 
		{
			if (m_pVecGroundMesh[i]->m_isBreakable) {
				auto board = (CDynamicObject*)m_pBoard[i];
				board->GetActor()->setGlobalPose(
					PxTransform(PxVec3(16262.f, 16766.f, 26967.f), PxQuat(0, 0, 0, 1)));
				board->GetActor()->clearForce();
				board->GetActor()->putToSleep();

				PK_MSG_MOVE packet{};
				packet.size = sizeof(PK_MSG_MOVE);
				packet.type = MSGTYPE::MSGUPDATE::UPDATE_OBJECT;
				packet.id = board->GetID();
				Assign_float3(packet.p, PxVec3(16262.f, 16766.f, 26967.f));
				packet.q = { 0,0,0,1 };
				m_pRoomInfo->SendPacket(&packet);
			}
		}
	}

	delete pVerts;
}

PxExtendedVec3 CScene::GetRespawnPos(int idx)
{
	PxExtendedVec3 pos = { 16262.f, 21507.f, 26967.f };
	switch (idx % 4) {
	case 0: pos = { 16262.f, 21507.f, 28790.f }; break;
	case 1: pos = { 28571.f, 19284.f, 28002.f }; break;
	case 2: pos = { 22473.f, 17380.f, 22029.f }; break;
	case 3: pos = { 12849.f, 19000.f, 20363.f }; break;
	}
	return pos;
}

PxRigidDynamic * CScene::createDynamic(const PxTransform & t, const PxGeometry & geometry, const PxVec3 & velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*m_pPhysics, t, geometry, *m_pMaterial, 100.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	m_pScene->addActor(*dynamic);
	return dynamic;
}


