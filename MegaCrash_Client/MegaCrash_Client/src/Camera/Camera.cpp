#include "stdafx.h"
#include "Object\Player\Player.h"
#include "Camera.h"

CCamera::CCamera()
	: m_xmf4x4View			{ Matrix4x4::Identity() }
	, m_xmf4x4Projection	{ Matrix4x4::Identity() }
	, m_d3dViewport			{ 0, 0, CLIENT_WIDTH , CLIENT_HEIGHT, 0.0f, 1.0f }
	, m_d3dScissorRect		{ 0, 0, CLIENT_WIDTH , CLIENT_HEIGHT }
	, m_xmf3Position		{ XMFLOAT3(0.0f, 0.0f, 0.0f) }
	, m_xmf3Right			{ XMFLOAT3(1.0f, 0.0f, 0.0f) }
	, m_xmf3Up				{ XMFLOAT3(0.0f, 1.0f, 0.0f) }
	, m_xmf3Look			{ XMFLOAT3(0.0f, 0.0f, 1.0f) }
	, m_fPitch				{ 0.0f }
	, m_fRoll				{ 0.0f }
	, m_fYaw				{ 0.0f }
	, m_nMode				{ 0x00 }
	, m_xmf3LookAtOffset	{ XMFLOAT3(0.0f, 0.0f, 2.0f) }
	, m_xmf3Offset			{ XMFLOAT3(0.0f, 0.0f, 0.0f) }
	, m_xmf3OffsetOrigin	{ XMFLOAT3(0.0f, 0.0f, 0.0f) }
	, m_fTimeLag			{ 0.0f }
	, m_bShake				{ false }
	, m_xmf3ShakeOffset		{ XMFLOAT3(0.0f, 0.0f, 0.0f) }
	, m_fShakeTime			{ 0.0f }
	, m_fShakeForce			{ 0.0f }
	, m_pPlayer				{ nullptr }
	, m_pcbMapped			{ nullptr }
{
}
CCamera::CCamera(CCamera *pCamera) : CCamera()
{
	if (pCamera) memcpy(this, pCamera, sizeof(CCamera));
}
CCamera::~CCamera()
{ 
}

void CCamera::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCmdLst)
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbDesc, sizeof(CB_INFO));
	m_pcbMapped = static_cast<CB_INFO*>(m_cbDesc.pMappedPtr);
	m_cbDesc.RootParamIdx = ROOT_PARAMETER_CAMERA;
}

void CCamera::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCmdLst)
{
	//XMStoreFloat4x4(&m_pcbMapped->xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4View)));
	//XMStoreFloat4x4(&m_pcbMapped->xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Projection)));
	XMStoreFloat4x4(&m_pcbMapped->xmf4x4ViewProjection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4ViewProjection)));
	::memcpy(&m_pcbMapped->xmf3Position, &m_xmf3Position, sizeof(XMFLOAT3));
	
	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbDesc.RootParamIdx, m_cbDesc.view_desc.BufferLocation);
}

void CCamera::ReleaseShaderVariables()
{
}

void CCamera::SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCmdLst)
{
	pd3dCmdLst->RSSetViewports(1, &m_d3dViewport);
	pd3dCmdLst->RSSetScissorRects(1, &m_d3dScissorRect);
}

void CCamera::Update(float fTimeElapsed)
{
	if (m_bShake) {
		if (m_fShakeForce < 0) {
			m_bShake = false;
			m_fShakeTime = 0.0f;
			m_fShakeForce = 0.0f;
			m_xmf3ShakeOffset = XMFLOAT3();
		}
		m_xmf3ShakeOffset = {
			  static_cast<float>(-(rand_int() % 2)) * rand_float()
			, static_cast<float>(-(rand_int() % 2)) * rand_float()
			, static_cast<float>(-(rand_int() % 2)) * rand_float()
		};
		m_xmf3ShakeOffset = Vector3::ScalarProduct(
			Vector3::Normalize(m_xmf3ShakeOffset), m_fShakeForce, false);
		m_fShakeForce -= m_fShakeForce * (fTimeElapsed / m_fShakeTime);
	}
}

void CCamera::GenerateFrustum()
{
	BoundingFrustum::CreateFromMatrix(m_xmFrustum, XMLoadFloat4x4(&m_xmf4x4Projection));
	XMMATRIX xmmtxInversView = XMMatrixInverse(NULL, XMLoadFloat4x4(&m_xmf4x4View));
	m_xmFrustum.Transform(m_xmFrustum, xmmtxInversView);
}

