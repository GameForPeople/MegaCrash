#pragma once
#include "Object\Object.h"
#include "Camera\Camera.h"
#include "Animation\AnimationControler.h"

constexpr UINT		DIR_FORWARD					{ 0x01 };
constexpr UINT		DIR_BACKWARD				{ 0x02 };
constexpr UINT		DIR_LEFT					{ 0x04 };
constexpr UINT		DIR_RIGHT					{ 0x08 };
constexpr UINT		DIR_UP						{ 0x10 };
constexpr UINT		DIR_DOWN					{ 0x20 };

constexpr float		DEBUG_SPEED					{ 256.0f };
constexpr float		MAX_CAMERA_RIGHT_OFFSET		{ 50.0f };
constexpr float		MAX_CAMERA_DISTANCE_OFFSET	{ -200.0f };
constexpr float		CAMERA_OFFSET_MOVE_TIME		{ 0.2f };
constexpr float		CAMERA_OFFSET_BOOST_RATE	{ 1.5f };
constexpr XMFLOAT3	CAMERA_LOOK_AT_OFFSET		{ 0.0f, 120.0f, 0.0f };
constexpr XMFLOAT3	CAMERA_OFFSET				{ 0.0f, 0.0f, MAX_CAMERA_DISTANCE_OFFSET };

constexpr UINT		NUM_FONT_CHAR				{ 128 };

class CFont;
class CSkillIconUI;
class CPlayer : public CGameObject
{
public:
	enum class Tag { Ruby, Legion, Epsilon, Gravis };

protected:
	Tag											m_Tag;

	XMFLOAT3									m_xmf3Position;
	XMFLOAT3									m_xmf3Right;
	XMFLOAT3									m_xmf3Up;
	XMFLOAT3									m_xmf3Look;
	XMFLOAT3									m_xmf3ShootDir;

	bool										m_bRotate;
	bool										m_bLanding;

	float           							m_fPitch;
	float           							m_fYaw;
	float           							m_fRoll;

	float										m_fCameraFov;
	float										m_fCameraRightOffset;
	float										m_fCameraDistanceOffset;

	float										m_fJumpCoolTime;
	float										m_fDashCoolTime;
	float										m_fSkillCoolTime;

	shared_ptr<CFont>							m_pFont;

	shared_ptr<CPlayerUI>						m_pUI;
	shared_ptr<CSkillIconUI>					m_pDashSkillIcon;
	shared_ptr<CSkillIconUI>					m_pJumpSkillIcon;
	shared_ptr<CSkillIconUI>					m_pSignatureSkillIcon;
	unordered_map<Tag, shared_ptr<CAnimCtrlr>>	m_AnimCtrlrs;

	CCamera*									m_pCamera;

public:
	CPlayer(
		  ID3D12Device*						pd3dDevice
		, ID3D12GraphicsCommandList*		pd3dCmdLst
		, void*								pContext = nullptr
		, int								nMeshes = 1);
	virtual ~CPlayer();

	virtual void Update(float fTimeElapsed);
	virtual void Animate(float fTimeElapsed);

	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr);
	virtual void RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr);
	virtual void RenderShadowMap(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera = nullptr);

	virtual bool ProcessInput(const UCHAR* pKeysBuffer, float fTimeElapsed);

	virtual CCamera* ChangeCamera(DWORD nNewCameraMode);
	CCamera* OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode);

	void SyncMemberVariableToMatrix();
	void SyncMatrixToMemberVariable();

	void SendMoveMsg(BYTE move_type, DWORD dwDirection);
	void SendAtkMsg(BYTE atk_type);
	void SendAnimMsg(CAnimCtrlr::Tag tag);
	bool IsShooting(BYTE atk_type);
	
	const XMFLOAT3&					GetPosition()		const { return m_xmf3Position; }
	const XMFLOAT3&					GetLookVector()		const { return m_xmf3Look; }
	const XMFLOAT3&					GetUpVector()		const { return m_xmf3Up; }
	const XMFLOAT3&					GetRightVector()	const { return m_xmf3Right; }
	float							GetYaw()			const { return m_fYaw; }
	float							GetPitch()			const { return m_fPitch; }
	float							GetRoll()			const { return m_fRoll; }

	const shared_ptr<CPlayerUI>&	GetPlayerUI()		const { return m_pUI; }
	CCamera*						GetCamera()			const { return m_pCamera; }
	Tag								GetPlayerTag()		const { return m_Tag; }
	CAnimCtrlr::Tag					GetCurrAnimTag()		  { return m_AnimCtrlrs[m_Tag]->GetCurrTag(); }

	XMFLOAT3						GetMoveDirection(DWORD dwDirection, float fSpeed = 1.0f);

	void TakingOff()			{ m_bLanding = false; }
	void Landing()				{ m_bLanding = true; }
	void Jumping()				{ m_fJumpCoolTime = 0.0f; }
	void UseSkiil()				{ m_fSkillCoolTime = 0.0f; }
	void ResetCameraOffset()	{ m_fCameraRightOffset = 0.0f; }

	void SetWorldMtx(const XMFLOAT4X4& mtxWorld);
	void SetLookDir(const XMFLOAT3& xmf3LookDir);
	void SetPosition(const XMFLOAT3& xmf3Position)	{ m_xmf3Position = xmf3Position; }
	void SetShootDir(const XMFLOAT3& xmf3ShootDir)	{ m_xmf3ShootDir = xmf3ShootDir; m_bRotate = true; }
	void SetCamera(CCamera *pCamera)				{ m_pCamera = pCamera; }

	void ChangeTag(Tag tag);
	void ChangeCurrAnim(CAnimCtrlr::Tag tag);

	bool CollisionCheck(
		  const XMFLOAT3&	ray_origin
		, const XMFLOAT3&	ray_dir
		, float*			output_t);

	void Move(DWORD dwDirection, float fSpeed);
	void Move(const XMFLOAT3& xmf3Shift);
	void Rotate(float x, float y, float z);
};