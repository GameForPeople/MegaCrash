#pragma once
constexpr UINT	NUM_SWAP_CHAIN_BUFS				{ 2 };
constexpr UINT	NUM_RENDER_TARGET_BUFS			{ 8 };
constexpr UINT	NUM_DEFERRED_LIGHTING_BUFS		{ 6 };

constexpr FLOAT BUF_CLEAR_COLOR[]				{ 1.0f, 1.0f, 1.0f, 0.0f };

constexpr UINT	CONSTANT_BUFFER_DEFAULT_SIZE	{ 65536 };
constexpr UINT	MAX_UPLOAD_BUFFER_NUM			{ 512 };
constexpr UINT	MAX_UPLOAD_BUFFER_SIZE			{ CONSTANT_BUFFER_DEFAULT_SIZE * MAX_UPLOAD_BUFFER_NUM };
constexpr UINT	MAX_UPLOAD_VERTEX_BUFFER_NUM	{ 1024 };
constexpr UINT	MAX_UPLOAD_VERTEX_BUFFER_SIZE	{ CONSTANT_BUFFER_DEFAULT_SIZE * MAX_UPLOAD_VERTEX_BUFFER_NUM };
constexpr UINT	MAX_VERTEX_BUFFER_NUM			{ 512 };
constexpr UINT	MAX_VERTEX_BUFFER_SIZE			{ CONSTANT_BUFFER_DEFAULT_SIZE * MAX_VERTEX_BUFFER_NUM };
constexpr UINT	MAX_INDEX_BUFFER_NUM			{ 128 };
constexpr UINT	MAX_INDEX_BUFFER_SIZE			{ CONSTANT_BUFFER_DEFAULT_SIZE * MAX_INDEX_BUFFER_NUM };

extern UINT DESC_HANDLE_INCREMENT_SIZE_CBV_SRV_UAV;
extern UINT DESC_HANDLE_INCREMENT_SIZE_SAMPLER;
extern UINT DESC_HANDLE_INCREMENT_SIZE_RTV;
extern UINT DESC_HANDLE_INCREMENT_SIZE_DSV;

#include "Manager\ResourceEnum.h"
#include "Manager\ResourceStructure.h"

class CD3DBufMgr
{
private:
	CD3DBufMgr();

public:
	static CD3DBufMgr* const Instance();
	~CD3DBufMgr();
	
	void Release();
	void ReleaseUploadBuffers();

	void ConnectDevice(const ComPtr<ID3D12Device>& pd3dDevice);

	ComPtr<ID3D12Resource> CreateBufferResource(
		  ID3D12GraphicsCommandList*			pd3dCmdLst
		, void*									pData
		, UINT									nBytes
		, D3D12_HEAP_TYPE						d3dHeapType = D3D12_HEAP_TYPE_UPLOAD
		, D3D12_RESOURCE_STATES					d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
		, ID3D12Resource**						ppd3dUploadBuffer = nullptr);
	ComPtr<ID3D12Resource> CreateTextureResourceFromFile(
		  ID3D12GraphicsCommandList*			pd3dCmdLst
		, const wchar_t*						pszFileName
		, D3D12_RESOURCE_STATES					d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	ComPtr<ID3D12Resource> CreateTexture2DResource(
		  UINT									nWidth
		, UINT									nHeight
		, DXGI_FORMAT							dxgiFormat
		, D3D12_RESOURCE_FLAGS					d3dResourceFlags
		, D3D12_RESOURCE_STATES					d3dResourceStates
		, D3D12_CLEAR_VALUE*					pd3dClearValue);

	void SynchronizeResourceTransition(
		  ID3D12GraphicsCommandList*			pd3dCmdLst
		, ID3D12Resource*						pd3dResource
		, D3D12_RESOURCE_STATES					d3dStateBefore
		, D3D12_RESOURCE_STATES					d3dStateAfter);

	void CreateUploadBuffer(ID3D12GraphicsCommandList* pd3dCmdLst);
	void CreateVertexBuffer(ID3D12GraphicsCommandList* pd3dCmdLst);
	void CreateIndexBuffer(ID3D12GraphicsCommandList* pd3dCmdLst);

	bool AllocUploadBuffer(CB_DESC& cb_desc, UINT nBytes);
	bool AllocUploadVertexBuffer(
		  ID3D12GraphicsCommandList*			pd3dCmdLst
		, D3D12_VERTEX_BUFFER_VIEW&				vb_view
		, void*									pData
		, UINT									nBytes
		, UINT									nStride
		, void**								ppMappedPtr);
	bool AllocVertexBuffer(
		  ID3D12GraphicsCommandList*			pd3dCmdLst
		, D3D12_VERTEX_BUFFER_VIEW&				vb_view
		, void*									pData 
		, UINT									nBytes
		, UINT									nStride);
	bool AllocIndexBuffer(
		  ID3D12GraphicsCommandList*			pd3dCmdLst
		, D3D12_INDEX_BUFFER_VIEW&				ib_view
		, void*									pData
		, UINT									nBytes);

private:
	ComPtr<ID3D12Device>					m_pd3dDevice;

	ComPtr<ID3D12Resource>					m_pcbUploadBuffer;
	void*									m_pcbUBMappedPtr;
	long long								m_UBStartIdx;

	ComPtr<ID3D12Resource>					m_pcbUploadVertexBuffer;
	void*									m_pcbUVBMappedPtr;
	long long								m_UVBStartIdx;

	ComPtr<ID3D12Resource>					m_pcbVertexBuffer;
	long long								m_VertexBufStartIdx;

	ComPtr<ID3D12Resource>					m_pcbIndexBuffer;
	long long								m_IndexBufStartIdx;

	list<ComPtr<ID3D12Resource>>			m_lstUploadBufs;
};