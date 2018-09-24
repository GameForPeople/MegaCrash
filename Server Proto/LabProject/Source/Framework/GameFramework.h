#pragma once
#include "Scene\Scene.h"
#include "Timer\Timer.h"

struct RoomInfo;

class CGameFramework
{
private:
	RoomInfo*		m_pRoomInfo;
	CTimer			m_Timer;
	CScene*			m_pScene;
	PxFoundation	*m_pFoundation = NULL;

public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate();

	CScene*		GetScene()		const { return m_pScene; }
	RoomInfo*	GetRoomInfo()	const { return m_pRoomInfo; }

	void RegisterRoomInfo(RoomInfo* room) { m_pRoomInfo = room; }
	void RegisterPxFoundation(PxFoundation* PxF) { m_pFoundation = PxF; }

	void BuildObjects();
	void ReleaseObjects();
};