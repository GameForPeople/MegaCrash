//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Player.h"
#include "UI/UI.h"
#include "Object/Font/Font.h"
#include "Shader/Shader.h"
#include "Manager/ParticleMgr/ParticleMgr.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPlayer

CPlayer::CPlayer(
	  ID3D12Device*					pd3dDevice
	, ID3D12GraphicsCommandList*	pd3dCmdLst
	, void*							pContext
	, int							nMeshes) 
	: CGameObject(nMeshes)
	, m_xmf3Position			{ XMFLOAT3(0.0f, 0.0f, 0.0f) }
	, m_xmf3Right				{ XMFLOAT3(1.0f, 0.0f, 0.0f) }
	, m_xmf3Up					{ XMFLOAT3(0.0f, 1.0f, 0.0f) }
	, m_xmf3Look				{ XMFLOAT3(0.0f, 0.0f, 1.0f) }
	, m_xmf3ShootDir			{ XMFLOAT3(0.0f, 0.0f, 1.0f) }
	, m_bRotate					{ false }
	, m_bLanding				{ true }
	, m_fPitch					{ 0.0f }
	, m_fYaw					{ 0.0f }
	, m_fRoll					{ 0.0f }
	, m_fCameraFov				{ DEFAULT_FOV_ANGLE }
	, m_fCameraRightOffset		{ 0.0f }
	, m_fCameraDistanceOffset	{ MAX_CAMERA_DISTANCE_OFFSET }
	, m_fJumpCoolTime			{ 0.0f }
	, m_fDashCoolTime			{ 0.0f }
	, m_fSkillCoolTime			{ 0.0f }
	, m_pCamera					{ nullptr }
{
	m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA);
	if (m_pCamera) m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCmdLst);

	m_pFont = make_shared<CFont>(
		  pd3dDevice
		, pd3dCmdLst
		, eTex::Font::Default
		, CFont::DefaultFontPath
		, NUM_FONT_CHAR
		, CLIENT_WIDTH
		, CLIENT_HEIGHT);

	m_pUI = make_shared<CPlayerUI>(pd3dDevice, pd3dCmdLst);

	// 스킬 아이콘 배치간격 조정
	constexpr float x_factor	{ 1.0f / 1600.f };
	constexpr float y_factor	{ 1.0f / 900.f };
	constexpr float left		{ (50.0f - 1600.f) * x_factor };
	constexpr float top			{ (460.0f - 900.f) * y_factor };
	constexpr float x_size		{ 125.0f * x_factor };
	constexpr float y_size		{ 200.0f * y_factor };
	constexpr float x_offset	{ 10.0f * x_factor };
	constexpr float y_offset	{ 10.0f * y_factor };

	m_pDashSkillIcon = make_shared<CSkillIconUI>();
	m_pDashSkillIcon->SetSkillIconImage(eMat::Object::DashSkillIcon);
	m_pDashSkillIcon->SetSkillIconRenderInfoLeftTop(XMFLOAT2(left, top));
	m_pDashSkillIcon->SetSkillIconRenderInfoIconSize(XMFLOAT2(x_size, y_size));

	m_pJumpSkillIcon = make_shared<CSkillIconUI>();
	m_pJumpSkillIcon->SetSkillIconImage(eMat::Object::JumpSkillIcon);
	m_pJumpSkillIcon->SetSkillIconRenderInfoLeftTop(XMFLOAT2(left, top - y_size - y_offset));
	m_pJumpSkillIcon->SetSkillIconRenderInfoIconSize(XMFLOAT2(x_size, y_size));

	m_pSignatureSkillIcon = make_shared<CSkillIconUI>();
	m_pSignatureSkillIcon->SetSkillIconImage(eMat::Object::RubySkillIcon);
	m_pSignatureSkillIcon->SetSkillIconRenderInfoLeftTop(XMFLOAT2(left + x_size + x_offset, top));
	m_pSignatureSkillIcon->SetSkillIconRenderInfoIconSize(XMFLOAT2(410.0f * x_factor, 410.0f * y_factor));

	CResMgr* pRes = CResMgr::Instance();

	m_AnimCtrlrs[Tag::Ruby] = pRes->GetAnimCtrlr(eGeo::AnimCtrlr::Ruby);
	m_AnimCtrlrs[Tag::Legion] = pRes->GetAnimCtrlr(eGeo::AnimCtrlr::Legion);
	m_AnimCtrlrs[Tag::Epsilon] = pRes->GetAnimCtrlr(eGeo::AnimCtrlr::Epsilon);
	m_AnimCtrlrs[Tag::Gravis] = pRes->GetAnimCtrlr(eGeo::AnimCtrlr::Gravis);

	for (auto& p : m_AnimCtrlrs) {
		p.second->CreateShaderVariables(pd3dCmdLst);
	}

	ChangeTag(Tag::Ruby);
}

