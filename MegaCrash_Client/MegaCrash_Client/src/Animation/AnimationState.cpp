#include "stdafx.h"
#include "AnimationState.h"

CAnimState::CAnimState()
	: Anim						{ nullptr }
	, Tag						{ CAnimCtrlr::Tag::Idle }
	, fTimeRate_StartPos		{ 0.0f }
	, fTimeRate_EndPos			{ 1.0f }
	, fTimeRate_RepeatStartPos	{ 0.0f }
	, fTimeRate_RepeatEndPos	{ 1.0f }
	, fTimeLength				{ 0.0f }
	, fTimeRatePerSec			{ 0.0f }
	, bCancelable				{ false }
	, bRepeatable				{ false }
	, DefaultNextState			{ CAnimCtrlr::Tag::Idle }
{
}
CAnimState::CAnimState(const CAnimState & other)
	: Anim						{ other.Anim }
	, Tag						{ other.Tag }
	, fTimeRate_StartPos		{ other.fTimeRate_StartPos }
	, fTimeRate_EndPos			{ other.fTimeRate_EndPos }
	, fTimeRate_RepeatStartPos	{ other.fTimeRate_RepeatStartPos }
	, fTimeRate_RepeatEndPos	{ other.fTimeRate_RepeatEndPos }
	, fTimeLength				{ other.fTimeLength }
	, fTimeRatePerSec			{ other.fTimeRatePerSec }
	, bCancelable				{ other.bCancelable }
	, bRepeatable				{ other.bRepeatable }
	, DefaultNextState			{ other.DefaultNextState }
	, vecNextStates				{ other.vecNextStates }
{
}
CAnimState::~CAnimState() {
	vecNextStates.clear();
}

void CAnimState::operator=(const CAnimState & other)
{
	Anim = other.Anim;
	Tag = other.Tag;
	fTimeRate_StartPos = other.fTimeRate_StartPos;
	fTimeRate_EndPos = other.fTimeRate_EndPos;
	fTimeRate_RepeatStartPos = other.fTimeRate_RepeatStartPos;
	fTimeRate_RepeatEndPos = other.fTimeRate_RepeatEndPos;
	fTimeLength = other.fTimeLength;
	fTimeRatePerSec = other.fTimeRatePerSec;
	bCancelable = other.bCancelable;
	bRepeatable = other.bRepeatable;
	DefaultNextState = other.DefaultNextState;
	vecNextStates = other.vecNextStates;
}

bool CAnimState::IsNextState(CAnimCtrlr::Tag tag)
{
	for (auto p : vecNextStates) {
		if (p == tag) return true;
	}
	return false;
}

void CAnimState::SetParam(
	  CAnimCtrlr::Tag	tag
	, float				f_time_rate_start_pos
	, float				f_time_rate_end_pos
	, float				f_time_rate_repeat_start_pos
	, float				f_time_rate_repeat_end_pos
	, float				f_time_length
	, bool				b_cancelable
	, bool				b_repeatable)
{
	Tag = tag;
	fTimeRate_StartPos = f_time_rate_start_pos;
	fTimeRate_EndPos = f_time_rate_end_pos;
	fTimeRate_RepeatStartPos = f_time_rate_repeat_start_pos;
	fTimeRate_RepeatEndPos = f_time_rate_repeat_end_pos;
	fTimeLength = f_time_length;
	fTimeRatePerSec = (fTimeRate_EndPos - fTimeRate_StartPos) / fTimeLength;
	bCancelable = b_cancelable;
	bRepeatable = b_repeatable;
}