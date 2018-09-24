//-----------------------------------------------------------------------------
// File: CMesh.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Mesh.h"
#include "Object\Object.h"

CMesh::CMesh(ID3D12GraphicsCommandList *pd3dCmdLst)
	: m_d3dPrimitiveTopology	{ D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST }
	, m_nSlot					{ 0 }
	, m_nVertices				{ 0 }
	, m_pVertices				{ nullptr }
	, m_nStride					{ 0 }
	, m_nOffset					{ 0 }
	, m_pIndices				{ nullptr }
	, m_nIndices				{ 0 }
	, m_nStartIndex				{ 0 }
	, m_nBaseVertex				{ 0 }
{
	ZeroMemory(&m_d3dVertexBufferView, sizeof(m_d3dVertexBufferView));
	ZeroMemory(&m_d3dIndexBufferView, sizeof(m_d3dIndexBufferView));
}

CMesh::~CMesh()
{
	if (m_pIndices) delete[] m_pIndices;
}

void CMesh::Render(ID3D12GraphicsCommandList *pd3dCmdLst)
{
	pd3dCmdLst->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dCmdLst->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView);

	if (m_d3dIndexBufferView.SizeInBytes) {
		pd3dCmdLst->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCmdLst->DrawIndexedInstanced(m_nIndices, 1, m_nStartIndex, m_nBaseVertex, 0);
	}
	else {
		pd3dCmdLst->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}

void CMesh::CalculateTriangleListVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, int nVertices)
{
	int nPrimitives = nVertices / 3;
	UINT nIndex0, nIndex1, nIndex2;
	for (int i = 0; i < nPrimitives; i++)
	{
		nIndex0 = i*3+0;
		nIndex1 = i*3+1;
		nIndex2 = i*3+2;
		XMFLOAT3 xmf3Edge01 = Vector3::Subtract(pxmf3Positions[nIndex1], pxmf3Positions[nIndex0]);
		XMFLOAT3 xmf3Edge02 = Vector3::Subtract(pxmf3Positions[nIndex2], pxmf3Positions[nIndex0]);
		pxmf3Normals[nIndex0] = pxmf3Normals[nIndex1] = pxmf3Normals[nIndex2] = Vector3::CrossProduct(xmf3Edge01, xmf3Edge02, true);
	}
}

void CMesh::CalculateTriangleListVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, UINT nVertices, UINT *pnIndices, UINT nIndices)
{
	UINT nPrimitives = (pnIndices) ? (nIndices / 3) : (nVertices / 3);
	XMFLOAT3 xmf3SumOfNormal, xmf3Edge01, xmf3Edge02, xmf3Normal;
	UINT nIndex0, nIndex1, nIndex2;
	for (UINT j = 0; j < nVertices; j++)
	{
		xmf3SumOfNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		for (UINT i = 0; i < nPrimitives; i++)
		{
			nIndex0 = pnIndices[i*3+0];
			nIndex1 = pnIndices[i*3+1];
			nIndex2 = pnIndices[i*3+2];
			if (pnIndices && ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)))
			{
				xmf3Edge01 = Vector3::Subtract(pxmf3Positions[nIndex1], pxmf3Positions[nIndex0]);
				xmf3Edge02 = Vector3::Subtract(pxmf3Positions[nIndex2], pxmf3Positions[nIndex0]);
				xmf3Normal = Vector3::CrossProduct(xmf3Edge01, xmf3Edge02, false);
				xmf3SumOfNormal = Vector3::Add(xmf3SumOfNormal, xmf3Normal);
			}
		}
		pxmf3Normals[j] = Vector3::Normalize(xmf3SumOfNormal);
	}
}

void CMesh::CalculateTriangleStripVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, UINT nVertices, UINT *pnIndices, UINT nIndices)
{
	UINT nPrimitives = (pnIndices) ? (nIndices - 2) : (nVertices - 2);
	XMFLOAT3 xmf3SumOfNormal(0.0f, 0.0f, 0.0f);
	UINT nIndex0, nIndex1, nIndex2;
	for (UINT j = 0; j < nVertices; j++)
	{
		xmf3SumOfNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		for (UINT i = 0; i < nPrimitives; i++)
		{
			nIndex0 = ((i % 2) == 0) ? (i + 0) : (i + 1);
			if (pnIndices) nIndex0 = pnIndices[nIndex0];
			nIndex1 = ((i % 2) == 0) ? (i + 1) : (i + 0);
			if (pnIndices) nIndex1 = pnIndices[nIndex1];
			nIndex2 = (pnIndices) ? pnIndices[i + 2] : (i + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j))
			{
				XMFLOAT3 xmf3Edge01 = Vector3::Subtract(pxmf3Positions[nIndex1], pxmf3Positions[nIndex0]);
				XMFLOAT3 xmf3Edge02 = Vector3::Subtract(pxmf3Positions[nIndex2], pxmf3Positions[nIndex0]);
				XMFLOAT3 xmf3Normal = Vector3::CrossProduct(xmf3Edge01, xmf3Edge02, true);
				xmf3SumOfNormal = Vector3::Add(xmf3SumOfNormal, xmf3Normal);
			}
		}
		pxmf3Normals[j] = Vector3::Normalize(xmf3SumOfNormal);
	}
}

