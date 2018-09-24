#pragma once
constexpr char NAME_LENGTH{ 32 };

struct Bone
{
	char					strName[NAME_LENGTH];
	int						iParentIdx;
	XMMATRIX				mtxBoneOffset;
};

struct Skeleton
{
	UINT					nBones;
	Bone*					arrBones;

	Skeleton() : nBones(0), arrBones(nullptr) {}
	~Skeleton() { if (arrBones) delete[] arrBones; }
	void Init(FBXExporter& LoadedAnimation, UINT mesh_idx, bool bSaveToFile = true);
	void Init(const char* strFileName);
};

struct KeyFrameBone
{
	XMMATRIX				mtxTransform;
};

struct KeyFrame 
{
	KeyFrameBone*			arrKeyFrameBones;

	KeyFrame() : arrKeyFrameBones(nullptr) {}
	~KeyFrame() { if (arrKeyFrameBones) delete[] arrKeyFrameBones; }
};

class CAnimation
{
public:
	char							strName[NAME_LENGTH];

	UINT							nKeyFrames;
	KeyFrame*						arrKeyFrames;

public:
	CAnimation(const CAnimation&) = delete;
	CAnimation(CAnimation&& other);
	CAnimation(UINT nBones, FBXExporter& LoadedAnimation, UINT mesh_idx = 0, bool bSaveToFile = true);
	CAnimation(UINT nBones, const char* strFileName);
	~CAnimation();
};