CPlayer::~CPlayer()
{
	if (m_pCamera) {
		m_pCamera->ReleaseShaderVariables();
		delete m_pCamera;
	}
}

void CPlayer::Update(float fTimeElapsed)
{
	if (m_bStun) {
		m_fStunTimer += fTimeElapsed;
		if (m_fStunTimer > m_fStunTime) {
			m_bStun = false;
			m_fStunTimer = 0.0f;
		}
		else {
			return;
		}
	}

	if (m_bDelayMove) {
		static XMFLOAT4X4 mtx;
		m_fLerpRate += fTimeElapsed / m_fMovingTime;
		if (m_fLerpRate > 1.0f) {
			m_bDelayMove = false;
			m_fMovingTime = 0.0f;
			m_fLerpRate = 0.0f;
			Matrix4x4::ToTransform(&mtx, m_xmf3DestPos, m_xmf4DestQuat);
		}
		else {
			XMFLOAT3 lerp_pos{ Vector3::Lerp(m_xmf3StartPos, m_xmf3DestPos, m_fLerpRate) };
			XMFLOAT4 slerp_quat{ Vector4::QuatSlerp(m_xmf4StartQuat, m_xmf4DestQuat, m_fLerpRate) };
			Matrix4x4::ToTransform(&mtx, lerp_pos, slerp_quat);
		}
		SetWorldMtx(mtx);
	}
	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) {
		m_pCamera->SetLookAtOffsetX(m_fCameraRightOffset);
		m_pCamera->SetOffsetZ(m_fCameraDistanceOffset);
		m_pCamera->Update(fTimeElapsed);
		m_pCamera->GenerateProjectionMatrix(NEAR_DISTANCE, FAR_DISTANCE, ASPECT_RATIO, m_fCameraFov);
	}
}

