#include "stdafx.h"
#include "D3DBufMgr.h"
#include "External\DDSTextureLoader12.h"

UINT DESC_HANDLE_INCREMENT_SIZE_CBV_SRV_UAV;
UINT DESC_HANDLE_INCREMENT_SIZE_SAMPLER;
UINT DESC_HANDLE_INCREMENT_SIZE_RTV;
UINT DESC_HANDLE_INCREMENT_SIZE_DSV;

CD3DBufMgr::CD3DBufMgr() 
	: m_pd3dDevice				{ nullptr }
	, m_pcbUploadBuffer			{ nullptr }
	, m_pcbUBMappedPtr			{ nullptr }
	, m_pcbUploadVertexBuffer	{ nullptr }
	, m_pcbUVBMappedPtr			{ nullptr }
	, m_pcbVertexBuffer			{ nullptr }
	, m_pcbIndexBuffer			{ nullptr }
{
}

CD3DBufMgr* const CD3DBufMgr::Instance()
{
	static CD3DBufMgr* bm{ nullptr };
	if (!bm) bm = new CD3DBufMgr();
	return bm;
}

CD3DBufMgr::~CD3DBufMgr()
{
	ReleaseUploadBuffers();
	if (m_pcbUploadBuffer)
		m_pcbUploadBuffer->Unmap(0, NULL);
	if (m_pcbUploadVertexBuffer)
		m_pcbUploadVertexBuffer->Unmap(0, NULL);
}

void CD3DBufMgr::Release()
{
	m_pd3dDevice.Reset();
	m_pcbUploadBuffer.Reset();
	m_pcbUploadVertexBuffer.Reset();
	m_pcbVertexBuffer.Reset();
	m_pcbIndexBuffer.Reset();
	ReleaseUploadBuffers();
}

void CD3DBufMgr::ReleaseUploadBuffers()
{
	m_lstUploadBufs.clear();
}

