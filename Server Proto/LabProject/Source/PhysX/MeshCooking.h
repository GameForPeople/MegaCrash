#pragma once
#include "stdafx.h"
#include "Mesh\Mesh.h"
#include "Animation.h"

#define BOX_NVERTS 24

struct FBXMeshContainer {
	int			nVerts;
	bool		bBreakable;
	XMFLOAT3*	pVerts;
};

float rand(float loVal, float hiVal)
{
	return loVal + (float(rand()) / RAND_MAX)*(hiVal - loVal);
}

PxU32 rand(PxU32 loVal, PxU32 hiVal)
{
	return loVal + PxU32(rand() % (hiVal - loVal));
}

void setupCommonCookingParams(PxCookingParams& params, bool skipMeshCleanup, bool skipEdgeData)
{
	params.suppressTriangleMeshRemapTable = true;

	if (!skipMeshCleanup)
		params.meshPreprocessParams &= ~static_cast<PxMeshPreprocessingFlags>(PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH);
	else
		params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;

	if (!skipEdgeData)
		params.meshPreprocessParams &= ~static_cast<PxMeshPreprocessingFlags>(PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE);
	else
		params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
}

void createRandomTerrain(const PxVec3& origin, PxU32 numRows, PxU32 numColumns,
	PxReal cellSizeRow, PxReal cellSizeCol, PxReal heightScale,
	PxVec3*& vertices, PxU32*& indices)
{
	PxU32 numX = (numColumns + 1);
	PxU32 numZ = (numRows + 1);
	PxU32 numVertices = numX * numZ;
	PxU32 numTriangles = numRows * numColumns * 2;

	if (vertices == NULL)
		vertices = new PxVec3[numVertices];
	if (indices == NULL)
		indices = new PxU32[numTriangles * 3];

	PxU32 currentIdx = 0;
	for (PxU32 i = 0; i <= numRows; i++)
	{
		for (PxU32 j = 0; j <= numColumns; j++)
		{
			PxVec3 v(origin.x + PxReal(j)*cellSizeRow, origin.y, origin.z + PxReal(i)*cellSizeCol);
			vertices[currentIdx++] = v;
		}
	}

	currentIdx = 0;
	for (PxU32 i = 0; i < numRows; i++)
	{
		for (PxU32 j = 0; j < numColumns; j++)
		{
			PxU32 base = (numColumns + 1)*i + j;
			indices[currentIdx++] = base + 1;
			indices[currentIdx++] = base;
			indices[currentIdx++] = base + numColumns + 1;
			indices[currentIdx++] = base + numColumns + 2;
			indices[currentIdx++] = base + 1;
			indices[currentIdx++] = base + numColumns + 1;
		}
	}

	for (PxU32 i = 0; i < numVertices; i++)
	{
		PxVec3& v = vertices[i];
		v.y += heightScale * rand(-1.0f, 1.0f);
	}
}

PxTriangleMesh* createBV34TriangleMesh(
	PxPhysics* physics, PxCooking* cooking,
	PxU32 numVertices, const PxVec3* vertices, PxU32 numTriangles, const PxU32* indices,
	bool skipMeshCleanup, bool skipEdgeData)
{
	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = numVertices;
	meshDesc.points.data = vertices;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.triangles.count = numTriangles;
	meshDesc.triangles.data = indices;
	meshDesc.triangles.stride = 3 * sizeof(PxU32);

	PxCookingParams params = cooking->getParams();

	params.midphaseDesc = PxMeshMidPhase::eBVH34;

	setupCommonCookingParams(params, skipMeshCleanup, skipEdgeData);

	params.midphaseDesc.mBVH34Desc.numTrisPerLeaf = 15;

	cooking->setParams(params);

	PxTriangleMesh* triMesh = NULL;

	return triMesh = cooking->createTriangleMesh(meshDesc, physics->getPhysicsInsertionCallback());
}

inline void ExportFBXAnimation(char* filePath)
{
	FBXExporter		FBXLoader;
	FBXLoader.Initialize();
	FBXLoader.LoadScene(filePath);
	FBXLoader.ExportFBX();

	CSkeleton* skeleton = new CSkeleton();
	skeleton->Init(FBXLoader, 0);

	CAnimationModelMesh* animationMesh = new CAnimationModelMesh(FBXLoader, 0);
	CAnimation* animation = new CAnimation(skeleton->nBones, FBXLoader);

	delete skeleton;
	delete animationMesh;
	delete animation;

	std::cout << "Export Finished" << std::endl;
	system("pause");
	exit(0);
}