void CMesh::CalculateVertexNormals(XMFLOAT3 *pxmf3Normals, XMFLOAT3 *pxmf3Positions, int nVertices, UINT *pnIndices, int nIndices)
{
	switch (m_d3dPrimitiveTopology)
	{
		case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
			if (pnIndices)
				CalculateTriangleListVertexNormals(pxmf3Normals, pxmf3Positions, nVertices, pnIndices, nIndices);
			else
				CalculateTriangleListVertexNormals(pxmf3Normals, pxmf3Positions, nVertices);
			break;
		case D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
			CalculateTriangleStripVertexNormals(pxmf3Normals, pxmf3Positions, nVertices, pnIndices, nIndices);
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
CRectMeshTextured::CRectMeshTextured(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, float							fWidth
	, float							fHeight
	, float							fDepth
	, float							fxPosition
	, float							fyPosition
	, float							fzPosition)
	: CMesh(pd3dCmdLst)
{
	m_nVertices = 6;
	m_nStride = sizeof(CTexturedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CTexturedVertex pVertices[6];

	float fx = (fWidth * 0.5f) + fxPosition
		, fy = (fHeight * 0.5f) + fyPosition,
		fz = (fDepth * 0.5f) + fzPosition;

	if (fWidth == 0.0f)
	{
		if (fxPosition > 0.0f)
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(fx, +fy, -fz), XMFLOAT2(1.0f, 0.0f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(fx, -fy, -fz), XMFLOAT2(1.0f, 1.0f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(fx, -fy, +fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(fx, -fy, +fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(fx, +fy, +fz), XMFLOAT2(0.0f, 0.0f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(fx, +fy, -fz), XMFLOAT2(1.0f, 0.0f));
		}
		else
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(fx, +fy, +fz), XMFLOAT2(1.0f, 0.0f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(fx, -fy, +fz), XMFLOAT2(1.0f, 1.0f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(fx, -fy, -fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(fx, -fy, -fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(fx, +fy, -fz), XMFLOAT2(0.0f, 0.0f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(fx, +fy, +fz), XMFLOAT2(1.0f, 0.0f));
		}
	}
	else if (fHeight == 0.0f)
	{
		if (fyPosition > 0.0f)
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(+fx, fy, -fz), XMFLOAT2(1.0f, 0.0f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(+fx, fy, +fz), XMFLOAT2(1.0f, 1.0f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(-fx, fy, +fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(-fx, fy, +fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(-fx, fy, -fz), XMFLOAT2(0.0f, 0.0f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(+fx, fy, -fz), XMFLOAT2(1.0f, 0.0f));
		}
		else
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(+fx, fy, +fz), XMFLOAT2(1.0f, 0.0f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(+fx, fy, -fz), XMFLOAT2(1.0f, 1.0f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(-fx, fy, -fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(-fx, fy, -fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(-fx, fy, +fz), XMFLOAT2(0.0f, 0.0f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(+fx, fy, +fz), XMFLOAT2(1.0f, 0.0f));
		}
	}
	else if (fDepth == 0.0f)
	{
		if (fzPosition > 0.0f)
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(1.0f, 0.0f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(1.0f, 1.0f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(0.0f, 0.0f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(1.0f, 0.0f));
		}
		else
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(1.0f, 0.0f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(1.0f, 1.0f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(0.0f, 0.0f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(1.0f, 0.0f));
		}
	}
	CD3DBufMgr::Instance()->AllocVertexBuffer(
		  pd3dCmdLst
		, m_d3dVertexBufferView
		, pVertices
		, m_nStride * m_nVertices
		, m_nStride);
}
CRectMeshTextured::~CRectMeshTextured()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
CStaticBillBoardMesh::CStaticBillBoardMesh(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, UINT							nBillboards
	, XMFLOAT3						xmf3CenterPos
	, XMFLOAT2						xmf2Size
	, float							fClusterBillboardsRadius
	, void*							pContext)
	: CMesh(pd3dCmdLst)
{
	m_nVertices = nBillboards;
	m_nStride = sizeof(CBillboardVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	//CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;
	//XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	//int HeightMapWidth = pTerrain->GetHeightMapWidth() * xmf3Scale.x;
	//int HeightMapLength = pTerrain->GetHeightMapLength() * xmf3Scale.z;
	//
	//CBillboardVertex* pVertices = new CBillboardVertex[m_nVertices];
	//UINT idx = 0;
	//const int randomFactor = 10000;
	//const float LeanFactorThreshold = 0.95f;
	//const XMFLOAT2 fHalfScale = XMFLOAT2(xmf2Size.x * 0.5f, xmf2Size.y * 0.5f);
	//while (idx < m_nVertices)
	//{
	//	float fRadiusRandomFactor = 
	//		static_cast<float>(rand() % randomFactor) / static_cast<float>(randomFactor);
	//	XMFLOAT3 xmf3OffSetNormal = Vector3::Normalize(XMFLOAT3(
	//		  (rand() % randomFactor) - (randomFactor / 2)
	//		, 0
	//		, (rand() % randomFactor) - (randomFactor / 2)));
	//	XMFLOAT3 xmf3Pos = Vector3::Add(xmf3CenterPos, Vector3::ScalarProduct(
	//		xmf3OffSetNormal, fRadiusRandomFactor * fClusterBillboardsRadius));
	//	if (xmf3Pos.x < fHalfScale.x || xmf3Pos.x > HeightMapWidth -fHalfScale.x) continue;
	//	if (xmf3Pos.z < fHalfScale.x || xmf3Pos.z > HeightMapLength-fHalfScale.x) continue;
	//	
	//	XMFLOAT3 xmf3TerrainNormal = 
	//		pTerrain->GetHeightMapImage()->GetHeightMapNormal(xmf3Pos.x, xmf3Pos.z);
	//
	//	float fLeanFactor = Vector3::DotProduct(xmf3TerrainNormal, XMFLOAT3(0, 1, 0));
	//	if(fLeanFactor < 0) fLeanFactor = -fLeanFactor;
	//
	//	if (fLeanFactor > LeanFactorThreshold)
	//	{
	//		xmf3Pos.y = pTerrain->GetHeight(xmf3Pos.x, xmf3Pos.z) + fHalfScale.y;
	//		pVertices[idx] = CBillboardVertex(xmf3Pos, xmf2Size);
	//		++idx;
	//	}
	//}
	//
	//m_pd3dVertexBuffer = ::CreateBufferResource(
	//	pd3dDevice
	//	, pd3dCmdLst
	//	, pVertices
	//	, m_nStride * m_nVertices
	//	, D3D12_HEAP_TYPE_DEFAULT
	//	, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
	//	, &m_pd3dVertexUploadBuffer);
	//
	//m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	//m_d3dVertexBufferView.StrideInBytes = m_nStride;
	//m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;

	//m_pVertices = pVertices;
}

CStaticBillBoardMesh::~CStaticBillBoardMesh()
{
	//if (m_pVertices) delete[] reinterpret_cast<CBillboardVertex*>(m_pVertices);
}
/////////////////////////////////////////////////////////////////////////////////////

CParticleBillBoardMesh::CParticleBillBoardMesh(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, UINT							nBillboards
	, XMFLOAT2						szBillboards)
	: CMesh(pd3dCmdLst)
{
	m_nVertices = nBillboards;
	m_nStride = sizeof(CParticleBillboardVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	CParticleBillboardVertex* pVertices = new CParticleBillboardVertex[m_nVertices];
	for (int i = 0; i < m_nVertices; ++i)
		pVertices[i].m_xmf2Size = szBillboards;

	CD3DBufMgr::Instance()->AllocUploadVertexBuffer(
		  pd3dCmdLst
		, m_d3dVertexBufferView
		, pVertices
		, m_nStride * m_nVertices
		, m_nStride
		, reinterpret_cast<LPVOID*>(&m_pVertices));

	delete[] pVertices;
}

CParticleBillBoardMesh::~CParticleBillBoardMesh()
{
	//if (m_pVertices) delete[] reinterpret_cast<CParticleBillboardVertex*>(m_pVertices);
}
/////////////////////////////////////////////////////////////////////////////////////

CFontMesh::CFontMesh(ID3D12GraphicsCommandList * pd3dCmdLst, UINT nTexts) : CMesh(pd3dCmdLst)
{
	m_nVertices = nTexts;
	m_nStride = sizeof(CTextVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	CD3DBufMgr::Instance()->AllocUploadVertexBuffer(
		pd3dCmdLst
		, m_d3dVertexBufferView
		, nullptr
		, m_nStride * m_nVertices
		, m_nStride
		, reinterpret_cast<LPVOID*>(&m_pVertices));
}

CFontMesh::~CFontMesh()
{
	//if (m_pVertices) delete[] reinterpret_cast<CTextVertex*>(m_pVertices);
}
/////////////////////////////////////////////////////////////////////////////////////

CCubeMesh::CCubeMesh(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, FBXExporter&					FBXLoader
	, UINT							mesh_idx
	, const float					fScale
	, bool							bSaveToFile)
	: CMesh(pd3dCmdLst) 
{
	m_nVertices = FBXLoader.mOutputDataContainer[0].mVertices.size();
	m_nStride = sizeof(CCubeVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nIndices = FBXLoader.mOutputDataContainer[0].mTriangles.size() * 3;
	m_pIndices = new UINT[m_nIndices];

	int i = 0;
	for (auto& p : FBXLoader.mOutputDataContainer[0].mTriangles)
	{
		m_pIndices[i++] = p.mIndices[0];
		m_pIndices[i++] = p.mIndices[1];
		m_pIndices[i++] = p.mIndices[2];
	}

	CD3DBufMgr::Instance()->AllocIndexBuffer(
		  pd3dCmdLst
		, m_d3dIndexBufferView
		, m_pIndices
		, sizeof(UINT) * m_nIndices);

	CCubeVertex* pVertices = new CCubeVertex[m_nVertices];
	i = 0;
	for (auto& p : FBXLoader.mOutputDataContainer[0].mVertices)
	{
		pVertices[i].m_xmf3Pos = Vector3::ScalarProduct(p.mPosition, fScale, false);
		pVertices[i].m_xmf3Normal = p.mNormal;
		++i;
	}

	CD3DBufMgr::Instance()->AllocVertexBuffer(
		  pd3dCmdLst
		, m_d3dVertexBufferView
		, pVertices
		, m_nStride * m_nVertices
		, m_nStride);

	if (bSaveToFile)
	{
		std::string file_name = "./Assets/Meshes/" + FBXLoader.mGenericFileName + ".mbox";
		FILE* fp = nullptr;
		fopen_s(&fp, file_name.c_str(), "wb");
		fwrite(&m_nVertices, sizeof(UINT), 1, fp);
		fwrite(pVertices, m_nStride, m_nVertices, fp);
		fwrite(&m_nIndices, sizeof(UINT), 1, fp);
		fwrite(m_pIndices, sizeof(UINT), m_nIndices, fp);
		fclose(fp);
	}

	m_pVertices = pVertices;
}
CCubeMesh::CCubeMesh(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, const char*					strFileName
	, const float					fScale)
	: CMesh(pd3dCmdLst)
{
	m_nStride = sizeof(CCubeVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	FILE* fp = nullptr;
	fopen_s(&fp, strFileName, "rb");
	fread_s(&m_nVertices, sizeof(UINT), sizeof(UINT), 1, fp);
	CCubeVertex* pVertices = new CCubeVertex[m_nVertices];
	fread_s(pVertices, m_nStride * m_nVertices, m_nStride, m_nVertices, fp);

	for (int i = 0; i < m_nVertices; ++i)
		pVertices[i].m_xmf3Pos = Vector3::ScalarProduct(pVertices[i].m_xmf3Pos, fScale, false);

	fread_s(&m_nIndices, sizeof(UINT), sizeof(UINT), 1, fp);
	m_pIndices = new UINT[m_nIndices];
	fread_s(m_pIndices, sizeof(UINT) * m_nIndices, sizeof(UINT), m_nIndices, fp);
	fclose(fp);

	CD3DBufMgr::Instance()->AllocVertexBuffer(
		  pd3dCmdLst
		, m_d3dVertexBufferView
		, pVertices
		, m_nStride * m_nVertices
		, m_nStride);

	CD3DBufMgr::Instance()->AllocIndexBuffer(
		  pd3dCmdLst
		, m_d3dIndexBufferView
		, m_pIndices
		, sizeof(UINT) * m_nIndices);

	m_pVertices = pVertices;
}
CCubeMesh::~CCubeMesh()
{
	if (m_pVertices) delete[] reinterpret_cast<CCubeVertex*>(m_pVertices);
}
/////////////////////////////////////////////////////////////////////////////////////

CModelMesh::CModelMesh(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, FBXExporter&					FBXLoader
	, UINT							mesh_idx
	, const float					fScale
	, bool							bSaveToFile)
	: CMesh(pd3dCmdLst)
{
	m_nVertices = FBXLoader.mOutputDataContainer[mesh_idx].mVertices.size();
	m_nStride = sizeof(CModelVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nIndices = FBXLoader.mOutputDataContainer[mesh_idx].mTriangles.size() * 3;
	m_pIndices = new UINT[m_nIndices];

	int i = 0;
	for (auto& p : FBXLoader.mOutputDataContainer[mesh_idx].mTriangles)
	{
		m_pIndices[i++] = p.mIndices[0];
		m_pIndices[i++] = p.mIndices[1];
		m_pIndices[i++] = p.mIndices[2];
	}

	CD3DBufMgr::Instance()->AllocIndexBuffer(
		  pd3dCmdLst
		, m_d3dIndexBufferView
		, m_pIndices
		, sizeof(UINT) * m_nIndices);

	CModelVertex* pVertices = new CModelVertex[m_nVertices];
	i = 0;
	for (auto& p : FBXLoader.mOutputDataContainer[mesh_idx].mVertices)
	{
		pVertices[i].m_xmf3Pos = Vector3::ScalarProduct(p.mPosition, fScale, false);
		pVertices[i].m_xmf3Normal = p.mNormal;
		pVertices[i].m_xmf3Tangent = p.mTangent;
		p.mUV.y = 1.f - p.mUV.y;
		pVertices[i].m_xmf2TexCoord = p.mUV;
		++i;
	}

	CD3DBufMgr::Instance()->AllocVertexBuffer(
		  pd3dCmdLst
		, m_d3dVertexBufferView
		, pVertices
		, m_nStride * m_nVertices
		, m_nStride);

	if (bSaveToFile)
	{
		std::string file_name = "./Assets/Meshes/" + FBXLoader.mGenericFileName + std::to_string(mesh_idx) + ".mbox";
		FILE* fp = nullptr;
		fopen_s(&fp, file_name.c_str(), "wb");
		fwrite(&m_nVertices, sizeof(UINT), 1, fp);
		fwrite(pVertices, m_nStride, m_nVertices, fp);
		fwrite(&m_nIndices, sizeof(UINT), 1, fp);
		fwrite(m_pIndices, sizeof(UINT), m_nIndices, fp);
		fclose(fp);
	}

	m_pVertices = pVertices;
}

CModelMesh::CModelMesh(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, const char*					strFileName
	, const float					fScale)
	: CMesh(pd3dCmdLst) 
{
	m_nStride = sizeof(CModelVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	FILE* fp = nullptr;
	fopen_s(&fp, strFileName, "rb");
	fread_s(&m_nVertices, sizeof(UINT), sizeof(UINT), 1, fp);
	CModelVertex* pVertices = new CModelVertex[m_nVertices];
	fread_s(pVertices, m_nStride * m_nVertices, m_nStride, m_nVertices, fp);

	for (int i = 0; i < m_nVertices; ++i)
		pVertices[i].m_xmf3Pos = Vector3::ScalarProduct(pVertices[i].m_xmf3Pos, fScale, false);

	fread_s(&m_nIndices, sizeof(UINT), sizeof(UINT), 1, fp);
	m_pIndices = new UINT[m_nIndices];
	fread_s(m_pIndices, sizeof(UINT) * m_nIndices, sizeof(UINT), m_nIndices, fp);
	fclose(fp);

	CD3DBufMgr::Instance()->AllocVertexBuffer(
		  pd3dCmdLst
		, m_d3dVertexBufferView
		, pVertices
		, m_nStride * m_nVertices
		, m_nStride);

	CD3DBufMgr::Instance()->AllocIndexBuffer(
		  pd3dCmdLst
		, m_d3dIndexBufferView
		, m_pIndices
		, sizeof(UINT) * m_nIndices);

	m_pVertices = pVertices;
}

CModelMesh::~CModelMesh()
{
	if (m_pVertices) delete[] reinterpret_cast<CModelVertex*>(m_pVertices);
}

CAnimationModelMesh::CAnimationModelMesh(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, FBXExporter&					FBXLoader
	, UINT							mesh_idx
	, const float					fScale
	, bool							bSaveToFile)
	: CMesh(pd3dCmdLst)
{
	m_nVertices = FBXLoader.mOutputDataContainer[mesh_idx].mVertices.size();
	m_nIndices = FBXLoader.mOutputDataContainer[mesh_idx].mTriangles.size() * 3;
	m_nStride = sizeof(CSkinnedAnimationVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	CSkinnedAnimationVertex* pVertices = new CSkinnedAnimationVertex[m_nVertices];
	m_pIndices = new UINT[m_nIndices];

	int i = 0;
	for (auto& p : FBXLoader.mOutputDataContainer[mesh_idx].mTriangles)
	{
		m_pIndices[i++] = p.mIndices[0];
		pVertices[p.mIndices[0]].m_iMaterialIdx = static_cast<BYTE>(p.mMaterialIndex);
		m_pIndices[i++] = p.mIndices[1];
		pVertices[p.mIndices[1]].m_iMaterialIdx = static_cast<BYTE>(p.mMaterialIndex);
		m_pIndices[i++] = p.mIndices[2];
		pVertices[p.mIndices[2]].m_iMaterialIdx = static_cast<BYTE>(p.mMaterialIndex);
	}

	CD3DBufMgr::Instance()->AllocIndexBuffer(
		  pd3dCmdLst
		, m_d3dIndexBufferView
		, m_pIndices
		, sizeof(UINT) * m_nIndices);

	i = 0;
	for (auto& p : FBXLoader.mOutputDataContainer[mesh_idx].mVertices)
	{
		pVertices[i].m_xmf3Pos = Vector3::ScalarProduct(p.mPosition, fScale, false);
		pVertices[i].m_xmf3Normal = p.mNormal;
		pVertices[i].m_xmf3Tangent = p.mTangent;
		p.mUV.y = 1.f - p.mUV.y;
		pVertices[i].m_xmf2TexCoord = p.mUV;
		pVertices[i].m_xmf3BoneWeights = XMFLOAT3(
			  p.mVertexBlendingInfos[0].mBlendingWeight
			, p.mVertexBlendingInfos[1].mBlendingWeight
			, p.mVertexBlendingInfos[2].mBlendingWeight);
		pVertices[i].m_iBoneIndices[0] = static_cast<BYTE>(p.mVertexBlendingInfos[0].mBlendingIndex);
		pVertices[i].m_iBoneIndices[1] = static_cast<BYTE>(p.mVertexBlendingInfos[1].mBlendingIndex);
		pVertices[i].m_iBoneIndices[2] = static_cast<BYTE>(p.mVertexBlendingInfos[2].mBlendingIndex);
		pVertices[i].m_iBoneIndices[3] = static_cast<BYTE>(p.mVertexBlendingInfos[3].mBlendingIndex);
		++i;
	}

	CD3DBufMgr::Instance()->AllocVertexBuffer(
		  pd3dCmdLst
		, m_d3dVertexBufferView
		, pVertices
		, m_nStride * m_nVertices
		, m_nStride);

	if (bSaveToFile)
	{
		std::string file_name = "./Assets/Meshes/" + FBXLoader.mGenericFileName + std::to_string(mesh_idx) + ".mbox";
		FILE* fp = nullptr;
		fopen_s(&fp, file_name.c_str(), "wb");
		fwrite(&m_nVertices, sizeof(UINT), 1, fp);
		fwrite(pVertices, m_nStride, m_nVertices, fp);
		fwrite(&m_nIndices, sizeof(UINT), 1, fp);
		fwrite(m_pIndices, sizeof(UINT), m_nIndices, fp);
		fclose(fp);
	}

	m_pVertices = pVertices;
}

CAnimationModelMesh::CAnimationModelMesh(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, const char*					strFileName
	, const float					fScale)
	: CMesh(pd3dCmdLst) 
{
	m_nStride = sizeof(CSkinnedAnimationVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	FILE* fp = nullptr;
	fopen_s(&fp, strFileName, "rb");
	fread_s(&m_nVertices, sizeof(UINT), sizeof(UINT), 1, fp);
	CSkinnedAnimationVertex* pVertices = new CSkinnedAnimationVertex[m_nVertices];
	fread_s(pVertices, m_nStride * m_nVertices, m_nStride, m_nVertices, fp);

	fread_s(&m_nIndices, sizeof(UINT), sizeof(UINT), 1, fp);
	m_pIndices = new UINT[m_nIndices];
	fread_s(m_pIndices, sizeof(UINT) * m_nIndices, sizeof(UINT), m_nIndices, fp);
	fclose(fp);

	CD3DBufMgr::Instance()->AllocVertexBuffer(
		  pd3dCmdLst
		, m_d3dVertexBufferView
		, pVertices
		, m_nStride * m_nVertices
		, m_nStride);

	CD3DBufMgr::Instance()->AllocIndexBuffer(
		  pd3dCmdLst
		, m_d3dIndexBufferView
		, m_pIndices
		, sizeof(UINT) * m_nIndices);

	m_pVertices = pVertices;
}

CAnimationModelMesh::~CAnimationModelMesh()
{
	if (m_pVertices) delete[] reinterpret_cast<CSkinnedAnimationVertex*>(m_pVertices);
}

CStageMesh::CStageMesh(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, FBXExporter&					FBXLoader
	, UINT							mesh_idx
	, const float					fScale
	, bool							bSaveToFile)
	: CMesh(pd3dCmdLst)
{
	m_nVertices = FBXLoader.mOutputDataContainer[mesh_idx].mVertices.size();
	m_nIndices = FBXLoader.mOutputDataContainer[mesh_idx].mTriangles.size() * 3;
	m_nStride = sizeof(CStageVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CStageVertex* pVertices = new CStageVertex[m_nVertices];
	m_pIndices = new UINT[m_nIndices];

	int i = 0;
	for (auto& p : FBXLoader.mOutputDataContainer[mesh_idx].mTriangles)
	{
		m_pIndices[i++] = p.mIndices[0];
		//pVertices[p.mIndices[0]].m_iMaterialIdx = static_cast<BYTE>(p.mMaterialIndex);
		m_pIndices[i++] = p.mIndices[1];
		//pVertices[p.mIndices[1]].m_iMaterialIdx = static_cast<BYTE>(p.mMaterialIndex);
		m_pIndices[i++] = p.mIndices[2];
		//pVertices[p.mIndices[2]].m_iMaterialIdx = static_cast<BYTE>(p.mMaterialIndex);
	}

	CD3DBufMgr::Instance()->AllocIndexBuffer(
		pd3dCmdLst
		, m_d3dIndexBufferView
		, m_pIndices
		, sizeof(UINT) * m_nIndices);

	i = 0;
	for (auto& p : FBXLoader.mOutputDataContainer[mesh_idx].mVertices)
	{
		pVertices[i].m_xmf3Pos = Vector3::ScalarProduct(p.mPosition, fScale, false);
		pVertices[i].m_xmf3Normal = p.mNormal;
		p.mUV.y = 1.f - p.mUV.y;
		pVertices[i].m_xmf3Tangent = p.mTangent;
		pVertices[i].m_xmf2TexCoord = p.mUV;
		pVertices[i].m_iMaterialIdx = p.mMaterialID;
		++i;
	}

	CD3DBufMgr::Instance()->AllocVertexBuffer(
		pd3dCmdLst
		, m_d3dVertexBufferView
		, pVertices
		, m_nStride * m_nVertices
		, m_nStride);

	if (bSaveToFile)
	{
		std::string file_name = "./Assets/Meshes/" + FBXLoader.mGenericFileName + std::to_string(mesh_idx) + ".mbox";
		FILE* fp = nullptr;
		fopen_s(&fp, file_name.c_str(), "wb");
		fwrite(&m_nVertices, sizeof(UINT), 1, fp);
		fwrite(pVertices, m_nStride, m_nVertices, fp);
		fwrite(&m_nIndices, sizeof(UINT), 1, fp);
		fwrite(m_pIndices, sizeof(UINT), m_nIndices, fp);
		fclose(fp);
	}

	m_pVertices = pVertices;
}
CStageMesh::CStageMesh(
	  ID3D12GraphicsCommandList*	pd3dCmdLst
	, const char*					strFileName
	, const float					fScale)
	: CMesh(pd3dCmdLst)
{
	m_nStride = sizeof(CStageVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	FILE* fp = nullptr;
	fopen_s(&fp, strFileName, "rb");
	fread_s(&m_nVertices, sizeof(UINT), sizeof(UINT), 1, fp);
	CStageVertex* pVertices = new CStageVertex[m_nVertices];
	fread_s(pVertices, m_nStride * m_nVertices, m_nStride, m_nVertices, fp);

	fread_s(&m_nIndices, sizeof(UINT), sizeof(UINT), 1, fp);
	m_pIndices = new UINT[m_nIndices];
	fread_s(m_pIndices, sizeof(UINT) * m_nIndices, sizeof(UINT), m_nIndices, fp);
	fclose(fp);

	for (int i = 0; i < m_nVertices; ++i)
	{
		pVertices[i].m_xmf3Pos = Vector3::ScalarProduct(pVertices[i].m_xmf3Pos, fScale, false);
	}

	CD3DBufMgr::Instance()->AllocVertexBuffer(
		pd3dCmdLst
		, m_d3dVertexBufferView
		, pVertices
		, m_nStride * m_nVertices
		, m_nStride);

	CD3DBufMgr::Instance()->AllocIndexBuffer(
		pd3dCmdLst
		, m_d3dIndexBufferView
		, m_pIndices
		, sizeof(UINT) * m_nIndices);

	m_pVertices = pVertices;
}

CStageMesh::~CStageMesh()
{
	if (m_pVertices) delete[] reinterpret_cast<CStageVertex*>(m_pVertices);
}

bool CStageMesh::CollisionCheck(
	  const XMFLOAT3&	ray_origin
	, const XMFLOAT3&	ray_dir
	, float*			output_t)
{
	bool bCollide{ false };
	float u{ 0.0f }, v{ 0.0f }, t{ 0.0f };
	float det{ 0.0f }, ReverseDet{ 0.0f };
	UINT Idx0{ 0 }, Idx1{ 0 }, Idx2{ 0 };
	const int nPrimitives{ static_cast<int>(m_nVertices) / 3 };
	CStageVertex* pVertices = reinterpret_cast<CStageVertex*>(m_pVertices);
	// Ray - Triangle Intersection checking with mesh
	for (int i = 0; i < nPrimitives; i++) {
		Idx0 = i * 3;
		if (Vector3::DotProduct(ray_dir, pVertices[Idx0].m_xmf3Normal) > -FLT_EPSILON) continue;

		Idx1 = Idx0 + 1;
		Idx2 = Idx0 + 2;

		XMFLOAT3 e1 = Vector3::Subtract(pVertices[Idx1].m_xmf3Pos, pVertices[Idx0].m_xmf3Pos);
		XMFLOAT3 e2 = Vector3::Subtract(pVertices[Idx2].m_xmf3Pos, pVertices[Idx0].m_xmf3Pos);
		XMFLOAT3 p = Vector3::CrossProduct(ray_dir, e2, false);
		det = Vector3::DotProduct(e1, p);
		if (IsZero(det)) continue;

		ReverseDet = 1.0f / det;
		XMFLOAT3 s = Vector3::Subtract(ray_origin, pVertices[Idx0].m_xmf3Pos);
		u = ReverseDet * Vector3::DotProduct(s, p);
		if (u < 0 || u > 1) continue;

		XMFLOAT3 q = Vector3::CrossProduct(s, e1, false);
		v = ReverseDet * Vector3::DotProduct(ray_dir, q);
		if (v < 0 || u + v > 1) continue;

		float t_val = ReverseDet * Vector3::DotProduct(e2, q);
		if (t_val > FLT_EPSILON && (IsZero(t) || t_val < t)) {
			t = t_val;
			bCollide = true;
		}
	}
	if (!bCollide) t = 50000.0f;
	*output_t = t;
	return bCollide;
}

CAdvancedParticleBillBoardMesh::CAdvancedParticleBillBoardMesh(ID3D12GraphicsCommandList * pd3dCmdLst, UINT nBillboards, XMFLOAT2 szBillboards) : CMesh(pd3dCmdLst)
{
	m_nVertices = nBillboards;
	m_nStride = sizeof(CAdvancedParticleBillboardVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	CAdvancedParticleBillboardVertex* pVertices = new CAdvancedParticleBillboardVertex[m_nVertices];
	for (int i = 0; i < m_nVertices; ++i)
		pVertices[i].m_xmf2Size = szBillboards;

	CD3DBufMgr::Instance()->AllocUploadVertexBuffer(
		pd3dCmdLst
		, m_d3dVertexBufferView
		, pVertices
		, m_nStride * m_nVertices
		, m_nStride
		, reinterpret_cast<LPVOID*>(&m_pVertices));

	delete[] pVertices;
}

CAdvancedParticleBillBoardMesh::~CAdvancedParticleBillBoardMesh()
{
}
