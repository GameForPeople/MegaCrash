#pragma once

class CVertex
{
public:
	XMFLOAT3						xmf3Position;

public:
	CVertex()
		: xmf3Position(XMFLOAT3(0.0f, 0.0f, 0.0f)){}
	CVertex(const XMFLOAT3& xmf3Pos)
		: xmf3Position(xmf3Pos) {}
	~CVertex() { }
};

class CMesh
{
public:
	CMesh();
	virtual ~CMesh();
	
private:
	int m_nReferences;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

protected:
	UINT						m_nSlot;
	UINT						m_nVertices;
	UINT						m_nStride;
	UINT						m_nOffset;

	UINT						m_nIndices = 0;
	UINT						m_nStartIndex = 0;
	int							m_nBaseVertex = 0;


	CVertex						*m_pVertices = NULL;
	UINT						*m_pnIndices = NULL;

public:
	CVertex*	GetVertices() { return m_pVertices; }
	UINT		GetnVertices() { return m_nVertices; }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CModelMesh : public CMesh
{
public:
	PxTriangleMesh*		m_pTrimesh = NULL;
	PxConvexMesh*		m_pConvexmesh = NULL;
	bool				m_isBreakable = false;

	CModelMesh(
		PxPhysics* physics
		, PxCooking* cooking
		, const char* filepath
		, const float fSizeMeter = 1.0f
	);

	CModelMesh(
		PxPhysics* physics
		, PxCooking* cooking
		, FILE* filepointer
		, const float fSizeMeter = 1.0f
	);

	virtual ~CModelMesh();
};

class CSkinnedAnimationVertex
{
public:
	XMFLOAT3						m_xmf3Pos;
	XMFLOAT3						m_xmf3Normal;
	XMFLOAT3						m_xmf3Tangent;
	XMFLOAT2						m_xmf2TexCoord;
	XMFLOAT3						m_xmf3BoneWeights;
	BYTE							m_iBoneIndices[4];
	BYTE							m_iMaterialIdx;

public:
	CSkinnedAnimationVertex()
		: m_xmf3Pos(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf3Normal(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf3Tangent(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf2TexCoord(XMFLOAT2(0.0f, 0.0f))
		, m_xmf3BoneWeights(XMFLOAT3())
		, m_iMaterialIdx(0)
	{
		::ZeroMemory(m_iBoneIndices, sizeof(BYTE) * 4);
	}
	CSkinnedAnimationVertex(
		const XMFLOAT3& xmf3Pos
		, const XMFLOAT3& xmf3Normal
		, const XMFLOAT3& xmf3Tangent
		, const XMFLOAT2& xmf2TexCoord
		, const XMFLOAT3& xmf3BoneWeight
		, const BYTE boneidx0
		, const BYTE boneidx1
		, const BYTE boneidx2
		, const BYTE boneidx3
		, const BYTE materialidx)
		: m_xmf3Pos(xmf3Pos)
		, m_xmf3Normal(xmf3Normal)
		, m_xmf3Tangent(xmf3Tangent)
		, m_xmf2TexCoord(xmf2TexCoord)
		, m_xmf3BoneWeights(xmf3BoneWeight)
		, m_iMaterialIdx(materialidx)
	{
		m_iBoneIndices[0] = boneidx0;
		m_iBoneIndices[1] = boneidx1;
		m_iBoneIndices[2] = boneidx2;
		m_iBoneIndices[3] = boneidx3;
	}
	~CSkinnedAnimationVertex() {}
};

class CAnimationModelMesh : public CMesh
{

public:
	CSkinnedAnimationVertex*	pVerts = NULL;
	UINT*						pIdx = NULL;

	CAnimationModelMesh(
		const char*					strFileName
		, const float					fScale = 1.0f);
	CAnimationModelMesh(
		FBXExporter&					FBXLoader
		, UINT							mesh_idx = 0
		, const float					fScale = 1.0f
		, bool							bSaveToFile = true);
	~CAnimationModelMesh() { 
		if (pVerts) delete[] pVerts;
		if (pIdx) delete[] pIdx;
	}
};