inline void ExportFBXMesh(char* filePath)
{
	FBXExporter		FBXLoader;
	FBXLoader.Initialize();
	FBXLoader.LoadScene(filePath);
	FBXLoader.ExportFBX();

	float Scale = 1.f;

	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(-90.0f), 0.0f, 0.0f);

	for (int i = 0; i < FBXLoader.mOutputDataContainer.size(); ++i) {
		UINT m_nVertices = FBXLoader.mOutputDataContainer[i].mVertices.size();
		UINT m_nStride = sizeof(CVertex);

		CVertex* pVertices = new CVertex[m_nVertices];
		int j = 0;
		for (auto& p : FBXLoader.mOutputDataContainer[i].mVertices)
		{
			pVertices[j].xmf3Position = Vector3::ScalarProduct(p.mPosition, Scale, false);
			XMStoreFloat3(&pVertices[j].xmf3Position, XMVector3Transform(XMLoadFloat3(&pVertices[j].xmf3Position), mtxRotate));
			++j;
		}

		std::string file_name = "./ServerAssets/Meshes/" + FBXLoader.mGenericFileName +"/"+ FBXLoader.mGenericFileName +"_" + std::to_string(i) + ".mbox";
		
		bool flag = FBXLoader.mOutputDataContainer[i].mMeshName.find("UB") == std::string::npos ? true : false;

		FILE* fp = nullptr;
		fopen_s(&fp, file_name.c_str(), "wb");
		fwrite(&m_nVertices, sizeof(UINT), 1, fp);
		fwrite(&flag, sizeof(UINT), 1, fp);
		fwrite(pVertices, m_nStride, m_nVertices, fp);
		fclose(fp);

		delete[] pVertices;
	}

	std::cout << "Export Finished" << std::endl;

	system("pause");
	exit(0);
}

inline void ExportFBXMesh2(char* filePath)
{
	FBXExporter		FBXLoader;
	FBXLoader.Initialize();
	FBXLoader.LoadScene(filePath);
	FBXLoader.ExportFBX();

	float Scale = 1.f;

	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(-90.0f), 0.0f, 0.0f);

	int nMeshes = FBXLoader.mOutputDataContainer.size();
	FBXMeshContainer* meshes = new FBXMeshContainer[nMeshes];

	std::string file_name = "./ServerAssets/Meshes/" + FBXLoader.mGenericFileName + "/" + FBXLoader.mGenericFileName + ".mbox";
	FILE* fp = nullptr;
	fopen_s(&fp, file_name.c_str(), "wb");
	
	fwrite(&nMeshes, sizeof(UINT), 1, fp);

	for (int i = 0; i < nMeshes; ++i) {
		meshes[i].nVerts = FBXLoader.mOutputDataContainer[i].mVertices.size();

		if (FBXLoader.mOutputDataContainer[i].mMeshName.find("UB")) {
			meshes[i].bBreakable = true;
		}
		else
			meshes[i].bBreakable = false;

		meshes[i].pVerts = new XMFLOAT3[meshes[i].nVerts];
		
		int j = 0;
		for (auto& p : FBXLoader.mOutputDataContainer[i].mVertices)
		{
			XMStoreFloat3(&meshes[i].pVerts[j], XMVector3Transform(XMLoadFloat3(&p.mPosition), mtxRotate));
			++j;
		}

		meshes[i].bBreakable = FBXLoader.mOutputDataContainer[i].mMeshName.find("UB") == std::string::npos ? true : false;

		fwrite(&meshes[i].nVerts, sizeof(UINT), 1, fp);
		fwrite(&meshes[i].bBreakable, sizeof(UINT), 1, fp);
		fwrite(meshes[i].pVerts, sizeof(XMFLOAT3), meshes[i].nVerts, fp);
	}
	
	fclose(fp);

	std::cout << "Export Finished" << std::endl;

	system("pause");
	exit(0);
}

