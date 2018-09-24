//-----------------------------------------------------------------------------
// File: UI.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "UI.h"
#include "Mesh\Mesh.h"
#include "Camera\Camera.h"
#include "Shader\Shader.h"

CLogoUI::CLogoUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst)
	: m_pMaterial				{ nullptr }
	, m_pcbMapped				{ nullptr }
{
	CResMgr* pRes = CResMgr::Instance();
	shared_ptr<CTexture> pTexture = make_shared<CTexture>(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
	pTexture->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
	pTexture->SetTexture(pd3dDevice, 0, 0, pRes->GetTexture(eTex::Billboard::Logo));

	shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>();
	pMaterial->SetTexture(pTexture);
	pMaterial->SetGlobalMaterialIdx(0);
	pMaterial->SetShader(pRes->GetShader(ePSO::Graphic::Logo));
	SetMaterial(pMaterial);

	CreateShaderVariables();
}

CLogoUI::~CLogoUI()
{
	ReleaseShaderVariables();
}

void CLogoUI::SetShader(const shared_ptr<CShader>& pShader)
{
	if (!m_pMaterial) {
		m_pMaterial = make_shared<CMaterial>();
	}
	m_pMaterial->SetShader(pShader);
}

void CLogoUI::SetTextureSet(const shared_ptr<CTexture>& pTexture)
{
	if (!m_pMaterial) {
		m_pMaterial = make_shared<CMaterial>();
	}
	m_pMaterial->SetTexture(pTexture);
}

void CLogoUI::SetMaterial(const shared_ptr<CMaterial>& pMaterial)
{
	m_pMaterial = pMaterial;
}

void CLogoUI::CreateShaderVariables()
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbDesc, sizeof(CB_INFO));
	m_pcbMapped = static_cast<CB_INFO*>(m_cbDesc.pMappedPtr);
	m_cbDesc.RootParamIdx = ROOT_PARAMETER_CONSTANT1;
}

void CLogoUI::ReleaseShaderVariables()
{
	m_pMaterial->ReleaseShaderVariables();
}

void CLogoUI::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst)
{
	memcpy(m_pcbMapped, &m_cbInfo, sizeof(CB_INFO));
	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbDesc.RootParamIdx, m_cbDesc.view_desc.BufferLocation);
}

void CLogoUI::Animate(float fTimeElapsed)
{
}

void CLogoUI::RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	UpdateShaderVariables(pd3dCmdLst);
	m_pMaterial->m_pShader->Render(pd3dCmdLst, pCamera);
	m_pMaterial->m_pTexture->UpdateShaderVariables(pd3dCmdLst);

	pd3dCmdLst->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCmdLst->DrawInstanced(6, 1, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWindowUI::CWindowUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst)
	: m_pMaterial{ nullptr }
	, m_pcbMapped{ nullptr }
{
	CResMgr* pRes = CResMgr::Instance();
	shared_ptr<CTexture> pTexture = make_shared<CTexture>(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
	pTexture->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
	pTexture->SetTexture(pd3dDevice, 0, 0, pRes->GetTexture(eTex::Billboard::LobbyBaseFrame));

	shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>();
	pMaterial->SetTexture(pTexture);
	pMaterial->SetGlobalMaterialIdx(0);
	pMaterial->SetShader(pRes->GetShader(ePSO::Graphic::LobbyWindow));
	SetMaterial(pMaterial);

	CreateShaderVariables();
}

CWindowUI::~CWindowUI()
{
	ReleaseShaderVariables();
}

void CWindowUI::SetShader(const shared_ptr<CShader>& pShader)
{
	if (!m_pMaterial) {
		m_pMaterial = make_shared<CMaterial>();
	}
	m_pMaterial->SetShader(pShader);
}

void CWindowUI::SetTextureSet(const shared_ptr<CTexture>& pTexture)
{
	if (!m_pMaterial) {
		m_pMaterial = make_shared<CMaterial>();
	}
	m_pMaterial->SetTexture(pTexture);
}

void CWindowUI::SetMaterial(const shared_ptr<CMaterial>& pMaterial)
{
	m_pMaterial = pMaterial;
}

void CWindowUI::CreateShaderVariables()
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbDesc, sizeof(CB_INFO));
	m_pcbMapped = static_cast<CB_INFO*>(m_cbDesc.pMappedPtr);
	m_cbDesc.RootParamIdx = ROOT_PARAMETER_CONSTANT1;
}

void CWindowUI::ReleaseShaderVariables()
{
	m_pMaterial->ReleaseShaderVariables();
}