void CCamera::GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up)
{
	m_xmf3Position = xmf3Position;
	m_xmf3LookAtOffset = xmf3LookAt;
	m_xmf3Up = xmf3Up;
	m_xmf4x4View = Matrix4x4::LookAtLH(m_xmf3Position, m_xmf3LookAtOffset, m_xmf3Up);

	GenerateFrustum();
	GenerateViewProjection();
}

void CCamera::RegenerateViewMatrix()
{
	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);

	XMFLOAT3 xmf3Position = Vector3::Add(m_xmf3Position, m_xmf3ShakeOffset);
	m_xmf4x4View._11 = m_xmf3Right.x; m_xmf4x4View._12 = m_xmf3Up.x; m_xmf4x4View._13 = m_xmf3Look.x;
	m_xmf4x4View._21 = m_xmf3Right.y; m_xmf4x4View._22 = m_xmf3Up.y; m_xmf4x4View._23 = m_xmf3Look.y;
	m_xmf4x4View._31 = m_xmf3Right.z; m_xmf4x4View._32 = m_xmf3Up.z; m_xmf4x4View._33 = m_xmf3Look.z;
	m_xmf4x4View._41 = -Vector3::DotProduct(xmf3Position, m_xmf3Right);
	m_xmf4x4View._42 = -Vector3::DotProduct(xmf3Position, m_xmf3Up);
	m_xmf4x4View._43 = -Vector3::DotProduct(xmf3Position, m_xmf3Look);

	GenerateFrustum();
	GenerateViewProjection();
}

void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	m_xmf4x4Projection = Matrix4x4::PerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
	GenerateViewProjection();
}

void CCamera::GenerateOrthographicMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fViewWidth, float fViewHeight)
{
	m_xmf4x4Projection = Matrix4x4::OrthographicLH(fViewWidth, fViewHeight, fNearPlaneDistance, fFarPlaneDistance);
	GenerateViewProjection();
}

void CCamera::GenerateViewProjection()
{
	m_xmf4x4ViewProjection = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4Projection);
}

void CCamera::ShakeCamera(float fShakeTime, float fShakeForce)
{
	m_bShake = true; 
	m_fShakeTime = fShakeTime; 
	m_fShakeForce = fShakeForce;
	m_fShakeTimer = 0.0f;
}

void CCamera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ, float fMaxZ)
{
	m_d3dViewport.TopLeftX = float(xTopLeft);
	m_d3dViewport.TopLeftY = float(yTopLeft);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;
}

void CCamera::SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom)
{
	m_d3dScissorRect.left = xLeft;
	m_d3dScissorRect.top = yTop;
	m_d3dScissorRect.right = xRight;
	m_d3dScissorRect.bottom = yBottom;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFirstPersonCamera

CFirstPersonCamera::CFirstPersonCamera(CCamera *pCamera) : CCamera(pCamera)
{
	m_nMode = FIRST_PERSON_CAMERA;
	if (pCamera)
	{
		if (pCamera->GetMode() == SPACESHIP_CAMERA)
		{
			m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_xmf3Right.y = 0.0f;
			m_xmf3Look.y = 0.0f;
			m_xmf3Right = Vector3::Normalize(m_xmf3Right);
			m_xmf3Look = Vector3::Normalize(m_xmf3Look);
		}
	}
}

void CFirstPersonCamera::Rotate(float x, float y, float z)
{
	if (x != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
	}
	if (m_pPlayer && (y != 0.0f))
	{
		XMFLOAT3 xmf3Up = m_pPlayer->GetUpVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Up), XMConvertToRadians(y));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
	}
	if (m_pPlayer && (z != 0.0f))
	{
		XMFLOAT3 xmf3Look = m_pPlayer->GetLookVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Look), XMConvertToRadians(z));
		m_xmf3Position = Vector3::Subtract(m_xmf3Position, m_pPlayer->GetPosition());
		m_xmf3Position = Vector3::TransformCoord(m_xmf3Position, xmmtxRotate);
		m_xmf3Position = Vector3::Add(m_xmf3Position, m_pPlayer->GetPosition());
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CThirdPersonCamera

