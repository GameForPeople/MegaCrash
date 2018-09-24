//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------
#pragma once
#include "Texture\Texture.h"
#include "Material\Material.h"

class CMesh;
class CCamera;
class CShader;
class CParticleMgr;

class CGameObject
{
public:
	struct CB_INFO
	{
		XMFLOAT4X4						m_xmf4x4World;
		UINT							m_nMaterial;
	};

public:
	CGameObject(int nMeshes=1);
	virtual ~CGameObject();

private:
	bool							m_bActive;

public:
	void Awake() { m_bActive = true; }
	void Sleep() { m_bActive = false; }
	bool IsActive() const { return m_bActive; }

protected:
	UINT							m_ID;

	wstring							m_strFrameName;

	XMFLOAT4X4						m_xmf4x4ToParentTransform;
	XMFLOAT4X4						m_xmf4x4ToRootTransform;
	XMFLOAT4X4						m_xmf4x4World;

	vector<shared_ptr<CMesh>>		m_vecMeshes;

	shared_ptr<CMaterial>			m_pMaterial;
	shared_ptr<CParticleMgr>		m_pParticleMgr;

	CB_INFO*						m_pcbMapped;
	CB_DESC							m_cbDesc;
	
	bool							m_bDelayMove;
	float           				m_fMovingTime;
	float							m_fLerpRate;
	XMFLOAT3						m_xmf3StartPos;
	XMFLOAT4						m_xmf4StartQuat;
	XMFLOAT3						m_xmf3DestPos;
	XMFLOAT4						m_xmf4DestQuat;

	bool							m_bStun;
	float           				m_fStunTime;
	float           				m_fStunTimer;

public:
	void SetID(UINT id) { m_ID = id; }
	void SetMesh(size_t nIndex, const shared_ptr<CMesh>& pMesh);
	void SetShader(const shared_ptr<CShader>& pShader);
	void SetMaterial(const shared_ptr<CMaterial>& pMaterial);
	void SetParticleMgr(const shared_ptr<CParticleMgr>& pParticleMgr);

	virtual void CreateShaderVariables();
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst);

	virtual void Update(float fTimeElapsed);
	virtual void Animate(float fTimeElapsed);

	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera = nullptr);
	virtual void RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr);
	virtual void RenderShadowMap(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera = nullptr);

	virtual void UpdateTransform(XMFLOAT4X4 *pxmf4x4Parent = nullptr);
	virtual bool ProcessInput(const UCHAR* pKeysBuffer, float fTimeElapsed) { return false; }

	UINT							GetID() const { return m_ID; }
	const shared_ptr<CMaterial>&	GetMaterial() { return m_pMaterial; }
	XMFLOAT4X4						GetWorldMtx() { return m_xmf4x4World; }
	XMFLOAT4						GetQuat();
	XMFLOAT3						GetPosition();
	XMFLOAT3						GetLook();
	XMFLOAT3						GetUp();
	XMFLOAT3						GetRight();

	void SetLook(const XMFLOAT3& xmf3Look);

	void SetWorldMtx(const XMFLOAT4X4& mtxWorld) { m_xmf4x4ToParentTransform = mtxWorld; };
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& xmf3Position);
	void SetLocalPosition(XMFLOAT3 xmf3Position);
	void SetScale(float x, float y, float z);

	void DelayTransformation(const XMFLOAT3& DestPos, const XMFLOAT4& DestQuat, float moving_time);
	void Stun(float fStunTime);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
	void Rotate(XMFLOAT4 *pxmf4Quaternion);

public:
	CGameObject*					m_pParent = nullptr;
	CGameObject*					m_pChild = nullptr;
	CGameObject*					m_pSibling = nullptr;

	void SetChild(CGameObject *pChild);
	CGameObject* GetParent() { return m_pParent; }
	CGameObject* FindFrame(wchar_t* pstrFrameName);
};

class CSkyBox : public CGameObject
{
public:
	CSkyBox(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual ~CSkyBox();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera = nullptr);
};

class CStage : public CGameObject
{
public:
	CStage() : CGameObject(1) {}
	CStage(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual ~CStage();

	bool CollisionCheck(
		  const XMFLOAT3&	ray_origin
		, const XMFLOAT3&	ray_dir
		, float*			output_t);
};

class CBullet : public CGameObject
{
private:
	UINT							m_ShooterID;
	bool							m_bSkill;

public:
	CBullet() : CGameObject(1) {}
	CBullet(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual ~CBullet();

	void SetShooterID(UINT id) { m_ShooterID = id; }
	void SetAtkTypeIsSkill(bool bSkill) { m_bSkill = bSkill; }
	UINT GetShooterID() const { return m_ShooterID; }
	bool IsSkill() const { return m_bSkill; }
};

class CPlayerUI : public CGameObject
{
public:
	struct CB_PLAYER_INFO
	{
		float hp;
		float sp;
		float cp;
	};

public:
	CPlayerUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst);
	virtual ~CPlayerUI();
	virtual void CreateShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst);

	void SetRotateAngleY(float fRotateY) { m_fRotateY = fRotateY; }
	void SetPlayerInfo(const CB_PLAYER_INFO& info) { m_PlayerInfo = info; }
	void SetPlayerInfoHP(float hp) { m_PlayerInfo.hp = hp; }
	void SetPlayerInfoSP(float sp) { m_PlayerInfo.sp = sp; }
	void SetPlayerInfoCP(float cp) { m_PlayerInfo.cp = cp; }
	CB_PLAYER_INFO GetPlayerStatInfo() { return m_PlayerInfo; }
private:
	float								m_fRotateY;

	CB_PLAYER_INFO						m_PlayerInfo;
	CB_PLAYER_INFO*						m_pcbMappedInfo;
	CB_DESC								m_cbInfoDesc;
};