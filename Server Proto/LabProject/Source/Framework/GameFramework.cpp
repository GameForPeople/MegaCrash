//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"

CGameFramework::CGameFramework()
	: m_pScene(NULL)
{
}

CGameFramework::~CGameFramework()
{
	ReleaseObjects();
}

bool CGameFramework::OnCreate()
{
	BuildObjects();

	return true;
}

void CGameFramework::BuildObjects()
{
	m_pScene = new CScene();
	m_pScene->SetFramework(this);
	m_pScene->RegisterFoundation(m_pFoundation);
	m_pScene->RegisterRoomInfo(m_pRoomInfo);
	m_pScene->BuildObjects();

	m_Timer.Reset();
}

void CGameFramework::ReleaseObjects()
{
	if (m_pScene)
	{
		delete m_pScene;
	}
}
