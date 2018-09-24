#include "stdafx.h"
#include "AnimationState.h"
#include "AnimationControler.h"

CAnimCtrlr::CAnimCtrlr()
	: m_pSkeleton					{ nullptr }
	, m_pCurrAnim					{ nullptr }
	, m_CurrTag						{ Tag::Idle }
	, m_NextTag						{ Tag::Idle }
	, m_fAnimTimer					{ 0.0f }
	, m_iIgnoreBoneIdx				{ -1 }
	, m_pmtxCurrFinalTransforms		{ nullptr }
	, m_pcbMappedFinalTransforms	{ nullptr }
{
}
CAnimCtrlr::~CAnimCtrlr()
{
	ReleaseShaderVariables();

	m_Anims.clear();

	if (m_pmtxCurrFinalTransforms)	delete[] m_pmtxCurrFinalTransforms;
}

void CAnimCtrlr::Animate(float fTimeElapsed)
{
	m_fAnimTimer += m_Anims[m_CurrTag].fTimeRatePerSec * fTimeElapsed;
	if (m_CurrTag == m_NextTag && m_Anims[m_CurrTag].bRepeatable) {
		if (m_fAnimTimer > m_Anims[m_CurrTag].fTimeRate_RepeatEndPos) { 
			m_fAnimTimer = m_Anims[m_NextTag].fTimeRate_RepeatStartPos;
			m_NextTag = m_Anims[m_NextTag].DefaultNextState;
		}
	}
	else {
		if (m_fAnimTimer > m_Anims[m_CurrTag].fTimeRate_EndPos) {
			m_fAnimTimer = m_Anims[m_NextTag].fTimeRate_StartPos;
			m_CurrTag = m_NextTag;
			m_NextTag = m_Anims[m_NextTag].DefaultNextState;
			m_pCurrAnim = m_Anims[m_CurrTag].Anim.get();
		}
	}
	
	float fFrameFactor = m_fAnimTimer * static_cast<float>(m_pCurrAnim->nKeyFrames - 1);
	UINT iCurrFrame = static_cast<UINT>(fFrameFactor);
	UINT iNextFrame = iCurrFrame + 1;
	float interpolationFactor = fFrameFactor - static_cast<float>(iCurrFrame);

	KeyFrame* CurrKeyframe{ &(m_pCurrAnim->arrKeyFrames[iCurrFrame]) };
	KeyFrame* NextKeyframe{ &(m_pCurrAnim->arrKeyFrames[iNextFrame]) };

	for (int iCurrBoneIdx = 0; iCurrBoneIdx < m_pSkeleton->nBones; ++iCurrBoneIdx)
	{
		XMMATRIX mtxTransform;
		Matrix4x4::InterpolateMtx(
			  &mtxTransform
			, CurrKeyframe->arrKeyFrameBones[iCurrBoneIdx].mtxTransform
			, NextKeyframe->arrKeyFrameBones[iCurrBoneIdx].mtxTransform
			, interpolationFactor);
		if (iCurrBoneIdx == m_iIgnoreBoneIdx) {
			m_pmtxCurrFinalTransforms[iCurrBoneIdx] = XMMatrixTranspose(XMMatrixMultiply(
				  m_pSkeleton->arrBones[iCurrBoneIdx].mtxBoneOffset
				, CurrKeyframe->arrKeyFrameBones[iCurrBoneIdx].mtxTransform));
		}
		else {
			m_pmtxCurrFinalTransforms[iCurrBoneIdx] = XMMatrixTranspose(XMMatrixMultiply(
			  	  m_pSkeleton->arrBones[iCurrBoneIdx].mtxBoneOffset
				, mtxTransform));
		}
	}
}