void CPlayer::Animate(float fTimeElapsed)
{
	static const float offset_move_time		{ MAX_CAMERA_RIGHT_OFFSET / CAMERA_OFFSET_MOVE_TIME };
	static const float cool_time_factor		{ 1.0f / 3.0f };
	static CAnimCtrlr::Tag prevTag			{ CAnimCtrlr::Tag::Idle };
	static float move_to_fov				{ m_fCameraFov };
	static float move_to_distance_offset	{ m_fCameraDistanceOffset };

	if (m_bStun) return;

	m_AnimCtrlrs[m_Tag]->Animate(fTimeElapsed);

	XMFLOAT3 p = {
			GetPosition().x - m_xmf3Look.x*3
			, GetPosition().y
			, GetPosition().z - m_xmf3Look.z*3
	};

	CAnimCtrlr::Tag tag{ m_AnimCtrlrs[m_Tag]->GetCurrTag() };
	if (prevTag != tag) {
		switch (tag) {
		case CAnimCtrlr::Tag::Dash:
			m_fDashCoolTime = 1.0f;
			move_to_distance_offset = MAX_CAMERA_DISTANCE_OFFSET * CAMERA_OFFSET_BOOST_RATE;
			move_to_fov = BOOST_FOV_ANGLE;
			switch (m_Tag)
			{
			case Tag::Ruby:
				p = Vector3::Add(p,XMFLOAT3(0,2,0));
				m_pParticleMgr->PopRubyBoostTrail(p);
				break;
			case Tag::Legion:
				m_pParticleMgr->PopLegionBoostTrail(p);
				break;
			case Tag::Epsilon:
				m_pParticleMgr->PopEpsilonBoostTrail(p);
				break;
			case Tag::Gravis:
				m_pParticleMgr->PopGravisBoostTrail(p);
				break;
			}
			break;
		case CAnimCtrlr::Tag::Jump:
			m_fJumpCoolTime = 0.0f;
			move_to_distance_offset = MAX_CAMERA_DISTANCE_OFFSET * CAMERA_OFFSET_BOOST_RATE;
			move_to_fov = BOOST_FOV_ANGLE;
			switch (m_Tag)
			{
			case Tag::Ruby:
				p = Vector3::Add(p, XMFLOAT3(0, 2, 0));
				m_pParticleMgr->PopRubyBoostTrail(p);
				break;
			case Tag::Legion:
				m_pParticleMgr->PopLegionBoostTrail(p);
				break;
			case Tag::Epsilon:
				m_pParticleMgr->PopEpsilonBoostTrail(p);
				break;
			case Tag::Gravis:
				m_pParticleMgr->PopGravisBoostTrail(p);
				break;
			}
			break;
		case CAnimCtrlr::Tag::Skill:
			m_fSkillCoolTime = 0.0f;
			break;
		default:
			if (prevTag == CAnimCtrlr::Tag::Dash || prevTag == CAnimCtrlr::Tag::Jump) {
				move_to_distance_offset = MAX_CAMERA_DISTANCE_OFFSET;
				move_to_fov = DEFAULT_FOV_ANGLE;
			}
			break;
		}
		prevTag = tag;
	}
	else {
		
		switch (tag) {
		case CAnimCtrlr::Tag::Dash:

			switch (m_Tag)
			{
			case Tag::Ruby:
				p = Vector3::Add(p, XMFLOAT3(0, 2, 0));
				m_pParticleMgr->PopRubyBoostTrail(p);
				break;
			case Tag::Legion:
				m_pParticleMgr->PopLegionBoostTrail(p);
				break;
			case Tag::Epsilon:
				m_pParticleMgr->PopEpsilonBoostTrail(p);
				break;
			case Tag::Gravis:
				m_pParticleMgr->PopGravisBoostTrail(p);
				break;
			}

			
			break;
		case CAnimCtrlr::Tag::Jump: 
			switch (m_Tag)
			{
			case Tag::Ruby:
				p = Vector3::Add(p, XMFLOAT3(0, 2, 0));
				m_pParticleMgr->PopRubyBoostTrail(p);
				break;
			case Tag::Legion:
				m_pParticleMgr->PopLegionBoostTrail(p);
				break;
			case Tag::Epsilon:
				m_pParticleMgr->PopEpsilonBoostTrail(p);
				break;
			case Tag::Gravis:
				m_pParticleMgr->PopGravisBoostTrail(p);
				break;
			}
			break; 
		case CAnimCtrlr::Tag::Skill:
			break;
		default:
			break;
		}
		if (tag != CAnimCtrlr::Tag::Dash) {
			m_fDashCoolTime = 1.5f;
		}
	}
	
	float offset_move_timeElapsed{ offset_move_time * fTimeElapsed };
	if (m_fCameraDistanceOffset > move_to_distance_offset) {
		m_fCameraDistanceOffset -= offset_move_timeElapsed;
		if (m_fCameraDistanceOffset < move_to_distance_offset) {
			m_fCameraDistanceOffset = move_to_distance_offset;
		}
	}
	else if (m_fCameraDistanceOffset < move_to_distance_offset) {
		m_fCameraDistanceOffset += offset_move_timeElapsed;
		if (m_fCameraDistanceOffset > move_to_distance_offset) {
			m_fCameraDistanceOffset = move_to_distance_offset;
		}
	}
	if (m_fCameraFov > move_to_fov) {
		m_fCameraFov -= offset_move_timeElapsed;
		if (m_fCameraFov < move_to_fov) {
			m_fCameraFov = move_to_fov;
		}
	}
	else if (m_fCameraFov < move_to_fov) {
		m_fCameraFov += offset_move_timeElapsed;
		if (m_fCameraFov > move_to_fov) {
			m_fCameraFov = move_to_fov;
		}
	}

	m_fJumpCoolTime += cool_time_factor * fTimeElapsed;
	m_fSkillCoolTime += cool_time_factor * fTimeElapsed;
	m_pDashSkillIcon->SetSkillIconRenderInfoCoolTime(m_fDashCoolTime);
	m_pJumpSkillIcon->SetSkillIconRenderInfoCoolTime(m_fJumpCoolTime);
	m_pSignatureSkillIcon->SetSkillIconRenderInfoCoolTime(m_fSkillCoolTime);

	SyncMemberVariableToMatrix();

	//XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(-90.0f), 0.0f, 0.0f);
	//m_xmf4x4ToParentTransform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4ToParentTransform);
}

void CPlayer::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCmdLst)
{
	m_AnimCtrlrs[m_Tag]->UpdateShaderVariables(pd3dCmdLst);
	UpdateTransform();
}

void CPlayer::Render(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera)
{
	OnPrepareRender(pd3dCmdLst);

	//DWORD nCameraMode = (pCamera) ? pCamera->GetMode() : 0x00;
	//if (nCameraMode == THIRD_PERSON_CAMERA || nCameraMode == MINIMAP_CAMERA)
	CGameObject::Render(pd3dCmdLst, pCamera);
}

