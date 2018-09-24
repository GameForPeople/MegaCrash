#include "stdafx.h"
#include "Server/MainServer.h"
#include "Mesh/Mesh.h"
#include "GameObject.h"

CGameObject::CGameObject()
	: m_ID(INVALID_OBJECT_ID)
{
	m_bActive = true;
}

////////////////////////////////////////////////////////////////////////

int CPlayer::AttackedByObj(ObjContainer * Attacker, PxVec3 HitPos)
{
	int retcode = 0;
	switch (Attacker->objType) {
	case ObjectTag::Bullet: 
	{
		auto bullet = static_cast<CBullet*>(Attacker->data);
		int dmg = bullet->GetAtkType() == AtkType::Normal2 ? DMG_BULLET : DMG_NORMAL;
		Damaged(dmg, Attacker->Owner);
		SetKnockBack(bullet->m_Pos);
		bullet->SetCollisionFlag(true);
		bullet->SetReadyToShoot(false);
		bullet->SetActivation(false);
		break; 
	}

	case ObjectTag::Weapon:
	{
		if (Attacker->Owner == GetID()) {
			retcode = -1;
		}
		else {
			Damaged(DMG_NORMAL, Attacker->Owner);
			HitPos = reinterpret_cast<RagdollNode*>(Attacker->data)->actor[0]->getGlobalPose().p;
			SetKnockBack(HitPos);
		}
		break;
	}

	case ObjectTag::Weapon_Skill:
	{
		if (Attacker->Owner == GetID()) {
			retcode = -1;
		}
		else {
			Damaged(DMG_SKILL, Attacker->Owner);
			HitPos = reinterpret_cast<RagdollNode*>(Attacker->data)->actor[1]->getGlobalPose().p;
			SetKnockBack(HitPos);
			break;
		}
		break;
	}

	case ObjectTag::Block:
	{
		auto block = reinterpret_cast<CDynamicObject*>(Attacker->data);
		if (block->GetActor()->getLinearVelocity().magnitudeSquared() > DMGTHRESHOLD_CUB) {
			HitPos = block->GetActor()->getGlobalPose().p;
			SetKnockBack(HitPos);
			Damaged(DMG_BRICK, Attacker->Owner);
		}
		else
			retcode = -1;
		break;
	}

	case ObjectTag::GroundBoard:
	{
		auto board = (CDynamicObject*)Attacker->data;
		if (board->IsActive()) {
			auto& velo = board->GetActor()->getLinearVelocity();
			if (velo.y < -10 && velo.magnitudeSquared() > DMGTHRESHOLD_GRD_SOFT) {
				Damaged(DMG_GROUND, INVALID_OBJECT_ID);
			}
			else if (velo.magnitudeSquared() > DMGTHRESHOLD_GRD_HARD) {
				Damaged(DMG_GROUND, INVALID_OBJECT_ID);
			}
			else retcode = -1;
		}
		else retcode = -1;
		break;
	}
	default:
		retcode = -1;
	}
	return retcode;
}

void CPlayer::SetMoveType(MoveType type)
{
	if (type == MoveType::Jump) {
		if (SkillCoolDownTime[SkillType::HighJump] <= 0) {
			m_MoveType = MoveType::Jump;
			PK_MSG_SKILL p;
			p.size = sizeof(PK_MSG_SKILL);
			p.type = MSGTYPE::MSGACTION::SKILLCOOLDOWN;
			p.id = GetID();
			p.skilltype = SkillType::HighJump;
			m_pRoominfo->SendPacket(GetID(), &p);
		}
		else 
			m_MoveType = MoveType::Stop;
	}
	else 
		m_MoveType = type;
}

void CPlayer::SetKnockBack(PxVec3 atkpos)
{
	IsKnockbacked = true;
	Dir_KnockBack = m_Pos - atkpos;
	Dir_KnockBack = Dir_KnockBack.getNormalized();
}

