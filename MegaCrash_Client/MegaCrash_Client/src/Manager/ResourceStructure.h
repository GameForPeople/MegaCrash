#pragma once

struct CB_DESC {
	UINT								nMappedData;
	LPVOID								pMappedPtr;
	UINT								RootParamIdx;
	D3D12_CONSTANT_BUFFER_VIEW_DESC		view_desc;
	D3D12_CPU_DESCRIPTOR_HANDLE			hCPUDescTable;
	D3D12_GPU_DESCRIPTOR_HANDLE			hGPUDescTable;
};
struct SR_DESC {
	UINT								RootParamIdx;
	ID3D12Resource*						pResource;
	D3D12_SHADER_RESOURCE_VIEW_DESC		view_desc;
	D3D12_CPU_DESCRIPTOR_HANDLE			hCPUDescTable;
	D3D12_GPU_DESCRIPTOR_HANDLE			hGPUDescTable;
};
struct UA_DESC {
	ID3D12Resource*						pResource;
	D3D12_UNORDERED_ACCESS_VIEW_DESC	view_desc;
	D3D12_CPU_DESCRIPTOR_HANDLE			hCPUDescTable;
	D3D12_GPU_DESCRIPTOR_HANDLE			hGPUDescTable;
};
struct RT_DESC {
	ComPtr<ID3D12Resource>				pResource;
	D3D12_RENDER_TARGET_VIEW_DESC		view_desc;
	D3D12_CPU_DESCRIPTOR_HANDLE			hCPUDescTable;
	D3D12_GPU_DESCRIPTOR_HANDLE			hGPUDescTable;
};
struct DS_DESC {
	ComPtr<ID3D12Resource>				pResource;
	D3D12_DEPTH_STENCIL_VIEW_DESC		view_desc;
	D3D12_CPU_DESCRIPTOR_HANDLE			hCPUDescTable;
	D3D12_GPU_DESCRIPTOR_HANDLE			hGPUDescTable;
};


struct DescriptorHeap
{
	UINT								nViews;
	ComPtr<ID3D12DescriptorHeap>		pDescriptorHeap;

	D3D12_CPU_DESCRIPTOR_HANDLE			hCPUDescStart; // view 생성(등록)에 사용
	D3D12_GPU_DESCRIPTOR_HANDLE			hGPUDescStart; // set root desc에 사용