void CPlayer::RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	m_pUI->SetLook(pCamera->GetLookVector());
	m_pUI->SetPosition(Vector3::Add(m_xmf3Position, pCamera->GetRightVector(), -80.0f));
	m_pUI->SetRotateAngleY(150.0f);
	m_pUI->UpdateTransform();
	m_pUI->Render(pd3dCmdLst, pCamera);

	m_pDashSkillIcon->Render(pd3dCmdLst, pCamera);
	m_pJumpSkillIcon->Render(pd3dCmdLst, pCamera);
	m_pSignatureSkillIcon->Render(pd3dCmdLst, pCamera);
	m_pFont->Render(pd3dCmdLst, pCamera);
}

void CPlayer::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	OnPrepareRender(pd3dCmdLst);
	CGameObject::RenderShadowMap(pd3dCmdLst, pCamera);
}

bool CPlayer::ProcessInput(const UCHAR * pKeysBuffer, float fTimeElapsed)
{
	static const float offset_move_time{ MAX_CAMERA_RIGHT_OFFSET / CAMERA_OFFSET_MOVE_TIME };

	if (m_bStun) return false;

	DWORD dwDirection{ 0 };
	bool bAction{ false };
	bool bLShiftDown{ false };
	bool bLMouseDown{ false };
	bool bRMouseDown{ false };

	if (pKeysBuffer['W'] & 0xF0)			dwDirection |= DIR_FORWARD;
	if (pKeysBuffer['S'] & 0xF0)			dwDirection |= DIR_BACKWARD;
	if (pKeysBuffer['A'] & 0xF0)			dwDirection |= DIR_LEFT;
	if (pKeysBuffer['D'] & 0xF0)			dwDirection |= DIR_RIGHT;
	//if (pKeysBuffer['F'] & 0xF0)			dwDirection |= DIR_DOWN;
	if (pKeysBuffer[VK_SPACE] & 0xF0)		dwDirection |= DIR_UP;
	if (pKeysBuffer[VK_LSHIFT] & 0xF0)		bLShiftDown = true;
	if (pKeysBuffer[VK_LBUTTON] & 0xF0)		bLMouseDown = true;
	if (pKeysBuffer[VK_RBUTTON] & 0xF0)		bRMouseDown = true;

	if (dwDirection) {
		if ((dwDirection & DIR_FORWARD) || (dwDirection & DIR_BACKWARD)) {
			float offset_move_timeElapsed{ offset_move_time * fTimeElapsed };
			if (m_fCameraRightOffset > 0.0f) {
				m_fCameraRightOffset -= offset_move_timeElapsed;
				if (m_fCameraRightOffset < 0.0f) {
					m_fCameraRightOffset = 0.0f;
				}
			}
			else if (m_fCameraRightOffset < 0.0f) {
				m_fCameraRightOffset += offset_move_timeElapsed;
				if (m_fCameraRightOffset > 0.0f) {
					m_fCameraRightOffset = 0.0f;
				}
			}
		}
		if (false == ((dwDirection & DIR_LEFT) && (dwDirection & DIR_RIGHT))) {
			float offset_move_timeElapsed{ offset_move_time * fTimeElapsed };
			if (dwDirection & DIR_LEFT) {
				m_fCameraRightOffset -= offset_move_timeElapsed;
				if (m_fCameraRightOffset < -MAX_CAMERA_RIGHT_OFFSET) {
					m_fCameraRightOffset = -MAX_CAMERA_RIGHT_OFFSET;
				}
			}
			else if (dwDirection & DIR_RIGHT) {
				m_fCameraRightOffset += offset_move_timeElapsed;
				if (m_fCameraRightOffset > MAX_CAMERA_RIGHT_OFFSET) {
					m_fCameraRightOffset = MAX_CAMERA_RIGHT_OFFSET;
				}
			}
		}
	}

	CAnimCtrlr::Tag tag{ m_AnimCtrlrs[m_Tag]->GetCurrTag() };

	if (bLMouseDown) {
		if (bLShiftDown) {
#ifndef USE_SERVER_CONNECT
			m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::Skill);
#endif
			SendAtkMsg(ATK_TYPE_SKILL);
		}
		else {
			bAction = m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::Attack_1);
			if (bAction) {
				SendAnimMsg(CAnimCtrlr::Tag::Attack_1);
				SendAtkMsg(ATK_TYPE_ATTACK1);
			}
		}
	}
	if (bRMouseDown && m_Tag != CPlayer::Tag::Epsilon) {
		bAction = m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::Attack_2);
		if (bAction) {
			SendAnimMsg(CAnimCtrlr::Tag::Attack_2);
			SendAtkMsg(ATK_TYPE_ATTACK2);
		}
	}

	if (m_AnimCtrlrs[m_Tag]->IsAttacking()) {
		if (!(bLMouseDown || bRMouseDown)) {
			bAction = m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::Idle);
			if (bAction) {
				SendAnimMsg(CAnimCtrlr::Tag::Idle);
				SendMoveMsg(MOVE_TYPE_NORMAL, 0);
			}
			else {
				SendMoveMsg(MOVE_TYPE_NORMAL, dwDirection);
			}
		}
		SetLookDir(m_xmf3ShootDir);
	}
	else {
		if (false == Vector3::IsZero(GetMoveDirection(dwDirection))) {
			if (bLShiftDown) {
				if (dwDirection) {
					if (dwDirection & DIR_UP) {
#ifndef USE_SERVER_CONNECT
						m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::Jump);
#endif
						SendMoveMsg(MOVE_TYPE_JUMP, dwDirection);
					}
					else {
#ifndef USE_SERVER_CONNECT
						m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::Dash);
#endif
						SendMoveMsg(MOVE_TYPE_DASH, dwDirection);
						if (tag == CAnimCtrlr::Tag::Dash) {
							m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::Dash);
						}
					}
				}
				else {
					m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::Idle);
				}
			}
			else if (tag == CAnimCtrlr::Tag::Dash) {
				m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::Idle);
			}
			if (m_bLanding) {
				if (tag == CAnimCtrlr::Tag::Move) {
					bAction = m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::Move);
					if (bAction) SendAnimMsg(CAnimCtrlr::Tag::Move);
				}
				else {
					bAction = m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::MoveStart);
					if (bAction) SendAnimMsg(CAnimCtrlr::Tag::MoveStart);
				}
			}
			else {
				if (tag == CAnimCtrlr::Tag::Hovering) {
					bAction = m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::Hovering);
					if (bAction) SendAnimMsg(CAnimCtrlr::Tag::Hovering);
				}
				else {
					bAction = m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::TakeOff);
					if (bAction) SendAnimMsg(CAnimCtrlr::Tag::TakeOff);
				}
			}
			if (m_AnimCtrlrs[m_Tag]->IsMoving()) {
				if(dwDirection != DIR_UP)
					SetLookDir(GetMoveDirection(dwDirection));
			}
			if (!bLShiftDown) SendMoveMsg(MOVE_TYPE_NORMAL, dwDirection);
