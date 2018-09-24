//-----------------------------------------------------------------------------
// File: CGameObject.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Mesh\Mesh.h"
#include "Camera\Camera.h"
#include "Shader\Shader.h"
#include "Manager\ParticleMgr\ParticleMgr.h"
#include "Object.h"

CGameObject::CGameObject(int nMeshes)
	: m_bActive					{ false }
	, m_vecMeshes				{ nMeshes }
	, m_pMaterial				{ nullptr }
	, m_xmf4x4ToParentTransform	{ Matrix4x4::Identity() }
	, m_xmf4x4World				{ Matrix4x4::Identity() }
	, m_bDelayMove				{ false }
	, m_fMovingTime				{ 0.0f }
	, m_xmf3StartPos			{ XMFLOAT3(0.0f, 0.0f, 0.0f) }
	, m_xmf4StartQuat			{ Vector4::QuatIdentity() }
	, m_xmf3DestPos				{ XMFLOAT3(0.0f, 0.0f, 0.0f) }
	, m_xmf4DestQuat			{ Vector4::QuatIdentity() }
	, m_bStun					{ false }
	, m_fStunTimer				{ 0.0f }
{
}

CGameObject::~CGameObject()
{
	ReleaseShaderVariables();

	m_vecMeshes.clear();

	if (m_pSibling) delete m_pSibling;
	if (m_pChild) delete m_pChild;
}

void CGameObject::SetMesh(size_t nIndex, const shared_ptr<CMesh>& pMesh)
{
	if (m_vecMeshes.size() <= nIndex) return;
	m_vecMeshes[nIndex] = pMesh;
}

void CGameObject::SetShader(const shared_ptr<CShader>& pShader)
{
	if (!m_pMaterial) {
		m_pMaterial = make_shared<CMaterial>();
	}
	else {
		m_pMaterial->SetShader(pShader);
	}
}

void CGameObject::SetMaterial(const shared_ptr<CMaterial>& pMaterial)
{
	m_pMaterial = pMaterial;
}

void CGameObject::SetParticleMgr(const shared_ptr<CParticleMgr>& pParticleMgr)
{
	m_pParticleMgr = pParticleMgr;
}

void CGameObject::CreateShaderVariables()
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbDesc, sizeof(CB_INFO));
	m_pcbMapped = static_cast<CB_INFO*>(m_cbDesc.pMappedPtr);
	m_cbDesc.RootParamIdx = ROOT_PARAMETER_OBJECT;
}

void CGameObject::ReleaseShaderVariables()
{
	if (m_pMaterial) m_pMaterial->ReleaseShaderVariables();
}

void CGameObject::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst)
{
	//XMStoreFloat4x4(&m_pcbMapped->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));
	static XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(-90.0f), 0.0f, 0.0f);
	XMStoreFloat4x4(&m_pcbMapped->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&Matrix4x4::Multiply(mtxRotate, m_xmf4x4World))));
	if (m_pMaterial) m_pcbMapped->m_nMaterial = m_pMaterial->m_GlobalMaterialIdx;

	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbDesc.RootParamIdx, m_cbDesc.view_desc.BufferLocation);
}

void CGameObject::Update(float fTimeElapsed)
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
		m_fLerpRate += fTimeElapsed / m_fMovingTime;
		if (m_fLerpRate > 1.0f) {
			m_bDelayMove = false;
			m_fMovingTime = 0.0f;
			m_fLerpRate = 0.0f;
			Matrix4x4::ToTransform(&m_xmf4x4ToParentTransform, m_xmf3DestPos, m_xmf4DestQuat);
		}
		else {
			XMFLOAT3 lerp_pos{ Vector3::Lerp(m_xmf3StartPos, m_xmf3DestPos, m_fLerpRate) };
			XMFLOAT4 slerp_quat{ Vector4::QuatSlerp(m_xmf4StartQuat, m_xmf4DestQuat, m_fLerpRate) };
			Matrix4x4::ToTransform(&m_xmf4x4ToParentTransform, lerp_pos, slerp_quat);
		}
		UpdateTransform();
	}
}

void CGameObject::Animate(float fTimeElapsed)
{
	if (m_pSibling) m_pSibling->Animate(fTimeElapsed);
	if (m_pChild) m_pChild->Animate(fTimeElapsed);
}

