#pragma once

struct FBXBlendingIndexWeightPair
{
	unsigned int mBlendingIndex;
	double mBlendingWeight;

	FBXBlendingIndexWeightPair() : 
		mBlendingIndex(0),
		mBlendingWeight(0)
	{}
};

// Each Control Point in FBX is basically a vertex
// in the physical world. For example, a cube has 8
// vertices(Control Points) in FBX
// Joints are associated with Control Points in FBX
// The mapping is one joint corresponding to 4
// Control Points(Reverse of what is done in a game engine)
// As a result, this struct stores a XMFLOAT3 and a 
// vector of joint indices
struct FBXCtrlPoint
{
	XMFLOAT3 mPosition;
	std::vector<FBXBlendingIndexWeightPair> mBlendingInfo;

	FBXCtrlPoint()
	{
		mBlendingInfo.reserve(4);
	}
};

// This stores the information of each key frame of each joint
// This is a linked list and each node is a snapshot of the
// global transformation of the joint at a certain frame
struct FBXKeyframe
{
	FbxLongLong mFrameNum;
	FbxAMatrix mGlobalTransform;
	FBXKeyframe* mNext;

	FBXKeyframe() :
		mNext(nullptr)
	{}
};

// This is the actual representation of a joint in a game engine
struct FBXJoint
{
	std::string mName;
	int mParentIndex;
	FbxAMatrix mGlobalBindposeInverse;
	FBXKeyframe* mAnimation;
	FbxNode* mNode;

	FBXJoint() :
		mNode(nullptr),
		mAnimation(nullptr)
	{
		mGlobalBindposeInverse.SetIdentity();
		mParentIndex = -1;
	}

	~FBXJoint()
	{
		while(mAnimation)
		{
			FBXKeyframe* temp = mAnimation->mNext;
			//delete mAnimation;
			mAnimation = temp;
		}
	}
};

struct FBXSKELETON
{
	std::vector<FBXJoint> mJoints;

	FBXSKELETON() {}
	~FBXSKELETON() {}
};

struct FBXTRIANGLE
{
	std::vector<unsigned int> mIndices; 
	std::string mMaterialName;
	unsigned int mMaterialIndex;

	bool operator<(const FBXTRIANGLE& rhs)
	{
		return mMaterialIndex < rhs.mMaterialIndex;
	}
};


class FBXUtilities
{
public:

	// This function should be changed if exporting to another format
	static void WriteMatrix(std::ostream& inStream, FbxMatrix& inMatrix, bool inIsRoot);

	static void PrintMatrix(FbxMatrix& inMatrix);
	
	static FbxAMatrix GetGeometryTransformation(FbxNode* inNode);

	static std::string GetFileName(const std::string& inInput);

	static std::string RemoveSuffix(const std::string& inInput);
};


