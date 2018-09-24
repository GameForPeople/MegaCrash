//------------------------------------------------------- ----------------------
// File: Mesh.h
//-----------------------------------------------------------------------------

#pragma once
#include "Vertex.h"

class CMesh
{
public:
	CMesh(ID3D12GraphicsCommandList *pd3dCmdLst);
	virtual ~CMesh();

protected:
	D3D12_VERTEX_BUFFER_VIEW		m_d3dVertexBufferView;
	D3D12_INDEX_BUFFER_VIEW			m_d3dIndexBufferView;

	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology;
	UINT							m_nSlot;
	UINT							m_nVertices;
	void*							m_pVertices;
	UINT							m_nStride;
	UINT							m_nOffset;

	UINT*							m_pIndices;
	UINT							m_nIndices;
	UINT							m_nStartIndex;
	INT								m_nBaseVertex;

public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCmdLst);

	const UINT GetnVertices() const { return m_nVertices; }

	void CalculateTriangleListVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, int nVertices);
	void CalculateTriangleListVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, UINT nVertices, UINT *pnIndices, UINT nIndices);
	void CalculateTriangleStripVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, UINT nVertices, UINT *pnIndices, UINT nIndices);
	void CalculateVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, int nVertices, UINT *pnIndices, int nIndices);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CRectMeshTextured : public CMesh
{
public:
	CRectMeshTextured(
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, float							fWidth = 20.0f
		, float							fHeight = 20.0f
		, float							fDepth = 20.0f
		, float							fxPosition = 0.0f
		, float							fyPosition = 0.0f
		, float							fzPosition = 0.0f);
	virtual ~CRectMeshTextured();
};

class CStaticBillBoardMesh : public CMesh
{
private:

public:
	CStaticBillBoardMesh (
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, UINT							nBillboards
		, XMFLOAT3						xmf3CenterPos
		, XMFLOAT2						xmf2Size
		, float							fClusterBillboardsRadius
		, void*							pContext = NULL);
	virtual ~CStaticBillBoardMesh();
};

class CParticleBillBoardMesh : public CMesh
{
public:
	CParticleBillBoardMesh(
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, UINT							nBillboards
		, XMFLOAT2						szBillboards);
	virtual ~CParticleBillBoardMesh();

	CParticleBillboardVertex* GetVertices() { 
		return reinterpret_cast<CParticleBillboardVertex*>(m_pVertices); 
	}
};

class CAdvancedParticleBillBoardMesh : public CMesh
{
public:
	CAdvancedParticleBillBoardMesh(
		ID3D12GraphicsCommandList*	pd3dCmdLst
		, UINT							nBillboards
		, XMFLOAT2						szBillboards);
	virtual ~CAdvancedParticleBillBoardMesh();

	CAdvancedParticleBillboardVertex* GetVertices() {
		return reinterpret_cast<CAdvancedParticleBillboardVertex*>(m_pVertices);
	}
};


class CFontMesh : public CMesh
{
public:
	CFontMesh(
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, UINT							nText
	);
	virtual ~CFontMesh();

	CTextVertex* GetVertices() { return reinterpret_cast<CTextVertex*>(m_pVertices); }
};

class CCubeMesh : public CMesh
{
public:
	CCubeMesh(
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, FBXExporter&					FBXLoader
		, UINT							mesh_idx = 0
		, const float					fScale = 1.0f
		, bool							bSaveToFile = true);
	CCubeMesh(
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, const char*					strFileName
		, const float					fScale = 1.0f);
	virtual ~CCubeMesh();
};

class CModelMesh : public CMesh
{
public:
	CModelMesh(
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, FBXExporter&					FBXLoader
		, UINT							mesh_idx = 0
		, const float					fScale = 1.0f
		, bool							bSaveToFile = true);
	CModelMesh(
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, const char*					strFileName
		, const float					fScale = 1.0f);
	virtual ~CModelMesh();
};

class CAnimationModelMesh : public CMesh
{
public:
	CAnimationModelMesh(
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, FBXExporter&					FBXLoader
		, UINT							mesh_idx = 0
		, const float					fScale = 1.0f
		, bool							bSaveToFile = true);
	CAnimationModelMesh(
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, const char*					strFileName
		, const float					fScale = 1.0f);
	virtual ~CAnimationModelMesh();
};

class CStageMesh : public CMesh
{
public:
	CStageMesh(
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, FBXExporter&					FBXLoader
		, UINT							mesh_idx = 0
		, const float					fScale = 1.0f
		, bool							bSaveToFile = true);
	CStageMesh(
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, const char*					strFileName
		, const float					fScale = 1.0f);
	virtual ~CStageMesh();

	bool CollisionCheck(
		  const XMFLOAT3&				ray_origin
		, const XMFLOAT3&				ray_dir
		, float*						output_t);
};