	DescriptorHeap(
		  ID3D12Device*					pd3dDevice
		, D3D12_DESCRIPTOR_HEAP_TYPE	heap_type
		, UINT							num_views
		, D3D12_DESCRIPTOR_HEAP_FLAGS	flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
		: pDescriptorHeap	{ nullptr }
		, nViews			{ num_views }
	{
		D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
		d3dDescriptorHeapDesc.NumDescriptors = nViews;
		d3dDescriptorHeapDesc.Type = heap_type;
		d3dDescriptorHeapDesc.Flags = flags;
		d3dDescriptorHeapDesc.NodeMask = 0;

		pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, IID_PPV_ARGS(&pDescriptorHeap));

		hCPUDescStart = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		hGPUDescStart = pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	}
	bool CreateView(ID3D12Device* pd3dDevice, CB_DESC& cb_desc, UINT view_idx)
	{
		if (view_idx >= nViews) return false;
		cb_desc.hCPUDescTable.ptr = hCPUDescStart.ptr + DESC_HANDLE_INCREMENT_SIZE_CBV_SRV_UAV * view_idx;
		cb_desc.hGPUDescTable.ptr = hGPUDescStart.ptr + DESC_HANDLE_INCREMENT_SIZE_CBV_SRV_UAV * view_idx;
		pd3dDevice->CreateConstantBufferView(&cb_desc.view_desc, cb_desc.hCPUDescTable);
		return true;
	}
	bool CreateView(ID3D12Device* pd3dDevice, SR_DESC& sr_desc, UINT view_idx)
	{
		if (view_idx >= nViews) return false;
		sr_desc.hCPUDescTable.ptr = hCPUDescStart.ptr + DESC_HANDLE_INCREMENT_SIZE_CBV_SRV_UAV * view_idx;
		sr_desc.hGPUDescTable.ptr = hGPUDescStart.ptr + DESC_HANDLE_INCREMENT_SIZE_CBV_SRV_UAV * view_idx;
		pd3dDevice->CreateShaderResourceView(sr_desc.pResource, &sr_desc.view_desc, sr_desc.hCPUDescTable);
		return true;
	}
	bool CreateView(ID3D12Device* pd3dDevice, UA_DESC& ua_desc, UINT view_idx)
	{
		if (view_idx >= nViews) return false;
		ua_desc.hCPUDescTable.ptr = hCPUDescStart.ptr + DESC_HANDLE_INCREMENT_SIZE_CBV_SRV_UAV * view_idx;
		ua_desc.hGPUDescTable.ptr = hGPUDescStart.ptr + DESC_HANDLE_INCREMENT_SIZE_CBV_SRV_UAV * view_idx;
		pd3dDevice->CreateUnorderedAccessView(ua_desc.pResource, nullptr, &ua_desc.view_desc, ua_desc.hCPUDescTable);
		return true;
	}
	bool CreateView(ID3D12Device* pd3dDevice, RT_DESC& rt_desc, UINT view_idx)
	{
		if (view_idx >= nViews) return false;
		rt_desc.hCPUDescTable.ptr = hCPUDescStart.ptr + DESC_HANDLE_INCREMENT_SIZE_RTV * view_idx;
		rt_desc.hGPUDescTable.ptr = hGPUDescStart.ptr + DESC_HANDLE_INCREMENT_SIZE_RTV * view_idx;
		pd3dDevice->CreateRenderTargetView(rt_desc.pResource.Get(), &rt_desc.view_desc, rt_desc.hCPUDescTable);
		return true;
	}
	bool CreateView(ID3D12Device* pd3dDevice, DS_DESC& ds_desc, UINT view_idx)
	{
		if (view_idx >= nViews) return false;

		D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
		::ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
		d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		d3dDepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

		ds_desc.view_desc = d3dDepthStencilViewDesc;
		ds_desc.hCPUDescTable.ptr = hCPUDescStart.ptr + DESC_HANDLE_INCREMENT_SIZE_DSV * view_idx;
		ds_desc.hGPUDescTable.ptr = hGPUDescStart.ptr + DESC_HANDLE_INCREMENT_SIZE_DSV * view_idx;
		pd3dDevice->CreateDepthStencilView(ds_desc.pResource.Get(), &ds_desc.view_desc, ds_desc.hCPUDescTable);
		return true;
	}
};

struct RootSignature
{
public:
	RootSignature(UINT nParams, UINT nSamplers) {
		Desc.NumParameters = nParams;
		Desc.NumStaticSamplers = nSamplers;
		pRootParameters = new D3D12_ROOT_PARAMETER[nParams];
		pSamplerDescs = new D3D12_STATIC_SAMPLER_DESC[nSamplers];
		Desc.pParameters = pRootParameters;
		Desc.pStaticSamplers = pSamplerDescs;
		Desc.Flags =
			  D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
			| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
			| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
	}
	~RootSignature() {
		delete[] pRootParameters;
		delete[] pSamplerDescs;
	}
	ID3D12RootSignature* Get() { return ptr.Get(); }
	void CreateRootSignature(ID3D12Device* pd3dDevice) {
		ComPtr<ID3DBlob> pRSBlob{ nullptr };
		ComPtr<ID3DBlob> pErrorBlob{ nullptr };
		D3D12SerializeRootSignature(&Desc, D3D_ROOT_SIGNATURE_VERSION_1, &pRSBlob, &pErrorBlob);
		pd3dDevice->CreateRootSignature(0, pRSBlob->GetBufferPointer(), pRSBlob->GetBufferSize(), IID_PPV_ARGS(&ptr));
	}
	void SetStaticSamplerParam(UINT idx, const D3D12_STATIC_SAMPLER_DESC& SamplerDesc) { pSamplerDescs[idx] = SamplerDesc; }
	void SetRootParam(UINT idx, const D3D12_ROOT_PARAMETER& param) { pRootParameters[idx] = param; }

private:
	ComPtr<ID3D12RootSignature>			ptr;
	D3D12_ROOT_SIGNATURE_DESC			Desc;
	D3D12_ROOT_PARAMETER*				pRootParameters;
	D3D12_STATIC_SAMPLER_DESC*			pSamplerDescs;
};