void CWindowUI::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst)
{
	memcpy(m_pcbMapped, &m_cbInfo, sizeof(CB_INFO));
	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbDesc.RootParamIdx, m_cbDesc.view_desc.BufferLocation);
}

void CWindowUI::Animate(float fTimeElapsed)
{
}

void CWindowUI::Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	UpdateShaderVariables(pd3dCmdLst);
	m_pMaterial->m_pShader->Render(pd3dCmdLst, pCamera);
	m_pMaterial->m_pTexture->UpdateShaderVariables(pd3dCmdLst);

	pd3dCmdLst->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCmdLst->DrawInstanced(6, 1, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTextBoxUI::CTextBoxUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCmdLst)
	: m_pMaterial{ nullptr }
	, m_pcbMapped{ nullptr }
	, m_bSelected{ false }
{
	CResMgr* pRes = CResMgr::Instance();
	shared_ptr<CTexture> pTexture = make_shared<CTexture>(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
	pTexture->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
	pTexture->SetTexture(pd3dDevice, 0, 0, pRes->GetTexture(eTex::Billboard::GreySquare));

	shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>();
	pMaterial->SetTexture(pTexture);
	pMaterial->SetGlobalMaterialIdx(0);
	pMaterial->SetShader(pRes->GetShader(ePSO::Graphic::LobbyRoomSlot));
	SetMaterial(pMaterial);

	CreateShaderVariables();

	m_pFont = make_shared<CFont>(
		  pd3dDevice
		, pd3dCmdLst
		, eTex::Font::Default
		, CFont::DefaultFontPath
		, 32
		, CLIENT_WIDTH
		, CLIENT_HEIGHT);
}

CTextBoxUI::~CTextBoxUI()
{
	ReleaseShaderVariables();
}

void CTextBoxUI::SetShader(const shared_ptr<CShader>& pShader)
{
	if (!m_pMaterial) {
		m_pMaterial = make_shared<CMaterial>();
	}
	m_pMaterial->SetShader(pShader);
}

void CTextBoxUI::SetTextureSet(const shared_ptr<CTexture>& pTexture)
{
	if (!m_pMaterial) {
		m_pMaterial = make_shared<CMaterial>();
	}
	m_pMaterial->SetTexture(pTexture);
}

void CTextBoxUI::SetMaterial(const shared_ptr<CMaterial>& pMaterial)
{
	m_pMaterial = pMaterial;
}

void CTextBoxUI::CreateShaderVariables()
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbDesc, sizeof(CB_INFO));
	m_pcbMapped = static_cast<CB_INFO*>(m_cbDesc.pMappedPtr);
	m_cbDesc.RootParamIdx = ROOT_PARAMETER_CONSTANT1;
}

void CTextBoxUI::ReleaseShaderVariables()
{
	m_pMaterial->ReleaseShaderVariables();
}

void CTextBoxUI::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst)
{
	memcpy(m_pcbMapped, &m_cbInfo, sizeof(CB_INFO));
	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbDesc.RootParamIdx, m_cbDesc.view_desc.BufferLocation);
}

void CTextBoxUI::Animate(float fTimeElapsed)
{
}

void CTextBoxUI::Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	UpdateShaderVariables(pd3dCmdLst);
	m_pMaterial->m_pShader->Render(pd3dCmdLst, pCamera);
	m_pMaterial->m_pTexture->UpdateShaderVariables(pd3dCmdLst);

	pd3dCmdLst->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCmdLst->DrawInstanced(6, 1, 0, 0);

	m_pFont->ResetTextData();
	XMFLOAT2 txt_pos{
		  0.145f + ((m_cbInfo.Center.x + 1.0f) * 0.5f) - (m_cbInfo.Size.x * 0.5f)
		, 1.04f - ((m_cbInfo.Center.y + 1.0f) * 0.5f) - (m_cbInfo.Size.y * 0.5f)
	};

	constexpr float width = 1.95f * CLIENT_WIDTH / 1600;
	constexpr float height = 2.5f * CLIENT_HEIGHT / 900;
	XMFLOAT2 txt_size{ width, height };
	XMFLOAT2 txt_padding{ 0.5f, 0.0f };
	if (m_cbInfo.bCursorOn == 1 || m_bSelected) {
		m_pFont->AppendTextData(
			  m_str
			, txt_pos
			, txt_size
			, txt_padding
			, XMFLOAT4(245.f / 255.f, 184.f / 255.f, 40.f / 255.f, 1));
	}
	else {
		m_pFont->AppendTextData(
			  m_str
			, txt_pos
			, txt_size
			, txt_padding
			, XMFLOAT4(1, 1, 1, 1));
	}
	m_pFont->Render(pd3dCmdLst, pCamera);
}