int CPlayer::UpdateFrame(float fTimeElapsed)
{
	// Dead
	if (IsActive() == false) {
		IncreaseRespawnTimer(fTimeElapsed);
		if (GetRespawnTimer() > 3) {
			SetActivation(true);
			ResetRespawnTimer();
			m_HP = MAX_HP;
			m_SP = MAX_SP;
			m_CP = MAX_CP;
			GetController()->setPosition(PxExtendedVec3(25000.f, 45000.f, 25000.f));
			m_LastHitId = INVALID_OBJECT_ID;
			return MSGTYPE::MSGUPDATE::RESET_OBJECT;
		}
		return 0;
	}

	// Drown
	if (GetController()->getPosition().y < 5000) {
		m_HP -= 25;
		GetController()->setPosition(PxExtendedVec3(25000.f, 45000.f, 25000.f));
		return MSGTYPE::MSGUPDATE::UPDATE_PLAYER;
	}

	// No hp
	if (GetHP() == 0 || GetHP() > MAX_HP) {
		
		GetController()->setPosition(PxExtendedVec3(25000.f, 45000.f, 25000.f));
		SetActivation(false);

		return MSGTYPE::MSGUPDATE::DELETE_OBJECT;
	}

	AnimationFrameAdvance(fTimeElapsed);

	// Shield
	if (m_SP < MAX_SP)
		SPRegenerateTimer += fTimeElapsed;

	if (m_SP < MAX_SP && SPRegenerateTimer > REGENTIEM_s)
	{
		m_SP += 15;
		if (m_SP > MAX_SP) m_SP = MAX_SP;
		SPRegenerateTimer = 0;
	}

	if (StopCounter >= 0)
		StopCounter -= fTimeElapsed;

	if (IsStop()) {
		m_MoveType = MoveType::Stop;
	}

	return 0;
}

void CPlayer::AnimationFrameAdvance(float fTimeElapsed)
{
	float angle = 0.f;
	PxVec3 axis(0);

	if (m_AnimationState == 1 || m_AnimationState == 2)
		Ani_Time += fTimeElapsed;
	else {
		Ani_Frame = 0;
		for (int boneidx = 0; boneidx < m_pRagdoll->nBones; ++boneidx) {
			if (m_pRagdoll->pRagdollNode[boneidx].Valid) {
				m_pRagdoll->pRagdollNode[boneidx].actor[0] ->setKinematicTarget(
					PxTransform(PxVec3(-7000.f * (GetID() + 0))));
				m_pRagdoll->pRagdollNode[boneidx].actor[1]->setKinematicTarget(
					PxTransform(PxVec3(-7000.f * (GetID() + 1))));
			}
		}
	}

	if (m_AnimationState != 0 && m_pRagdoll->AnimationFrameDelayTime < Ani_Time)
	{
		Ani_Frame++;
		if (m_pRagdoll->ActiveFrame_Start <= Ani_Frame && Ani_Frame <= m_pRagdoll->ActiveFrame_End) {
			for (int boneidx = 0; boneidx < m_pRagdoll->nBones; ++boneidx) {
				if (m_pRagdoll->pRagdollNode[boneidx].Valid) {
					m_Quat.toRadiansAndUnitAxis(angle, axis);
					XMMATRIX mtxRotate2 = XMMatrixRotationRollPitchYaw(axis.x * angle, axis.y* angle, axis.z*angle);
					XMMATRIX rotated = XMMatrixMultiply(m_pRagdoll->pRagdollNode[boneidx].MatMidResult[Ani_Frame], mtxRotate2);

					XMFLOAT3 finalpos =
						Vector3::TransformCoord(toXmfloat3(m_pRagdoll->pRagdollNode[boneidx].pAniVertPos[Ani_Frame]), rotated);
					if (fpclassify(finalpos.x) != FP_NORMAL || fpclassify(finalpos.y) != FP_NORMAL || fpclassify(finalpos.z) != FP_NORMAL) continue;

					m_pRagdoll->pRagdollNode[boneidx].actor[m_AnimationState - 1]->setKinematicTarget(
						PxTransform(
							toPxVec3(finalpos) + GetActor()->getGlobalPose().p,
							m_Quat
						));
				}
			}
		}
		else {
			for (int boneidx = 0; boneidx < m_pRagdoll->nBones; ++boneidx) {
				if (m_pRagdoll->pRagdollNode[boneidx].Valid) {
					m_pRagdoll->pRagdollNode[boneidx].actor[m_AnimationState - 1]->setKinematicTarget(
						PxTransform(
							PxVec3(-7000.f * (GetID() + m_AnimationState - 1)),
							m_pRagdoll->pRagdollNode[boneidx].q
						));
				}
			}
		}

		if (Ani_Frame > m_pRagdoll->nAniKeyFrame - 1)
		{
			m_AnimationState = 0;
			Ani_Frame = 0;
		}
		Ani_Time -= m_pRagdoll->AnimationFrameDelayTime;
	}

	m_Pos = m_pController->getActor()->getGlobalPose().p;
}

