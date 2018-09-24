#include "stdafx.h"
#include "FBX\FBXExporter.h"
#include "Mesh.h"

CMesh::CMesh()
{
	m_nReferences = 0;

	m_nSlot = 0;
	m_nVertices = 0;
	m_nStride = 0;
	m_nOffset = 0;
}
CMesh::~CMesh()
{
	if (m_pVertices) delete[] m_pVertices;
	if (m_pnIndices) delete[] m_pnIndices;
}

CModelMesh::CModelMesh(PxPhysics* physics, PxCooking* cooking, const char * strFileName, const float fScale)
	: CMesh()
{
	m_nStride = sizeof(CVertex);
	m_nOffset = 0;
	m_nSlot = 0;

	FILE* fp = nullptr;
	fopen_s(&fp, strFileName, "rb");
	fread_s(&m_nVertices, sizeof(UINT), sizeof(UINT), 1, fp);
	fread_s(&m_isBreakable, sizeof(UINT), sizeof(UINT), 1, fp);
	m_pVertices = new CVertex[m_nVertices];
	fread_s(m_pVertices, m_nStride * m_nVertices, m_nStride, m_nVertices, fp);
	fclose(fp);

	PxVec3* Verts = new PxVec3[m_nVertices];

	for (UINT i = 0; i < m_nVertices; ++i)
	{
		m_pVertices[i].xmf3Position = Vector3::ScalarProduct(m_pVertices[i].xmf3Position, fScale, false);
		Verts[i].x = m_pVertices[i].xmf3Position.x;
		Verts[i].y = m_pVertices[i].xmf3Position.y;
		Verts[i].z = m_pVertices[i].xmf3Position.z;
	}

	PxCookingParams params = cooking->getParams();
	params.convexMeshCookingType = PxConvexMeshCookingType::eINFLATION_INCREMENTAL_HULL;
	params.gaussMapLimit = 256;
	cooking->setParams(params);

	PxConvexMeshDesc desc;
	desc.points.data = Verts;
	desc.points.count = m_nVertices;
	desc.points.stride = sizeof(PxVec3);
	desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	m_pConvexmesh = cooking->createConvexMesh(desc, physics->getPhysicsInsertionCallback());

	delete[] Verts;
}

CModelMesh::CModelMesh(PxPhysics * physics, PxCooking * cooking, FILE * fp, const float fSizeMeter)
{
	m_nStride = sizeof(CVertex);
	m_nOffset = 0;
	m_nSlot = 0;

	fread_s(&m_nVertices, sizeof(UINT), sizeof(UINT), 1, fp);
	fread_s(&m_isBreakable, sizeof(UINT), sizeof(UINT), 1, fp);
	m_pVertices = new CVertex[m_nVertices];
	fread_s(m_pVertices, m_nStride * m_nVertices, m_nStride, m_nVertices, fp);

	PxVec3* Verts = new PxVec3[m_nVertices];

	for (UINT i = 0; i < m_nVertices; ++i)
	{
		m_pVertices[i].xmf3Position = Vector3::ScalarProduct(m_pVertices[i].xmf3Position, fSizeMeter, false);
		Verts[i].x = m_pVertices[i].xmf3Position.x;
		Verts[i].y = m_pVertices[i].xmf3Position.y;
		Verts[i].z = m_pVertices[i].xmf3Position.z;
	}

	PxCookingParams params = cooking->getParams();
	params.convexMeshCookingType = PxConvexMeshCookingType::eINFLATION_INCREMENTAL_HULL;
	params.gaussMapLimit = 256;
	cooking->setParams(params);

	PxConvexMeshDesc desc;
	desc.points.data = Verts;
	desc.points.count = m_nVertices;
	desc.points.stride = sizeof(PxVec3);
	desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	m_pConvexmesh = cooking->createConvexMesh(desc, physics->getPhysicsInsertionCallback());

	delete[] Verts;
}


CModelMesh::~CModelMesh()
{
}

CAnimationModelMesh::CAnimationModelMesh(
	const char*					strFileName
	, const float					fScale)
	: CMesh()
{
	m_nStride = sizeof(CSkinnedAnimationVertex);
	m_nOffset = 0;
	m_nSlot = 0;

	FILE* fp = nullptr;
	fopen_s(&fp, strFileName, "rb");
	fread_s(&m_nVertices, sizeof(UINT), sizeof(UINT), 1, fp);
	
	pVerts = new CSkinnedAnimationVertex[m_nVertices];
	fread_s(pVerts, m_nStride * m_nVertices, m_nStride, m_nVertices, fp);

	fread_s(&m_nIndices, sizeof(UINT), sizeof(UINT), 1, fp);
	pIdx = new UINT[m_nIndices];
	fread_s(pIdx, sizeof(UINT) * m_nIndices, sizeof(UINT), m_nIndices, fp);
	fclose(fp);
}

CAnimationModelMesh::CAnimationModelMesh(FBXExporter & FBXLoader, UINT mesh_idx, const float fScale, bool bSaveToFile)
{
	m_nVertices = FBXLoader.mOutputDataContainer[mesh_idx].mVertices.size();
	m_nIndices = FBXLoader.mOutputDataContainer[mesh_idx].mTriangles.size() * 3;
	m_nStride = sizeof(CSkinnedAnimationVertex);
	m_nOffset = 0;
	m_nSlot = 0;

	CSkinnedAnimationVertex* pVertices = new CSkinnedAnimationVertex[m_nVertices];
	UINT* pnIndices = new UINT[m_nIndices];

	int i = 0;
	for (auto& p : FBXLoader.mOutputDataContainer[mesh_idx].mTriangles)
	{
		pnIndices[i++] = p.mIndices[0];
		pVertices[p.mIndices[0]].m_iMaterialIdx = static_cast<BYTE>(p.mMaterialIndex);
		pnIndices[i++] = p.mIndices[1];
		pVertices[p.mIndices[1]].m_iMaterialIdx = static_cast<BYTE>(p.mMaterialIndex);
		pnIndices[i++] = p.mIndices[2];
		pVertices[p.mIndices[2]].m_iMaterialIdx = static_cast<BYTE>(p.mMaterialIndex);
	}

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

	if (bSaveToFile)
	{
		std::string file_name = "./ServerAssets/" + FBXLoader.mGenericFileName + std::to_string(mesh_idx) + ".mbox";
		FILE* fp = nullptr;
		fopen_s(&fp, file_name.c_str(), "wb");
		fwrite(&m_nVertices, sizeof(UINT), 1, fp);
		fwrite(pVertices, m_nStride, m_nVertices, fp);
		fwrite(&m_nIndices, sizeof(UINT), 1, fp);
		fwrite(pnIndices, sizeof(UINT), m_nIndices, fp);
		fclose(fp);
	}

	delete[] pVertices;
	delete[] pnIndices;
}