void CGameObject::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCmdLst)
{
}

void CGameObject::Render(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera)
{
	//if (!m_bActive) return;

	OnPrepareRender(pd3dCmdLst);

	if (m_pMaterial) {
		if (m_pMaterial->m_pShader) {
			m_pMaterial->m_pShader->Render(pd3dCmdLst, pCamera);
		}
		if (m_pMaterial->m_pTexture) {
			m_pMaterial->m_pTexture->UpdateShaderVariables(pd3dCmdLst);
		}
	}

	if (!m_vecMeshes.empty()) {
		UpdateShaderVariables(pd3dCmdLst);

		for (int i = 0; i < m_vecMeshes.size(); ++i) {
			m_vecMeshes[i]->Render(pd3dCmdLst);
		}
	}

	if (m_pSibling) m_pSibling->Render(pd3dCmdLst, pCamera);
	if (m_pChild) m_pChild->Render(pd3dCmdLst, pCamera);
}

void CGameObject::RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
}

void CGameObject::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	OnPrepareRender(pd3dCmdLst);

	if (m_pMaterial) {
		if (m_pMaterial->m_pShadowMapShader) {
			m_pMaterial->m_pShadowMapShader->Render(pd3dCmdLst, pCamera);
		}
	}

	if (!m_vecMeshes.empty()) {
		UpdateShaderVariables(pd3dCmdLst);

		for (int i = 0; i < m_vecMeshes.size(); ++i) {
			m_vecMeshes[i]->Render(pd3dCmdLst);
		}
	}

	if (m_pSibling) m_pSibling->RenderShadowMap(pd3dCmdLst, pCamera);
	if (m_pChild) m_pChild->RenderShadowMap(pd3dCmdLst, pCamera);
}

void CGameObject::UpdateTransform(XMFLOAT4X4 *pxmf4x4Parent)
{
	m_xmf4x4World = (pxmf4x4Parent) ? Matrix4x4::Multiply(m_xmf4x4ToParentTransform, *pxmf4x4Parent) : m_xmf4x4ToParentTransform;

	if (m_pSibling) m_pSibling->UpdateTransform(pxmf4x4Parent);
	if (m_pChild) m_pChild->UpdateTransform(&m_xmf4x4World);
}

XMFLOAT4 CGameObject::GetQuat()
{
	return Vector4::QuatFromMtx(m_xmf4x4World);
}

XMFLOAT3 CGameObject::GetPosition()
{
	return Vector3::PosFromMtx(m_xmf4x4World);
}

XMFLOAT3 CGameObject::GetLook()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33)));
}

XMFLOAT3 CGameObject::GetUp()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23)));
}

XMFLOAT3 CGameObject::GetRight()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13)));
}

void CGameObject::SetLook(const XMFLOAT3& xmf3Look)
{
	XMFLOAT3 up		{ XMFLOAT3{0, 1, 0} };
	XMFLOAT3 look	{ Vector3::Normalize(xmf3Look) };
	XMFLOAT3 right	{ Vector3::CrossProduct(up, look, true) };
	look = Vector3::CrossProduct(right, up, true);

	m_xmf4x4ToParentTransform._11 = right.x;
	m_xmf4x4ToParentTransform._12 = right.y;
	m_xmf4x4ToParentTransform._13 = right.z;
	m_xmf4x4ToParentTransform._21 = up.x;
	m_xmf4x4ToParentTransform._22 = up.y;
	m_xmf4x4ToParentTransform._23 = up.z;
	m_xmf4x4ToParentTransform._31 = look.x;
	m_xmf4x4ToParentTransform._32 = look.y;
	m_xmf4x4ToParentTransform._33 = look.z;
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4ToParentTransform._41 = x;
	m_xmf4x4ToParentTransform._42 = y;
	m_xmf4x4ToParentTransform._43 = z;
}

