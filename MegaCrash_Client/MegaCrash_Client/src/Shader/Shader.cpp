///-----------------------------------------------------------------------------
// File: Shader.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Shader.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSkyBoxShader::CSkyBoxShader(ID3D12Device* pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::SkyBox));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::SkyBox));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
CCubeShader::CCubeShader(ID3D12Device* pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::Cube));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::Default));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestDefalut));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::Cube));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CStageShader::CStageShader(ID3D12Device* pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::Stage));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::Default));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestDefalut));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::Stage));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CModelShader::CModelShader(ID3D12Device* pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::Model));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::Default));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestDefalut));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::Model));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAnimationShader::CAnimationShader(ID3D12Device* pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::SkinnedAnim));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::Default));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestDefalut));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::SkinnedAnim));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSkyBoxDeferredShader::CSkyBoxDeferredShader(ID3D12Device* pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::DeferredSkyBox));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::SkyBox));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(6);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(1, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(2, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(3, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(5, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
CCubeDeferredShader::CCubeDeferredShader(ID3D12Device* pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::DeferredCube));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::Default));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestDefalut));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::Cube));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(6);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(1, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(2, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(3, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(5, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CStageDeferredShader::CStageDeferredShader(ID3D12Device* pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::DeferredStage));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::Default));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestDefalut));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::Stage));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(6);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(1, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(2, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(3, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(5, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CModelDeferredShader::CModelDeferredShader(ID3D12Device* pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::DeferredModel));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::Default));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestDefalut));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::Model));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(6);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(1, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(2, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(3, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(5, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAnimationDeferredShader::CAnimationDeferredShader(ID3D12Device* pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::DeferredSkinnedAnim));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::Default));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestDefalut));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::SkinnedAnim));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(6);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(1, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(2, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(3, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetRTVFormat(5, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDeferredLightingShader::CDeferredLightingShader(ID3D12Device* pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::DeferredLighting));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Alpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

void CDeferredLightingShader::Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	static ID3D12RootSignature* pRootSignature = CResMgr::Instance()->GetRootSignature();
	pd3dCmdLst->SetGraphicsRootSignature(pRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCmdLst);

	pd3dCmdLst->SetPipelineState(m_pPipelineState->Get());

	m_pTexture->UpdateShaderVariables(pd3dCmdLst);

	pd3dCmdLst->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCmdLst->DrawInstanced(6, 1, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CBlendRenderTargetShader::CBlendRenderTargetShader(ID3D12Device* pd3dDevice) 
	: CShader(pd3dDevice)
	, m_pTexture{ nullptr }
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::FullScreen));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Alpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

void CBlendRenderTargetShader::Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	static ID3D12RootSignature* pRootSignature = CResMgr::Instance()->GetRootSignature();
	pd3dCmdLst->SetGraphicsRootSignature(pRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCmdLst);

	pd3dCmdLst->SetPipelineState(m_pPipelineState->Get());

	m_pTexture->UpdateShaderVariable(pd3dCmdLst, 0, m_RT_IDX);

	pd3dCmdLst->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCmdLst->DrawInstanced(6, 1, 0, 0);
}

/////////////////////////////////////////////////////////////////////
CHUDUIShader::CHUDUIShader(ID3D12Device * pd3dDevice) 
	: CShader(pd3dDevice)
	, m_pTexture{ nullptr }
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::HUD_UI));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);

	m_pTexture = new CTexture(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
	m_pTexture->CreateTextureSet(0, 1, ROOT_PARAMETER_RESOURCE1);
	m_pTexture->SetTexture(pd3dDevice, 0, 0, CResMgr::Instance()->GetTexture(eTex::Billboard::HUD_UI));
	//m_pTexture->SetTexture(pd3dDevice, 0, 0, CResMgr::Instance()->GetShadowMap()->GetDSBuf(), DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
}

