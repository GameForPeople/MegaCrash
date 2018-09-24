#pragma once
#include "AnimationControler.h"

class CAnimation;
class CAnimState
{
public:
	shared_ptr<CAnimation>		Anim;
	CAnimCtrlr::Tag				Tag;
	float						fTimeRate_StartPos;
	float						fTimeRate_EndPos;
	float						fTimeRate_RepeatStartPos;
	float						fTimeRate_RepeatEndPos;
	float						fTimeLength;
	float						fTimeRatePerSec;
	bool						bCancelable;
	bool						bRepeatable;
	CAnimCtrlr::Tag				DefaultNextState;
	vector<CAnimCtrlr::Tag>		vecNextStates;

public:
	CAnimState();
	CAnimState(const CAnimState& other);
	virtual ~CAnimState();

	void operator=(const CAnimState& other);

	bool IsNextState(CAnimCtrlr::Tag tag);

	void SetParam(
		  CAnimCtrlr::Tag	tag
		, float				f_time_rate_start_pos
		, float				f_time_rate_end_pos
		, float				f_time_rate_repeat_start_pos
		, float				f_time_rate_repeat_end_pos
		, float				f_time_length
		, bool				b_cancelable
		, bool				b_repeatable);
	void SetAnim(const shared_ptr<CAnimation>& anim)	{ Anim = anim; }
	void SetNextState(CAnimCtrlr::Tag tag)				{ vecNextStates.push_back(tag); }
	void SetDefaultNextState(CAnimCtrlr::Tag tag)		{ DefaultNextState = tag; }
};