void CGameObject::SetPosition(const XMFLOAT3& xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

void CGameObject::SetLocalPosition(XMFLOAT3 xmf3Position)
{
	XMMATRIX mtxTranslation = XMMatrixTranslation(xmf3Position.x, xmf3Position.y, xmf3Position.z);
	m_xmf4x4ToParentTransform = Matrix4x4::Multiply(m_xmf4x4ToParentTransform, mtxTranslation);
}

void CGameObject::SetScale(float x, float y, float z)
{
	XMMATRIX mtxScale = XMMatrixScaling(x, y, z);
	m_xmf4x4ToParentTransform = Matrix4x4::Multiply(mtxScale, m_xmf4x4ToParentTransform);
}

void CGameObject::DelayTransformation(const XMFLOAT3 & DestPos, const XMFLOAT4 & DestQuat, float moving_time)
{
	m_xmf3DestPos = DestPos;
	m_xmf4DestQuat = DestQuat;
	m_fMovingTime = moving_time;
	m_fLerpRate = 0.0f;
	if (IsZero(moving_time)) {
		m_bDelayMove = false;
		Matrix4x4::ToTransform(&m_xmf4x4ToParentTransform, m_xmf3DestPos, m_xmf4DestQuat);
		UpdateTransform();
	}
	else {
		m_bDelayMove = true;
	}
	m_xmf3StartPos = Vector3::PosFromMtx(m_xmf4x4World);
	m_xmf4StartQuat = Vector4::QuatFromMtx(m_xmf4x4World);
}

void CGameObject::Stun(float fStunTime)
{
	m_bStun = true;
	m_fStunTime = fStunTime;
	m_fStunTimer = 0.0f;
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_xmf4x4ToParentTransform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4ToParentTransform);
}

void CGameObject::Rotate(XMFLOAT3 *pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	m_xmf4x4ToParentTransform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4ToParentTransform);
}

void CGameObject::Rotate(XMFLOAT4 *pxmf4Quaternion)
{
	XMMATRIX mtxRotate = XMMatrixRotationQuaternion(XMLoadFloat4(pxmf4Quaternion));
	m_xmf4x4ToParentTransform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4ToParentTransform);
}

void CGameObject::SetChild(CGameObject *pChild)
{
	if (m_pChild)
	{
		if (pChild) pChild->m_pSibling = m_pChild->m_pSibling;
		m_pChild->m_pSibling = pChild;
	}
	else
	{
		m_pChild = pChild;
	}
	if (pChild) pChild->m_pParent = this;
}