#ifndef USE_SERVER_CONNECT
			Move(dwDirection, DEBUG_SPEED*fTimeElapsed);
#endif
		}
		else {
			if (m_bLanding) {
				bAction = m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::MoveEnd);
				if (bAction) {
					SendAnimMsg(CAnimCtrlr::Tag::MoveEnd);
					SendMoveMsg(MOVE_TYPE_NORMAL, dwDirection);
				}
			}
			else {
				bAction = m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(CAnimCtrlr::Tag::Landing);
				if (bAction) {
					SendAnimMsg(CAnimCtrlr::Tag::Landing);
					SendMoveMsg(MOVE_TYPE_NORMAL, dwDirection);
				}
			}
		}
	}
	if (m_bRotate) SendMoveMsg(MOVE_TYPE_NORMAL, dwDirection);
	return false;
}

CCamera* CPlayer::ChangeCamera(DWORD nNewCameraMode)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return m_pCamera;
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetPosition(GetPosition());
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 50.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(NEAR_DISTANCE, FAR_DISTANCE, ASPECT_RATIO, DEFAULT_FOV_ANGLE);
		m_pCamera->SetViewport(0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
		m_pCamera->GenerateFrustum();
		break;
	case THIRD_PERSON_CAMERA:
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(TIME_LAG);
		m_pCamera->SetOffset(CAMERA_OFFSET);
		m_pCamera->SetLookAtOffset(CAMERA_LOOK_AT_OFFSET);
		m_pCamera->GenerateProjectionMatrix(NEAR_DISTANCE, FAR_DISTANCE, ASPECT_RATIO, DEFAULT_FOV_ANGLE);
		m_pCamera->SetViewport(0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
		m_pCamera->GenerateFrustum();
		break;
	default:
		break;
	}
	return m_pCamera;
}

CCamera *CPlayer::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	CCamera *pNewCamera = NULL;
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = new CFirstPersonCamera(m_pCamera);
		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = new CThirdPersonCamera(m_pCamera);
		break;
	}

	m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf3Right.x, 0.0f, m_xmf3Right.z));
	m_xmf3Up = Vector3::Normalize(XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf3Look.x, 0.0f, m_xmf3Look.z));

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = Vector3::Angle(XMFLOAT3(0.0f, 0.0f, 1.0f), m_xmf3Look);
	if (m_xmf3Look.x < 0.0f) m_fYaw = -m_fYaw;

	if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera)
	{
		m_xmf3Right = m_pCamera->GetRightVector();
		m_xmf3Up = m_pCamera->GetUpVector();
		m_xmf3Look = m_pCamera->GetLookVector();
	}

	if (pNewCamera)
	{
		pNewCamera->SetMode(nNewCameraMode);
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) delete m_pCamera;

	return(pNewCamera);
}

