#pragma once

struct FBXPNTVertex
{
	XMFLOAT3 mPosition;
	XMFLOAT3 mNormal;
	XMFLOAT2 mUV;
	unsigned char mMaterialID;

	bool operator==(const FBXPNTVertex& rhs) const
	{
		uint32_t position;
		uint32_t normal;
		uint32_t uv;

		XMVectorEqualR(&position, XMLoadFloat3(&(this->mPosition)), XMLoadFloat3(&rhs.mPosition));
		XMVectorEqualR(&normal, XMLoadFloat3(&(this->mNormal)), XMLoadFloat3(&rhs.mNormal));
		XMVectorEqualR(&uv, XMLoadFloat2(&(this->mUV)), XMLoadFloat2(&rhs.mUV));
		bool result = mMaterialID == rhs.mMaterialID;

		return result && XMComparisonAllTrue(position) && XMComparisonAllTrue(normal) && XMComparisonAllTrue(uv);
	}
};

struct FBXVertexBlendingInfo
{
	unsigned int mBlendingIndex;
	double mBlendingWeight;

	FBXVertexBlendingInfo():
		mBlendingIndex(0),
		mBlendingWeight(0.0)
	{}

	bool operator < (const FBXVertexBlendingInfo& rhs)
	{
		return (mBlendingWeight > rhs.mBlendingWeight);
	}
};

struct FBXPNTIWVertex
{
	XMFLOAT3 mPosition;
	XMFLOAT3 mNormal;
	XMFLOAT3 mBiNormal;
	XMFLOAT3 mTangent;
	XMFLOAT2 mUV;
	unsigned char mMaterialID;

	std::vector<FBXVertexBlendingInfo> mVertexBlendingInfos;

	void SortBlendingInfoByWeight()
	{
		std::sort(mVertexBlendingInfos.begin(), mVertexBlendingInfos.end());
	}

	bool operator==(const FBXPNTIWVertex& rhs) const
	{
		bool sameBlendingInfo = true;

		// We only compare the blending info when there is blending info
		if(!(mVertexBlendingInfos.empty() && rhs.mVertexBlendingInfos.empty()))
		{
			// Each vertex should only have 4 index-weight blending info pairs
			for (unsigned int i = 0; i < 4; ++i)
			{
				if (mVertexBlendingInfos[i].mBlendingIndex != rhs.mVertexBlendingInfos[i].mBlendingIndex ||
					abs(mVertexBlendingInfos[i].mBlendingWeight - rhs.mVertexBlendingInfos[i].mBlendingWeight) > 0.001)
				{
					sameBlendingInfo = false;
					break;
				}
			}
		}
		
		bool result1 = Vector3::CompareVector3WithEpsilon(mPosition, rhs.mPosition);
		bool result2 = Vector3::CompareVector3WithEpsilon(mNormal, rhs.mNormal);
		bool result3 = Vector2::CompareVector2WithEpsilon(mUV, rhs.mUV);
		bool result4 = mMaterialID == rhs.mMaterialID;

		return result1 && result2 && result3 && result4 && sameBlendingInfo;
	}
};
