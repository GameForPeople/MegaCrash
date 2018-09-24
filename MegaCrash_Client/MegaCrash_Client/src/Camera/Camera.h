#pragma once

constexpr float ASPECT_RATIO		{ float(CLIENT_WIDTH) / float(CLIENT_HEIGHT) };
constexpr float NEAR_DISTANCE		{ 1.f };
constexpr float FAR_DISTANCE		{ 86603.f };
constexpr float DEFAULT_FOV_ANGLE	{ 75.0f };
constexpr float BOOST_FOV_ANGLE		{ 65.0f };
constexpr float TIME_LAG			{ 0.0f };

enum{
	  FIRST_PERSON_CAMERA	=	0x01
	, SPACESHIP_CAMERA		=	0x02
	, THIRD_PERSON_CAMERA	=	0x03
	, MINIMAP_CAMERA		=	0x04
};

class CPlayer;

class CCamera
{
public:
	struct CB_INFO
	{
		XMFLOAT4X4					xmf4x4ViewProjection;
		XMFLOAT3					xmf3Position;
	};

protected:
	BoundingFrustum					m_xmFrustum;

	XMFLOAT3						m_xmf3Position;
	XMFLOAT3						m_xmf3Right;
	XMFLOAT3						m_xmf3Up;
	XMFLOAT3						m_xmf3Look;

	float           				m_fPitch;
	float           				m_fRoll;
	float           				m_fYaw;

	DWORD							m_nMode;

	XMFLOAT3						m_xmf3LookAtOffset;
	XMFLOAT3						m_xmf3Offset;
	XMFLOAT3						m_xmf3OffsetOrigin;
	float           				m_fTimeLag;

	bool							m_bShake;
	XMFLOAT3						m_xmf3ShakeOffset;
	float           				m_fShakeTimer;
	float           				m_fShakeTime;
	float           				m_fShakeForce;

	XMFLOAT4X4						m_xmf4x4View;
	XMFLOAT4X4						m_xmf4x4Projection;
	XMFLOAT4X4						m_xmf4x4ViewProjection;

	D3D12_VIEWPORT					m_d3dViewport;
	D3D12_RECT						m_d3dScissorRect;

	CPlayer*						m_pPlayer;

	CB_INFO*						m_pcbMapped;
	CB_DESC							m_cbDesc;

public:
	CCamera();
	CCamera(CCamera *pCamera);
	virtual ~CCamera();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCmdLst);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst);

	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCmdLst);

	virtual void Update(float fTimeElapsed);
	virtual void Move(const XMFLOAT3& xmf3Shift) { m_xmf3Position.x += xmf3Shift.x; m_xmf3Position.y += xmf3Shift.y; m_xmf3Position.z += xmf3Shift.z; }
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	virtual void SetLookAt(XMFLOAT3& xmf3LookAt) { }

	void GenerateFrustum();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);
	void GenerateOrthographicMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fViewWidth, float fViewHeight);

	void GenerateViewProjection();

	void ShakeCamera(float fShakeTime, float fShakeForce);
	void ResetRotate() { m_fPitch = 0.0f; m_fRoll = 0.0f; m_fYaw = 0.0f; }

	// Setter
	void SetMode(DWORD nMode) { m_nMode = nMode; }

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	void SetPosition(XMFLOAT3 xmf3Position) { m_xmf3Position = xmf3Position; }
	void SetLookAtOffset(const XMFLOAT3& xmf3LookAtOffset) { m_xmf3LookAtOffset = xmf3LookAtOffset; }
	void SetLookAtOffset(float x, float y, float z) { m_xmf3LookAtOffset = XMFLOAT3{ x, y, z }; }
	void SetLookAtOffsetX(float x) { m_xmf3LookAtOffset.x = x; }
	void SetLookAtOffsetY(float y) { m_xmf3LookAtOffset.y = y; }
	void SetLookAtOffsetZ(float z) { m_xmf3LookAtOffset.z = z; }
	void SetOffset(const XMFLOAT3& xmf3Offset) { m_xmf3OffsetOrigin = xmf3Offset; }
	void SetOffsetX(float x) { m_xmf3OffsetOrigin.x = x; }
	void SetOffsetY(float y) { m_xmf3OffsetOrigin.y = y; }
	void SetOffsetZ(float z) { m_xmf3OffsetOrigin.z = z; }
	 
	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }

	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }

	// Getter
	BoundingFrustum&	GetFrustum()			{ return m_xmFrustum; }
	XMFLOAT4X4			GetViewMatrix()			{ return m_xmf4x4View; }
	XMFLOAT4X4			GetProjectionMatrix()	{ return m_xmf4x4Projection; }
														 
	DWORD				GetMode()				{ return m_nMode; }
														 
	D3D12_VIEWPORT		GetViewport()			{ return m_d3dViewport; }
	D3D12_RECT			GetScissorRect()		{ return m_d3dScissorRect; }
														 
	XMFLOAT3			GetPosition()			{ return Vector3::Add(m_xmf3Position, m_xmf3ShakeOffset); }
	XMFLOAT3&			GetLookAtOffset()		{ return m_xmf3LookAtOffset; }
	XMFLOAT3&			GetRightVector()		{ return m_xmf3Right; }
	XMFLOAT3&			GetUpVector()			{ return m_xmf3Up; }
	XMFLOAT3&			GetLookVector()			{ return m_xmf3Look; }
	XMFLOAT3&			GetOffset()				{ return m_xmf3Offset; }

	float				GetPitch()				{ return(m_fPitch); }
	float				GetRoll()				{ return(m_fRoll); }
	float				GetYaw()				{ return(m_fYaw); }
	float				GetTimeLag()			{ return(m_fTimeLag); }

	CPlayer*			GetPlayer()				{ return(m_pPlayer); }
};

class CFirstPersonCamera : public CCamera
{
public:
	CFirstPersonCamera(CCamera *pCamera);
	virtual ~CFirstPersonCamera() { }

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

class CThirdPersonCamera : public CCamera
{
public:
	CThirdPersonCamera(CCamera *pCamera);
	virtual ~CThirdPersonCamera() { }

	virtual void Update(float fTimeElapsed);
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	virtual void SetLookAt(XMFLOAT3& vLookAt);
};