CThirdPersonCamera::CThirdPersonCamera(CCamera *pCamera) : CCamera(pCamera)
{
	m_nMode = THIRD_PERSON_CAMERA;
	if (pCamera)
	{
		if (pCamera->GetMode() == SPACESHIP_CAMERA)
		{
			m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_xmf3Right.y = 0.0f;
			m_xmf3Look.y = 0.0f;
			m_xmf3Right = Vector3::Normalize(m_xmf3Right);
			m_xmf3Look = Vector3::Normalize(m_xmf3Look);
		}
	}
}

void CThirdPersonCamera::Update(float fTimeElapsed)
{
	m_xmf3Offset = Vector3::TransformNormal(
		  m_xmf3OffsetOrigin
		, XMMatrixRotationRollPitchYaw(m_fPitch, m_fYaw, m_fRoll));

	if (m_bShake) {
		m_fShakeTimer += fTimeElapsed;
		if (m_fShakeTimer > m_fShakeTime)
		{
			m_fShakeTimer = 0.0f;
			m_bShake = false;
			m_fShakeTime = 0.0f;
			m_fShakeForce = 0.0f;
			m_xmf3ShakeOffset = XMFLOAT3();
		}
		m_xmf3ShakeOffset = {
			static_cast<float>(-(rand_int() % 2)) * rand_float()
			, static_cast<float>(-(rand_int() % 2)) * rand_float()
			, static_cast<float>(-(rand_int() % 2)) * rand_float()
		};
		float force = m_fShakeForce * sinf(m_fShakeTimer*500.0f) * powf(0.5f, m_fShakeTimer);
		m_xmf3ShakeOffset = Vector3::ScalarProduct(
			Vector3::Normalize(m_xmf3ShakeOffset), force, false);
	}

	if (m_pPlayer) {
		XMFLOAT3 xmf3OffsetX	{ Vector3::ScalarProduct(m_xmf3Right, m_xmf3LookAtOffset.x) };
		XMFLOAT3 xmf3OffsetY	{ Vector3::ScalarProduct(m_xmf3Up, m_xmf3LookAtOffset.y) };
		XMFLOAT3 xmf3OffsetZ	{ Vector3::ScalarProduct(m_xmf3Look, m_xmf3LookAtOffset.z) };
		XMFLOAT3 xmf3Offset		{ Vector3::Add(m_xmf3Offset, Vector3::Add(xmf3OffsetX, xmf3OffsetY)) };
		XMFLOAT3 xmf3Position	{ Vector3::Add(m_pPlayer->GetPosition(), xmf3Offset) };
		XMFLOAT3 xmf3Direction	{ Vector3::Subtract(xmf3Position, m_xmf3Position) };
		float fLength = Vector3::Length(xmf3Direction);
		xmf3Direction = Vector3::Normalize(xmf3Direction);
		float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
		float fDistance = fLength * fTimeLagScale;
		if (fDistance > fLength) fDistance = fLength;
		if (fLength < 0.01f) fDistance = fLength;
		if (fDistance > 0) {
			m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Direction, fDistance);

			XMFLOAT3 xmf3LookAt{ m_pPlayer->GetPosition() };
			xmf3LookAt = Vector3::Add(xmf3LookAt, xmf3OffsetX);
			xmf3LookAt = Vector3::Add(xmf3LookAt, xmf3OffsetY);
			xmf3LookAt = Vector3::Add(xmf3LookAt, xmf3OffsetZ);
			SetLookAt(xmf3LookAt);
		}
		RegenerateViewMatrix();
	}
}

void CThirdPersonCamera::Rotate(float x, float y, float z)
{
	static constexpr float RpD{ fPI / 180.0f };
	if (x != 0.0f) {
		m_fPitch += XMConvertToRadians(x);
		if (m_fPitch > +80.0f * RpD) { m_fPitch = +80.0f * RpD; }
		if (m_fPitch < -60.0f * RpD) { m_fPitch = -60.0f * RpD; }
	}
	if (y != 0.0f) {
		m_fYaw += XMConvertToRadians(y);
		if (m_fYaw > fPI) m_fYaw -= 2 * fPI;
		if (m_fYaw < -fPI) m_fYaw += 2 * fPI;
	}
}

void CThirdPersonCamera::SetLookAt(XMFLOAT3& xmf3LookAt)
{
	XMFLOAT3 xmf3Position = Vector3::Add(m_xmf3Position, m_xmf3ShakeOffset);
	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(xmf3Position, xmf3LookAt, m_pPlayer->GetUpVector());
	m_xmf3Right = XMFLOAT3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
	m_xmf3Up = XMFLOAT3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
	m_xmf3Look = XMFLOAT3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
}
