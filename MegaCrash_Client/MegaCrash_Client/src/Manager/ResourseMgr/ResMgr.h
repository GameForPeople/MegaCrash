#pragma once
#include "Manager/D3DBufMgr/D3DBufMgr.h"
#include "Mesh/Mesh.h"
#include "Animation/AnimationControler.h"
#include "Animation/AnimationState.h"
#include "Shader/Shader.h"
#include "ShadowMap/ShadowMap.h"
#include "Material/Material.h"
#include "Texture/Texture.h"


class CResMgr
{
private:
	CResMgr();

public:
	static CResMgr* const Instance();
	~CResMgr();

	void ConnectDevice(const ComPtr<ID3D12Device>& pd3dDevice);
	void LoadResources(ID3D12GraphicsCommandList* pd3dCmdLst);
	void BuildDescs();
	void CreateRootSignature();
	void Release();

	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(
		  D3D12_RESOURCE_DESC	d3dResourceDesc
		, eTex::Type			eTextureType);
	D3D12_SHADER_BYTECODE CompileShaderFromFile(
		  LPCWSTR				pFileName
		, LPCSTR				pShaderFuncName
		, LPCSTR				pShaderType_ver);
	void CompiledShaderToCacheFile(const CompiledShader& compiled_shader, LPCSTR pFileName);
	CompiledShader CompiledShaderFromCacheFile(LPCSTR pFileName);

	DXGI_SAMPLE_DESC					GetSampleDesc();

	ID3D12RootSignature*				GetRootSignature()								{ return m_pRootSignature->Get(); }
	const shared_ptr<CShadowMap>&		GetShadowMap()									{ return m_pShadowMap; }

	ID3D12Resource*						GetTexture(eTex::Diffuse type)					{ return m_Diffusetexes[type].Get(); }
	ID3D12Resource*						GetTexture(eTex::Normal type)					{ return m_Normaltexes[type].Get(); }
	ID3D12Resource*						GetTexture(eTex::Specular type)					{ return m_Speculartexes[type].Get(); }
	ID3D12Resource*						GetTexture(eTex::Glossiness type)				{ return m_Glossinesstexes[type].Get(); }
	ID3D12Resource*						GetTexture(eTex::Emissive type)					{ return m_Emissivetexes[type].Get(); }
	ID3D12Resource*						GetTexture(eTex::SkyBox type)					{ return m_SkyBoxtexes[type].Get(); }
	ID3D12Resource*						GetTexture(eTex::Billboard type)				{ return m_Billboardtexes[type].Get(); }
	ID3D12Resource*						GetTexture(eTex::Button type)					{ return m_Buttontexes[type].Get(); }
	ID3D12Resource*						GetTexture(eTex::SkillColorName type)			{ return m_SkillColorIcontexes[type].Get(); }
	ID3D12Resource*						GetTexture(eTex::SkillGreyName type)			{ return m_SkillGreyIcontexes[type].Get(); }
	ID3D12Resource*						GetTexture(eTex::Font type)						{ return m_Fonttexes[type].Get(); }

	const shared_ptr<CMesh>&			GetMesh(eGeo::Mesh type)						{ return m_Meshes[type]; }
	const vector<shared_ptr<CStageMesh>>GetStageMeshes()								{ return m_StageMeshes; }
	const shared_ptr<Skeleton>&			GetSkeleton(eGeo::Skel type)					{ return m_Skeletons[type]; }
	const shared_ptr<CAnimation>&		GetAnimation(eGeo::Anim type)					{ return m_Animations[type]; }
	const shared_ptr<CShader>&			GetShader(ePSO::Graphic type)					{ return m_Shaders[type]; }
	const shared_ptr<CTexture>&			GetTexture(eMat::Object type)					{ return m_Textures[type]; }
	const shared_ptr<CMaterial>&		GetMaterial(eMat::Object type)					{ return m_Materials[type]; }
	const CompiledShader&				GetCompiledShader(ePSO::Graphic type)			{ return m_CompiledShaders[type]; }
	const D3D12_INPUT_LAYOUT_DESC&		GetInputLayoutDesc(eDESC::InputLayout type)		{ return m_InputLayoutDescs[type]->Get(); }
	const D3D12_RASTERIZER_DESC&		GetRasterizerDesc(eDESC::Rasterizer type)		{ return m_RasterizerDescs[type]; }
	const D3D12_BLEND_DESC&				GetBlendDesc(eDESC::Blend type)					{ return m_BlendDescs[type]; }
	const D3D12_DEPTH_STENCIL_DESC&		GetDepthStencilDesc(eDESC::DepthStencil type)	{ return m_DepthStencilDescs[type]; }
	const D3D12_STATIC_SAMPLER_DESC&	GetStaticSampler(eDESC::StaticSampler type)		{ return m_SamplerDescs[type]; }