inline Ragdoll* ReadAnimationfile(std::string filename)
{
	CSkeleton* ske = new CSkeleton();
	RagdollNode* ragdollnode;
	CAnimation* animation;
	CAnimationModelMesh* animodelmesh;
	Ragdoll* ragdoll = new Ragdoll();

	std::string path = "./ServerAssets/" + filename + ".mbox";
	animodelmesh = new CAnimationModelMesh(path.c_str(), 1.0f);

	path = "./ServerAssets/" + filename + ".sbox";
	ske->Init(path.c_str());

	path = "./ServerAssets/" + filename + ".abox";
	animation = new CAnimation(ske->nBones, path.c_str());

	ragdollnode = new RagdollNode[ske->nBones];

	for (int i = 0; i < ske->nBones; ++i) {
		ragdollnode[i].pAniVertPos = new PxVec3[animation->nKeyFrames];
		ragdollnode[i].MatMidResult = new XMMATRIX[animation->nKeyFrames];

		ragdollnode[i].ParentIndex = ske->arrBones[i].iParentIdx;

		std::string bonename = ske->arrBones[i].strName;

		if (bonename.find("Nub") == std::string::npos) 
		{
			if (bonename.find("Bip001 Xtra01") != std::string::npos)
				//bonename.find("Bip001 Xtra02") != std::string::npos) 
			{
				ragdollnode[i].Valid = true;
				ragdollnode[i].name = bonename;
			}
		}
	}

	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(-1.5708f, 0.f, 0.0f);
	for (int ani_frame = 0; ani_frame < animation->nKeyFrames; ++ani_frame) 
	{
		PxVec3 pos(0);

		for (int i = 0; i < animodelmesh->GetnVertices(); ++i)
		{
			if (ragdollnode[animodelmesh->pVerts[i].m_iBoneIndices[0]].Valid) {
				XMMATRIX mat_transform = animation->arrKeyFrames[ani_frame].arrKeyFrameBones[animodelmesh->pVerts[i].m_iBoneIndices[0]].mtxTransform;
				XMMATRIX mat_offset = ske->arrBones[animodelmesh->pVerts[i].m_iBoneIndices[0]].mtxBoneOffset;
				XMMATRIX mat_final = XMMatrixMultiply(mat_offset, mat_transform);

				XMFLOAT4 tmp_q3{};
				XMStoreFloat4(&tmp_q3, XMQuaternionRotationMatrix(mat_final));
				pos += PxVec3(animodelmesh->pVerts[i].m_xmf3Pos.x, animodelmesh->pVerts[i].m_xmf3Pos.y, animodelmesh->pVerts[i].m_xmf3Pos.z);
				ragdollnode[animodelmesh->pVerts[i].m_iBoneIndices[0]].q = PxQuat(tmp_q3.x, tmp_q3.y, tmp_q3.z, tmp_q3.w);
			}
		}

		for (int i = 0; i < ske->nBones; ++i)
		{
			if (ragdollnode[i].Valid) {
				XMMATRIX mat_transform = animation->arrKeyFrames[ani_frame].arrKeyFrameBones[i].mtxTransform;
				XMMATRIX mat_offset = ske->arrBones[i].mtxBoneOffset;
				XMMATRIX mat_final = XMMatrixMultiply(mat_offset, mat_transform);
				ragdollnode[i].MatMidResult[ani_frame] = XMMatrixMultiply(mat_final, mtxRotate);

				pos /= BOX_NVERTS;
				ragdollnode[i].pAniVertPos[ani_frame] = pos;
				int a = 0;
			}
		}
	}

	ragdoll->nBones = ske->nBones;
	ragdoll->pRagdollNode = ragdollnode;
	ragdoll->nAniKeyFrame = animation->nKeyFrames;

	delete ske;
	delete animation;
	delete animodelmesh;
	return ragdoll;
}

inline void ReadPositionFile(std::string filename) {
	int nStride = sizeof(PxVec3);
	int nVerts;
	PxVec3* pVerts;

	std::ifstream in;
	in.open(filename);
	if (!in) {
		std::cout << "Invalid FileName" << std::endl;
		exit(0);
	}
	in >> nVerts;

	pVerts = new PxVec3[nVerts];
	int i = 0;
	while (in) {
		in >> pVerts[i].x;
		in >> pVerts[i].y;
		in >> pVerts[i].z;
		i++;
	}

	std::string file_name = "./ServerAssets/BlockPosition.pos";
	FILE* fp = nullptr;
	fopen_s(&fp, file_name.c_str(), "wb");
	fwrite(&nVerts, sizeof(UINT), 1, fp);
	fwrite(pVerts, nStride, nVerts, fp);
	fclose(fp);

	delete pVerts;

	std::cout <<"Read File : "<< filename << std::endl;
	std::cout <<"nVerts : "<< nVerts << std::endl;

	system("pause");
	exit(0);
}