void CTextBoxUI::SetText(const wstring & str)
{
	m_str = str;
}

void CTextBoxUI::CollisionCheck(const POINT & pos)
{
	float offsetX{ m_cbInfo.Size.x * 0.5f };
	float offsetY{ m_cbInfo.Size.y * 0.5f };
	XMFLOAT2 lefttop{ m_cbInfo.Center.x - offsetX, m_cbInfo.Center.y - offsetY };
	XMFLOAT2 rightbottom{ m_cbInfo.Center.x + offsetX, m_cbInfo.Center.y + offsetY };
	XMFLOAT2 cursorpos{
		  2.0f * (static_cast<float>(pos.x) / static_cast<float>(CLIENT_WIDTH)) - 1.0f
		, -(2.0f * (static_cast<float>(pos.y) / static_cast<float>(CLIENT_HEIGHT)) - 1.0f) };
	if (cursorpos.x < lefttop.x) { m_cbInfo.bCursorOn = 0; return; }
	if (cursorpos.x > rightbottom.x) { m_cbInfo.bCursorOn = 0; return; }
	if (cursorpos.y < lefttop.y) { m_cbInfo.bCursorOn = 0; return; }
	if (cursorpos.y > rightbottom.y) { m_cbInfo.bCursorOn = 0; return; }
	m_cbInfo.bCursorOn = 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSkillIconUI::CSkillIconUI()
{
	m_pMaterial = CResMgr::Instance()->GetMaterial(eMat::Object::RubySkillIcon);
	CreateShaderVariables();
}

CSkillIconUI::~CSkillIconUI()
{
}

void CSkillIconUI::CreateShaderVariables()
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbDesc, sizeof(CB_INFO));
	m_pcbMapped = static_cast<CB_INFO*>(m_cbDesc.pMappedPtr);
	m_cbDesc.RootParamIdx = ROOT_PARAMETER_CONSTANT1;
}

void CSkillIconUI::ReleaseShaderVariables()
{
	m_pMaterial->ReleaseShaderVariables();
}

void CSkillIconUI::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCmdLst)
{
	memcpy(m_pcbMapped, &m_cbInfo, sizeof(CB_INFO));
	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbDesc.RootParamIdx, m_cbDesc.view_desc.BufferLocation);
}

void CSkillIconUI::Render(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	UpdateShaderVariables(pd3dCmdLst);

	m_pMaterial->m_pShader->Render(pd3dCmdLst, pCamera);
	m_pMaterial->m_pTexture->UpdateShaderVariables(pd3dCmdLst);

	pd3dCmdLst->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCmdLst->DrawInstanced(6, 1, 0, 0);
}

void CSkillIconUI::SetSkillIconImage(eMat::Object obj)
{
	m_pMaterial = CResMgr::Instance()->GetMaterial(obj);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CCrosshair::CCrosshair()
{
	m_pMaterial = CResMgr::Instance()->GetMaterial(eMat::Object::Crosshair);
	CreateShaderVariables();
}

CCrosshair::~CCrosshair()
{
}

void CCrosshair::CreateShaderVariables()
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbDesc, sizeof(CB_INFO));
	m_pcbMapped = static_cast<CB_INFO*>(m_cbDesc.pMappedPtr);
	m_cbDesc.RootParamIdx = ROOT_PARAMETER_CONSTANT1;
}

void CCrosshair::ReleaseShaderVariables()
{
	m_pMaterial->ReleaseShaderVariables();
}

void CCrosshair::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCmdLst)
{
	memcpy(m_pcbMapped, &m_cbInfo, sizeof(CB_INFO));
	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbDesc.RootParamIdx, m_cbDesc.view_desc.BufferLocation);
}

void CCrosshair::Render(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	UpdateShaderVariables(pd3dCmdLst);

	m_pMaterial->m_pShader->Render(pd3dCmdLst, pCamera);
	//m_pMaterial->m_pTexture->UpdateShaderVariables(pd3dCmdLst);

	pd3dCmdLst->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCmdLst->DrawInstanced(6, 1, 0, 0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CButton::CButton()
{
	CreateShaderVariables();
}

CButton::~CButton()
{
}

void CButton::CreateShaderVariables()
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbDesc, sizeof(CB_INFO));
	m_pcbMapped = static_cast<CB_INFO*>(m_cbDesc.pMappedPtr);
	m_cbDesc.RootParamIdx = ROOT_PARAMETER_CONSTANT1;
}

void CButton::ReleaseShaderVariables()
{
	m_pMaterial->ReleaseShaderVariables();
}