int CPlayer::AttackProcess(int Attacktype)
{
	bool NeedtoShoot = false;

	switch (Attacktype) {
	case AtkType::Normal1:
		if (m_Type == PlayerType::Epsilon)
			return AtkType::Normal2;

		m_AnimationState = 1;
		break;

	case AtkType::Normal2:
		return AtkType::Normal2;
		break;

	case AtkType::Skill:
		if (SkillCoolDownTime[SkillType::AtkSkill] <= 0) {
			SkillCoolDownTime[SkillType::AtkSkill] = 3.0f;
			if (m_Type == PlayerType::Epsilon) {
				return AtkType::Skill_Shoot;
			}
			m_AnimationState = 2;
		}
		else
			return AtkType::InvalidAtkType;
		break;

	default:
		break;
	}

	return 0;
}

int CPlayer::MoveProcess(float fTimeElapsed)
{
	PxVec3 vel = { 0,0,0 };
	
	// Skill CoolDown
	for (auto& time : SkillCoolDownTime) {
		if (time > 0.f)
			time -= fTimeElapsed;
		else
			time = 0.f;
	}
	
	switch (m_MoveType) {
	case MoveType::Normal:
		vel = m_Dir;
		if (vel.y > 3 && m_CP > 0)
			m_CP -= 2;
		break;

	case MoveType::Dash:
		vel = m_Dir * 2;
		if (m_CP > 0) {
			m_CP -= 2;
			if (m_CP <= 0) {
				m_CP = 0;
				vel = m_Dir * 0.5;
			}
		}
		break;

	case MoveType::Jump:
		if (SkillCoolDownTime[SkillType::HighJump] <= 0.f && m_CP >= 0) {
			SkillCoolDownTime[SkillType::HighJump] = 3.0f;
			jumpduration = 0.3f;
		}
		break;

	case MoveType::Stop:
		vel = PxVec3{ 0, GravityVelocity , 0 };
		break;

	default: break;
	}

	if (jumpduration >= 0) { 
		jumpduration -= fTimeElapsed; 
		vel.y += 75.f * (1 - jumpduration);
	}

	// Gravity Velocity
	if ((m_pController->getPosition().y - PrevHeight) * (m_pController->getPosition().y - PrevHeight) > 0.5) {
		IsLanded = false;
		if (vel.y >= 0)
			GravityVelocity = -3.f;
	}

	if (!IsLanded && m_pDynamicActor->getLinearVelocity().y <= 0) {
		GravityVelocity += fTimeElapsed * -35.f;
	}

	if (m_CP < MAX_CP)
		m_CP++;
	
	if (vel.y < 0 || m_CP <= 0) {
		vel.y = GravityVelocity - 3.0f;
	}
	
	if (IsKnockbacked) {
		vel += Dir_KnockBack * 200;
		IsKnockbacked = false;
	}

	PrevHeight = m_pController->getPosition().y;
	m_pController->move(vel, 0.005f, fTimeElapsed, m_filter);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void CBullet::ResetBullet()
{
	ResetToOriginPos();
	GetActor()->setLinearVelocity(PxVec3(0));
	GetActor()->putToSleep();
	ResetRespawnTimer();
	SetCollisionFlag(false);
	((ObjContainer*)GetActor()->userData)->Owner = INVALID_OBJECT_ID;
}