	shared_ptr<CAnimCtrlr>				GetAnimCtrlr(eGeo::AnimCtrlr type);

private:
	ComPtr<ID3D12Device>											m_pd3dDevice;
	bool															m_bMsaa4xEnable;
	UINT															m_nMsaa4xQualityLevels;

	shared_ptr<RootSignature>										m_pRootSignature;
	shared_ptr<CShadowMap>											m_pShadowMap;

	unordered_map<eTex::Diffuse, ComPtr<ID3D12Resource>>			m_Diffusetexes;
	unordered_map<eTex::Normal, ComPtr<ID3D12Resource>>				m_Normaltexes;
	unordered_map<eTex::Specular, ComPtr<ID3D12Resource>>			m_Speculartexes;	
	unordered_map<eTex::Glossiness, ComPtr<ID3D12Resource>>			m_Glossinesstexes;
	unordered_map<eTex::Emissive, ComPtr<ID3D12Resource>>			m_Emissivetexes;	

	unordered_map<eTex::SkyBox, ComPtr<ID3D12Resource>>				m_SkyBoxtexes;
	unordered_map<eTex::Billboard, ComPtr<ID3D12Resource>>			m_Billboardtexes;
	unordered_map<eTex::Button, ComPtr<ID3D12Resource>>				m_Buttontexes;

	unordered_map<eTex::SkillColorName, ComPtr<ID3D12Resource>>		m_SkillColorIcontexes;
	unordered_map<eTex::SkillGreyName, ComPtr<ID3D12Resource>>		m_SkillGreyIcontexes;
	unordered_map<eTex::Font, ComPtr<ID3D12Resource>>				m_Fonttexes;

	unordered_map<eGeo::Mesh, shared_ptr<CMesh>>					m_Meshes;
	vector<shared_ptr<CStageMesh>>									m_StageMeshes;
	unordered_map<eGeo::Skel, shared_ptr<Skeleton>>					m_Skeletons;
	unordered_map<eGeo::Anim, shared_ptr<CAnimation>>				m_Animations;
	unordered_map<ePSO::Graphic, shared_ptr<CShader>>				m_Shaders;
	unordered_map<eMat::Object, shared_ptr<CTexture>>				m_Textures;
	unordered_map<eMat::Object, shared_ptr<CMaterial>>				m_Materials;

	list<ComPtr<ID3DBlob>>											m_CompiledShaderBlobs;
	list<CompiledShader>											m_CacheShaders;
	unordered_map<ePSO::Graphic, CompiledShader>					m_CompiledShaders;

	unordered_map<eDESC::InputLayout, shared_ptr<InputLayout>>		m_InputLayoutDescs;
	unordered_map<eDESC::Rasterizer, D3D12_RASTERIZER_DESC>			m_RasterizerDescs;
	unordered_map<eDESC::Blend, D3D12_BLEND_DESC>					m_BlendDescs;
	unordered_map<eDESC::DepthStencil, D3D12_DEPTH_STENCIL_DESC>	m_DepthStencilDescs;
	unordered_map<eDESC::StaticSampler, D3D12_STATIC_SAMPLER_DESC>	m_SamplerDescs;
};