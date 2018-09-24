#pragma once
#include <unordered_map>
#include <algorithm>
#include "FBXVertex.h"
#include "FBXUtilities.h"
#include "FBXMaterial.h"

struct FBXOutputData {
	unsigned int				mTriangleCount;
	std::vector<FBXTRIANGLE>	mTriangles;
	std::vector<FBXPNTIWVertex>	mVertices;

	std::string					mAnimationName;
	FbxLongLong					mAnimationLength;
	FBXSKELETON					mSkeleton;

	std::unordered_map<unsigned int, FBXMaterial*> mMaterialLookUp;
};

class FBXExporter
{
public:
	FBXExporter();
	~FBXExporter();
	bool Initialize();
	bool LoadScene(const char* inFileName, const char* inOutputPath = NULL);
	
	void ExportFBX();

public:
	FbxManager*					mFBXManager;
	FbxScene*					mFBXScene;
	std::string					mInputFilePath;
	std::string					mOutputFilePath;
	std::string					mGenericFileName;

	std::unordered_map<unsigned int, FBXCtrlPoint*> mControlPoints;

	UINT						mProcessGeometryCount;
	std::vector<FBXOutputData>	mOutputDataContainer;

	LARGE_INTEGER				mCPUFreq;

private:
	void ProcessGeometry(FbxNode* inNode);
	void ProcessSkeletonHierarchy(FbxNode* inRootNode);
	void ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex);
	void ProcessControlPoints(FbxNode* inNode);
	void ProcessJointsAndAnimations(FbxNode* inNode);
	int FindJointIndexUsingName(const std::string& inJointName);
	void ProcessMesh(FbxNode* inNode);
	void ReadUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, XMFLOAT2& outUV);
	void ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal);
	void ReadBinormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outBinormal);
	void ReadTangent(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outTangent);
	void Optimize();
	int FindVertex(const FBXPNTIWVertex& inTargetVertex, const std::vector<FBXPNTIWVertex>& uniqueVertices);

	void AssociateMaterialToMesh(FbxNode* inNode);
	void ProcessMaterials(FbxNode* inNode);
	void ProcessMaterialAttribute(FbxSurfaceMaterial* inMaterial, unsigned int inMaterialIndex);
	void ProcessMaterialTexture(FbxSurfaceMaterial* inMaterial, FBXMaterial* ioMaterial);
	void PrintMaterial();
	void PrintTriangles();
	
	void CleanupFbxManager();
	void WriteMeshToStream(std::ostream& inStream);
	void WriteAnimationToStream(std::ostream& inStream);
};