void CPlayer::SyncMemberVariableToMatrix()
{
	m_xmf4x4ToParentTransform._11 = m_xmf3Right.x;
	m_xmf4x4ToParentTransform._12 = m_xmf3Right.y;
	m_xmf4x4ToParentTransform._13 = m_xmf3Right.z;
	m_xmf4x4ToParentTransform._21 = m_xmf3Up.x;
	m_xmf4x4ToParentTransform._22 = m_xmf3Up.y;
	m_xmf4x4ToParentTransform._23 = m_xmf3Up.z;
	m_xmf4x4ToParentTransform._31 = m_xmf3Look.x;
	m_xmf4x4ToParentTransform._32 = m_xmf3Look.y;
	m_xmf4x4ToParentTransform._33 = m_xmf3Look.z;
	m_xmf4x4ToParentTransform._41 = m_xmf3Position.x;
	m_xmf4x4ToParentTransform._42 = m_xmf3Position.y;
	m_xmf4x4ToParentTransform._43 = m_xmf3Position.z;
}

void CPlayer::SyncMatrixToMemberVariable()
{
	m_xmf3Right.x = m_xmf4x4ToParentTransform._11;
	m_xmf3Right.y = m_xmf4x4ToParentTransform._12;
	m_xmf3Right.z = m_xmf4x4ToParentTransform._13;
	m_xmf3Up.x = m_xmf4x4ToParentTransform._21;
	m_xmf3Up.y = m_xmf4x4ToParentTransform._22;
	m_xmf3Up.z = m_xmf4x4ToParentTransform._23;
	m_xmf3Look.x = m_xmf4x4ToParentTransform._31;
	m_xmf3Look.y = m_xmf4x4ToParentTransform._32;
	m_xmf3Look.z = m_xmf4x4ToParentTransform._33;
	m_xmf3Position.x = m_xmf4x4ToParentTransform._41;
	m_xmf3Position.y = m_xmf4x4ToParentTransform._42;
	m_xmf3Position.z = m_xmf4x4ToParentTransform._43;
}

void CPlayer::SendMoveMsg(BYTE move_type, DWORD dwDirection)
{
#if USE_SERVER_CONNECT
	static CClient* pClient = CClient::Instance();
	static long long tStartTime{ GetCurrTime() };
	if (GetCurrTime() - tStartTime > SEND_MOVE_PACKET_MS || !dwDirection) {
		PK_MSG_MOVE* p = reinterpret_cast<PK_MSG_MOVE*>(pClient->GetSendBuf());
		p->size = sizeof(PK_MSG_MOVE);
		p->type = MSGTYPE::MSGACTION::MOVE;
		p->movetype = move_type;
		p->id = m_ID;
		p->p = GetMoveDirection(dwDirection, 16.0f);
		XMStoreFloat4(&p->q, XMQuaternionRotationMatrix(XMLoadFloat4x4(&m_xmf4x4World)));
		pClient->SendMsg(p->size);
		m_bRotate = false;

		tStartTime = GetCurrTime();
	}
#endif
}

void CPlayer::SendAtkMsg(BYTE atk_type)
{
#if USE_SERVER_CONNECT
	static CClient* pClient = CClient::Instance();
	static long long tStartTime{ GetCurrTime() };
	if (GetCurrTime() - tStartTime > SEND_ATK_PACKET_MS) {
		PK_MSG_ATTACK* p = reinterpret_cast<PK_MSG_ATTACK*>(pClient->GetSendBuf());
		p->size = sizeof(PK_MSG_ATTACK);
		p->type = MSGTYPE::MSGACTION::ATTACK;
		p->atktype = atk_type;
		p->id = m_ID;
		p->p = m_xmf3Position;
		if (IsShooting(atk_type)) { 
			p->d = m_xmf3ShootDir;
		}
		else {
			p->d = m_xmf3Look;
		}
		pClient->SendMsg(p->size);

		tStartTime = GetCurrTime();
	}
#endif
}

void CPlayer::SendAnimMsg(CAnimCtrlr::Tag tag)
{
#if USE_SERVER_CONNECT
	static CClient* pClient = CClient::Instance();
	PK_MSG_STATECHANGE* p = reinterpret_cast<PK_MSG_STATECHANGE*>(pClient->GetSendBuf());
	p->size = sizeof(PK_MSG_STATECHANGE);
	p->type = MSGTYPE::MSGACTION::ANIMATE;
	p->id = m_ID;
	p->data1 = static_cast<UINT>(tag);
	pClient->SendMsg(p->size);
#endif
}