void CAnimCtrlr::CreateShaderVariables(ID3D12GraphicsCommandList* pd3dCmdLst)
{
	CD3DBufMgr::Instance()->AllocUploadBuffer(m_cbFinalTransformsDesc, sizeof(XMMATRIX) * m_pSkeleton->nBones);
	m_pcbMappedFinalTransforms = static_cast<XMMATRIX*>(m_cbFinalTransformsDesc.pMappedPtr);
	m_cbFinalTransformsDesc.RootParamIdx = ROOT_PARAMETER_CONSTANT1;
}

void CAnimCtrlr::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCmdLst)
{
	memcpy(m_pcbMappedFinalTransforms, m_pmtxCurrFinalTransforms, sizeof(XMMATRIX) * m_pSkeleton->nBones);
	pd3dCmdLst->SetGraphicsRootConstantBufferView(m_cbFinalTransformsDesc.RootParamIdx, m_cbFinalTransformsDesc.view_desc.BufferLocation);
}

void CAnimCtrlr::ReleaseShaderVariables()
{
}

void CAnimCtrlr::SetSkeleton(const shared_ptr<Skeleton>& pSkeleton)
{
	m_pSkeleton = pSkeleton;
	if (m_pmtxCurrFinalTransforms)	delete[] m_pmtxCurrFinalTransforms;
	m_pmtxCurrFinalTransforms = new XMMATRIX[m_pSkeleton->nBones];
}

void CAnimCtrlr::SetAnimState(Tag tag, const CAnimState & animState)
{
	m_Anims[tag] = animState;
	if (tag == Tag::Idle) {
		m_CurrTag = tag;
		m_NextTag = m_Anims[m_CurrTag].DefaultNextState;
		m_fAnimTimer = m_Anims[m_CurrTag].fTimeRate_StartPos;
		m_pCurrAnim = m_Anims[m_CurrTag].Anim.get();
	}
}

bool CAnimCtrlr::ChangeCurrAnim(Tag tag)
{
	static bool bOverlap{ false };
	if (!m_Anims[m_CurrTag].IsNextState(tag)) return false;
	if (m_CurrTag != tag && m_Anims[m_CurrTag].bCancelable) {
		m_CurrTag = tag;
		m_NextTag = m_Anims[m_CurrTag].DefaultNextState;
		m_fAnimTimer = m_Anims[m_CurrTag].fTimeRate_StartPos;
		m_pCurrAnim = m_Anims[m_CurrTag].Anim.get();
		bOverlap = false;
		return true;
	}
	else if (m_NextTag != tag) {
		m_NextTag = tag;
		bOverlap = false;
		return true;
	}
	else if (m_NextTag == tag) {
		if (false == bOverlap) {
			bOverlap = true;
			return true;
		}
	}
	return false;
}

bool CAnimCtrlr::IsMoving() const
{
	return (
		   m_CurrTag == CAnimCtrlr::Tag::MoveStart
		|| m_CurrTag == CAnimCtrlr::Tag::Move
		|| m_CurrTag == CAnimCtrlr::Tag::TakeOff
		|| m_CurrTag == CAnimCtrlr::Tag::Hovering
		|| m_CurrTag == CAnimCtrlr::Tag::Dash
		|| m_CurrTag == CAnimCtrlr::Tag::Jump
		);
}

bool CAnimCtrlr::IsMovingOnGround() const
{
	return (
		   m_CurrTag == CAnimCtrlr::Tag::MoveStart
		|| m_CurrTag == CAnimCtrlr::Tag::Move
		);
}

bool CAnimCtrlr::IsMovingInAir() const
{
	return (
		   m_CurrTag == CAnimCtrlr::Tag::TakeOff
		|| m_CurrTag == CAnimCtrlr::Tag::Hovering
		);
}

bool CAnimCtrlr::IsAttacking() const
{
	return (
		   m_CurrTag == CAnimCtrlr::Tag::Attack_1
		|| m_CurrTag == CAnimCtrlr::Tag::Attack_2
		|| m_CurrTag == CAnimCtrlr::Tag::Skill
		);
}
