#pragma once
constexpr char NAME_LENGTH = 32;

struct Bone
{
	char					strName[NAME_LENGTH];
	int						iParentIdx;
	XMMATRIX				mtxBoneOffset;
};

struct CSkeleton
{
	UINT					nBones;
	Bone*					arrBones = nullptr;

	CSkeleton() : nBones(0), arrBones(nullptr) {}
	~CSkeleton() { if (arrBones) delete[] arrBones; }
	
	void Init(FBXExporter& LoadedAnimation, UINT mesh_idx, bool bSaveToFile = true);
	void Init(const char* strFileName);
};

struct KeyFrameBone
{
	XMMATRIX				mtxTransform;
};

struct KeyFrame
{
	KeyFrameBone*			arrKeyFrameBones = nullptr;

	KeyFrame() : arrKeyFrameBones(nullptr) {}
	~KeyFrame() { if (arrKeyFrameBones) delete[] arrKeyFrameBones; }
};

class CAnimation
{
public:
	char							strName[NAME_LENGTH];

	UINT							nKeyFrames;
	KeyFrame*						arrKeyFrames = nullptr;

public:
	CAnimation(UINT nBones, FBXExporter& LoadedAnimation, UINT mesh_idx = 0, bool bSaveToFile = true);
	CAnimation(UINT nBones, const char* strFileName);
	~CAnimation() { if (arrKeyFrames) delete[] arrKeyFrames; }
};

struct RagdollNode
{
	std::string				name;

	int						ParentIndex = -1;
	PxVec3					p;
	PxQuat					q;
	PxRigidDynamic*			actor[2];

	PxVec3*					pAniVertPos = nullptr;
	XMMATRIX*				MatMidResult = nullptr;

	bool					Valid = false;

	~RagdollNode()
	{
		if (pAniVertPos) delete[] pAniVertPos;
		if (MatMidResult) delete[] MatMidResult;
	}
};

struct Ragdoll
{
	RagdollNode*	pRagdollNode;
	UINT			nBones;
	UINT			nAniKeyFrame;
	CAnimation*		Animation;
	CSkeleton*		Skeleton;
	UINT			ActiveFrame_Start = 0;
	UINT			ActiveFrame_End = 0;
	float			FrameRatio = 0.f;
	float			AnimationFrameDelayTime = 0.f;

	void SetActiveFrame(UINT start, UINT end, float ratio) { 
		ActiveFrame_Start = start; ActiveFrame_End = end; FrameRatio = ratio;
		AnimationFrameDelayTime = 1.0f / (float)nAniKeyFrame * FrameRatio;
	}

	~Ragdoll() {
		if (pRagdollNode) delete[] pRagdollNode;
		if (Animation) delete Animation;
		if (Skeleton) delete Skeleton;
	}
};