void CHUDUIShader::Render(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	static ID3D12RootSignature* pRootSignature = CResMgr::Instance()->GetRootSignature();
	pd3dCmdLst->SetGraphicsRootSignature(pRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCmdLst);

	pd3dCmdLst->SetPipelineState(m_pPipelineState->Get());

	m_pTexture->UpdateShaderVariables(pd3dCmdLst);

	pd3dCmdLst->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCmdLst->DrawInstanced(6, 1, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CRaderUIShader::CRaderUIShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::RADER_UI));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);

	m_pTexture = new CTexture(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
	m_pTexture->CreateTextureSet(0, 1, ROOT_PARAMETER_RESOURCE1);
	m_pTexture->SetTexture(pd3dDevice, 0, 0, CResMgr::Instance()->GetTexture(eTex::Billboard::RADER_UI));
}
void CRaderUIShader::Render(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	static ID3D12RootSignature* pRootSignature = CResMgr::Instance()->GetRootSignature();
	pd3dCmdLst->SetGraphicsRootSignature(pRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCmdLst);

	pd3dCmdLst->SetPipelineState(m_pPipelineState->Get());

	m_pTexture->UpdateShaderVariables(pd3dCmdLst);

	pd3dCmdLst->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCmdLst->DrawInstanced(6, 1, 0, 0);
}

/////////////////////////////////////////////////////////////////////
CSkillIconShader::CSkillIconShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::Skill_Icon));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CCrossHairShader::CCrossHairShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::Crosshair));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CFontShader::CFontShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::Font));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Font));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::Font));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CLogoShader::CLogoShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::Logo));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CTitleShader::CTitleShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::Title));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CLobbyWindowShader::CLobbyWindowShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::LobbyWindow));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CLobbyRoomSlotShader::CLobbyRoomSlotShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::LobbyRoomSlot));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CButtonShader::CButtonShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::Button));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CGameoverShader::CGameoverShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::Gameover));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CDeathUIShader::CDeathUIShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::DeathUI));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::None));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPlayerUIShader::CPlayerUIShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::PlayerUI));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::Default));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::PlayerUI));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CParticleBillboardShader::CParticleBillboardShader(ID3D12Device* pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::Billboard));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::UIAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::BothDisable));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::Billboard));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CAdvancedParticleAdditiveBillboardShader::CAdvancedParticleAdditiveBillboardShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::ParticleBillboardAdditive));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Additive));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestNoWriteDepth));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::ParticleBillboard));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(1, 0);
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CAdvancedParticleAlphaBlendBillboardShader::CAdvancedParticleAlphaBlendBillboardShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::ParticleBillboardDefault));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::CullModeNone));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::ParticleAlpha));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestNoWriteDepth));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::ParticleBillboard));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
	m_pPipelineState->SetNumRenderTarget(1);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(1, 0);
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CStageShadowMapShader::CStageShadowMapShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::ShadowMapStage));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::ShadowMap));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestDefalut));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::Stage));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(0);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_UNKNOWN);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CCubeShadowMapShader::CCubeShadowMapShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::ShadowMapCube));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::ShadowMap));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestDefalut));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::Cube));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(0);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_UNKNOWN);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CModelShadowMapShader::CModelShadowMapShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::ShadowMapModel));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::ShadowMap));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestDefalut));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::Model));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(0);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_UNKNOWN);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}

/////////////////////////////////////////////////////////////////////
CAnimShadowMapShader::CAnimShadowMapShader(ID3D12Device * pd3dDevice) : CShader(pd3dDevice)
{
	auto pRes = CResMgr::Instance();
	m_pPipelineState = make_shared<GraphicsPipelineStateObject>();
	m_pPipelineState->SetRootSignature(pRes->GetRootSignature());
	m_pPipelineState->SetCompiledShader(pRes->GetCompiledShader(ePSO::Graphic::ShadowMapSkinnedAnim));
	m_pPipelineState->SetRasterizerDesc(pRes->GetRasterizerDesc(eDESC::Rasterizer::ShadowMap));
	m_pPipelineState->SetBlendDesc(pRes->GetBlendDesc(eDESC::Blend::Default));
	m_pPipelineState->SetDepthStencilDesc(pRes->GetDepthStencilDesc(eDESC::DepthStencil::DepthTestDefalut));
	m_pPipelineState->SetInputLayoutDesc(pRes->GetInputLayoutDesc(eDESC::InputLayout::SkinnedAnim));
	m_pPipelineState->SetSampleMask(UINT_MAX);
	m_pPipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_pPipelineState->SetNumRenderTarget(0);
	m_pPipelineState->SetRTVFormat(0, DXGI_FORMAT_UNKNOWN);
	m_pPipelineState->SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPipelineState->SetSampleDesc(pRes->GetSampleDesc());
	m_pPipelineState->SetFlags(D3D12_PIPELINE_STATE_FLAG_NONE);
	m_pPipelineState->CreatePSO(pd3dDevice);
}
