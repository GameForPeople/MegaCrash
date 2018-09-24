#include "stdafx.h"
#include "ShaderHelper.h"

CShaderHelper::CShaderHelper(ID3D12Device * pd3dDevice)
{
	::ZeroMemory(&m_Helper, sizeof(m_Helper));

	m_pTexture = new CTexture(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
	m_pTexture->CreateTextureSet(0, 1, ROOT_PARAMETER_RESOURCE2);
	m_pTexture->SetTexture(pd3dDevice, 0, 0, CResMgr::Instance()->GetTexture(eTex::Billboard::ShaderHelper));

	CreateShaderVariables();
}

CShaderHelper::~CShaderHelper()
{
	delete m_pTexture;
}

void CShaderHelper::CreateShaderVariables()
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbDesc, sizeof(CB_INFO));
	m_pcbMapped = static_cast<CB_INFO*>(m_cbDesc.pMappedPtr);
	m_cbDesc.RootParamIdx = ROOT_PARAMETER_CONSTANT3;
	::memcpy(m_pcbMapped, &m_Helper, sizeof(CB_INFO));
}

void CShaderHelper::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCmdLst)
{
	::memcpy(m_pcbMapped, &m_Helper, sizeof(CB_INFO));
	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbDesc.RootParamIdx, m_cbDesc.view_desc.BufferLocation); //Lights
	m_pTexture->UpdateShaderVariables(pd3dCmdLst);
}