bool CPlayer::IsShooting(BYTE atk_type)
{
	static bool bShootTypeLst[4][3] = {
		// | ATK1 | ATK2 | SKILL |
		  { false,	true,	false }	// CPlayer::Tag::Ruby
		, { false,	true,	false }	// CPlayer::Tag::Legion
		, { true,	false,	true }	// CPlayer::Tag::Epsilon
		, { false,	true,	false }	// CPlayer::Tag::Gravis
	};
	return bShootTypeLst[static_cast<BYTE>(m_Tag)][atk_type];
}

XMFLOAT3 CPlayer::GetMoveDirection(DWORD dwDirection, float fSpeed)
{
	XMFLOAT3 xmf3Shift{ XMFLOAT3(0, 0, 0) };
	if (dwDirection) {
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_pCamera->GetLookVector(), 1.0f);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_pCamera->GetLookVector(), -1.0f);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_pCamera->GetRightVector(), 1.0f);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_pCamera->GetRightVector(), -1.0f);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_pCamera->GetUpVector(), 1.0f);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_pCamera->GetUpVector(), -1.0f);
		return Vector3::ScalarProduct(Vector3::Normalize(xmf3Shift), fSpeed, false);
	}
	else {
		return xmf3Shift;
	}
}

void CPlayer::SetWorldMtx(const XMFLOAT4X4& mtxWorld)
{
	m_xmf4x4ToParentTransform = mtxWorld;

	SyncMatrixToMemberVariable();
}

void CPlayer::SetLookDir(const XMFLOAT3 & xmf3LookDir)
{
	m_xmf3Look = Vector3::Normalize(XMFLOAT3(xmf3LookDir.x, 0.0f, xmf3LookDir.z));
	m_xmf3Up = Vector3::Normalize(XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);

	SyncMemberVariableToMatrix();

	m_xmf4DestQuat = Vector4::QuatFromMtx(m_xmf4x4ToParentTransform);
	m_xmf4StartQuat = m_xmf4DestQuat;
}

void CPlayer::ChangeTag(Tag tag)
{
	CResMgr* pRes = CResMgr::Instance();

	m_Tag = tag;
	constexpr float x_factor{ 1.0f / 1600.f };
	constexpr float y_factor{ 1.0f / 900.f };
	constexpr float left{ 25.0f * x_factor };
	constexpr float top{ 625.0f * y_factor };
	constexpr float width = 2.25f * CLIENT_WIDTH / 1600;
	constexpr float height = 3.5f * CLIENT_HEIGHT / 900;
	switch (m_Tag) {
		case Tag::Ruby:
			SetMesh(0, pRes->GetMesh(eGeo::Mesh::Ruby));
			SetMaterial(pRes->GetMaterial(eMat::Object::Ruby));
			m_pCamera->SetLookAtOffsetY(120.f);
			m_AnimCtrlrs[m_Tag]->SetIgnoreBoneIdx(-1);
			m_pSignatureSkillIcon->SetSkillIconImage(eMat::Object::RubySkillIcon);
			m_pFont->ResetTextData();
			m_pFont->AppendTextData(
				  std::wstring(L"CodeName>>Ruby")
				, XMFLOAT2(left, top)
				, XMFLOAT2(width, height)
				, XMFLOAT2( 0.5f, 0.0f )
				, XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f));
			break;
		case Tag::Legion:
			SetMesh(0, pRes->GetMesh(eGeo::Mesh::Legion));
			SetMaterial(pRes->GetMaterial(eMat::Object::Legion));
			m_pCamera->SetLookAtOffsetY(120.f);
			m_AnimCtrlrs[m_Tag]->SetIgnoreBoneIdx(31); // 방패
			m_pSignatureSkillIcon->SetSkillIconImage(eMat::Object::LegionSkillIcon);
			m_pFont->ResetTextData();
			m_pFont->AppendTextData(
				  std::wstring(L"CodeName>>Legion")
				, XMFLOAT2(left, top)
				, XMFLOAT2(width, height)
				, XMFLOAT2(0.5f, 0.0f)
				, XMFLOAT4(0.2f, 1.0f, 0.2f, 1.0f));
			break;
		case Tag::Epsilon:
			SetMesh(0, pRes->GetMesh(eGeo::Mesh::Epsilon));
			SetMaterial(pRes->GetMaterial(eMat::Object::Epsilon));
			m_pCamera->SetLookAtOffsetY(120.f);
			m_AnimCtrlrs[m_Tag]->SetIgnoreBoneIdx(55); // 총
			m_pSignatureSkillIcon->SetSkillIconImage(eMat::Object::EpsilonSkillIcon);
			m_pFont->ResetTextData();
			m_pFont->AppendTextData(
				  std::wstring(L"CodeName>>Epsilon")
				, XMFLOAT2(left, top)
				, XMFLOAT2(width, height)
				, XMFLOAT2(0.5f, 0.0f)
				, XMFLOAT4(0.2f, 0.2f, 1.0f, 1.0f));
			break;
		case Tag::Gravis:
			SetMesh(0, pRes->GetMesh(eGeo::Mesh::Gravis));
			SetMaterial(pRes->GetMaterial(eMat::Object::Gravis));
			m_pCamera->SetLookAtOffsetY(75.f);
			m_AnimCtrlrs[m_Tag]->SetIgnoreBoneIdx(-1);
			m_pSignatureSkillIcon->SetSkillIconImage(eMat::Object::GravisSkillIcon);
			m_pFont->ResetTextData();
			m_pFont->AppendTextData(
				  std::wstring(L"CodeName>>Gravis")
				, XMFLOAT2(left, top)
				, XMFLOAT2(width, height)
				, XMFLOAT2(0.5f, 0.0f)
				, XMFLOAT4(0.9f, 0.9f, 0.f, 1.0f));
			break;
	}
}