void CButton::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCmdLst)
{
	memcpy(m_pcbMapped, &m_cbInfo, sizeof(CB_INFO));
	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbDesc.RootParamIdx, m_cbDesc.view_desc.BufferLocation);
}

void CButton::Render(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	UpdateShaderVariables(pd3dCmdLst);

	m_pMaterial->m_pShader->Render(pd3dCmdLst, pCamera);
	m_pMaterial->m_pTexture->UpdateShaderVariables(pd3dCmdLst);

	pd3dCmdLst->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCmdLst->DrawInstanced(6, 1, 0, 0);
}

void CButton::SetMaterial(eMat::Object obj)
{
	m_pMaterial = CResMgr::Instance()->GetMaterial(obj);
}

void CButton::CollisionCheck(const POINT & pos)
{
	float offsetX{ m_cbInfo.Size.x * 0.5f };
	float offsetY{ m_cbInfo.Size.y * 0.5f };
	XMFLOAT2 lefttop{ m_cbInfo.Center.x - offsetX, m_cbInfo.Center.y - offsetY };
	XMFLOAT2 rightbottom{ m_cbInfo.Center.x + offsetX, m_cbInfo.Center.y + offsetY };
	XMFLOAT2 cursorpos{ 
		  2.0f * (static_cast<float>(pos.x) / static_cast<float>(CLIENT_WIDTH)) - 1.0f
		, - (2.0f * (static_cast<float>(pos.y) / static_cast<float>(CLIENT_HEIGHT)) - 1.0f) };
	if (cursorpos.x < lefttop.x) { m_cbInfo.bCursorOn = 0; return; }
	if (cursorpos.x > rightbottom.x) { m_cbInfo.bCursorOn = 0; return; }
	if (cursorpos.y < lefttop.y) { m_cbInfo.bCursorOn = 0; return; }
	if (cursorpos.y > rightbottom.y) { m_cbInfo.bCursorOn = 0; return; }
	m_cbInfo.bCursorOn = 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGameEndUI::CGameEndUI()
{
	CreateShaderVariables();
}

CGameEndUI::~CGameEndUI()
{
}

void CGameEndUI::CreateShaderVariables()
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbDesc, sizeof(CB_INFO));
	m_pcbMapped = static_cast<CB_INFO*>(m_cbDesc.pMappedPtr);
	m_cbDesc.RootParamIdx = ROOT_PARAMETER_CONSTANT1;
}

void CGameEndUI::ReleaseShaderVariables()
{
	m_pMaterial->ReleaseShaderVariables();
}

void CGameEndUI::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCmdLst)
{
	memcpy(m_pcbMapped, &m_cbInfo, sizeof(CB_INFO));
	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbDesc.RootParamIdx, m_cbDesc.view_desc.BufferLocation);
}

void CGameEndUI::Render(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	UpdateShaderVariables(pd3dCmdLst);

	m_pMaterial->m_pShader->Render(pd3dCmdLst, pCamera);
	m_pMaterial->m_pTexture->UpdateShaderVariables(pd3dCmdLst);

	pd3dCmdLst->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCmdLst->DrawInstanced(6, 1, 0, 0);
}

void CGameEndUI::SetMaterial(eMat::Object obj)
{
	m_pMaterial = CResMgr::Instance()->GetMaterial(obj);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CDeathUI::CDeathUI()
{
	CreateShaderVariables();
}
CDeathUI::~CDeathUI()
{
}

void CDeathUI::CreateShaderVariables()
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbDesc, sizeof(CB_INFO));
	m_pcbMapped = static_cast<CB_INFO*>(m_cbDesc.pMappedPtr);
	m_cbDesc.RootParamIdx = ROOT_PARAMETER_CONSTANT1;
}

void CDeathUI::ReleaseShaderVariables()
{
	m_pMaterial->ReleaseShaderVariables();
}

void CDeathUI::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCmdLst)
{
	memcpy(m_pcbMapped, &m_cbInfo, sizeof(CB_INFO));
	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbDesc.RootParamIdx, m_cbDesc.view_desc.BufferLocation);
}

void CDeathUI::Render(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	UpdateShaderVariables(pd3dCmdLst);

	m_pMaterial->m_pShader->Render(pd3dCmdLst, pCamera);
	m_pMaterial->m_pTexture->UpdateShaderVariables(pd3dCmdLst);

	pd3dCmdLst->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCmdLst->DrawInstanced(6, 1, 0, 0);
}

void CDeathUI::SetMaterial(eMat::Object obj)
{
	m_pMaterial = CResMgr::Instance()->GetMaterial(obj);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
