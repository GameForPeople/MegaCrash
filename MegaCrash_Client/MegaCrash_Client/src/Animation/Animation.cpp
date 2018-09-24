#include "stdafx.h"
#include "Animation.h"

void Skeleton::Init(FBXExporter& LoadedAnimation, UINT mesh_idx, bool bSaveToFile)
{
	auto& bones = LoadedAnimation.mOutputDataContainer[mesh_idx].mSkeleton.mJoints;
	nBones = bones.size();
	arrBones = new Bone[nBones];
	int boneIdx = 0;
	for (auto& bone : bones)
	{
		strcpy_s(arrBones[boneIdx].strName, NAME_LENGTH, bone.mName.c_str());
		arrBones[boneIdx].iParentIdx = bone.mParentIndex;
		for (int i = 0; i < 4; ++i)
		{
			arrBones[boneIdx].mtxBoneOffset.r[i] = XMLoadFloat4(&XMFLOAT4(
				static_cast<float>(bone.mGlobalBindposeInverse.Get(i, 0))
				, static_cast<float>(bone.mGlobalBindposeInverse.Get(i, 1))
				, static_cast<float>(bone.mGlobalBindposeInverse.Get(i, 2))
				, static_cast<float>(bone.mGlobalBindposeInverse.Get(i, 3))));
		}
		++boneIdx;
	}
	if (bSaveToFile)
	{
		std::string file_name = "./Assets/Animations/Skeletons/" + LoadedAnimation.mGenericFileName + std::to_string(mesh_idx) + ".sbox";
		FILE* fp = nullptr;
		fopen_s(&fp, file_name.c_str(), "wb");
		fwrite(&nBones, sizeof(UINT), 1, fp);
		fwrite(arrBones, sizeof(Bone), nBones, fp);
		fclose(fp);
	}
}

void Skeleton::Init(const char* strFileName)
{
	FILE* fp = nullptr;
	fopen_s(&fp, strFileName, "rb");
	fread_s(&nBones, sizeof(UINT), sizeof(UINT), 1, fp);
	arrBones = new Bone[nBones];
	fread_s(arrBones, sizeof(Bone) * nBones, sizeof(Bone), nBones, fp);
	fclose(fp);
}

CAnimation::CAnimation(CAnimation && other)
{
	memcpy(this, &other, sizeof(other));
	arrKeyFrames = other.arrKeyFrames;
	other.arrKeyFrames = nullptr;
}

CAnimation::CAnimation(UINT nBones, FBXExporter& LoadedAnimation, UINT mesh_idx, bool bSaveToFile)
	: nKeyFrames{ 0 }
	, arrKeyFrames{ nullptr }
{
	strcpy_s(strName, NAME_LENGTH, LoadedAnimation.mGenericFileName.c_str());

	nKeyFrames = static_cast<UINT>(LoadedAnimation.mOutputDataContainer[mesh_idx].mAnimationLength);
	arrKeyFrames = new KeyFrame[nKeyFrames];
	auto IdentityMtx = Matrix4x4::Identity();
	for (int i = 0; i < nKeyFrames; ++i)
	{
		arrKeyFrames[i].arrKeyFrameBones = new KeyFrameBone[nBones];
		arrKeyFrames[i].arrKeyFrameBones->mtxTransform = XMLoadFloat4x4(&IdentityMtx);
	}
	auto& bones = LoadedAnimation.mOutputDataContainer[mesh_idx].mSkeleton.mJoints;

	int boneIdx = 0;
	for (auto& bone : bones)
	{
		int keyFrameIdx = 0;
		auto boneFrame = bone.mAnimation;
		while (boneFrame)
		{
			auto& transform = boneFrame->mGlobalTransform;

			for (int i = 0; i < 4; ++i)
			{
				arrKeyFrames[keyFrameIdx].arrKeyFrameBones[boneIdx].mtxTransform.r[i]
					= XMLoadFloat4(&XMFLOAT4(
						static_cast<float>(transform.Get(i, 0))
						, static_cast<float>(transform.Get(i, 1))
						, static_cast<float>(transform.Get(i, 2))
						, static_cast<float>(transform.Get(i, 3))));
			}
			boneFrame = boneFrame->mNext;
			++keyFrameIdx;
		}
		++boneIdx;
	}

	if (bSaveToFile)
	{
		std::string file_name = "./Assets/Animations/Keyframes/" + LoadedAnimation.mGenericFileName + std::to_string(mesh_idx) + ".abox";
		FILE* fp = nullptr;
		fopen_s(&fp, file_name.c_str(), "wb");
		fwrite(strName, sizeof(char), NAME_LENGTH, fp);
		fwrite(&nKeyFrames, sizeof(UINT), 1, fp);
		for(int i = 0; i < nKeyFrames; ++i)
			fwrite(arrKeyFrames[i].arrKeyFrameBones, sizeof(KeyFrameBone), nBones, fp);
		fclose(fp);
	}
}

CAnimation::CAnimation(UINT nBones, const char * strFileName)
{
	FILE* fp = nullptr;
	fopen_s(&fp, strFileName, "rb");
	fread_s(strName, sizeof(char) * NAME_LENGTH, sizeof(char), NAME_LENGTH, fp);
	fread_s(&nKeyFrames, sizeof(UINT), sizeof(UINT), 1, fp);
	arrKeyFrames = new KeyFrame[nKeyFrames];
	for (int i = 0; i < nKeyFrames; ++i)
	{
		arrKeyFrames[i].arrKeyFrameBones = new KeyFrameBone[nBones];
		fread_s(arrKeyFrames[i].arrKeyFrameBones, sizeof(KeyFrameBone) * nBones, sizeof(KeyFrameBone), nBones, fp);
	}
	fclose(fp);
}

CAnimation::~CAnimation()
{
	if (arrKeyFrames) delete[] arrKeyFrames;
}