void CPlayer::ChangeCurrAnim(CAnimCtrlr::Tag tag)
{
	m_AnimCtrlrs[m_Tag]->ChangeCurrAnim(tag);
}

bool CPlayer::CollisionCheck(
	  const XMFLOAT3& ray_origin
	, const XMFLOAT3& ray_dir
	, float * output_t)
{
	static XMFLOAT3&	sphere_center{ m_xmf3Position };
	static const float	sphere_radius{ 50.0f };
	float a{ Vector3::DotProduct(ray_dir, ray_dir) };
	if (IsZero(a)) {
		*output_t = 50000.0f;
		return false;
	}
	XMFLOAT3 RayToSphereCenter{ Vector3::Subtract(ray_origin, sphere_center) };
	float b{ Vector3::DotProduct(RayToSphereCenter, ray_dir) * 2.0f };
	float c{ Vector3::DotProduct(RayToSphereCenter, RayToSphereCenter) - sphere_radius * sphere_radius };
	float D{ sqrtf(b * b - 4 * a * c) };
	float rev_2a{ 1.0f / (2.0f * a) };
	float t1{ (-b - D) * rev_2a };
	float t2{ (-b + D) * rev_2a };
	if (t1 > -FLT_EPSILON && t2 > -FLT_EPSILON) {
		if (t1 < t2) {
			*output_t = t1;
			return true;
		}
		else {
			*output_t = t2;
			return true;
		}
	}
	else if (t1 > -FLT_EPSILON) {
		*output_t = t1;
		return true;
	}
	else if (t2 > -FLT_EPSILON) {
		*output_t = t2;
		return true;
	}
	else {
		*output_t = 50000.0f;
		return false;
	}
}

void CPlayer::Move(DWORD dwDirection, float fSpeed)
{
	if (dwDirection) {
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, 1.0f);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -1.0f);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, 1.0f);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -1.0f);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, 1.0f);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -1.0f);
		Move(Vector3::ScalarProduct(Vector3::Normalize(xmf3Shift), fSpeed, false));
	}
}

void CPlayer::Move(const XMFLOAT3& xmf3Shift)
{
	m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);
	m_pCamera->Move(xmf3Shift);
}

void CPlayer::Rotate(float x, float y, float z)
{
	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if ((nCurrentCameraMode == FIRST_PERSON_CAMERA)
		|| (nCurrentCameraMode == THIRD_PERSON_CAMERA)) {
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +40.0f) { x -= (m_fPitch - 40.0f); m_fPitch = +40.0f; }
			if (m_fPitch < -100.0f) { x -= (m_fPitch + 100.0f); m_fPitch = -100.0f; }
		}
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		if (z != 0.0f)
		{
			m_fRoll += z;
			if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		m_pCamera->Rotate(x, y, z);
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}
	else if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_pCamera->Rotate(x, y, z);
		if (x != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		}
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
		if (z != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(z));
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);

	SyncMemberVariableToMatrix();

	m_xmf4DestQuat = Vector4::QuatFromMtx(m_xmf4x4ToParentTransform);
	m_xmf4StartQuat = m_xmf4DestQuat;

	m_bRotate = true;
}