struct CompiledShader
{
	D3D12_SHADER_BYTECODE vertex;
	D3D12_SHADER_BYTECODE domain;
	D3D12_SHADER_BYTECODE hull;
	D3D12_SHADER_BYTECODE geomery;
	D3D12_SHADER_BYTECODE pixel;
	D3D12_SHADER_BYTECODE compute;
};

struct InputLayout
{
public:
	InputLayout(UINT nElements) : pInputElementDescs{ nullptr } {
		if(nElements > 0)
			pInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nElements];
		InputLayoutDesc.NumElements = nElements;
		InputLayoutDesc.pInputElementDescs = pInputElementDescs;
	}
	~InputLayout() { if(pInputElementDescs) delete[] pInputElementDescs; }
	void SetElement(UINT idx, const D3D12_INPUT_ELEMENT_DESC& element) {
		pInputElementDescs[idx] = element;
	}
	const D3D12_INPUT_LAYOUT_DESC& Get() const { return InputLayoutDesc; }

private:
	D3D12_INPUT_LAYOUT_DESC				InputLayoutDesc;
	D3D12_INPUT_ELEMENT_DESC*			pInputElementDescs;
};

struct GraphicsPipelineStateObject
{
	GraphicsPipelineStateObject() {
		::ZeroMemory(&Desc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	}
	ID3D12PipelineState* Get() { return ptr.Get(); }
	void CreatePSO(ID3D12Device* pd3dDevice) {
		pd3dDevice->CreateGraphicsPipelineState(&Desc, IID_PPV_ARGS(&ptr));
	}
	void SetRootSignature(ID3D12RootSignature* pRS) { Desc.pRootSignature = pRS; }
	void SetCompiledShader(const CompiledShader& CS) {
		Desc.VS = CS.vertex; Desc.PS = CS.pixel; Desc.DS = CS.domain; Desc.HS = CS.hull; Desc.GS = CS.geomery;
	}
	void SetStreamOutputDesc(const D3D12_STREAM_OUTPUT_DESC& desc) { Desc.StreamOutput = desc; }
	void SetBlendDesc(const D3D12_BLEND_DESC& desc) { Desc.BlendState = desc; }
	void SetSampleMask(UINT SampleMask) { Desc.SampleMask = SampleMask; }
	void SetRasterizerDesc(const D3D12_RASTERIZER_DESC& desc) { Desc.RasterizerState = desc; }
	void SetDepthStencilDesc(const D3D12_DEPTH_STENCIL_DESC& desc) { Desc.DepthStencilState = desc; }
	void SetInputLayoutDesc(const D3D12_INPUT_LAYOUT_DESC& desc) { Desc.InputLayout = desc; }
	void SetIBStripCutValue(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE eIBStripCutValue) { Desc.IBStripCutValue = eIBStripCutValue; }
	void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE eTopology) { Desc.PrimitiveTopologyType = eTopology; }
	void SetNumRenderTarget(UINT nRT) { Desc.NumRenderTargets = nRT; }
	void SetRTVFormat(UINT rt_idx, DXGI_FORMAT eRTFormat) { Desc.RTVFormats[rt_idx]= eRTFormat; }
	void SetDSVFormat(DXGI_FORMAT eDSFormat) { Desc.DSVFormat= eDSFormat; }
	void SetSampleDesc(UINT count, UINT quality) { Desc.SampleDesc = { count, quality }; }
	void SetSampleDesc(const DXGI_SAMPLE_DESC& SampleDesc) { Desc.SampleDesc = SampleDesc; }
	void SetNodeMask(UINT mask) { Desc.NodeMask = mask; }
	void SetCachedPSO(const D3D12_CACHED_PIPELINE_STATE& cps) { Desc.CachedPSO = cps; }
	void SetFlags(D3D12_PIPELINE_STATE_FLAGS eFlags) { Desc.Flags = eFlags; }

private:
	ComPtr<ID3D12PipelineState>			ptr;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	Desc;
};