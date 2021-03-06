#include "stdafx.h"
#include "Texture.h"

CTexture::CTexture(
	  int							numTextureSet
	, ID3D12Device*					pd3dDevice
	, D3D12_DESCRIPTOR_HEAP_TYPE	heap_type
	, eTex::Type					eTextureType)
	: m_nTextureType	{ eTextureType }
	, m_vecTextureSet	{ numTextureSet }
	, m_ppHeapSetter	{ nullptr }
{
	m_pHeap = new DescriptorHeap(
		  pd3dDevice
		, heap_type
		, numTextureSet * TextureSet::MAX_VIEWS);
	m_ppHeapSetter = m_pHeap->pDescriptorHeap.GetAddressOf();
}

CTexture::~CTexture()
{
	for (auto& p : m_vecTextureSet)
		p.tex_desc.clear();
	m_vecTextureSet.clear();
}

void CTexture::CreateTextureSet(
	  UINT							set_idx
	, UINT							num_views
	, UINT							rootparam_idx)
{
	for (UINT i = 0; i < num_views; ++i)
	{
		m_vecTextureSet[set_idx].tex_desc.emplace_back(SR_DESC());
		m_vecTextureSet[set_idx].tex_desc[i].RootParamIdx = rootparam_idx;
	}
}

void CTexture::SetTexture(
	  ID3D12Device*					pd3dDevice
	, UINT							set_idx
	, UINT							view_idx
	, ID3D12Resource*				pTexture)
{
	TextureSet& tset = m_vecTextureSet[set_idx];
	tset.tex_desc[view_idx].pResource = pTexture;
	tset.tex_desc[view_idx].view_desc = 
		CResMgr::Instance()->GetShaderResourceViewDesc(pTexture->GetDesc(), m_nTextureType);
	m_pHeap->CreateView(
		  pd3dDevice
		, tset.tex_desc[view_idx]
		, view_idx + (set_idx * TextureSet::MAX_VIEWS));
}

void CTexture::SetTexture(
	  ID3D12Device*					pd3dDevice
	, UINT							set_idx
	, UINT							view_idx
	, ID3D12Resource*				pTexture
	, DXGI_FORMAT					view_format)
{
	TextureSet& tset = m_vecTextureSet[set_idx];
	tset.tex_desc[view_idx].pResource = pTexture;
	tset.tex_desc[view_idx].view_desc =
		CResMgr::Instance()->GetShaderResourceViewDesc(pTexture->GetDesc(), m_nTextureType);
	tset.tex_desc[view_idx].view_desc.Format = view_format;
	m_pHeap->CreateView(
		pd3dDevice
		, tset.tex_desc[view_idx]
		, view_idx + (set_idx * TextureSet::MAX_VIEWS));
}

void CTexture::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst)
{
	pd3dCmdLst->SetDescriptorHeaps(1, m_ppHeapSetter);
	for (int i = 0; i < m_vecTextureSet.size(); ++i)
		pd3dCmdLst->SetGraphicsRootDescriptorTable(
			  m_vecTextureSet[i].tex_desc[0].RootParamIdx
			, m_vecTextureSet[i].tex_desc[0].hGPUDescTable
		);
}

void CTexture::UpdateShaderVariable(ID3D12GraphicsCommandList * pd3dCmdLst, UINT set_idx, UINT view_idx)
{
	pd3dCmdLst->SetDescriptorHeaps(1, m_ppHeapSetter);
	pd3dCmdLst->SetGraphicsRootDescriptorTable(
		m_vecTextureSet[set_idx].tex_desc[view_idx].RootParamIdx
		, m_vecTextureSet[set_idx].tex_desc[view_idx].hGPUDescTable
	);
}