void CD3DBufMgr::ConnectDevice(const ComPtr<ID3D12Device>& pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	DESC_HANDLE_INCREMENT_SIZE_CBV_SRV_UAV = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DESC_HANDLE_INCREMENT_SIZE_SAMPLER = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	DESC_HANDLE_INCREMENT_SIZE_RTV = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	DESC_HANDLE_INCREMENT_SIZE_DSV = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

ComPtr<ID3D12Resource> CD3DBufMgr::CreateBufferResource(
	  ID3D12GraphicsCommandList*		pd3dCmdLst
	, void*								pData
	, UINT								nBytes
	, D3D12_HEAP_TYPE					d3dHeapType
	, D3D12_RESOURCE_STATES				d3dResourceStates
	, ID3D12Resource**					ppd3dUploadBuffer)
{
	ComPtr<ID3D12Resource> pd3dBuffer{ nullptr };

	D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
	::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
	d3dHeapPropertiesDesc.Type = d3dHeapType;
	d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapPropertiesDesc.CreationNodeMask = 1;
	d3dHeapPropertiesDesc.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC d3dResourceDesc;
	::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = nBytes;
	d3dResourceDesc.Height = 1;
	d3dResourceDesc.DepthOrArraySize = 1;
	d3dResourceDesc.MipLevels = 1;
	d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	d3dResourceDesc.SampleDesc.Count = 1;
	d3dResourceDesc.SampleDesc.Quality = 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_RESOURCE_STATES d3dResourceInitialStates = d3dResourceStates;
	if (pData) d3dResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;
	if (d3dHeapType == D3D12_HEAP_TYPE_UPLOAD)
	{
		d3dResourceInitialStates = D3D12_RESOURCE_STATE_GENERIC_READ;
	}
	else if (d3dHeapType == D3D12_HEAP_TYPE_READBACK)
	{
		d3dResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;
	}

	HRESULT hResult = m_pd3dDevice->CreateCommittedResource(
		  &d3dHeapPropertiesDesc
		, D3D12_HEAP_FLAG_NONE
		, &d3dResourceDesc
		, d3dResourceInitialStates
		, NULL
		, IID_PPV_ARGS(&pd3dBuffer));
	
	if (pData)
	{
		switch (d3dHeapType)
		{
		case D3D12_HEAP_TYPE_DEFAULT:
		{
			m_lstUploadBufs.push_back(CreateBufferResource(pd3dCmdLst, nullptr, nBytes));

			D3D12_SUBRESOURCE_DATA d3dSubResourceData;
			::ZeroMemory(&d3dSubResourceData, sizeof(D3D12_SUBRESOURCE_DATA));
			d3dSubResourceData.pData = pData;
			d3dSubResourceData.SlicePitch = d3dSubResourceData.RowPitch = nBytes;
			::UpdateSubresources<1>(
				  pd3dCmdLst
				, pd3dBuffer.Get()
				, m_lstUploadBufs.back().Get()
				, 0
				, 0
				, 1
				, &d3dSubResourceData);

			SynchronizeResourceTransition(
				  pd3dCmdLst
				, pd3dBuffer.Get()
				, D3D12_RESOURCE_STATE_COPY_DEST
				, d3dResourceStates);
			break;
		}
		case D3D12_HEAP_TYPE_UPLOAD:
		{
			D3D12_RANGE d3dReadRange = { 0, 0 };
			UINT8 *pBufferDataBegin = NULL;
			pd3dBuffer->Map(0, &d3dReadRange, (void **)&pBufferDataBegin);
			memcpy(pBufferDataBegin, pData, nBytes);
			pd3dBuffer->Unmap(0, NULL);
			break;
		}
		case D3D12_HEAP_TYPE_READBACK:
			break;
		}
	}
	return(pd3dBuffer);
}

ComPtr<ID3D12Resource> CD3DBufMgr::CreateTextureResourceFromFile(
	  ID3D12GraphicsCommandList*		pd3dCmdLst
	, const wchar_t*					pszFileName
	, D3D12_RESOURCE_STATES				d3dResourceStates)
{
	ComPtr<ID3D12Resource> pd3dTexture{ nullptr };
	std::unique_ptr<uint8_t[]> ddsData;
	std::vector<D3D12_SUBRESOURCE_DATA> vSubresources;
	DDS_ALPHA_MODE ddsAlphaMode = DDS_ALPHA_MODE_UNKNOWN;
	bool bIsCubeMap = false;

	HRESULT hResult = DirectX::LoadDDSTextureFromFileEx(
		  m_pd3dDevice.Get()
		, pszFileName
		, 0
		, D3D12_RESOURCE_FLAG_NONE
		, DDS_LOADER_DEFAULT
		, &pd3dTexture
		, ddsData
		, vSubresources
		, &ddsAlphaMode
		, &bIsCubeMap);

	UINT nSubResources = (UINT)vSubresources.size();
	UINT64 nBytes = GetRequiredIntermediateSize(pd3dTexture.Get(), 0, nSubResources);

	m_lstUploadBufs.push_back(CreateBufferResource(pd3dCmdLst, nullptr, nBytes));

	UINT64 nBytesUpdated = ::UpdateSubresources(
		  pd3dCmdLst
		, pd3dTexture.Get()
		, m_lstUploadBufs.back().Get()
		, 0
		, 0
		, nSubResources
		, &vSubresources[0]);

	SynchronizeResourceTransition(
		  pd3dCmdLst
		, pd3dTexture.Get()
		, D3D12_RESOURCE_STATE_COPY_DEST
		, d3dResourceStates);

	return(pd3dTexture);
}

ComPtr<ID3D12Resource> CD3DBufMgr::CreateTexture2DResource(
	  UINT							nWidth
	, UINT							nHeight
	, DXGI_FORMAT					dxgiFormat
	, D3D12_RESOURCE_FLAGS			d3dResourceFlags
	, D3D12_RESOURCE_STATES			d3dResourceStates
	, D3D12_CLEAR_VALUE *			pd3dClearValue)
{
	ComPtr<ID3D12Resource> pd3dTexture{ nullptr };

	D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
	::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
	d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_DEFAULT;
	d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapPropertiesDesc.CreationNodeMask = 1;
	d3dHeapPropertiesDesc.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC d3dTextureResourceDesc;
	::ZeroMemory(&d3dTextureResourceDesc, sizeof(D3D12_RESOURCE_DESC));
	d3dTextureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3dTextureResourceDesc.Alignment = 0;
	d3dTextureResourceDesc.Width = nWidth;
	d3dTextureResourceDesc.Height = nHeight;
	d3dTextureResourceDesc.DepthOrArraySize = 1;
	d3dTextureResourceDesc.MipLevels = 1;
	d3dTextureResourceDesc.Format = dxgiFormat;
	d3dTextureResourceDesc.SampleDesc.Count = 1;
	d3dTextureResourceDesc.SampleDesc.Quality = 0;
	d3dTextureResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	d3dTextureResourceDesc.Flags = d3dResourceFlags;

	HRESULT hResult = m_pd3dDevice->CreateCommittedResource(
		  &d3dHeapPropertiesDesc
		, D3D12_HEAP_FLAG_NONE
		, &d3dTextureResourceDesc
		, d3dResourceStates
		, pd3dClearValue
		, IID_PPV_ARGS(&pd3dTexture));

	return(pd3dTexture);
}

void CD3DBufMgr::SynchronizeResourceTransition(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, ID3D12Resource*				pd3dResource
	, D3D12_RESOURCE_STATES			d3dStateBefore
	, D3D12_RESOURCE_STATES			d3dStateAfter)
{
	D3D12_RESOURCE_BARRIER d3dResourceBarrier;
	d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier.Transition.pResource = pd3dResource;
	d3dResourceBarrier.Transition.StateBefore = d3dStateBefore;
	d3dResourceBarrier.Transition.StateAfter = d3dStateAfter;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	pd3dCmdLst->ResourceBarrier(1, &d3dResourceBarrier);
}

void CD3DBufMgr::CreateUploadBuffer(ID3D12GraphicsCommandList * pd3dCmdLst)
{
	m_pcbUploadBuffer = CreateBufferResource(pd3dCmdLst, nullptr, MAX_UPLOAD_BUFFER_SIZE);
	m_pcbUploadBuffer->Map(0, NULL, (void **)&m_pcbUBMappedPtr);
	m_UBStartIdx = 0;
}

void CD3DBufMgr::CreateVertexBuffer(ID3D12GraphicsCommandList * pd3dCmdLst)
{
	m_pcbVertexBuffer = CreateBufferResource(
		  pd3dCmdLst
		, nullptr
		, MAX_VERTEX_BUFFER_SIZE
		, D3D12_HEAP_TYPE_DEFAULT
		, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	m_VertexBufStartIdx = 0;

	m_pcbUploadVertexBuffer = CreateBufferResource(
		  pd3dCmdLst
		, nullptr
		, MAX_UPLOAD_VERTEX_BUFFER_SIZE
		, D3D12_HEAP_TYPE_UPLOAD
		, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	m_pcbUploadVertexBuffer->Map(0, NULL, (void **)&m_pcbUVBMappedPtr);
	m_UVBStartIdx = 0;
}

void CD3DBufMgr::CreateIndexBuffer(ID3D12GraphicsCommandList * pd3dCmdLst)
{
	m_pcbIndexBuffer = CreateBufferResource(
		  pd3dCmdLst
		, nullptr
		, MAX_INDEX_BUFFER_SIZE
		, D3D12_HEAP_TYPE_DEFAULT
		, D3D12_RESOURCE_STATE_INDEX_BUFFER);
	m_IndexBufStartIdx = 0;
}

bool CD3DBufMgr::AllocUploadBuffer(CB_DESC& cb_desc, UINT nBytes)
{
	if(m_pcbUploadBuffer == nullptr)
		return false;
	UINT ncbElementBytes = ((nBytes + 255) & ~255); //256ÀÇ ¹è¼ö
	
	cb_desc.nMappedData = ncbElementBytes;
	cb_desc.view_desc.SizeInBytes = ncbElementBytes;
	cb_desc.view_desc.BufferLocation = m_pcbUploadBuffer->GetGPUVirtualAddress() + m_UBStartIdx;

	cb_desc.pMappedPtr = reinterpret_cast<LPVOID>(
		m_UBStartIdx + reinterpret_cast<long long>(m_pcbUBMappedPtr));
	m_UBStartIdx += ncbElementBytes;
	return true;
}

bool CD3DBufMgr::AllocUploadVertexBuffer(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, D3D12_VERTEX_BUFFER_VIEW&		vb_view
	, void*							pData
	, UINT							nBytes
	, UINT							nStride
	, void**						ppMappedPtr)
{
	if (m_pcbUploadVertexBuffer == nullptr)
		return false;

	vb_view.SizeInBytes = nBytes;
	vb_view.StrideInBytes = nStride;
	vb_view.BufferLocation = m_pcbUploadVertexBuffer->GetGPUVirtualAddress() + m_UVBStartIdx;
	*ppMappedPtr = reinterpret_cast<LPVOID>(
		m_UVBStartIdx + reinterpret_cast<long long>(m_pcbUVBMappedPtr));
	m_UVBStartIdx += nBytes;

	if(pData)
		memcpy(*ppMappedPtr, pData, nBytes);
	return true;
}

bool CD3DBufMgr::AllocVertexBuffer(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, D3D12_VERTEX_BUFFER_VIEW&		vb_view
	, void*							pData
	, UINT							nBytes
	, UINT							nStride)
{
	if (m_pcbVertexBuffer == nullptr)
		return false;

	SynchronizeResourceTransition(
		pd3dCmdLst
		, m_pcbVertexBuffer.Get()
		, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
		, D3D12_RESOURCE_STATE_COPY_DEST);

	m_lstUploadBufs.push_back(CreateBufferResource(pd3dCmdLst, pData, nBytes));

	pd3dCmdLst->CopyBufferRegion(
		m_pcbVertexBuffer.Get()
		, m_VertexBufStartIdx
		, m_lstUploadBufs.back().Get()
		, 0
		, nBytes);

	vb_view.SizeInBytes = nBytes;
	vb_view.StrideInBytes = nStride;
	vb_view.BufferLocation = m_pcbVertexBuffer->GetGPUVirtualAddress() + m_VertexBufStartIdx;
	m_VertexBufStartIdx += nBytes;

	SynchronizeResourceTransition(
		pd3dCmdLst
		, m_pcbVertexBuffer.Get()
		, D3D12_RESOURCE_STATE_COPY_DEST
		, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	return true;
}

bool CD3DBufMgr::AllocIndexBuffer(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, D3D12_INDEX_BUFFER_VIEW&		ib_view
	, void*							pData
	, UINT							nBytes)
{
	if (m_pcbIndexBuffer == nullptr)
		return false;

	SynchronizeResourceTransition(
		  pd3dCmdLst
		, m_pcbIndexBuffer.Get()
		, D3D12_RESOURCE_STATE_INDEX_BUFFER
		, D3D12_RESOURCE_STATE_COPY_DEST);

	m_lstUploadBufs.push_back(CreateBufferResource(pd3dCmdLst, pData, nBytes));

	pd3dCmdLst->CopyBufferRegion(
		  m_pcbIndexBuffer.Get()
		, m_IndexBufStartIdx
		, m_lstUploadBufs.back().Get()
		, 0
		, nBytes);

	ib_view.BufferLocation = m_pcbIndexBuffer->GetGPUVirtualAddress() + m_IndexBufStartIdx;
	ib_view.SizeInBytes = nBytes;
	ib_view.Format = DXGI_FORMAT_R32_UINT;
	m_IndexBufStartIdx += nBytes;

	SynchronizeResourceTransition(
		  pd3dCmdLst
		, m_pcbIndexBuffer.Get()
		, D3D12_RESOURCE_STATE_COPY_DEST
		, D3D12_RESOURCE_STATE_INDEX_BUFFER);

	return true;
}

