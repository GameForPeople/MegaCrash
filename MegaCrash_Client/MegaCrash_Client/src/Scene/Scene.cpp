//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Framework/Framework.h"
#include "Camera/Camera.h"
#include "Scene.h"

CScene::CScene() 
	: m_hInstance			{ nullptr }
	, m_hWnd				{ nullptr }
	, m_pFramework			{ nullptr }
	, m_pShaderHelper		{ nullptr }
	, m_pObjectManager		{ nullptr }
	, m_pParticleManager	{ nullptr }
{
}
CScene::~CScene()
{
}