CGameObject* CGameObject::FindFrame(wchar_t* pstrFrameName)
{
	CGameObject *pFrameObject = NULL;

	if (!_tcsncmp(m_strFrameName.c_str(), pstrFrameName, _tcslen(pstrFrameName))) return(this);

	if (m_pSibling) if (pFrameObject = m_pSibling->FindFrame(pstrFrameName)) return(pFrameObject);
	if (m_pChild) if (pFrameObject = m_pChild->FindFrame(pstrFrameName)) return(pFrameObject);

	return(NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSkyBox::CSkyBox(
	  ID3D12Device*					pd3dDevice
	, ID3D12GraphicsCommandList*	pd3dCmdLst)
	: CGameObject(6)
{
	shared_ptr<CRectMeshTextured> pSkyBoxMesh = make_shared<CRectMeshTextured>(
		CRectMeshTextured(pd3dCmdLst, 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, +10.0f));
	SetMesh(0, pSkyBoxMesh);
	pSkyBoxMesh = make_shared<CRectMeshTextured>(
		CRectMeshTextured(pd3dCmdLst, 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, -10.0f));
	SetMesh(1, pSkyBoxMesh);
	pSkyBoxMesh = make_shared<CRectMeshTextured>(
		CRectMeshTextured(pd3dCmdLst, 0.0f, 20.0f, 20.0f, -10.0f, 0.0f, 0.0f));
	SetMesh(2, pSkyBoxMesh);
	pSkyBoxMesh = make_shared<CRectMeshTextured>(
		CRectMeshTextured(pd3dCmdLst, 0.0f, 20.0f, 20.0f, +10.0f, 0.0f, 0.0f));
	SetMesh(3, pSkyBoxMesh);
	pSkyBoxMesh = make_shared<CRectMeshTextured>(
		CRectMeshTextured(pd3dCmdLst, 20.0f, 0.0f, 20.0f, 0.0f, +10.0f, 0.0f));
	SetMesh(4, pSkyBoxMesh);
	pSkyBoxMesh = make_shared<CRectMeshTextured>(
		CRectMeshTextured(pd3dCmdLst, 20.0f, 0.0f, 20.0f, 0.0f, -10.0f, 0.0f));
	SetMesh(5, pSkyBoxMesh);

	CreateShaderVariables();

	SetMaterial(CResMgr::Instance()->GetMaterial(eMat::Object::SkyBox));

	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.0f), 0.0f, 0.0f);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
	m_xmf4x4ToParentTransform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4ToParentTransform);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::Render(ID3D12GraphicsCommandList *pd3dCmdLst, CCamera *pCamera)
{
	//if (!IsActive()) return;

	XMFLOAT3 xmf3CameraPos = pCamera->GetPosition();
	SetPosition(xmf3CameraPos.x, xmf3CameraPos.y, xmf3CameraPos.z);
	UpdateTransform();

	OnPrepareRender(pd3dCmdLst);

	if (m_pMaterial)
		if (m_pMaterial->m_pShader)
		{
			m_pMaterial->m_pShader->Render(pd3dCmdLst, pCamera);
		}

	if (!m_vecMeshes.empty())
	{
		UpdateShaderVariables(pd3dCmdLst);

		for (int i = 0; i < m_vecMeshes.size(); ++i)
		{
			if (m_pMaterial)
				if (m_pMaterial->m_pTexture)
					m_pMaterial->m_pTexture->UpdateShaderVariable(pd3dCmdLst, 0, 0);
			m_vecMeshes[i]->Render(pd3dCmdLst);
		}
	}

	if (m_pSibling) m_pSibling->Render(pd3dCmdLst, pCamera);
	if (m_pChild) m_pChild->Render(pd3dCmdLst, pCamera);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CStage::CStage(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList *pd3dCmdLst)
	: CGameObject(1)
{
}
CStage::~CStage()
{
}

bool CStage::CollisionCheck(
	  const XMFLOAT3&	ray_origin
	, const XMFLOAT3&	ray_dir
	, float*			output_t)
{
	CStageMesh* pMesh = reinterpret_cast<CStageMesh*>(m_vecMeshes[0].get());
	return pMesh->CollisionCheck(ray_origin, ray_dir, output_t);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CBullet::CBullet(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCmdLst)
	: CGameObject(1)
{
}

CBullet::~CBullet()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CPlayerUI::CPlayerUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst)
{
	shared_ptr<CRectMeshTextured> pSkyBoxMesh = make_shared<CRectMeshTextured>(
		CRectMeshTextured(pd3dCmdLst, 72.0f, 45.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	SetMesh(0, pSkyBoxMesh);
	SetMaterial(CResMgr::Instance()->GetMaterial(eMat::Object::PlayerUI));
	CreateShaderVariables();
}
CPlayerUI::~CPlayerUI()
{
}

void CPlayerUI::CreateShaderVariables()
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbDesc, sizeof(CB_INFO));
	m_pcbMapped = static_cast<CB_INFO*>(m_cbDesc.pMappedPtr);
	m_cbDesc.RootParamIdx = ROOT_PARAMETER_OBJECT;

	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbInfoDesc, sizeof(CB_PLAYER_INFO));
	m_pcbMappedInfo = static_cast<CB_PLAYER_INFO*>(m_cbInfoDesc.pMappedPtr);
	m_cbInfoDesc.RootParamIdx = ROOT_PARAMETER_CONSTANT1;
}

void CPlayerUI::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCmdLst)
{
	static XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(0.0f, XMConvertToRadians(m_fRotateY), 0.0f);
	XMStoreFloat4x4(&m_pcbMapped->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&Matrix4x4::Multiply(mtxRotate, m_xmf4x4World))));
	if (m_pMaterial) m_pcbMapped->m_nMaterial = m_pMaterial->m_GlobalMaterialIdx;
	memcpy(m_pcbMappedInfo, &m_PlayerInfo, sizeof(CB_PLAYER_INFO));

	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbDesc.RootParamIdx, m_cbDesc.view_desc.BufferLocation);
	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbInfoDesc.RootParamIdx, m_cbInfoDesc.view_desc.BufferLocation);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
