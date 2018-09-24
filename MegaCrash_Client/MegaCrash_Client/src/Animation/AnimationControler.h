#pragma once
#include "Animation.h"

class CAnimState;
class CAnimCtrlr
{
public:
	enum class Tag : BYTE { 
		  Idle
		, MoveStart
		, Move
		, MoveEnd
		, TakeOff
		, Hovering
		, Landing
		, Jump
		, Dash
		, Attack_1
		, Attack_2
		, Skill
		, Count 
	};

private:
	shared_ptr<Skeleton>				m_pSkeleton;

	unordered_map<Tag, CAnimState>		m_Anims;
	CAnimation*							m_pCurrAnim;
	Tag									m_CurrTag;
	Tag									m_NextTag;

	float								m_fAnimTimer;

	int									m_iIgnoreBoneIdx;

	XMMATRIX*							m_pmtxCurrFinalTransforms;

	XMMATRIX*							m_pcbMappedFinalTransforms;
	CB_DESC								m_cbFinalTransformsDesc;

public:
	CAnimCtrlr();
	virtual ~CAnimCtrlr();

	virtual void Animate(float fTimeElapsed);

	void CreateShaderVariables(ID3D12GraphicsCommandList* pd3dCmdLst);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCmdLst);
	void ReleaseShaderVariables();

	void SetSkeleton(const shared_ptr<Skeleton>& pSkeleton);
	void SetAnimState(Tag tag, const CAnimState& animState);
	void SetIgnoreBoneIdx(int boneIdx) { m_iIgnoreBoneIdx = boneIdx; }
	Tag GetCurrTag() const { return m_CurrTag; }

	bool ChangeCurrAnim(Tag tag);

	bool IsMoving() const;
	bool IsMovingOnGround() const;
	bool IsMovingInAir() const;
	bool IsAttacking() const;
};

